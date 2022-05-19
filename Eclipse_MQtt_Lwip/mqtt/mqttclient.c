/*
 * mqttclient.c
 *
 * created: 2022/5/19
 *  author: 
 */

 void mqtt_recv_thread(void *pvParameters)
 {
     uint32_t curtick;
     uint8_t no_mqtt_msg_exchange = 1;
     uint8_t buf[MSG_MAX_LEN];
     int32_t buflen = sizeof(buf);
     int32_t type;
     fd_set readfd;
     struct timeval tv;      //等待时间
     tv.tv_sec = 0;
     tv.tv_usec = 10;


 MQTT_START:
     //开始连接
     Client_Connect();
     //获取当前滴答，作为心跳包起始时间
     curtick = xTaskGetTickCount();
     while (1)
     {
         //表明无数据交换
         no_mqtt_msg_exchange = 1;

         FD_ZERO(&readfd);
         FD_SET(MQTT_Socket,&readfd);

         //等待可读事件
         select(MQTT_Socket+1,&readfd,NULL,NULL,&tv);

         //判断MQTT服务器是否有数据
         if (FD_ISSET(MQTT_Socket,&readfd) != 0)
         {
             //读取数据包--注意这里参数为0，不阻塞
             type = ReadPacketTimeout(MQTT_Socket,buf,buflen,0);
             if (type != -1)
             {
                 mqtt_pktype_ctl(type,buf,buflen);
                 //表明有数据交换
                 no_mqtt_msg_exchange = 0;
                 //获取当前滴答，作为心跳包起始时间
                 curtick = xTaskGetTickCount();
             }
         }

         //这里主要目的是定时向服务器发送PING保活命令
         if ((xTaskGetTickCount() - curtick) >(KEEPLIVE_TIME/2*1000))
         {
             curtick = xTaskGetTickCount();
             //判断是否有数据交换
             if (no_mqtt_msg_exchange == 0)
             {
                 //如果有数据交换，这次就不需要发送PING消息
                 continue;
             }

             if (MQTT_PingReq(MQTT_Socket) < 0)
             {
                 //重连服务器
                 PRINT_DEBUG("发送保持活性ping失败....\n");
                 goto CLOSE;
             }

             //心跳成功
             PRINT_DEBUG("发送保持活性ping作为心跳成功....\n");
             //表明有数据交换
             no_mqtt_msg_exchange = 0;
         }
     }

 CLOSE:
     //关闭链接
     transport_close();
     //重新链接服务器
     goto MQTT_START;
 }

 void mqtt_send_thread(void *pvParameters)
 {
     int32_t ret;
     uint8_t no_mqtt_msg_exchange = 1;
     uint32_t curtick;
     uint8_t res;
     /* 定义一个创建信息返回值，默认为pdTRUE */
     BaseType_t xReturn = pdTRUE;
     /* 定义一个接收消息的变量 */
 //    uint32_t* r_data;
     DHT11_Data_TypeDef* recv_data;
     //初始化json数据
     cJSON* cJSON_Data = NULL;
     cJSON_Data = cJSON_Data_Init();
     double a,b;
 MQTT_SEND_START:

     while (1)
     {

         xReturn = xQueueReceive( MQTT_Data_Queue,    /* 消息队列的句柄 */
                                 &recv_data,      /* 发送的消息内容 */
                                 3000); /* 等待时间 3000ms */
         if (xReturn == pdTRUE)
         {
             a = recv_data->temperature;
             b = recv_data->humidity;
             printf("a = %f,b = %f\n",a,b);
             //更新数据
             res = cJSON_Update(cJSON_Data,TEMP_NUM,&a);
             res = cJSON_Update(cJSON_Data,HUM_NUM,&b);

             if (UPDATE_SUCCESS == res)
             {
                 //更新数据成功，
                 char* p = cJSON_Print(cJSON_Data);
                 //发布消息
                 ret = MQTTMsgPublish(MQTT_Socket,(char*)TOPIC,QOS0,(uint8_t*)p);
                 if (ret >= 0)
                 {
                     //表明有数据交换
                     no_mqtt_msg_exchange = 0;
                     //获取当前滴答，作为心跳包起始时间
                     curtick = xTaskGetTickCount();
                 }
                 vPortFree(p);
                 p = NULL;
             }
             else
                 PRINT_DEBUG("update fail\n");
         }
         //这里主要目的是定时向服务器发送PING保活命令
         if ((xTaskGetTickCount() - curtick) >(KEEPLIVE_TIME/2*1000))
         {
             curtick = xTaskGetTickCount();
             //判断是否有数据交换
             if (no_mqtt_msg_exchange == 0)
             {
                 //如果有数据交换，这次就不需要发送PING消息
                 continue;
             }

             if (MQTT_PingReq(MQTT_Socket) < 0)
             {
                 //重连服务器
                 PRINT_DEBUG("发送保持活性ping失败....\n");
                 goto MQTT_SEND_CLOSE;
             }

             //心跳成功
             PRINT_DEBUG("发送保持活性ping作为心跳成功....\n");
             //表明有数据交换
             no_mqtt_msg_exchange = 0;
         }
     }
 MQTT_SEND_CLOSE:
     //关闭链接
     transport_close();
     //开始连接
     Client_Connect();
     goto MQTT_SEND_START;
 }

 void
 mqtt_thread_init(void)
 {
     sys_thread_new("mqtt_recv_thread", mqtt_recv_thread, NULL, 2048, 6);
     sys_thread_new("mqtt_send_thread", mqtt_send_thread, NULL, 2048, 7);
 }

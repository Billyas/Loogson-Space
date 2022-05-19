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
     struct timeval tv;      //�ȴ�ʱ��
     tv.tv_sec = 0;
     tv.tv_usec = 10;


 MQTT_START:
     //��ʼ����
     Client_Connect();
     //��ȡ��ǰ�δ���Ϊ��������ʼʱ��
     curtick = xTaskGetTickCount();
     while (1)
     {
         //���������ݽ���
         no_mqtt_msg_exchange = 1;

         FD_ZERO(&readfd);
         FD_SET(MQTT_Socket,&readfd);

         //�ȴ��ɶ��¼�
         select(MQTT_Socket+1,&readfd,NULL,NULL,&tv);

         //�ж�MQTT�������Ƿ�������
         if (FD_ISSET(MQTT_Socket,&readfd) != 0)
         {
             //��ȡ���ݰ�--ע���������Ϊ0��������
             type = ReadPacketTimeout(MQTT_Socket,buf,buflen,0);
             if (type != -1)
             {
                 mqtt_pktype_ctl(type,buf,buflen);
                 //���������ݽ���
                 no_mqtt_msg_exchange = 0;
                 //��ȡ��ǰ�δ���Ϊ��������ʼʱ��
                 curtick = xTaskGetTickCount();
             }
         }

         //������ҪĿ���Ƕ�ʱ�����������PING��������
         if ((xTaskGetTickCount() - curtick) >(KEEPLIVE_TIME/2*1000))
         {
             curtick = xTaskGetTickCount();
             //�ж��Ƿ������ݽ���
             if (no_mqtt_msg_exchange == 0)
             {
                 //��������ݽ�������ξͲ���Ҫ����PING��Ϣ
                 continue;
             }

             if (MQTT_PingReq(MQTT_Socket) < 0)
             {
                 //����������
                 PRINT_DEBUG("���ͱ��ֻ���pingʧ��....\n");
                 goto CLOSE;
             }

             //�����ɹ�
             PRINT_DEBUG("���ͱ��ֻ���ping��Ϊ�����ɹ�....\n");
             //���������ݽ���
             no_mqtt_msg_exchange = 0;
         }
     }

 CLOSE:
     //�ر�����
     transport_close();
     //�������ӷ�����
     goto MQTT_START;
 }

 void mqtt_send_thread(void *pvParameters)
 {
     int32_t ret;
     uint8_t no_mqtt_msg_exchange = 1;
     uint32_t curtick;
     uint8_t res;
     /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdTRUE */
     BaseType_t xReturn = pdTRUE;
     /* ����һ��������Ϣ�ı��� */
 //    uint32_t* r_data;
     DHT11_Data_TypeDef* recv_data;
     //��ʼ��json����
     cJSON* cJSON_Data = NULL;
     cJSON_Data = cJSON_Data_Init();
     double a,b;
 MQTT_SEND_START:

     while (1)
     {

         xReturn = xQueueReceive( MQTT_Data_Queue,    /* ��Ϣ���еľ�� */
                                 &recv_data,      /* ���͵���Ϣ���� */
                                 3000); /* �ȴ�ʱ�� 3000ms */
         if (xReturn == pdTRUE)
         {
             a = recv_data->temperature;
             b = recv_data->humidity;
             printf("a = %f,b = %f\n",a,b);
             //��������
             res = cJSON_Update(cJSON_Data,TEMP_NUM,&a);
             res = cJSON_Update(cJSON_Data,HUM_NUM,&b);

             if (UPDATE_SUCCESS == res)
             {
                 //�������ݳɹ���
                 char* p = cJSON_Print(cJSON_Data);
                 //������Ϣ
                 ret = MQTTMsgPublish(MQTT_Socket,(char*)TOPIC,QOS0,(uint8_t*)p);
                 if (ret >= 0)
                 {
                     //���������ݽ���
                     no_mqtt_msg_exchange = 0;
                     //��ȡ��ǰ�δ���Ϊ��������ʼʱ��
                     curtick = xTaskGetTickCount();
                 }
                 vPortFree(p);
                 p = NULL;
             }
             else
                 PRINT_DEBUG("update fail\n");
         }
         //������ҪĿ���Ƕ�ʱ�����������PING��������
         if ((xTaskGetTickCount() - curtick) >(KEEPLIVE_TIME/2*1000))
         {
             curtick = xTaskGetTickCount();
             //�ж��Ƿ������ݽ���
             if (no_mqtt_msg_exchange == 0)
             {
                 //��������ݽ�������ξͲ���Ҫ����PING��Ϣ
                 continue;
             }

             if (MQTT_PingReq(MQTT_Socket) < 0)
             {
                 //����������
                 PRINT_DEBUG("���ͱ��ֻ���pingʧ��....\n");
                 goto MQTT_SEND_CLOSE;
             }

             //�����ɹ�
             PRINT_DEBUG("���ͱ��ֻ���ping��Ϊ�����ɹ�....\n");
             //���������ݽ���
             no_mqtt_msg_exchange = 0;
         }
     }
 MQTT_SEND_CLOSE:
     //�ر�����
     transport_close();
     //��ʼ����
     Client_Connect();
     goto MQTT_SEND_START;
 }

 void
 mqtt_thread_init(void)
 {
     sys_thread_new("mqtt_recv_thread", mqtt_recv_thread, NULL, 2048, 6);
     sys_thread_new("mqtt_send_thread", mqtt_send_thread, NULL, 2048, 7);
 }

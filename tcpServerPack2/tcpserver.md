## API 详解



##### 创建服务端

创建一个 TCP 的服务端，传入服务端的 ip 地址和端口号。

```
struct tcpserver *tcpserver_create(const char *ip, rt_uint16_t port);
```

**函数参数**

| 参数     | 描述                              |
| -------- | --------------------------------- |
| ip       | 要绑定的 ip 地址                  |
| port     | 要绑定的 端口号                   |
| **返回** | **——**                            |
| >  0     | 成功，返回一个 tcp 服务端的句柄。 |
| = 0      | 失败                              |

##### 销毁服务端

销毁一个 TCP 的服务端，并回收资源。

```
rt_err_t tcpserver_destroy(struct tcpserver *server);
```

**函数参数**

| 参数     | 描述                      |
| -------- | ------------------------- |
| server   | 要销毁的 tcp 服务端的句柄 |
| **返回** | **——**                    |
| =  0     | 成功                      |
| < 0      | 失败                      |

##### 获取客户端

获取一个 TCP 的客户端，阻塞式的获取一个客户端。

```
tcpclient_t tcpserver_accept(struct tcpserver *server, rt_int32_t timeout);
```

**函数参数**

| 参数     | 描述                                              |
| -------- | ------------------------------------------------- |
| server   | tcp 服务端的句柄                                  |
| timeout  | 等待的超时时间，RT_WAITING_FOREVER 表示永久等待。 |
| **返回** | **——**                                            |
| >  0     | 成功，返回一个 tcp 客户端的句柄。                 |
| = 0      | 失败                                              |

##### 关闭客户端

关闭一个 TCP 的客户端。

```
rt_err_t tcpserver_close(tcpclient_t client);
```

**函数参数**

| 参数     | 描述             |
| -------- | ---------------- |
| client   | tcp 客户端的句柄 |
| **返回** | **——**           |
| =  0     | 成功。           |
| < 0      | 失败             |

##### 从客户端接收数据

从客户端接收数据。

```
rt_size_t tcpserver_recv(tcpclient_t client, void *buf, rt_size_t size, rt_int32_t timeout);
```

**函数参数**

| 参数     | 描述                                              |
| -------- | ------------------------------------------------- |
| client   | tcp 客户端的句柄                                  |
| buf      | 缓冲区地址                                        |
| size     | 缓冲区大小                                        |
| timeout  | 等待的超时时间，RT_WAITING_FOREVER 表示永久等待。 |
| **返回** | **——**                                            |
| >  0     | 成功，返回接收到的数据长度。                      |
| = 0      | 失败                                              |

##### 向客户端发送数据

向客户端发送数据。

```
rt_size_t tcpserver_send(tcpclient_t client, void *buf, rt_size_t size, rt_int32_t timeout);
```

**函数参数**

| 参数     | 描述                         |
| -------- | ---------------------------- |
| client   | tcp 客户端的句柄             |
| buf      | 缓冲区地址                   |
| size     | 缓冲区大小                   |
| timeout  | 等待的超时时间，**暂未实现** |
| **返回** | **——**                       |
| >  0     | 成功，已发送的数据长度。     |
| = 0      | 失败                         |

##### 设定事件通知回调函数

设定服务器的事件通知回调函数。

```
void tcpserver_set_notify_callback(struct tcpserver *server,
                                   void (*tcpserver_event_notify)(tcpclient_t client, rt_uint8_t event));
```

**函数参数**

| 参数                   | 描述             |
| ---------------------- | ---------------- |
| server                 | tcp 服务端的句柄 |
| tcpserver_event_notify | 要设定的函数指针 |
| **返回**               | **——**           |

## tcpserver包移植



![image-20220505081923515](https://cdn.jsdelivr.net/gh/Billyas/myimg/img/image-20220505081923515.png)



![image-20220505081845964](https://cdn.jsdelivr.net/gh/Billyas/myimg/img/image-20220505081845964.png)

![image-20220505082022171](https://cdn.jsdelivr.net/gh/Billyas/myimg/img/image-20220505082022171.png)

![image-20220505082046915](https://cdn.jsdelivr.net/gh/Billyas/myimg/img/image-20220505082046915.png)




#ifndef _tcpserver_h_
#define _tcpserver_h_

#include "EventLoopThreadPool.h"
#include "TcpServer.h"
#include "HandleEvent.h"
#include <string>
#include <memory>
#include <functional>
#include <map>

/*
负责tcp连接的建立和断开，成员变量包含一个baseloop，
建立连接后将当前连接分配到workloop
*/
class TcpServer{
public:
    using HandleCb=std::function<void(int)>;    
    TcpServer(EventLoop*,int thread_num,std::string addr,int port);
    TcpServer(const TcpServer&)=delete;
    TcpServer& operator=(const TcpServer&)=delete;
    ~TcpServer();
    void Run();
    void HandleNewConnection(int fd);//处理新连接
    void HandleAccept();
    void SetRead(EventCallback&& event){read_event=event;}//
    void SetMessage(MessageCallback&& event){message_back_=event;}
    void SetConnectionCb(NewConnection &&cb){connect_cb_=cb;}
private:
    void SetConnect(HandleCb &&cb){conn_cb_=cb;}
    //void SetAcceptCb(HandleCb &&cb){accpet_cb_=cb;}
    int Init();
    void SetKeepAlive(int fd,bool);
    void SetReuseAddr(int sockfd);
    void SetTcpNodelay(int sockfd);
    void SetNoBlocking(int sockfd);
    void SetCloseCb(CloseCallback && cb){close_event=cb;}
    void CloseEvent(const TcpConnPtr conn);
    void CloseEventInLoop(const TcpConnPtr conn);
private:
    HandleEvent event_;//处理连接的event
    EventCallback read_event;//读事件回调
    MessageCallback message_back_;//回复事件回调
    CloseCallback close_event;//关闭事件回调
private:
    int listenfd_;
    int connfd_;
    int port_;
    std::string addr_str;
private:
    std::map<int,TcpConnPtr>connections_;//所有的连接
    NewConnection connect_cb_;
    HandleCb conn_cb_;
    EventLoop* base_loop;//负责连接的loop
    std::unique_ptr<EventLoopThreadPool> pool_;//线程池    
};
#endif
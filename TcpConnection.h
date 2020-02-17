#ifndef _tcp_connection_h_
#define _tcp_connection_h_
#include <memory>
#include "EventLoop.h"
#include "Buffer.h"

/*每个TcpConnection类维护一个i/o fd，负责连接的状态和事件注册*/
class TcpConnection:public std::enable_shared_from_this<TcpConnection>{
public:
    enum ConnectState{disconnected_,connecting_,connected_,disconnecting_};
    TcpConnection(EventLoop*,int);
    TcpConnection(const TcpConnection&)=delete;
    TcpConnection& operator=(const TcpConnection&)=delete;
    ~TcpConnection();
    TcpConnPtr GetThis(){return shared_from_this();}
    void Shutdown();
    void HandleRead();//处理读事件函数
    void HandleWrite();//处理写事件函数
    
    void SetRead(const EventCallback& event){read_event_=event;}
    void SetMessageBack(const MessageCallback& event){message_back_=event;}
    void SetCloseEvent(const CloseCallback& event){close_cb_=event;}
    void SetConnectionCb(const NewConnection& event){connect_cb_=event;}
public:
    void AttachToLoop();
    int Sockfd()const{return sockfd_;}
    void Send(std::string &);
private:
    void HandleError();
    void HandleClose();
    void ShutdownWrite();
    int ReadFromFd(std::string &);
    
    void SendInLoop(std::string&);
private:
    EventCallback read_event_;//读事件
    MessageCallback message_back_;//回复事件
    NewConnection connect_cb_;//连接事件
    CloseCallback close_cb_;//关闭事件
private:
    ConnectState status_;
    EventLoop* loop_=nullptr;
    int sockfd_;//维护的描述符
    HandleEvent event_;//eventfd的描述符
    Buffer in_buffer_;
};

#endif
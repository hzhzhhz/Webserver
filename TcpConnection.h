#ifndef _tcp_connection_h_
#define _tcp_connection_h_
#include <memory>
#include "EventLoop.h"

class TcpConnection:public std::enable_shared_from_this<TcpConnection>{
public:
    enum ConnectState{disconnected_,connecting_,connected_,disconnecting_};
    TcpConnection(EventLoop*,int);
    ~TcpConnection();
    TcpConnPtr GetThis(){return shared_from_this();}
    void Shutdown();
    void HandleRead();
    void HandleWrite();
    
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
    EventCallback read_event_;
    MessageCallback message_back_;
    NewConnection connect_cb_;
    CloseCallback close_cb_;
private:
    ConnectState status_;
    EventLoop* loop_=nullptr;
    int sockfd_;
    HandleEvent event_;
};

#endif
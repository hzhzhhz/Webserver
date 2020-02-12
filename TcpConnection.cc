#include "TcpConnection.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

TcpConnection::TcpConnection(EventLoop* loop,int fd)
:loop_(loop)
,sockfd_(fd)
,event_(loop,fd)
,status_(disconnected_)
{
    event_.SetRead([this](){HandleRead();});
    event_.SetWrite([this](){HandleWrite();});
}
TcpConnection::~TcpConnection(){
    close(sockfd_);
}
void TcpConnection::AttachToLoop(){
    status_=connected_;
    //printf("%s%d\n","add to loop :",event_.GetFd());
    event_.AddToLoop();
    connect_cb_(shared_from_this());
}

void TcpConnection::Shutdown(){
    if(status_==connected_){
        status_=disconnecting_;
    }
    loop_->RunInLoop([this](){ShutdownWrite();});
}

void TcpConnection::ShutdownWrite(){
    shutdown(sockfd_,SHUT_WR);
}
void TcpConnection::HandleRead(){
    std::string recv_str;
    int read_size_=ReadFromFd(recv_str);
    //std::cout<<"received :"<<recv_str<<std::endl;
    if(read_size_>0)
        message_back_(shared_from_this(),std::move(recv_str));//调用注册的回调函数，可以自定义
    else if(read_size_==0){
        HandleClose();
    }
    else HandleError();
}

void TcpConnection::HandleError(){
    HandleClose();
}
int TcpConnection::ReadFromFd(std::string& tmp){
    char buf[4096];
    int n=::read(sockfd_,buf,sizeof buf);
    tmp=std::move(std::string(buf));
    //std::string tmp(buf);
    printf("%s%d%s%d\n","received length :",n,"from :",sockfd_);
    return n;
}
void TcpConnection::HandleWrite(){

}

void TcpConnection::Send(std::string& str){
    if(status_!=connected_){
        return ;
    }
    if(loop_->IsInthisThread()){
        SendInLoop(str);//判断是否在当前线程
        return ;    
    }
    else{
        loop_->RunInLoop(std::bind(&TcpConnection::SendInLoop,shared_from_this(),str));
        return ;
    }
}

void TcpConnection::SendInLoop(std::string &str){
    int n=::write(sockfd_,str.c_str(),str.length());
    //printf("%s%d\n","Send to",sockfd_);
}


void TcpConnection::HandleClose(){
    event_.DisableAll();
    status_=disconnected_;
    std::shared_ptr<TcpConnection>conn(shared_from_this());
    if(connect_cb_){
        connect_cb_(conn);
    }
    close_cb_(conn);
    //printf("%s\n","closed");
    printf("%s%d\n","closed connection :",sockfd_);
}
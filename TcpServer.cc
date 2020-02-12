#include "TcpServer.h"
#include "TcpConnection.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <memory.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <iostream>

TcpServer::TcpServer(EventLoop* loop,int thread_num,std::string addr,int port)
    :base_loop(loop)
    ,pool_(new EventLoopThreadPool(base_loop,thread_num))
    ,event_(loop)
    ,addr_str(addr)
    ,port_(port){
           
}

TcpServer::~TcpServer(){
    
}

void TcpServer::Run(){
    pool_->Start();//线程池开启
    Init();
}
int TcpServer::Init(){
    SetConnect([this](int fd){HandleNewConnection(fd);});
    struct sockaddr_in address;
    
    listenfd_ = socket(AF_INET,SOCK_STREAM,0);
    SetReuseAddr(listenfd_);
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port_);
    address.sin_addr.s_addr = inet_addr(addr_str.c_str());
    int ret;
    ret = bind(listenfd_, (struct sockaddr*)&address, sizeof(address));
    if(ret<0){
        printf("%s\n","bind error");
    }
    int sockfd_ = listen(listenfd_,1024);
    if(sockfd_<0){
        printf("%s\n","listen error");
    }
    //printf("%d%s\n",listenfd_,"listenfd is created");
    event_.SetFd(listenfd_);
    event_.SetRead([this](){HandleAccept();});
    base_loop->RunInLoop([this](){event_.EnableRead();});
}


void TcpServer::HandleAccept(){
    sockaddr_in addr;
	socklen_t len = sizeof(addr);
    connfd_=accept(listenfd_,(struct sockaddr*)&addr,&len);
    //printf("%d%s\n",connfd_," : connected");
    SetNoBlocking(connfd_);
    conn_cb_(connfd_);//建立新的连接后，分配到work loop
}
void TcpServer::HandleNewConnection(int fd){
    EventLoop* loop=pool_->GetNextLoop();

    //SetKeepAlive(connfd_,true);
    auto tcp_conn {std::make_shared<TcpConnection>(loop,connfd_)};
    //printf("%s\n","new tcp conn");
    tcp_conn->SetMessageBack(message_back_);
    tcp_conn->SetCloseEvent([this](const std::shared_ptr<TcpConnection> conn){CloseEvent(conn);});
    tcp_conn->SetConnectionCb(connect_cb_);
    loop->RunInLoop([tcp_conn](){tcp_conn->AttachToLoop();});
    connections_[tcp_conn->Sockfd()]=tcp_conn;
}

void TcpServer::CloseEvent(const std::shared_ptr<TcpConnection> conn){
    base_loop->RunInLoop([this,conn]{CloseEventInLoop(conn);});
}

void TcpServer::CloseEventInLoop(const std::shared_ptr<TcpConnection> conn){
    connections_.erase(conn->Sockfd());
}
void TcpServer::SetReuseAddr(int sockfd){
    int reuse=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
}

void TcpServer::SetTcpNodelay(int sockfd){
    int nodelay=1;
    setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,&nodelay,sizeof(nodelay));
}

void TcpServer::SetNoBlocking(int sockfd){
    fcntl(sockfd,F_SETFL,fcntl(sockfd, F_GETFL,0)|O_NONBLOCK);
}
void TcpServer::SetKeepAlive(int fd, bool on){
    int optval = on ? 1 : 0;
    int rc = ::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
                            reinterpret_cast<const char *>(&optval), static_cast<socklen_t>(sizeof optval));
    if (rc != 0) {
        int serrno = errno;
    }
}
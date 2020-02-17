#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../TcpConnection.h"
#include <iostream>
#include <string>

void OnMessage(const std::shared_ptr<TcpConnection>conn,std::string&& recv_str){
    std::string str = "HTTP/1.1 200 OK\r\n"
                 "Server: net server\r\n"
                 "Content-Length: 100\r\n"
                 "Content-Type: text/html\r\n\r\n<html>net server echo</html>";
    str.resize(214);
    conn->Send(str);
    //std::cout<<"send !"<<std::endl;
}

void OnConnection(const std::shared_ptr<TcpConnection>conn){
    //printf("%s\n","new conn");
    std::cout<<"new conn :"<<conn->Sockfd()<<std::endl;
    return ;
}
int main(){
    EventLoop loop;
    TcpServer server(&loop,1,"127.0.0.1",8888);
    server.SetConnectionCb(OnConnection);
    server.SetMessage(OnMessage);
    server.Run();
    loop.Run();
}
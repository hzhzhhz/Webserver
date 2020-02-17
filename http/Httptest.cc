#include "HttpServer.h"
//#include "HttpResponse.h"
#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../TcpConnection.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/time.h>
using namespace std;


std::string GetTime()
{
    struct timeval current_time;
    char time_buff[64];
    gettimeofday(&current_time,NULL);
    strftime(time_buff,sizeof(time_buff)-1,"%Y-%m-%d %H:%M:%S",localtime(&current_time.tv_sec));
    string time_str=time_buff;
    return time_str;
}
void OnMessage(const HttpRequest* req,HttpResponse* resp)
{
    //std::cout<<"header :"<<req->Method()<<endl<<"path :"<<req->Path()<<endl;
    const std::map<std::string,std::string>&headers=req->Headers();
    for(auto &it:headers)
    {
       // cout<<it.first<<" : "<<it.second<<endl;
    }
    if(req->Path()=="/")
    {
        resp->SetStatus(HttpResponse::Ok200);
        resp->SetMessageStatus("OK");
        resp->SetContentType("text/html");
        resp->AddHeader("Webserver","zheng");
        std::string time_now=GetTime();
        resp->SetBody("<html><head><title>This is title</title></head>"
            "<body><h1>Hello</h1>Now is " + time_now +
            "</body></html>");
        //resp->SetBody(favicon);
    }
}

int main()
{
    EventLoop loop;
    HttpServer server_(&loop,0,"127.0.0.1",8000);
    server_.SetHttpCb(OnMessage);
    server_.Start();
    //server_.Start();
    loop.Run();
}
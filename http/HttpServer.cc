#include "HttpServer.h"
//#include "HttpResponse.h"
#include "HttpRequest.h"
HttpServer::HttpServer(EventLoop* loop,int thread_num,std::string addr,int port)
    :server_(loop,thread_num,addr,port)
{
    server_.SetConnectionCb([this](const TcpConnPtr conn){OnConnection(conn);});
    server_.SetMessage([this](const TcpConnPtr conn,std::string&& str){OnMessage(conn,std::move(str));});
}

HttpServer::~HttpServer()
{

}

void HttpServer::OnMessage(const TcpConnPtr conn,std::string&& str)
{
    
    HttpContext context_(std::move(str));
    context_.Parse();//解析报文
    HttpRequest* request_=context_.GetRequest();
    std::string connection=request_->FindHeader("Connection");
    bool is_close=connection=="close";//检验是否是长连接
    HttpResponse response(is_close);
    http_cb_(request_,&response);
    std::string send_str;
    response.AddToStr(send_str);
    conn->Send(send_str);
    if(response.CloseConnection())
    {
        conn->Shutdown();
    }
}

void HttpServer::OnConnection(const TcpConnPtr conn)
{

}


void HttpServer::Start(){
    server_.Run();
}


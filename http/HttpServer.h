#ifndef _http_server_h_
#define _http_server_h_

#include "../TcpServer.h"
#include "../TcpConnection.h"
#include "HttpContext.h"
#include "HttpResponse.h"
#include <string>
#include <vector>

using HttpCallback=std::function<void(const HttpRequest*, HttpResponse*)>;
class HttpServer{
public:
    
    HttpServer(EventLoop*,int thread_num,std::string addr,int port);
    HttpServer(const HttpServer&)=delete;
    HttpServer& operator=(const HttpServer&)=delete;
    ~HttpServer();
    void Start();
    void SetHttpCb(HttpCallback&& httpcb){http_cb_=httpcb;}//用户逻辑的回调函数
private:
    
    void OnConnection(const TcpConnPtr conn);
    void OnMessage(const TcpConnPtr conn,std::string&& str);
    TcpServer server_;
    HttpCallback http_cb_;
};
#endif
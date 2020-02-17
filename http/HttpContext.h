//解析报文类
#ifndef _http_context_h_
#define _http_context_h_

#include <string>
#include <vector>
#include <map>
#include "HttpRequest.h"

class HttpContext{
public:
    HttpContext(std::string&& context);
    HttpContext(const HttpContext&)=delete;
    HttpContext& operator==(const HttpContext&)=delete;
    ~HttpContext();

    void Parse();
    HttpRequest* GetRequest(){return &request_;}
private:
    void ParseFirstLine();//解析第一行，请求方法，url
    void ParseLine();//解析所有行
    void ParseHeaders();//解析头部其它部分
private:
    std::vector<std::string>str_lines;//按行保存
    std::string context_;//报文
    HttpRequest request_;
};

#endif
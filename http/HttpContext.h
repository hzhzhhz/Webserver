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
    HttpContext(HttpContext &&)=delete;
    ~HttpContext();

    void Parse();
    HttpRequest* GetRequest(){return &request_;}
private:
    void ParseFirstLine();
    void ParseLine();
    void ParseHeaders();
private:
    std::vector<std::string>str_lines;
    std::string context_;
    HttpRequest request_;
};

#endif
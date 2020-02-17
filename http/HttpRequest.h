//负责解析请求的类
#ifndef _http_request_h_
#define _http_request_h_

#include <string>
#include <vector>
#include <map>
class HttpRequest{

public:
    enum METHOD{
        Invalid,
        Get,
        Put
    };
    enum VERSION{
        Unknow,
        Http10,
        Http11,
    };
    HttpRequest()
    :method_(Invalid)
    ,version_(Unknow)
    {

    }
    ~HttpRequest()
    {

    }
    void SetMethod(const std::string& str){
        if(str=="GET"){
            method_=Get;
            return ;
        }
        else if(str=="PUT"){
            method_=Put;
            return ;
        }
        else method_=Invalid;
    }
    void SetVersion(VERSION v){
        version_==v;
    }
    void SetUri(std::string uri){uri_=uri;}
    void SetAccept(std::string&& accept){accept_=std::move(accept);}
    std::string FindHeader(const std::string& key)
    {
        std::string result;
        auto it=headers.find(key);
        if(it!=headers.end())
        {
            result=it->second;
        }
        else
        {
            result="close";
        }
        return result;
    }
    std::string Path()const{return uri_;}
    std::string Method()const
    {
        switch (method_)
        {
            case Get:return "GET";
            case Put:return "PUT";
            default:return "Invalid";
        }
    }
    void AddHeader(std::string key,std::string value)
    {
        headers[key]=value;
    }
    const std::map<std::string,std::string>& Headers()const{return headers;}
private:
    METHOD method_;
    std::string uri_;
    VERSION version_;
    //std::string header_;
    std::map<std::string,std::string>headers;
    std::string accept_;
};
#endif
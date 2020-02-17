//回复报文
#ifndef _hhtp_response_h_
#define _http_response_h_
#include <string>
#include <map>
class HttpResponse{
public:
    enum Status{
        Unknow,
        Ok200=200,
        BadRequset400=400,
        NotFound404=404
    };
    HttpResponse(bool close):close_connection(close){}
    HttpResponse(const HttpResponse&)=delete;
    HttpResponse& operator=(const HttpResponse&)=delete;
    ~HttpResponse(){}
    void AddToStr(std::string& send_str)
    {
        char buf[32];
        snprintf(buf,sizeof buf,"HTTP/1.1 %d",status_);
        send_str.append(buf);
        send_str.append(message_status_);
        send_str.append("\r\n");
        if(CloseConnection())
        {
            send_str.append("Connection:Closed\r\n");
        }
        else
        {
            snprintf(buf,sizeof buf,"Content-length: %d\r\n",static_cast<int>(body_.length()));
            send_str.append(buf);
            send_str.append("Connection:Keep-Alive\r\n");
        }
        for(auto it:headers)
        {
            send_str.append(it.first);
            send_str.append(": ");
            send_str.append(it.second);
            send_str.append("\r\n");
        }
        send_str.append("\r\n");
        send_str.append(body_);
    }
    void SetStatus(const Status sta){status_=sta;}
    void SetBody(const std::string& str){body_=str;}
    void SetContentType(const std::string& type){AddHeader("Content-type",type);}
    void AddHeader(const std::string& key,const std::string& value )
    {
        headers[key]=value;
    }
    //void SetHeader(const std::string& header){header_=header;}
    void SetMessageStatus(const std::string& str){message_status_=str;}
    bool CloseConnection()const{return close_connection;}
private:
    std::map<std::string,std::string>headers;
    Status status_;
    bool close_connection;
    // std::string header_;
    // std::string content_type_;
    std::string body_;
    std::string message_status_;
};
#endif
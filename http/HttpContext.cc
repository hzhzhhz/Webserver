#include "HttpContext.h"
#include <algorithm>
#include <iostream>

HttpContext::HttpContext(std::string&& context)
:context_(context)
{

}

HttpContext::~HttpContext()
{

}

void HttpContext::Parse()
{
    ParseLine();
    ParseFirstLine();
    ParseHeaders();
}

void HttpContext::ParseLine()
{
    int left_=0;
    int right_=0;
    int str_size=context_.length();
    while(right_!=str_size)
    {
        if(context_[right_]=='\r'||context_[right_]==' ')
        {
            if(context_[right_+1]=='\n')
            {
                str_lines.push_back(std::string(context_,left_,right_-left_));
                right_+=2;
                left_=right_;
            }
            else
            {
                right_++;
            }
        }
        else
        {
            right_++;
        }
    }
}

void HttpContext::ParseFirstLine()
{
    const char* begin=str_lines[0].data();
    const char* end=str_lines[0].data()+str_lines[0].length();
    const char* index_=std::find(begin,end,' ');
    if(index_!=end)
    {
        request_.SetMethod(std::string(begin,index_));
        begin=index_+1;
        index_=std::find(begin,end,' ');
        if(index_!=end)
        {
            request_.SetUri(std::string(begin,index_));
            begin=index_+1;
            bool is_succ=end-begin==8&&std::equal(begin,end-1,"Http1.");
            if(is_succ)
            {
                if(*(end-1)=='1')
                {
                    request_.SetVersion(HttpRequest::VERSION::Http11);
                }
                else if(*(end-1)=='0')
                {
                    request_.SetVersion(HttpRequest::VERSION::Http10);
                }
            }
        }
    }
}
void HttpContext::ParseHeaders()
{
    for(auto &it:str_lines)
    {
        const char* begin=it.data();
        const char* end=it.data()+it.length();
        const char* index=std::find(begin,end,':');
        if(index!=end)
        {
            request_.AddHeader(std::string(begin,index),std::string(index+2,end));
        }
    }
}


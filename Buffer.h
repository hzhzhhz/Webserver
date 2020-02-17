#ifndef _buffer_h_
#define _buffer_h_

#include <vector>
#include <string.h>
#include <string>

class Buffer{
public:
    Buffer();
    Buffer(const Buffer&)=delete;
    Buffer& operator=(const Buffer&)=delete;
    ~Buffer();
    ssize_t ReadFd(int fd);
    void WriteFd(int fd);
    size_t LeftWriteSize()const{return buffer_.size()-write_index_;}
    char* WritePtr(){return buffer_.data()+write_index_;}
    std::string GetBuffer();
    void Reset(){read_index_=0;write_index_=0;}
private:
    size_t ReadableBytes()const{return write_index_-read_index_;}
    void HasWrite(size_t len){write_index_+=len;}
    void Append(const char* extra,size_t len);
    void EnoughSize(size_t len);//确保buffer_容量足够
private:
    static constexpr size_t Init_size=1024;
    std::vector<char>buffer_;
    size_t read_index_;//可读index
    size_t write_index_;//可写index，write_index-read_index为当前数据段
};
#endif
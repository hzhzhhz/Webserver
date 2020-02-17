#include "Buffer.h"
#include <sys/uio.h>


Buffer::Buffer()
:buffer_(Init_size)
,read_index_(0)
,write_index_(0)
{

}
Buffer::~Buffer()
{
    buffer_.clear();
}

ssize_t Buffer::ReadFd(int fd)
{
    char extra_buff[65536];
    struct iovec vec[2];
    const size_t write_size=LeftWriteSize();

    vec[0].iov_base=WritePtr();
    vec[0].iov_len=write_size;
    vec[1].iov_base=extra_buff;
    vec[1].iov_len=sizeof extra_buff;

    const int count=write_size<sizeof extra_buff?2:1;
    ssize_t len=readv(fd,vec,count);//散布读，效率高
    if(len<0)
    {
        //return -1;
    }
    else if(static_cast<size_t>(len)<=write_size)
    {
        HasWrite(static_cast<size_t>(len));
    }
    else
    {
        HasWrite(write_size);
        Append(extra_buff,static_cast<size_t>(len)-write_size);
    }
    return len;
}

void Buffer::Append(const char*data,size_t len)
{
    EnoughSize(len);
    std::copy(data,data+len,buffer_.begin()+write_index_);
}

void Buffer::EnoughSize(size_t len)
{
    if(buffer_.size()+read_index_<len)
    {
        buffer_.resize(write_index_+len);
    }
    else
    {
        size_t read_able=ReadableBytes();
        std::copy(buffer_.begin()+read_index_,buffer_.begin()+write_index_,buffer_.begin());
        write_index_=read_index_+read_able;
        read_index_=0;
    }
    
}
std::string Buffer::GetBuffer()
{
    std::string result(buffer_.data()+read_index_,ReadableBytes());
    return result;
}
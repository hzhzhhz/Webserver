#ifndef _callback_h_
#define _callback_h_

#include <functional>
#include <memory>
#include <string>

class TcpConnection;

using TcpConnPtr=std::shared_ptr<TcpConnection>;
using MessageCallback=std::function<void(const TcpConnPtr,std::string &&)>;//写事件函数
using NewConnection=std::function<void(const TcpConnPtr)>;//建立新连接
using CloseCallback=std::function<void(const TcpConnPtr)>;//关闭一个连接
using EventCallback=std::function<void()>;
#endif
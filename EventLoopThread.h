#ifndef _eventloop_thread_h_
#define _eventloop_thread_h_
#include "EventLoop.h"
#include <thread>

class EventLoopThread{
public:
    EventLoopThread();
    EventLoopThread(const EventLoopThread&)=delete;
    EventLoopThread& operator=(const EventLoopThread&)=delete;
    ~EventLoopThread();
    void Run();
    void Stop();
    EventLoop* Loop();
private:
    
    void Task();
    std::thread* thread_=nullptr;
    EventLoop *loop=nullptr;
};
#endif
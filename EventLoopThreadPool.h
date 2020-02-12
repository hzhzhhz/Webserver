#ifndef _eventloop_threadpool_h_
#define _eventloop_threadpool_h

#include <thread>
#include "EventLoopThread.h"
#include <vector>
#include <memory>

class EventLoopThreadPool{
public:
    EventLoopThreadPool(EventLoop* loop,size_t nums);
    ~EventLoopThreadPool();
    void Start();
    EventLoop* GetNextLoop();
private:
    void Stop();
    size_t GetNext();
    EventLoop* base_loop_;
    std::vector<std::shared_ptr<EventLoopThread>>loops;
    size_t thread_nums;
    size_t num_count;
};
#endif
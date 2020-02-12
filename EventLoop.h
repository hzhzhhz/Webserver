#ifndef _eventloop_h_
#define _eventloop_h_

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <map>
#include <memory>
#include "Epoll.h"
#include "HandleEvent.h"


class EventLoop{
public:
    EventLoop();
    ~EventLoop();
    void RunInLoop(EventCallback &&);
    void QueueInLoop(EventCallback &&);
    void Run();
    void StopLoop();
    void AddEvent(HandleEvent*);
    void ModifyEvent(HandleEvent*);
    void RemoveEvent(HandleEvent*);
    //判断是否在当前线程
    bool IsInthisThread(){
        return std::this_thread::get_id()==this_thread_id;
    }
    void ResetThreadId(){
        this_thread_id=std::this_thread::get_id();
    }
private:
    void DoPendingFun();
    bool call_pendingfuncs_;
    int wakeup_fd_;
    std::unique_ptr<HandleEvent> wakeup_event_;//每个loop维护一个唤醒event
    void Wakeup();//唤醒
    void HandleRead();//读取eventfd写入的uint64_t
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    bool is_running_;
    std::unique_ptr<Epoll> epoll_;//每个loop底层均由一个Epoll
    std::thread::id this_thread_id;//线程id
    //std::vector<int>fds;
    std::vector<EventCallback>pending_funcs;//待处理的事件
    std::vector<epoll_event>events_;
};

#endif
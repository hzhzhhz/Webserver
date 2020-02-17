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
    EventLoop(const EventLoop&)=delete;
    EventLoop& operator=(const EventLoop&)=delete;
    ~EventLoop();
    void RunInLoop(EventCallback &&);
    void QueueInLoop(EventCallback &&);
    void Run();
    void StopLoop();
    void AddEvent(HandleEvent*);//添加事件，调用epoll
    void ModifyEvent(HandleEvent*);
    void RemoveEvent(HandleEvent*);
    bool IsInthisThread(){
        return std::this_thread::get_id()==this_thread_id;
    }
    void ResetThreadId(){
        this_thread_id=std::this_thread::get_id();
    }
private:
    void DoPendingFun();
    bool call_pendingfuncs_;
    int wakeup_fd_;//eventfd
    std::unique_ptr<HandleEvent> wakeup_event_;
    void Wakeup();//eventfd激活写
    void HandleRead();
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    bool is_running_;
    std::unique_ptr<Epoll> epoll_;//每个eventloop均维护一个epoll
    std::thread::id this_thread_id;//本loop线程id
    //std::vector<int>fds;
    std::vector<EventCallback>pending_funcs;//待处理的事件
    std::vector<epoll_event>events_;//激活的事件
};

#endif
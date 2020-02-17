#ifndef _handle_event_h_
#define _handle_event_h_

#include <memory>
#include <functional>
#include <sys/epoll.h>
#include "Callback.h"

class TcpConnection;

//using Callback=std::function<void(int)>;

class EventLoop;

/*每个HandleEvent持有一个fd，更新fd的状态和执行事件，但是不控制fd的生死*/
class HandleEvent{
public:
    
    explicit HandleEvent(EventLoop* ,int fd=-1,bool readable=true);
    HandleEvent(const HandleEvent&)=delete;
    HandleEvent& operator=(const HandleEvent&)=delete;

    ~HandleEvent();
    void SetFd(int fd){fd_=fd;}
    void SetRead(EventCallback &&event);
    void EnableRead();
    void SetWrite(EventCallback &&event);
    void EnableWrite();
    void DisableAll();
    int  GetFd();
    void Handle();
    void Update();
    void AddToLoop();
    void RemoveToLoop();
    uint32_t Event(){return event_status;}
private:
    bool IsReadable()const{
        return (event_status & ReadStatus)!=0;
    }
    bool IsWriteable()const{
        return (event_status & WriteStatus)!=0;
    }
    bool IsNone()const{
        return event_status==NoneEventStatus;
    }
private:
    static constexpr uint32_t NoneEventStatus=0;
    static constexpr uint32_t ReadStatus=EPOLLIN | EPOLLPRI;
    static constexpr uint32_t WriteStatus=EPOLLOUT;
private:
    bool is_add_to_loop;
    EventLoop* loop_;
    uint32_t event_status;//fd所处状态，read write none
    int fd_;//每个Handleevent一个
    EventCallback ReadEvent;
    EventCallback WriteEvent;
};

#endif
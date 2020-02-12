#include "EventLoop.h"
#include <sys/eventfd.h>
#include <iostream>


int CreateEvent(){
    int fd=eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
    return fd;
}
EventLoop::EventLoop()
    :epoll_(new Epoll)
    ,is_running_(false)
    ,this_thread_id(std::this_thread::get_id())
    ,wakeup_fd_(CreateEvent())
    ,wakeup_event_(new HandleEvent(this))
    ,call_pendingfuncs_(false)
{
    std::cout<<"create loop :"<<std::this_thread::get_id()<<std::endl;
    //wakeup_fd_=eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(wakeup_fd_<0){
        printf("%s\n","eventfd create failed!");
    }
    //printf("%s%d\n","wakeup fd ;",wakeup_fd_);
    //注册唤醒事件
    wakeup_event_->SetFd(wakeup_fd_);
    wakeup_event_->SetRead([this]{HandleRead();});
    wakeup_event_->EnableRead();

}
EventLoop::~EventLoop(){

}

void EventLoop::Run(){
    is_running_=true;
    while(is_running_){
        //std::cout<<"this thread ::"<<this_thread_id<<std::endl;
        std::vector<epoll_event>tmp_fds;
        epoll_->EpollWait(-1,events_);
        tmp_fds.swap(events_);
        
        for(auto it:tmp_fds){
            reinterpret_cast<HandleEvent *>(it.data.ptr)->Handle();
        }
        DoPendingFun();
    }
}

void EventLoop::DoPendingFun(){
    std::vector<EventCallback>tmp;
    call_pendingfuncs_=true;{
        std::lock_guard<std::mutex> lock(mutex_);
        
        tmp.swap(pending_funcs);
    }
    for(auto &it:tmp){
        it();
    }
    call_pendingfuncs_=false;
}
void EventLoop::AddEvent(HandleEvent* event){
    //printf("%s%d\n","add a readfd :",event->GetFd());
    epoll_->Add(event);
}

void EventLoop::ModifyEvent(HandleEvent* event){
    //printf("%s%d\n","modify a fd :",event->GetFd());
    int operation=event->Event();
    epoll_->Update(operation,event);
}
void EventLoop::RemoveEvent(HandleEvent* event){
    epoll_->Remove(event);
}

//判断任务是否在当前线程，如果是直接执行，如果不是，唤醒当前线程
void EventLoop::RunInLoop(EventCallback &&event){
    if(IsInthisThread()){
        //printf("%s\n","in this thread");
        event();
    }
    else {
        printf("%s\n","is not in this thread");
        QueueInLoop(std::move(event));
    }
}

void EventLoop::QueueInLoop(EventCallback &&event){
    {
        std::lock_guard<std::mutex>lock(mutex_);
        pending_funcs.push_back(event);
    }
    if(!IsInthisThread()||call_pendingfuncs_) Wakeup();
    //printf("%s\n","wake up");
}
//由于wakeup事件已经注册，因此eventfd写入时，底层epoll会监测到，停止阻塞状态
void EventLoop::Wakeup(){
    uint64_t a=1;
    int size=write(wakeup_fd_,&a,sizeof a);
    if(size<0){
        printf("%s\n","eventfd write fail");
    }
}
void EventLoop::StopLoop(){
    is_running_=false;
}

void EventLoop::HandleRead(){
  uint64_t one = 1;
  ssize_t n = ::read(wakeup_fd_, &one, sizeof one);
  if (n != sizeof one){
      printf("%s\n","read eventfd failed");
  }
  printf("%s\n","waked up");
}
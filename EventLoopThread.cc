#include "EventLoopThread.h"
#include <iostream>

EventLoopThread::EventLoopThread():loop(new EventLoop){

}
EventLoopThread::~EventLoopThread(){
    if(loop!=nullptr){
        delete loop;
        loop=nullptr;
    }
    if(thread_!=nullptr||thread_->joinable()){
        thread_->join();
        delete thread_;
        thread_=nullptr;
    }
}

void EventLoopThread::Run(){
    thread_=new std::thread(&EventLoopThread::Task,this);
}

void EventLoopThread::Task(){
    //std::cout<<"this thread :"<<std::this_thread::get_id()<<std::endl;    
    loop->ResetThreadId();
    loop->Run();
}
void EventLoopThread::Stop(){
    loop->StopLoop();
}

EventLoop* EventLoopThread::Loop(){
    return loop;
}
#include "HandleEvent.h"
#include "EventLoop.h"


HandleEvent::HandleEvent(EventLoop* loop,int fd,bool readable)
:loop_(loop)
,fd_(fd)
,is_add_to_loop(false)
,event_status(readable?ReadStatus:NoneEventStatus)
{

}
HandleEvent::~HandleEvent(){

}
void HandleEvent::SetRead(EventCallback &&event){
    ReadEvent=std::move(event);
}
void HandleEvent::EnableRead(){
    if(!IsReadable()){
        event_status |= ReadStatus;
    }
    Update();
}

void HandleEvent::SetWrite(EventCallback &&event){
    WriteEvent=std::move(event);
}

//fd读取完后更新
void HandleEvent::EnableWrite(){
    if(!IsWriteable()){
        event_status |= WriteStatus;
    }
    Update();
}
void HandleEvent::Handle(){
    if(IsReadable() && ReadEvent){
        ReadEvent();//执行读事件
    }
    else if(IsWriteable() && WriteEvent){
        WriteEvent();//执行写事件
    }
}

int HandleEvent::GetFd(){
    return fd_;
}

void HandleEvent::DisableAll(){
    if(IsReadable()||IsWriteable()){
        event_status==NoneEventStatus;
    }
    RemoveToLoop();
}
void HandleEvent::AddToLoop(){

    if(is_add_to_loop){
        loop_->ModifyEvent(this);
    }
    else 
        loop_->AddEvent(this);
        is_add_to_loop=true;
}

void HandleEvent::RemoveToLoop(){
    if(is_add_to_loop){
        loop_->RemoveEvent(this);
        is_add_to_loop=false;
    } 
}

void HandleEvent::Update(){
    if(IsNone()){
        RemoveToLoop();
    }
    else AddToLoop();
}

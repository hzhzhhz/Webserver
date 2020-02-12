#include "Epoll.h"
#include <stdio.h>
#include <memory.h>

Epoll::Epoll():MAX_SIZE(100){
    epollfd=epoll_create(MAX_SIZE);
    if(epollfd<0)
    {
        printf("%s","epoll cteate failed!");
    }
}
Epoll::~Epoll(){

}

void Epoll::Add(HandleEvent* handle){
    struct epoll_event event;
    memset(&event,0,sizeof event);
    event.data.ptr=handle;
    event.events=handle->Event();
    //event.data.fd=handle->GetFd();//ptr会改变？
    int fd=handle->GetFd();
    //printf("%s%d\n","getfd :",fd);
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
}
void Epoll::Update(int operation,HandleEvent* handle){
    struct epoll_event event;
    memset(&event,0,sizeof event);
    event.data.ptr=handle;
    event.events=handle->Event();
    //event.data.fd=handle->GetFd();//ptr会改变？
    int fd=handle->GetFd();
    //printf("%s%d\n","getfd :",fd);
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}
void Epoll::Remove(HandleEvent* handle){
    struct epoll_event event;
    memset(&event,0,sizeof event);
    int fd=handle->GetFd();
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&event);
    printf("%s%d\n","remove fd :",handle->GetFd());
}

void Epoll::EpollWait(int timewaits,std::vector<epoll_event>&events){
    events.resize(MAX_SIZE);
    int nfds=epoll_wait(epollfd,events.data(),MAX_SIZE,timewaits);
    //printf("%s%d\n","epollfd :",epollfd);
    //printf("%s%d\n","nfds :",nfds);
    if(nfds<0){
        if(errno!=EINTR)
            return ;
    }
    if(nfds==0){
        printf("%s","no events happend");
    }
    if(nfds>0){
        events.resize(nfds);
    }
}
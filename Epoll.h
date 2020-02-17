#ifndef _epoll_h_
#define _epoll_h_

#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

#include "HandleEvent.h"
class Epoll{
public:
    Epoll();
    Epoll(const Epoll&)=delete;
    Epoll& operator=(const Epoll&)=delete;
    ~Epoll();
    void EpollWait(int timewaits,std::vector<epoll_event>& events);
    void Add(HandleEvent* event);//添加fd
    void Update(int operation,HandleEvent* event);//更新状态
    void Remove(HandleEvent*);//移除fd
private:
    //struct epoll_event events[1000];
    int epollfd;
    int MAX_SIZE;
};

#endif
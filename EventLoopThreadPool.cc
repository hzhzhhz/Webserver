#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop,size_t nums)
    :base_loop_(loop)
    ,thread_nums(nums)
    ,num_count(0)
{
    loops.resize(thread_nums);
    for(int i=0;i<thread_nums;++i){
        std::shared_ptr<EventLoopThread>temp(new EventLoopThread);
        loops[i]=temp;
    }
}

EventLoopThreadPool::~EventLoopThreadPool(){
    Stop();
    loops.clear();
}

void EventLoopThreadPool::Start(){
    for(auto &it:loops){
        it->Run();
    }
}
void EventLoopThreadPool::Stop(){
    for(auto &it:loops){
        it->Stop();
    }
}


size_t EventLoopThreadPool::GetNext(){
    size_t t = num_count++;
    num_count %= thread_nums;
    return t;
}
EventLoop* EventLoopThreadPool::GetNextLoop()
{
     if(loops.empty()){
         return base_loop_;
     }
     else return loops[GetNext()]->Loop();
    //return loops[num_count++]->Loop();
}
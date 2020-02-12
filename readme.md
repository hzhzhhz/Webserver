# Webserver
## 简介
本项目是一个c++编写的多线程Web服务器，借鉴了[Muduo](https://github.com/chenshuo/muduo)的思想；使用了Reactor并发模型，非阻塞IO+线程池；支持http get、head请求。

## 特点

- 使用`Reactor`模式 + `EPOLL(ET)`边沿触发的IO多路复用技术，非阻塞IO。
- 参考了`muduo`的`one loop per pthread`思想，主线程和工作线程各自维持了一个事件循环（`eventloop`）。
- 使用了多线程充分利用多核CPU，并创建了线程池避免线程频繁创建、销毁的开销。
- 主线程只负责新的连接，以轮循的方式分发给其他IO线程。
- 使用unique_ptr,shared_ptr等智能指针控制对象生命周期，同时减小了内存泄漏的风险。
- 使用function+bind/function完成回调
- 线程之间的高效通信，使用了`eventfd`实现了线程的唤醒。
- 支持优雅关闭连接。
- 使用`Webbench` 对服务器进行压测。


## todo
- 实现一个buffer类缓存数据
- 一个高效的定时器
- 支持ssl加密

## 参考

Muduo https://github.com/chenshuo/muduo

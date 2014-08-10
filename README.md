#Echo 网络库


##开发环境

    1.Ubuntu 12.04 LTS
    2.G++ 4.6.4

##编程模式

    1.单线程Reactor模式（Non-Blocking IO + IO multipling）
    2.Reactor + ThreadPool （主要是把计算任务交给线程池，  
      IO线程只负责IO任务

##概要说明

    1.Echo借鉴了大量muduo的代码，并对其进行简化。
    2.Echo体现了现代C++的两大风格，一是事件回调，  
    我们使用function/bind实现回调机制，  
    用户只需要向其注册回调函数即可，  
    必要时可以封装成class，  
    二是采用智能指针进行资源管理，  
    例如TcpConnection使用了shared_ptr管理其生命周期，  
    其他类采用了unique_ptr借助其销毁功能
    3.Echo的不足之处：
        a) 对错误的处理比较粗糙
        b) 没有使用一个比较规范的日志 
    4.Echo与muduo的主要差距
        a)Echo没有实现复用同一个Poller的功能，  
        这正是muduo中的EventLoop，里面封装了一个通用的Poll、Epoll模型
        b)我们采用的是阻塞IO，  
        不能成为严格意义上的Reactor模式，  
        而且我们没有实现缓冲区。
        c)缺乏大量的实用技巧。

##缺点

    1.没有使用非阻塞IO，  
    如果网络拥塞，IO会阻塞在read函数上，  
    从而降低整个系统的并发能力
    2.没有提供缓冲区，不应该让用户执行read操作

##下一步的计划

    1.增加非阻塞IO
    2.实现一个高性能的缓冲区，用户只需往buffer中取数据

##源码分析：

    1.NonCopyable、Copyable表示对象是否具有value语义（复制和赋值），  
    Echo中除了InetAddress之外，其余均禁用掉了value语义，  
    这是为了避免潜在的BUG。
    2.Exception相比标准库的exception，  
    增加了打印栈痕迹的功能
    3.ThreadPool系列，  
    主要包含MutexLock、Condition、Thread、ThreadPool。  
    其中大量采用了RAII技术，避免资源的泄露，  
    对于Thread和ThreadPool，我们采用了function作为泛型技术，  
    用户只需注册回调函数。
    4.Timer，内部采用timerfd系列的定时器，  
    不使用信号，而是使用fd可读作为定时器的触发事件，  
    这使得Timer可以加入到IO复用模型，  
    我们采用的是Poll模型。  
    也可以单独把Timer放到一个线程，这就是TimerThread的产生。
    5.TcpServer系列：
        a)Rio封装了网络编程中的三大函数readn、readLine、writen
        b)Socket则封装了基本的socket操作，但是不包含读写。
        c)InetAddress包装了sockaddr_in结构体，使之更易使用。
        d)TcpConnection则包装了Tcp连接的各种操作，  
        主要是数据的收发以及获取Tcp连接的信息。  
        TcpConnection采用shared_ptr管理其生存期，  
        还继承了enable_shared_from_this，  
        用于在类的内部获取自己的智能指针。
        e)PollPoller封装了Poll模型，  
        内部存在一个 map<int, TcpConnectionPtr>实现从文件描述符到Tcp连接的映射。
        f)TcpServer则是对以上组件的组合。
    6.用户注册事件与回调流程：
        a)先注册给TcpServer，  
        然后是PollPoller，之后是TcpConnection，这样完成了事件的注册
        b)回调函数由PollPoller触发，  
        通过map寻找到Tcp连接，然后调用里面的回调函数。
    7.TcpServer实质是一个IO复用模型，ThreadPool则是代表多线程，  
    用户在使用时，可以只选择其一。  
    如果计算任务负担较重，  
    可以将计算任务与Tcp回发封装成函数，交给线程池去计算。
    8.此时，运行TcpServer的线程是一个IO线程， 
    ThreadPool里面的线程专注于CPU密集型计算。


##使用方法见example文件夹

    1.只使用TcpServer，适用于计算任务很轻的情况
    2.在代码中组合TcpServer和ThreadPool，  
    把计算任务（和Tcp回发）分发到线程池中，  
    适合于计算任务较重的情况。
    

##安装方法

    执行make，生成echo头文件夹和静态库libecho.a  
    将echo安装到/usr/include/下，将libecho.a放置/usr/lib/下  
    编译的时候需要加上-std=c++0x -lecho




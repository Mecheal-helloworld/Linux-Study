### 进程间通信：信号

```c
SIGABRT        进程停止运行    6
SIGALRM        警告钟    
SIGFPE        算述运算例外
SIGHUP        系统挂断
SIGILL        非法指令
SIGINT        终端中断   2
SIGKILL        停止进程（此信号不能被忽略或捕获）
SIGPIPE        向没有读的管道写入数据
SIGSEGV        无效内存段访问
SIGQOUT        终端退出    3
SIGTERM        终止
SIGUSR1        用户定义信号1
SIGUSR2        用户定义信号2
SIGCHLD        子进程已经停止或退出
SIGCONT        如果被停止则继续执行
SIGSTOP        停止执行
SIGTSTP        终端停止信号
SIGTOUT        后台进程请求进行写操作
SIGTTIN        后台进程请求进行读操作
```

```c
//函数原型
#include <signal.h>
void (*signal(int signo, void (*func)(int)))(int);
//使用形式
signal(sig,handler);
//SIG_IGN忽略信号函数
//SIG_DFL默认执行函数
```

#### 相关函数

```c
int kill(pid_t pid,int sig);//成功返回0，失败返回-1并设置errno
int raise(int sig);//自身
unsigned int alarm(unsigned int secs);//返回0或以前设置的秒数 
int pause(void);//将进程挂起，不再消耗CPU
```

#### 信号的捕获与处理

1、接受默认动作

2、彻底忽略信号的发生

3、接受用户自定义的默认动作

### 线程

```c
//函数原型
#include<pthread.h>
ptread_t pthread_self();
//判等
pthread_equal();
```


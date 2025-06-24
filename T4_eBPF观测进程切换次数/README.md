该ebpf程序能够观察OpenEuler中指定进程被调度的次数

如何验证这个程序：
在另一个终端输入 sleep 60 & 创建一个休眠60s的进程，会返回该进程的pid，再在原来的终端输入 sudo bpftrace ./switchcnt.bt pid 即可
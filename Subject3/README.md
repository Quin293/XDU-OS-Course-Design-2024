题目三： Runqueue长度观测实验（相关知识点参见1.9节）：根据给定的ebpf编程框架（桌面/develop/src/01-sched-lab/ebpf/rqlen.bt），填写相关代码，
使得该ebpf程序能够观察信创操作系统中CFS调度队列上runqueue的长度。

踩坑：仅因为这个而从头学起linux内核与ebpf就是徒劳，直接问chatgpt，查找关于这道题的相关知识看懂代码即可

思路：
  //基本语法 变量均以$开头
  //Bpftrace内置变量可直接访问，curtask是当前 task_struct。
	//Linux内核通过一个被称为进程描述符的task_struct结构体来管理进程
	//包含调度策略字段struct sched_entity se;
	//struct sched_entity 描述一个调度实体，它包含了一个实体所有运行时的参数
	//其中就包含struct cfs_rq 结构，通过该结构直接获取运行队列长度nr_running即可
  //最后在终端用命令 sudo bpftrace ./rqlen.bt 运行

参考：
bpftrace 入门(4) 变量 https://zhuanlan.zhihu.com/p/562726230
Linux进程描述符task_struct结构体详解 https://zhuanlan.zhihu.com/p/112970875
linux调度子系统3 - cfs_rq 与 sched_entity https://zhuanlan.zhihu.com/p/363785756

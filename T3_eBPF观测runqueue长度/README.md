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

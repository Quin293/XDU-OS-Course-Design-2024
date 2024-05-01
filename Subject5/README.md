题目五：

      RR调度算法定制实验（相关知识点1.11节）：
      
      利用给定的RR调度程序(桌面/develop/src/01-sched-lab/kernel/patch.c)，填写相关代码，对信创操作系统中的RR调度算法进行定制，使其能够完成如下功能：
      
      若进程现在的执行顺序是：
      
      【1】【2】【3】
      
      那么当第四个进程转为运行态后，调度依然必须完全保证此顺序，即必须保持为：
      
      【1】【2】【3】【4】
      
      也就是说：若当前执行队列是【2】【3】【1】，那么进程【4】不能按照原始的RR那样，插到【1】之后，而是依然要插在【3】之后。即此时runqueue在接纳进程【4】后，runqueue内容变为：【2】【3】【4】【1】，而不是【2】【3】【1】【4】
      
      要求：理解测试程序，并运行测试程序对自己定制后的调度算法进行验证。

下面是要修改的地方：共三处


      // 1. kernel/sched/sched.h
      struct rt_rq {
      
      struct sched_rt_entity *last_enq_se;  // 新加字段
      int                     last_enq_prio; // 新加字段
      
      }
      
      // 2. kernel/sched/rt.c
      void init_rt_rq()
      {
      /* ... */
      
      /* delimiter for bitsearch: */
      __set_bit(MAX_RT_PRIO, array->bitmap);
      // 该行以上保持不变
      
      // 学生TODO: 请添加代码1 }
      
      
      
      // 3. kernel/sched/rt.c
      static void __enqueue_rt_entity(struct sched_rt_entity *rt_se, unsigned int flags)
      {
      /* ... */
      
      if (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
      if (rt_se->on_list)
      __delist_rt_entity(rt_se, array);
      return;
      }
      // 该行以上保持不变
      
        // 学生TODO: 请添加代码2
      if (move_entity(flags)) {
      WARN_ON_ONCE(rt_se->on_list);
      if (flags & ENQUEUE_HEAD)
      list_add(&rt_se->run_list, queue);
      else {
      // 学生TODO: 请添加代码3 }
      
      __set_bit(rt_se_prio(rt_se), array->bitmap);
      rt_se->on_list = 1;
      }
      rt_se->on_rq = 1;
      
      }
      
这道题看起来很唬人，但其实真的对于小白非常不好做，笔者摸了两周，但是接下来将通过几个部分帮助大伙理解做出这道题。

第一部分：理解何为 实时调度实体 sched_rt_entity & 实时运行队列 rt_rq 

      参考 ：
      【Linux 内核】实时调度类 ② ( 实时调度实体 sched_rt_entity 源码分析 | run_list、timeout、watchdog_stamp、time_slice 字段 ) https://blog.csdn.net/shulianghan/article/details/123871539
      【Linux 内核】实时调度类 ④ ( 实时运行队列 rt_rq 源码分析 | 实时运行队列 rt_rq 结构体字段分析 | active、rt_nr_running、curr、next 字段 ) https://blog.csdn.net/shulianghan/article/details/123882680
      这两篇讲的比较详细，故不多赘述

第二部分：理解重要函数 __enqueue_rt_entity() 原代码

       __enqueue_rt_entity 是 Linux 内核中实时调度器（Real-Time Scheduler）的一部分，用于将一个实时任务（或称为调度实体）加入到调度队列中。
      
      这个函数主要用于 Round Robin (RR) 调度策略，确保实时任务能够被正确管理和调度。
      
      struct rt_rq *rt_rq = rt_rq_of_se(rt_se);
      
      这行代码获取与调度实体 rt_se 相关联的实时运行队列（real-time runqueue）的指针。
      struct rt_prio_array *array = &rt_rq->active;
      
      获取实时运行队列中用于存储活跃任务的优先级数组的指针。
      struct rt_rq *group_rq = group_rt_rq(rt_se);
      
      获取与调度实体相关联的组运行队列（如果存在的话）。这通常用于处理任务组的调度。
      struct list_head *queue = array->queue + rt_se_prio(rt_se);
      
      计算出应该将调度实体插入的队列的位置。这是基于调度实体的优先级来确定的。
      接下来的注释解释了不应该将一个组加入队列的情况：如果该组被限制（throttled）或者为空。
      
      if (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
      
      检查是否存在组运行队列，并且该队列是否被限制或没有运行的任务。
      if (rt_se->on_list)
      
      如果调度实体已经在某个列表上，需要先将其从当前列表中删除。
      __delist_rt_entity(rt_se, array);
      
      从其当前的列表中删除调度实体。
      return;
      
      如果组被限制或为空，函数提前返回，不再执行后续操作。
      if (move_entity(flags)) {
      
      检查是否需要移动调度实体（基于传入的标志）。
      WARN_ON_ONCE(rt_se->on_list);
      
      如果调度实体已经在列表上，发出警告。这通常表明存在逻辑错误。
      if (flags & ENQUEUE_HEAD)
      
      根据标志判断是将调度实体添加到列表头部还是尾部。
      list_add(&rt_se->run_list, queue);
      
      将调度实体添加到列表头部。
      list_add_tail(&rt_se->run_list, queue);
      
      将调度实体添加到列表尾部。
      __set_bit(rt_se_prio(rt_se), array->bitmap);
      
      在优先级位图中设置相应的位，表示有任务在该优先级上运行。
      rt_se->on_list = 1;
      
      标记调度实体已经被加入到列表中。
      rt_se->on_rq = 1;
      
      标记调度实体已经在运行队列上。
      inc_rt_tasks(rt_se, rt_rq);
      
      增加运行队列上的实时任务计数。
      整个函数的目的是将一个实时调度实体加入到正确的运行队列和优先级队列中，确保实时任务能够按照预期的优先级和顺序被调度。

第三部分：厘清具体实现思路

      

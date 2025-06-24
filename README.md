# OpenEuler内核高效监控与分析工具
项目概述
随着现代操作系统的复杂性不断增加，实时监控和优化系统性能变得至关重要。本项目旨在通过eBPF（Extended Berkeley Packet Filter）技术对OpenEuler操作系统内核进行高效监控与分析，提供一套完整的解决方案来提升系统性能、资源利用率以及稳定性。

# 项目目标
实时监控：开发一系列基于eBPF的工具，能够实时监控关键系统指标，如进程调度情况、内存布局变化等。
性能优化：通过对监控数据的分析，提出并实现优化方案，提高系统整体性能。
用户友好：确保工具易于安装和使用，适用于开发者和系统管理员。
# 技术栈
eBPF：用于内核态与用户态之间的高效交互，实现实时监控功能。
BPFTrace：简化了eBPF程序的编写和调试过程。
Linux Kernel：OpenEuler基于Linux内核，提供了丰富的接口和工具支持。
# 功能特性

## 1. 进程调度监控
实时跟踪指定进程的调度情况，统计调度次数。
提供详细的调度事件日志，便于后续分析。
`
// switchcnt.bt
BEGIN {
    printf("统计某进程的切换次数，按 Ctrl-C 显示结果\n\n");
}
kprobe:dequeue_task_fair
{
    if(pid == $target_pid){
        @count++;
    }
}
END {
    printf("\n进程总共被调用%d次\n", @count);
}
`
## 2. 内存布局分析
监控指定进程中代码段、数据段及栈段的起始位置和结束位置。
输出详细的内存分配信息，帮助开发者理解进程内存使用情况。
`
// mm.bt
$mm = $task->mm;
$start_code = $mm->start_code;
$end_code = $mm->end_code;
$start_data = $mm->start_data;
$end_data = $mm->end_data;
$start_stack = $mm->start_stack;
printf("Code Segment: Start = %lx, End = %lx\n", $start_code, $end_code);
printf("Data Segment: Start = %lx, End = %lx\n", $start_data, $end_data);
printf("Stack Segment: Start = %lx\n", $start_stack);
`
## 3. Runqueue长度监控
实时获取CFS调度队列上的runqueue长度变化。
提供动态图表展示runqueue长度随时间的变化趋势。
`
// rqlen.bt
$my_q = (struct cfs_rq *)curtask->se.cfs_rq;
$len = $my_q->nr_running;
printf("获取队列长度为：%u\n", $len);
`
#安装指南
环境准备
确保您的系统已安装以下依赖项：

BPFTrace
eBPF工具链
OpenEuler操作系统
编译与安装

根据具体需求编译所需的eBPF程序：
`sudo make`

运行工具
启动进程调度监控
`sudo bpftrace ./switchcnt.bt <pid>`

分析内存布局
`bpftrace ./mm.bt <pid>`

监控Runqueue长度
`sudo bpftrace ./rqlen.bt`

#结果展示
所有输出都将显示在终端中，并可根据需要重定向到文件中进行进一步分析。例如：


`sudo bpftrace ./switchcnt.bt 1234 > switch_log.txt`

使用semaphore，生成2个进程，这两个进程写同一个文件
        a.互斥写，即只有一个进程写完后，才能让另一个进程写; 
        b. 一个进程写入内容：“293PROC1 MYFILE1”
        另一个进程写入内容：“293PROC2 MYFILE2”
将该程序的semaphore替换成使用strict alternation算法的忙等待互斥锁实现


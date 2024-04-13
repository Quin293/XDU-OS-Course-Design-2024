题目二：使用semaphore，并利用该程序生成2个进程（注意：非线程），这两个进程写同一个文件
        要求：
        a.互斥写，即只有一个进程写完后，才能让另一个进程写; 
        b. 一个进程写入内容：“自己学号的后3位PROC1 MYFILE1”；
        另一个进程写入内容：“自己学号的后3位PROC2 MYFILE2”）
        将该程序的semaphore替换成使用strict alternation算法的忙等待互斥锁完成。

遇到的问题：
        1.运行文件时，终端提示 bash: ./: 是一个目录
        解决方法：编译c文件 gcc -o Shiyan2_semaphore Shiyan2_semaphore.c
        2.编译文件时，错误：unist.h: 没有那个文件或目录
        改为unistd.h
        3.错误：stderr拼错，sem_unlink拼错
        4.-bash: 未预期的符号 `(’ 附近有语法错误
        解决方法：./Shiyan2_semaphore



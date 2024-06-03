#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/shm.h>

int main() {
    int shmid;
    // 生成一个唯一的 key
    key_t key = ftok("shared_memory_key", 1);

    // 新建或获取共享内存段
    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // 将共享内存附加到进程
    int *turn = (int *)shmat(shmid, NULL, 0);
    if (turn == (int *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    // 初始化共享变量
    *turn = 0;

    FILE *fp = fopen("test.txt", "a");
    if (fp == NULL) {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // 父进程
        while (*turn != 0) {
            // 忙等待
        }

        fprintf(fp, "256 PROC1 MYFILE1\n");

        // 将 turn 设置为 1，允许子进程写入
        *turn = 1;

        // 等待子进程完成
        wait(NULL);

        // 删除共享内存段
        shmctl(shmid, IPC_RMID, NULL);
    } else {
        // 子进程
        while (*turn != 1) {
            // 忙等待
        }

        fprintf(fp, "256 PROC2 MYFILE2\n");

        // 将 turn 设置为 0，允许父进程写入
        *turn = 0;

    }

    // 关闭文件
    fclose(fp);

    // 分离共享内存
    shmdt(turn);
    
    return 0;
}

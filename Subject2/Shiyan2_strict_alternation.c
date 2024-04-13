#include <stdio.h>
#include <stdlib.h> //子进程退出会隐式调用exit()
#include <sys/types.h> //包含pid_t数据类型
#include <unistd.h> //包含fork()用于创建进程
#include <sys/wait.h> //包含wait()用于等待子进程结束

int main() {
    FILE *fp = fopen("file.txt", "a");
    fclose(fp);

    int turn = 0;

    pid_t pid = fork(); //create process1

    if(pid < 0){
		fprintf(stderr, "Fork Error");
		return 1;
	}else if (pid == 0){
        while (turn != 0){ //wait

        }
        FILE *fp = fopen("file.txt", "a");
        fprintf(fp, "293PROC1 MYFILE1");
        fclose(fp);
        turn = 1;
    }else{
        pid = fork(); //create child process2

		if(pid < 0){
                fprintf(stderr, "Fork Error");
                return 1;
       		}else if(pid == 0){
                while (turn != 0){ //wait

            }
            FILE *fp = fopen("file.txt", "a");
            fprintf(fp, "293PROC2 MYFILE2");
            fclose(fp);
            turn = 1;
            }
    }
    wait(NULL);
    wait(NULL);
    

    return 0;
}
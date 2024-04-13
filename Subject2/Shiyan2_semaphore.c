#include <stdio.h>
#include <stdlib.h> //子进程退出会隐式调用exit()
#include <semaphore.h> //创建操作信号量
#include <fcntl.h> //用于设置文件描述符的属性O_CREAT need
#include <sys/stat.h> //用于获取文件的状态信息 程序中使用0644设置信号量的权限
#include <sys/types.h> //包含pid_t数据类型
#include <unistd.h> //包含fork()用于创建进程
#include <sys/wait.h> //包含wait()用于等待子进程结束
 
int main(){
	sem_t *sem = sem_open("/semaphore", O_CREAT, 0664, 1); //创建初始值为1并且对其他用户只读的信号量
	

	pid_t pid = fork(); //create child process1
	
	if(pid < 0){
		fprintf(stderr, "Fork Error");
		return 1;
	}else if(pid == 0){ //child process1
		sem_wait(sem); //apply for resource
		FILE *fp = fopen("file.txt", "a");
		fprintf(fp, "293PROC1 MYFILE1");
		fclose(fp);
		sem_post(sem); //release semaphore
	}else{ //father process
		pid = fork(); //create child process2

		if(pid < 0){
                fprintf(stderr, "Fork Error");
                return 1;
		
       		}else if(pid == 0){ //child process2
                	sem_wait(sem); //apply for resource
                	FILE *fp = fopen("file.txt", "a");
                	fprintf(fp, "293PROC2 MYFILE2");
                	fclose(fp);
                	sem_post(sem); //release semaphore
        	}else{ //father process wait for child process's finishing
			wait(NULL);
			wait(NULL);
			sem_unlink("/semaphore"); //delete semaphore which don't need
		}
	}

	return 0;
}

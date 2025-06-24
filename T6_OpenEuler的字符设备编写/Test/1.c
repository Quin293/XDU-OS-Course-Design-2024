#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define RWBUF_CLEAR 0x909090
int main(){
	char yourmsg[1000];
	int a= open("/dev/rwbuf", O_RDWR);
	if(a<0){
		printf("打开失败\n");
		return 0;
	}
	int b= ioctl (a, RWBUF_CLEAR, 0);
	if(b<0){
		printf("clean失败 \n");
		return 0;
	}
	else
		printf("清除！\n");
	close(a);
}

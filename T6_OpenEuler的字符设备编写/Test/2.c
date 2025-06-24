#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(){
	char yourmsg[ 1000];
	int a= open("/dev/rwbuf", O_RDWR);
	if(a<0){
		printf("打开失败\n");
	}
	scanf("%s", yourmsg);
	int b= write (a, yourmsg, sizeof (yourmsg)+1);
	if(b<0){
		printf("写设备error\n");
		return 0;
	}
	else
		printf("写入 %s\n", yourmsg);
	close(a);
}

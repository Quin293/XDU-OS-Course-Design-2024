#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_NODE "/dev/rwbuf"

int main() {
    int fd;
    char buf[1024];

    // 打开设备节点
    fd = open(DEVICE_NODE, O_RDWR);
    if (fd < 0) {
        perror("无法打开设备节点");
        return 1;
    }

    // 读取最新内容
    printf("读取最新内容:\n");
    if (read(fd, buf, sizeof(buf)) < 0) {
        perror("读取失败");
    } else {
        printf("%s\n", buf);
    }

    // 关闭设备节点
    close(fd);

    // 重新打开设备节点
    fd = open(DEVICE_NODE, O_RDONLY);
    if (fd < 0) {
        perror("无法再次打开设备节点");
        return 1;
    }

    // 读取最近一次修改前的内容
    printf("\n读取最近一次修改前的内容:\n");
    if (read(fd, buf, sizeof(buf)) < 0) {
        perror("读取失败");
    } else {
        printf("%s\n", buf);
    }

    // 关闭设备节点
    close(fd);

    return 0;
}

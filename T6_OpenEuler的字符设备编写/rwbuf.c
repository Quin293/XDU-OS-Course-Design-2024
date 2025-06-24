#include <linux/kernel.h>        // 内核编程所需的头文件
#include <linux/module.h>        // 内核模块结构体
#include <linux/fs.h>            // 文件操作结构体
#include <linux/uaccess.h>       // 用户空间和内核空间数据交互函数
#include <linux/cdev.h>
#include <linux/device.h>

#define RWBUF_CLEAR 0x909090     // 清除缓冲区的命令
#define RW_CLEAR 123             // 清除缓冲区的标识
#define DEVICE_NAME "rwbuf"      // 设备名称
#define rwbuf_size 1024          // 缓冲区大小

static char rwbuf[rwbuf_size] = "22009200293";  // 缓冲区存储字符串
static int rwlen = 0;                           // 字符串长度
static int inuse = 0;                           // 设备占用标识
static char rwbuf_backup[rwbuf_size];// 定义全局变量用于备份最近一次修改前的rwbuf内容
static int rwlen_backup = 0;

static dev_t rwbuf_dev_number;
static struct class *rwbuf_class;
static struct cdev rwbuf_cdev;
// 设备打开函数
static int rwbuf_open(struct inode *inode, struct file *filep) {
    if (inuse == 1)  // 如果设备已被占用
        return -1;
    inuse = 1;
    try_module_get(THIS_MODULE);  // 增加模块引用计数
    return 0;
}

// 设备关闭函数
static int rwbuf_close(struct inode *inode, struct file *filep) {
    inuse = 0;
    module_put(THIS_MODULE);  // 减少模块引用计数
    return 0;
}

// 读取函数
static ssize_t rwbuf_read(struct file *filep, char *buf, size_t count, loff_t *ppos) {
    if (count > rwbuf_size || count < 0) {
        printk("错误的输入");
        return 0;
    }
	int flags = filep->f_flags;

    if (flags & O_RDWR) {
        // 读取最新内容
        copy_to_user(buf, rwbuf, rwlen);
        return rwlen;

    } else {
        // 读取最近一次修改前的内容
        copy_to_user(buf, rwbuf_backup, rwlen_backup);
        return rwlen_backup;
    }
}
// ioctl函数
static long rwbuf_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    if (cmd == RWBUF_CLEAR) {
        // 清除缓冲区内容
        rwlen = 0;
        rwbuf[0] = '\0';
        printk("内核中的rwbuf清零\n");
    } else if (cmd == RW_CLEAR) {
        // 恢复最近一次修改前的rwbuf内容
        memcpy(rwbuf, rwbuf_backup, rwlen_backup);
        rwlen = rwlen_backup;
        printk("恢复最近一次修改前的rwbuf内容\n");
    }
    return 0;
}

// 写入函数
static ssize_t rwbuf_write(struct file *filep, const char *buf, size_t count, loff_t *ppos) {
    // 备份当前rwbuf内容
    memcpy(rwbuf_backup, rwbuf, rwlen);
    rwlen_backup = rwlen;

    // 写入新数据到rwbuf
    copy_from_user(rwbuf, buf, count);
    rwlen = count;

    return count;
}

// 文件操作结构体
static struct file_operations rwbuf_fops = {
    .open = rwbuf_open,
    .release = rwbuf_close,
    .read = rwbuf_read,
    .write = rwbuf_write,
    .unlocked_ioctl = rwbuf_ioctl,
};

// 初始化函数
int init_rwbuf(void) {
    if (alloc_chrdev_region(&rwbuf_dev_number, 0, 1, DEVICE_NAME) < 0) {
        printk("无法分配设备号\n");
        return -1;
    }

    cdev_init(&rwbuf_cdev, &rwbuf_fops);
    if (cdev_add(&rwbuf_cdev, rwbuf_dev_number, 1) < 0) {
        printk("无法向系统注册设备\n");
        unregister_chrdev_region(rwbuf_dev_number, 1);
        return -1;
    }

    rwbuf_class = class_create(DEVICE_NAME);
    device_create(rwbuf_class, NULL, rwbuf_dev_number, NULL, DEVICE_NAME);

    printk("注册成功，设备节点为/dev/%s\n", DEVICE_NAME);
    return 0;
}

void cleanup_rwbuf(void) {
    device_destroy(rwbuf_class, rwbuf_dev_number);
    class_destroy(rwbuf_class);
    cdev_del(&rwbuf_cdev);
    unregister_chrdev_region(rwbuf_dev_number, 1);
    printk("再见\n");
}

MODULE_LICENSE("GPL");
module_init(init_rwbuf);
module_exit(cleanup_rwbuf); 
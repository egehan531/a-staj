#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static dev_t dev_num;
static struct cdev my_cdev;
static char *kernel_buffer;
static unsigned int buffer_size = 1024;
static int data_size = 0;

static int my_open(struct inode *inode, struct file *filp)
{
    pr_info("Device file açıldı.\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *filp)
{
    pr_info("Device file kapandı.\n");
    return 0;
}

static ssize_t my_read(struct file *filp, char __user *user_buf, size_t count, loff_t *offset)
{
    ssize_t bytes_read;

    if (*offset >= data_size)
        return 0;

    if (count > data_size - *offset)
        count = data_size - *offset;

    bytes_read = copy_to_user(user_buf, kernel_buffer + *offset, count);
    if (bytes_read != 0)
        return -EFAULT;

    *offset += count;
    return count;
}

static ssize_t my_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *offset)
{
    if (count > buffer_size)
        count = buffer_size;

    if (copy_from_user(kernel_buffer, user_buf, count))
        return -EFAULT;

    data_size = count;
    *offset = 0;

    return count;
}


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

static int __init my_init(void)
{
    int num;

    kernel_buffer = kmalloc(buffer_size, GFP_KERNEL);
    if (!kernel_buffer) {
        pr_err("Bellek ayırma hatası!\n");
        return -ENOMEM;
    }

    num = alloc_chrdev_region(&dev_num, 0, 1, "mydevice");
    if (num < 0) {
        pr_err("Device number atanamadı.\n");
        kfree(kernel_buffer);
        return num;
    }

    cdev_init(&my_cdev, &fops);

    num = cdev_add(&my_cdev, dev_num, 1);
    if (num < 0) {
        unregister_chrdev_region(dev_num, 1);
        kfree(kernel_buffer);
        pr_err("cdev eklenemedi\n");
        return num;
    }

    pr_info("Modül yüklendi. Major: %d, Minor: %d\n", MAJOR(dev_num), MINOR(dev_num));
    return 0;
}

static void __exit my_exit(void)
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    kfree(kernel_buffer);
    pr_info("Modül kaldırıldı ve bellek serbest bırakıldı.\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Egehan");
MODULE_DESCRIPTION("kmalloc ile read write (driver3)");
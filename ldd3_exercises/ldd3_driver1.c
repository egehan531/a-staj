#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static dev_t dev_num;
static struct cdev my_cdev;

static int my_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
};

static int __init my_init(void)
{
    int num;

    num = alloc_chrdev_region(&dev_num, 0, 1, "mydevice");
    if (num < 0) {
        pr_err("Device number atanamadı.\n");
        return num;
    }

    cdev_init(&my_cdev, &fops);

    num = cdev_add(&my_cdev, dev_num, 1);
    if (num < 0) {
        unregister_chrdev_region(dev_num, 1);
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
    pr_info("Modül kaldırıldı.\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Egehan");
MODULE_DESCRIPTION("First Driver (driver1)");

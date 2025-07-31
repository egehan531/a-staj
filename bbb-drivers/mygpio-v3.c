//BBB için ioctl Driver

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/ioctl.h>

#define GPIO_PIN 524

#define MAGIC_NUM 'G'
#define GPIO_SET_HIGH _IO(MAGIC_NUM, 0)
#define GPIO_SET_LOW _IO(MAGIC_NUM, 1)
#define GPIO_SET_OUTPUT _IO(MAGIC_NUM, 2)
#define GPIO_SET_INPUT _IO(MAGIC_NUM, 3)
#define GPIO_GET_DIRECTION _IOR(MAGIC_NUM, 4, int)

static dev_t dev_num;
static struct class *dev_class;
static struct cdev my_cdev;
static char *kernel_buffer;
static unsigned int buffer_size = 1024;

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

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
        case GPIO_SET_HIGH:
            gpio_set_value(GPIO_PIN, 1);
            pr_info("GPIO %d IOCTL ile HIGH yapıldı.\n", GPIO_PIN);
            break;

        case GPIO_SET_LOW:
            gpio_set_value(GPIO_PIN, 0);
            pr_info("GPIO %d IOCTL ile LOW yapıldı.\n", GPIO_PIN);
            break;

        case GPIO_SET_OUTPUT:
            if (gpio_direction_output(GPIO_PIN, 0) < 0) {
                pr_err("GPIO %d OUTPUT olarak ayarlanamadı.\n", GPIO_PIN);
                return -EINVAL;
            }
            pr_info("GPIO %d OUTPUT olarak ayarlandı.\n", GPIO_PIN);
            break;

        case GPIO_SET_INPUT:
            if (gpio_direction_input(GPIO_PIN) < 0) {
                pr_err("GPIO %d INPUT olarak ayarlanamadı.\n", GPIO_PIN);
                return -EINVAL;
            }
            pr_info("GPIO %d INPUT olarak ayarlandı.\n", GPIO_PIN);
            break;

        case GPIO_GET_DIRECTION: {
            struct gpio_desc *desc = gpio_to_desc(GPIO_PIN);
            int dir;

            if (!desc) {
                pr_err("GPIO %d için descriptor alınamadı.\n", GPIO_PIN);
                return -EINVAL;
            }

            dir = gpiod_get_direction(desc);
            if (dir < 0) {
                pr_err("GPIO %d yönü okunamadı.\n", GPIO_PIN);
                return dir;
            }

            if (copy_to_user((int __user *)arg, &dir, sizeof(dir))) {
                pr_err("GPIO yön bilgisi kopyalanamadı.\n");
                return -EFAULT;
            }

            if (dir == 0) {
                pr_info("GPIO %d yönü: Output\n", GPIO_PIN);
            } 
            else if (dir == 1) {
                pr_info("GPIO %d yönü: Input\n", GPIO_PIN);
            } 
            else {
                pr_info("GPIO %d yönü: Bilinmiyor (%d)\n", GPIO_PIN, dir);
            }

            break;
        }

        default:
            pr_warn("Geçersiz IOCTL komutu: %u\n", cmd);
            return -EINVAL;
    }

    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = NULL,
    .write = NULL,
    .unlocked_ioctl = my_ioctl,
};

static int __init my_init(void)
{
    int num;

    kernel_buffer = kmalloc(buffer_size, GFP_KERNEL);
    if (!kernel_buffer)
        return -ENOMEM;

    num = alloc_chrdev_region(&dev_num, 0, 1, "mygpio");
    if (num < 0) {
        kfree(kernel_buffer);
        return num;
    }

    cdev_init(&my_cdev, &fops);

    num = cdev_add(&my_cdev, dev_num, 1);
    if (num < 0) {
        unregister_chrdev_region(dev_num, 1);
        kfree(kernel_buffer);
        return num;
    }

    dev_class = class_create("mygpio");
    if (IS_ERR(dev_class)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        kfree(kernel_buffer);
        return PTR_ERR(dev_class);
    }

    if (IS_ERR(device_create(dev_class, NULL, dev_num, NULL, "mygpio"))) {
        class_destroy(dev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        kfree(kernel_buffer);
        return -EINVAL;
    }

    if (gpio_request(GPIO_PIN, "mygpio") < 0) {
        device_destroy(dev_class, dev_num);
        class_destroy(dev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        kfree(kernel_buffer);
        return -EBUSY;
    }

    if (gpio_direction_output(GPIO_PIN, 0) < 0) {
        gpio_free(GPIO_PIN);
        device_destroy(dev_class, dev_num);
        class_destroy(dev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        kfree(kernel_buffer);
        return -EINVAL;
    }

    pr_info("Modül yüklendi. Major: %d, Minor: %d\n", MAJOR(dev_num), MINOR(dev_num));
    return 0;
}

static void __exit my_exit(void)
{
    gpio_direction_output(GPIO_PIN, 0);
    gpio_free(GPIO_PIN);
    device_destroy(dev_class, dev_num);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    kfree(kernel_buffer);
    pr_info("Modül kaldırıldı.\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Egehan");
MODULE_DESCRIPTION("BBB ile GPIO Driver");

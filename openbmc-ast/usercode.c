#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MAGIC_NUM 'G'
#define GPIO_SET_HIGH _IO(MAGIC_NUM, 0)
#define GPIO_SET_LOW  _IO(MAGIC_NUM, 1)

int main(void)
{
    int fd = open("/dev/mydevice", O_RDWR);
    if (fd < 0) {
        perror("aygıt açılamadı");
        return 1;
    }

    printf("→ HIGH\n");
    if (ioctl(fd, GPIO_SET_HIGH) < 0)
        perror("ioctl HIGH");

    sleep(1);

    printf("→ LOW\n");
    if (ioctl(fd, GPIO_SET_LOW) < 0)
        perror("ioctl LOW");

    close(fd);
    return 0;
}

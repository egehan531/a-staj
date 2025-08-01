#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define GPIO_PIN 706
#define MAGIC_NUM 'G'
#define GPIO_SET_HIGH _IO(MAGIC_NUM, 0)
#define GPIO_SET_LOW  _IO(MAGIC_NUM, 1)

int main() {
    int fd = open("/dev/mydevice", O_RDWR);
    if (fd < 0) {
        perror("Can't open /dev/mydevice");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        ioctl(fd, GPIO_SET_HIGH);
        usleep(200000);
        ioctl(fd, GPIO_SET_LOW);
        usleep(200000);
    }

    close(fd);
    return 0;
}

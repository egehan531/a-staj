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
    int fd, choice;

    fd = open("/dev/mydevice", O_RDWR);
    if (fd < 0) {
        perror("Can't open /dev/mydevice");
        return 1;
    }

    printf("Set GPIO %d: [1] HIGH  [2] LOW: ", GPIO_PIN);
    scanf("%d", &choice);

    if (choice == 1) {
        if (ioctl(fd, GPIO_SET_HIGH) < 0)
            perror("HIGH failed");
        else
            printf("GPIO %d -> HIGH\n", GPIO_PIN);
    } else if (choice == 2) {
        if (ioctl(fd, GPIO_SET_LOW) < 0)
            perror("LOW failed");
        else
            printf("GPIO %d -> LOW\n", GPIO_PIN);
    } else {
        printf("Invalid choice\n");
    }

    close(fd);
    return 0;
}

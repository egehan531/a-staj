#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MAGIC_NUM 'G'
#define GPIO_SET_HIGH _IO(MAGIC_NUM, 0)
#define GPIO_SET_LOW  _IO(MAGIC_NUM, 1)

int main() {
    int fd;
    char secim;

    printf("GPIO seçimi (1 = HIGH, 0 = LOW]): ");
    scanf(" %c", &secim);

    fd = open("/dev/mygpio", O_RDWR);
    if (fd < 0) {
        perror("Device açma hatası");
        return 1;
    }

    if (secim == '1') {
        ioctl(fd, GPIO_SET_HIGH);
        printf("GPIO HIGH yapıldı.\n");
    } else if (secim == '0') {
        ioctl(fd, GPIO_SET_LOW);
        printf("GPIO LOW yapıldı.\n");
    } else {
        printf("Hatalı! Sadece 1 ya da 0 girin.\n");
    }

    close(fd);
    return 0;
}
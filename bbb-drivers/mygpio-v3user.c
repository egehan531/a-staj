#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MAGIC_NUM 'G'
#define GPIO_SET_HIGH _IO(MAGIC_NUM, 0)
#define GPIO_SET_LOW _IO(MAGIC_NUM, 1)
#define GPIO_SET_OUTPUT _IO(MAGIC_NUM, 2)
#define GPIO_SET_INPUT _IO(MAGIC_NUM, 3)
#define GPIO_GET_DIRECTION _IOR(MAGIC_NUM, 4, int)

int main() {
    int fd;
    int secim;
    int direction;

    fd = open("/dev/mygpio", O_RDWR);
    if (fd < 0) {
        printf("Device file açılamadı\n");
        return 1;
    }

    printf("Direction Ayarları (0 = SET, 1 = GET): ");
    scanf("%d", &secim);

    if (secim == 1) {
        if (ioctl(fd, GPIO_GET_DIRECTION, &direction) == -1) {
            printf("GPIO yönü alınamadı.\n");
            close(fd);
            return 1;
        }

        if (direction == 1) {
            printf("GPIO yönü: INPUT\n");
        } 
        else if (direction == 0) {
            int val;
            printf("GPIO yönü: OUTPUT. Ne yapmak istersiniz? (1 = HIGH, 0 = LOW): ");
            scanf("%d", &val);

            if (val == 1) {
                ioctl(fd, GPIO_SET_HIGH);
                printf("GPIO HIGH yapıldı.\n");
            } else if (val == 0) {
                ioctl(fd, GPIO_SET_LOW);
                printf("GPIO LOW yapıldı.\n");
            } else {
                printf("Geçersiz seçim\n");
            }
        } 
        else {
            printf("GPIO yönü bilinmiyor: (%d)\n", direction);
        }

    } 
    else if (secim == 0) {
        printf("Yön ayarlamak için (0 = OUTPUT, 1 = INPUT): ");
        scanf("%d", &direction);

        if (direction == 1) {
            ioctl(fd, GPIO_SET_INPUT);
            printf("GPIO INPUT olarak ayarlandı.\n");

        } 
        else if (direction == 0) {
            ioctl(fd, GPIO_SET_OUTPUT);

            int val;
            printf("GPIO OUTPUT olarak ayarlandı. Ne yapmak istersiniz? (1 = HIGH, 0 = LOW): ");
            scanf("%d", &val);

            if (val == 1) {
                ioctl(fd, GPIO_SET_HIGH);
                printf("GPIO HIGH yapıldı.\n");
            } else if (val == 0) {
                ioctl(fd, GPIO_SET_LOW);
                printf("GPIO LOW yapıldı.\n");
            } else {
                printf("Geçersiz seçim\n");
            }
        } 
        else {
            printf("Geçersiz yön seçimi\n");
        }

    } 
    else {
        printf("Geçersiz seçim\n");
    }

    close(fd);
    return 0;
}

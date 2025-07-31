#include <stdio.h>
#include <stdlib.h>

#define SET_HARF 0
#define SET_AGE 1
#define GET_HARF 2
#define GET_AGE 3

struct Egehan {
    char harf;
    int age;
};

void manager(struct Egehan *e, void *input, int cmd) {
    
    if(cmd == SET_HARF) {
        e->harf = *(char*)input;
    }
    else if(cmd == SET_AGE) {
        e->age = *(int*)input;
    }
    else if(cmd == GET_HARF){
        char get_char = e->harf;
        *(char*)input = get_char;
    }
    else {
        int get_age = e->age;
        *(int*)input = get_age;
    }
}

int main() {

	struct Egehan e1 = {0};
	
	char harf = 'B';
	int sayi = 25;
	void *dummy = malloc(sizeof(int));
	
	printf("Baş Harf: %c ve Yaş: %d\n", e1.harf, e1.age);
    manager(&e1, &harf, SET_HARF);
    printf("Baş Harf: %c ve Yaş: %d\n", e1.harf, e1.age);
    manager(&e1, &sayi, SET_AGE);
    printf("Baş Harf: %c ve Yaş: %d\n", e1.harf, e1.age);
    
    manager(&e1, dummy, GET_HARF);
    printf("Harf: %c\n", *(char*)dummy);
    
    manager(&e1, dummy, GET_AGE);
    printf("Sayi: %d\n", *(int*)dummy);
    
    free(dummy);
	return 0;
}

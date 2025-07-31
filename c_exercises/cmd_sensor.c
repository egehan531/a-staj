#include <stdio.h>
#include <stdlib.h>

#define SET_TEMP  0
#define SET_HUM   1
#define GET_TEMP  2
#define GET_HUM   3

struct Sensor {
    float temperature;
    float humidity;
    struct Sensor* a;
};

void sensorManager(struct Sensor *s, void *data, int cmd) {
    if(cmd == SET_TEMP) {
        s->temperature = *(float*)data;
    }
    else if(cmd == SET_HUM) {
        s->humidity = *(float*)data;
    }
    else if(cmd == GET_TEMP) {
        float get_temp = s->temperature;
        *(float*)data = get_temp;
    }
    else {
        float get_hum = s->humidity;
        *(float*)data = get_hum;
    }
}

int main()
{
    struct Sensor s1 = {0};
    float humidity = 60;
    float temperature = 33.6;
    void *reader = malloc(sizeof(float));
    
    printf("Nem: %.2f%% ve Sicaklik: %.2f Celcius\n\n", s1.humidity, s1.temperature);
    
    sensorManager(&s1, &temperature, SET_TEMP);
    printf("Nem: %.2f%% ve Sicaklik: %.2f Celcius \n\n", s1.humidity, s1.temperature);
    
    sensorManager(&s1, &humidity, SET_HUM);
    printf("Nem: %.2f%% ve Sicaklik: %.2f Celcius \n\n", s1.humidity, s1.temperature);
    
    sensorManager(&s1, reader, GET_TEMP);
    printf("Okunan sicaklik: %.2f\n", *(float*)reader);
    
    sensorManager(&s1, reader, GET_HUM);
    printf("Okunan nem: %.2f\n", *(float*)reader);
    
    free(reader);
    return 0;
}













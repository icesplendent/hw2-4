#include "mbed.h"

Thread t1;
Thread t2;
EventQueue queue(32 * EVENTS_EVENT_SIZE);

PwmOut PWM1(D6);
AnalogIn input(D10);
const double pi = 3.141592653589793238462;
const double offset = 2.5f;
float ADCdata[1280];
int sample = 1280;

void PWMGEN() {
    int i = 0;
    double width = 0.0;
    double rads = 0.0;
    while (1) {
        PWM1.period_ms(5);

        rads = (pi * i) / 180.0f;
        width = ((offset * (cos(rads + pi))) + offset);
        // printf("%f %d\n\r", width, i);
        PWM1.pulsewidth_ms(width);
        // printf("%f\n\r", PWM1.read());
        ThisThread::sleep_for(5ms);
        i = (i + 1) % 360;
    }
}

void AOINFO() {
    for (int i = 0; i < sample; i++) {
        ADCdata[i] = input.read();
        printf("%f\r\n", ADCdata[i]);
    }
}

void AOCAP() {
    while (1) {
        queue.call(AOINFO);
        ThisThread::sleep_for(1ms);
    }
}

int main() {
    t1.start(PWMGEN);
    t2.start(callback(&queue, &EventQueue::dispatch_forever));

    AOCAP();
}

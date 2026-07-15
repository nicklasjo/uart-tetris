#ifndef LED_H
#define LED_H

void init_led();
void set_led_state(int state);
void delay(int time);
void blink_led(int time);
void toggle_led();

#endif
#ifndef _fun_h
#define _fun_h

void led_show(uint8_t led,uint8_t mode);
void key_scan();
void lcd_show();
double adc_value(ADC_HandleTypeDef *hadc);
void main_process();

#endif

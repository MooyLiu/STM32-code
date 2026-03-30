#include "headfile.h"

double CD_uint=0.5;
char text[20];
int lcd_mode=2;
int data_index=1,lock_index=0;
int lock_mode;
uint32_t pwm_fre;
double adcA,adcB;
int second=0,minute=0,hour=0;
int DS=1,DR=10,FS=100,FR=1000;
uint32_t PA7_fre;
int PA7_CD,CD_count;

void led_show(uint8_t led,uint8_t mode){

	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if(mode){
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(led-1),GPIO_PIN_RESET);
	}
	else{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(led-1),GPIO_PIN_SET);
	}
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);

}

uint8_t b1now,b2now,b3now,b4now;
uint8_t b1last=1,b2last=1,b3last=1,b4last=1;
void key_scan(){

	b1now=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	b2now=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	b3now=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	b4now=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	//lcd界面按键
	if(b1now==0&&b1last==1){
		
		if(lcd_mode==6){
			lcd_mode=0;
		}
		lcd_mode=lcd_mode+2;
		
	}
	
	//参数选择按键
	if(b2now==0&&b2last==1){
		//第三界面
		if(lcd_mode==6){
			if(data_index==4){
			data_index=0;
			}
			data_index++;
		}
		//第二界面
		if(lcd_mode==4){
			lock_index++;
			if((lock_index%2)==0){
				lock_mode=0;
			}
			else{
				lock_mode=1;
			}
		}
		
		
	}
	
	//加按键
	if(b3now==0&&b3last==1){
		if(data_index==1){
			DS++;
		}
		if(data_index==2){
			DR+=10;
		}
		if(data_index==3){
			FS+=100;
		}
		if(data_index==4){
			FR+=1000;
		}
	}
	//减按键
	if(b4now==0&&b4last==1){
		if(data_index==1){
			DS--;
		}
		if(data_index==2){
			DR-=10;
		}
		if(data_index==3){
			FS-=100;
		}
		if(data_index==4){
			FR-=1000;
		}
	}
	b1last=b1now;
	b2last=b2now;
	b3last=b3now;
	b4last=b4now;
}

void lcd_show(){

	uint16_t temp=GPIOC->ODR;
	
	if(lcd_mode==2){
		if(lock_mode==0){
		sprintf(text,"       PWM          ");
		LCD_DisplayStringLine(Line1,(uint8_t *)text);
		sprintf(text,"   CF=8800Hz        ");
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
		sprintf(text,"   CD=%d%%          ",PA7_CD);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
		sprintf(text,"   DF=8362Hz        ");
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
		sprintf(text,"   ST=UNLOCK        ");
		LCD_DisplayStringLine(Line6,(uint8_t *)text);
		sprintf(text,"   %.2dH%.2dM%.2dS  ",hour,minute,second);
		LCD_DisplayStringLine(Line7,(uint8_t *)text);
		}
		else if(lock_mode==1){
		sprintf(text,"       PWM          ");
		LCD_DisplayStringLine(Line1,(uint8_t *)text);
		sprintf(text,"   CF=8800Hz        ");
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
		sprintf(text,"   CD=%d%%          ",PA7_CD);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
		sprintf(text,"   DF=8362Hz        ");
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
		sprintf(text,"   ST=LOCK          ");
		LCD_DisplayStringLine(Line6,(uint8_t *)text);
		sprintf(text,"   %.2dH%.2dM%.2dS  ",hour,minute,second);
		LCD_DisplayStringLine(Line7,(uint8_t *)text);
		}
	}
	if(lcd_mode==4){ 
		sprintf(text,"       RECD         ");
		LCD_DisplayStringLine(Line1,(uint8_t *)text);
		sprintf(text,"   CF=8800Hz        ");
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
		sprintf(text,"   CD=30%%          ");
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
		sprintf(text,"   DF=8362Hz        ");
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
		sprintf(text,"   XF=438Hz         ");
		LCD_DisplayStringLine(Line6,(uint8_t *)text);
		sprintf(text,"   %.2dH%.2dM%.2dS  ",hour,minute,second);
		LCD_DisplayStringLine(Line7,(uint8_t *)text);
	}
	if(lcd_mode==6){
		if(data_index==1){
		sprintf(text,"       PARA       ");
		LCD_DisplayStringLine(Line1,(uint8_t *)text);
			LCD_SetBackColor(Blue);
		sprintf(text,"   DS=%d%%            ",DS);
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
			LCD_SetBackColor(Black);
		sprintf(text,"   DR=%d%%           ",DR);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
		sprintf(text,"   FS=%dHz          ",FS);
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
		sprintf(text,"   FR=%dHz         ",FR);
		LCD_DisplayStringLine(Line6,(uint8_t *)text);
		sprintf(text,"                     ");
		LCD_DisplayStringLine(Line7,(uint8_t *)text);
		}
		else if(data_index==2){
		sprintf(text,"       PARA          ");
		LCD_DisplayStringLine(Line1,(uint8_t *)text);
		sprintf(text,"   DS=%d%%            ",DS);
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
			LCD_SetBackColor(Blue); 
		sprintf(text,"   DR=%d%%           ",DR);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
			LCD_SetBackColor(Black);
		sprintf(text,"   FS=%dHz          ",FS);
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
		sprintf(text,"   FR=%dHz         ",FR);
		LCD_DisplayStringLine(Line6,(uint8_t *)text);
		sprintf(text,"                   ");
		LCD_DisplayStringLine(Line7,(uint8_t *)text);
		}
		else if(data_index==3){
		sprintf(text,"       PARA ");
		LCD_DisplayStringLine(Line1,(uint8_t *)text);
		sprintf(text,"   DS=%d%%            ",DS);
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
		sprintf(text,"   DR=%d%%           ",DR);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
			LCD_SetBackColor(Blue);
		sprintf(text,"   FS=%dHz          ",FS);
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
			LCD_SetBackColor(Black);
		sprintf(text,"   FR=%dHz         ",FR);
		LCD_DisplayStringLine(Line6,(uint8_t *)text);
			sprintf(text,"                   ");
		LCD_DisplayStringLine(Line7,(uint8_t *)text);
		}
		else if(data_index==4){
		
		sprintf(text,"       PARA ");
		LCD_DisplayStringLine(Line1,(uint8_t *)text);
		sprintf(text,"   DS=%d%%            ",DS);
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
		sprintf(text,"   DR=%d%%           ",DR);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
		sprintf(text,"   FS=%dHz          ",FS);
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
		LCD_SetBackColor(Blue);
		sprintf(text,"   FR=%dHz         ",FR);
		LCD_DisplayStringLine(Line6,(uint8_t *)text);
			LCD_SetBackColor(Black);
			sprintf(text,"                   ");
		LCD_DisplayStringLine(Line7,(uint8_t *)text);
		}
	}

		
	GPIOC->ODR=temp;

}

double adc_value(ADC_HandleTypeDef *hadc){

	HAL_ADC_Start(hadc);
	uint32_t value=HAL_ADC_GetValue(hadc);
	return 3.3*value/4030;

}

void data_process(){

	adcA=adc_value(&hadc2);
	adcB=adc_value(&hadc1);
	
	CD_count=adcB/CD_uint;
	PA7_CD=10+CD_count*DS;

}



void main_process(){
	key_scan();
	lcd_show();
	data_process();
	if(lcd_mode==2){
		led_show(1,1);
	}else{
		led_show(1,0);
	}
	
	if(lock_mode==1){
		led_show(2,1);
	}
	else if(lock_mode!=1){
		led_show(2,0);
	}
	
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance==TIM2){
		uint32_t pwm_value=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		TIM2->CNT=0;
		pwm_fre=80000000/(80*pwm_value);
	}
	

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance==TIM4){
		if(second==59){
			second=0;
			if(minute==59){
				minute=0;
				hour++;
			}
			minute++;
		}
		second++;
	}


}


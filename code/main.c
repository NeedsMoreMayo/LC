//F_CPU defined in makefile
//#define F_CPU 10000000ul

#include <avr/io.h>
#include <util/delay.h>		//temporary sin
#include <avr/interrupt.h>
//#include <math.h>

#include "LCD.h"


//PORT A
#define AVRCLK_PIN		0
#define CAL_PIN			1
#define UNUSED_1_PIN	2
#define UNUSED_2_PIN	3
#define UNUSED_3_PIN	4
#define UNUSED_4_PIN	5
#define LED_1_PIN		6
#define LED_2_PIN		7

//PORT C
#define BTN_ITER_PIN	0
#define UNUSED_5_PIN	1
#define DISP_SDA_PIN	2
#define DISP_SDC_PIN	3

//PORT D
#define BTN_ENTER_PIN	1	//ADC0 AIN1
#define MODE_PIN		2
#define SIG_RED_PIN		3
#define ADR2_PIN		4
#define ADR1_PIN		5
#define ADR0_PIN		6
#define SIG_OUT_PIN		7

//PORT F
#define IPOL_PIN		0
#define ICP1_PIN		1
#define RESET_PIN		6

void PORTS_init(void);
void I2C0_init(void);
void LCD_init(void);
void TOGGLE_LED(void);

int main(void){
	cli(); //disable interrupts when enabling interrupt handlers
	PORTS_init();
	I2C0_init();
	LCD_init();
	sei();//enable interrupts
    while (1){
		TOGGLE_LED();
		_delay_ms(1000);

		LCD1602_send_command(CLEAR_DISPLAY);
		_delay_us(100);
		LCD1602_send_command(RETURN_HOME);
		_delay_us(100);
		LCD1602_send_string("Testing 123");
		_delay_us(100);
    }
}


void TOGGLE_LED(){
	PORTD.OUTTGL = (1 << LED_1_PIN)
		 		|  (1 << LED_2_PIN);
}

void PORTS_init(){
	//CAL, LED_1 and LED_2 are output on PORTA
	PORTA.DIR =   (1 << CAL_PIN)
				| (1 << LED_1_PIN)
				| (1 << LED_2_PIN);
	PORTA.OUT = 0x00;
	
	//DISP_SDA and DISP_SDC are output on PORTC
	PORTC.DIR =   (1 << DISP_SDA_PIN)
				| (1 << DISP_SDC_PIN);
	PORTC.OUT = 0x00;
	
	//MODE and ADR0..2 are output on PORTD
	PORTD.DIR =   (1 << MODE_PIN)
				| (1 << ADR0_PIN)
				| (1 << ADR1_PIN)
				| (1 << ADR2_PIN);
	PORTD.OUT = 0x00;
	
	//no output on PORTD
	PORTF.DIR = 0;
	PORTF.OUT = 0x00;
}
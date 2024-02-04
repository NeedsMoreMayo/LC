#include <avr/io.h>
#include <util/delay.h>		//temporary sin
#include <avr/interrupt.h>

#include <string.h>

#include "LCD.h"

#define TWI0_BAUD(F_SCL, T_RISE)	((((((float)5000000.0 / (float)F_SCL)) - 10 - ((float)5000000.0 * T_RISE / 1000000))) / 2)

#define I2C_LCD_ADDR_DEFAULT 		0x27


#define LCD_NOBACKLIGHT 			0x00	//from sunfounder code zip
#define	CLEAR_DISPLAY				0x01	//Clear display screen
#define	RETURN_HOME					0x02	//Return home
#define	CURSOR_DECR					0x04	//Decrement cursor (shift cursor to left)
#define	CURSOR_INCR					0x06	//Increment cursor (shift cursor to right)
#define	SHIFT_RIGHT					0x05	//Shift display right
#define	SHIFT_LEFT					0x07	//Shift display left
#define	DISP_OFF_CURSOR_OFF			0x08	//Display off, cursor off
#define	DISP_OFF_CURSOR_ON			0x0A	//Display off, cursor on
#define	DISP_ON_CURSOR_OFF			0x0C	//Display on, cursor off
#define	DISP_ON_CURSOR_NO_BLINK		0x0E	//Display on, cursor not blinking
#define	DISP_ON_CURSOR_BLINK		0x0F	//Display on, cursor blinking
#define	SHIFT_CURSOR_LEFT			0x10	//Shift cursor position to left
#define	SHIFT_CURSOR_RIGHT			0x14	//Shift the cursor position to the right
#define	SHIFT_DISP_LEFT				0x18	//Shift the entire display to the left
#define	SHIFT_DISP_RIGHT			0x1C	//Shift the entire display to the right
#define	DISP_2_X_5x7				0x28	//2 lines and 5×7 matrix

#define SEL_FIRST_LINE				0x80 //Force cursor to the beginning of 1st line
#define SEL_SECOND_LINE				0xC0 //Force cursor to the beginning of 2nd line

#define I2C_ACKED					0x01
#define I2C_NACKED					0x02
#define I2C_ERROR					0x03

char TEXTBUFF[16] = "It's Working";
uint8_t DATABUFF[64] = "";
char *textbuff = TEXTBUFF;
uint8_t *databuff = DATABUFF;

static uint8_t TWI0_WaitW(void){
	uint8_t state = 0;
	do{
		if(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm)){
			if(!(TWI0.MSTATUS & TWI_RXACK_bm)){
				/* slave responded with ack - TWI goes to M1 state */
				state = I2C_ACKED;
			}else{
				/* address sent but no ack received - TWI goes to M3 state */
				state = I2C_NACKED;
			}
		}else if(TWI0.MSTATUS & (TWI_BUSERR_bm | TWI_ARBLOST_bm)){
			/* get here only in case of bus error or arbitration lost - M4 state */
			state = I2C_ERROR;
		}
	} while(!state);
	return state;
}

uint8_t TWI0_write_address(uint8_t address){
	uint8_t state = 0;
	
	TWI0.MADDR = address; /* Transmitting the slave address */
	state = TWI0_WaitW(); /* wait for error code */
	return state;
}

uint8_t TWI0_write_data(uint8_t data){
	uint8_t state = 0;
	
	TWI0.MDATA = data; /* Transmitting the data */
	state = TWI0_WaitW(); /* wait for error code */
	return state;
}

uint8_t I2C_Write(uint8_t slaveAddr7b, uint8_t *data, uint8_t size){
	uint8_t idx =0;
	uint8_t state = 0;
	
	state = TWI0_write_address(slaveAddr7b);
	for(idx = 0; idx < size; idx++){
		state = TWI0_write_data(data[idx]);
	}
	TWI0.MCTRLB = TWI_MCMD_STOP_gc;
	return state;
}

void LCD1602_send_char(unsigned char data){
	uint8_t UB = data & 0xF0;
	uint8_t LB = (data & 0x0F) << 4;

	databuff[0] = UB | 0x0D;
	databuff[1] = UB | 0x09;
	databuff[2] = LB | 0x0D;
	databuff[3] = LB | 0x09;
	I2C_Write(I2C_LCD_ADDR_DEFAULT, databuff, 4);
}

void LCD1602_send_command(uint8_t data){
	uint8_t UB = data & 0xF0;
	uint8_t LB = (data & 0x0F) << 4;
	
	databuff[0] = UB | 0x04;
	databuff[1] = UB | 0x00;
	databuff[2] = LB | 0x04;
	databuff[3] = LB | 0x00;
	I2C_Write(I2C_LCD_ADDR_DEFAULT, databuff, 4);
}

void LCD1602_send_UB_command(uint8_t data){
	uint8_t UB = data & 0xF0;

	databuff[0] = UB | 0x04;
	databuff[1] = UB | 0x00;
	I2C_Write(I2C_LCD_ADDR_DEFAULT, databuff, 2);
}

void LCD1602_send_string(char * data){
	uint8_t len = strlen((const char *)data);
	
	for (uint8_t i = 0; i < len; i++) {
		uint8_t UB = data[i] & 0xF0;
		uint8_t LB = (data[i] & 0x0F) << 4;

		databuff[(i << 2) + 0] = UB | 0x0D;
		databuff[(i << 2) + 1] = UB | 0x09;
		databuff[(i << 2) + 2] = LB | 0x0D;
		databuff[(i << 2) + 3] = LB | 0x09;
	}
	I2C_Write(I2C_LCD_ADDR_DEFAULT, databuff, (len << 2));
}


void LCD_init(void){
	_delay_ms(15);
	LCD1602_send_UB_command(0x30);	//D7=0, D6=0, D5=1, D4=1, RS,RW=0 EN=1
	_delay_us(4100);	//wait 4.1msec
	LCD1602_send_UB_command(0x30);	//D7=0, D6=0, D5=1, D4=1, RS,RW=0 EN=1
	_delay_us(100);	//wait 100usec
	LCD1602_send_UB_command(0x30);	//8-bit mode init complete
	_delay_us(4100);	//wait 4.1msec
	LCD1602_send_UB_command(0x20);	//switched now to 4-bit mode
	
	_delay_us(40);		// wait 40usec
	LCD1602_send_command(DISP_2_X_5x7);		// keep 4-bit mode, D3=2lines, D2=char5x8
	_delay_us(40);		// wait 40usec
	LCD1602_send_command(DISP_OFF_CURSOR_OFF);		// D3=1 D2=display_off, D1=cursor_off, D0=cursor_blink
	_delay_us(40);		// wait 40usec
	LCD1602_send_command(CLEAR_DISPLAY);		// D0=display_clear
	_delay_us(40);		// wait 40usec
	LCD1602_send_command(CURSOR_INCR);		// print left to right
	_delay_us(40);		// wait 40usec
	LCD1602_send_command(DISP_ON_CURSOR_BLINK);		// D3=1 D2=display_on, D1=cursor_on, D0=cursor_blink 0X0E
}


void I2C0_init(void){
	//PC2 = SDA
	//PC3 = SDC
	PORTMUX.TWIROUTEA = PORTMUX_TWI0_ALT2_gc; /* SDA: PC2, SCL: PC3. Dual mode: SDA: -, SCL: -. */
	//PCF8574 max 100kHz clock
	TWI0.MBAUD = (uint8_t)TWI0_BAUD(50000, 0); /* set MBAUD register */
	TWI0.MCTRLB =  TWI_FLUSH_bm;           // Clear TWI state (alternative: TWI_ACKACT_NACK_gc;  // send NACK)
	TWI0.MCTRLA = 1 << TWI_ENABLE_bp /* Enable TWI Master: enabled */
				| 0 << TWI_QCEN_bp /* Quick Command Enable: disabled */
				| 0 << TWI_RIEN_bp /* Read Interrupt Enable: disabled */
				| 0 << TWI_SMEN_bp /* Smart Mode Enable: disabled */
				| TWI_TIMEOUT_DISABLED_gc /* Bus Timeout Disabled */
				| 0 << TWI_WIEN_bp; /* Write Interrupt Enable: disabled */
	TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;   //initially sets the bus state to idle (alternative: TWI0.MSTATUS = 0xFF;)
}

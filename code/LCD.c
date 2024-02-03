#include <avr/io.h>
#include <util/delay.h>		//temporary sin
#include <avr/interrupt.h>

#include "LCD.h"
#define TWI0_BAUD(F_SCL, T_RISE)	((((((float)5000000.0 / (float)F_SCL)) - 10 - ((float)5000000.0 * T_RISE / 1000000))) / 2)



void I2C0_init(){
	//PC2 = SDA
	//PC3 = SDC
	PORTMUX.TWIROUTEA = PORTMUX_TWI0_ALT2_gc; /* SDA: PC2, SCL: PC3. Dual mode: SDA: -, SCL: -. */
	//PCF8574 max 100kHz clock
	TWI0.MBAUD = (uint8_t)TWI0_BAUD(10000, 0); /* set MBAUD register */
	TWI0.MCTRLB =  TWI_FLUSH_bm;           // Clear TWI state
	TWI0.MCTRLA = 1 << TWI_ENABLE_bp /* Enable TWI Master: enabled */
				| 0 << TWI_QCEN_bp /* Quick Command Enable: disabled */
				| 0 << TWI_RIEN_bp /* Read Interrupt Enable: disabled */
				| 0 << TWI_SMEN_bp /* Smart Mode Enable: disabled */
				| TWI_TIMEOUT_DISABLED_gc /* Bus Timeout Disabled */
				| 0 << TWI_WIEN_bp; /* Write Interrupt Enable: disabled */
	TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc;   //initially sets the bus state to idle
}

void LCD_init(){
	
}

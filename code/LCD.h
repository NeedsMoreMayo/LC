#define US2ST(usec)	((systime_t)(((((uint64_t)(usec)) * ((uint64_t)CH_CFG_ST_FREQUENCY)) + 999999ULL) / 1000000ULL))
#define I2C_WAIT	1000 // needs to be long enough to write a whole page
#define I2C_LCD_ADDR_DEFAULT 0x27

#define LCD_NOBACKLIGHT 		0x00	//from sunfounder code zip
#define	CLEAR_DISPLAY			0x01	//Clear display screen
#define	RETURN_HOME				0x02	//Return home
#define	CURSOR_DECR				0x04	//Decrement cursor (shift cursor to left)
#define	CURSOR_INCR				0x06	//Increment cursor (shift cursor to right)
#define	SHIFT_RIGHT				0x05	//Shift display right
#define	SHIFT_LEFT				0x07	//Shift display left
#define	DISP_OFF_CURSOR_OFF		0x08	//Display off, cursor off
#define	DISP_OFF_CURSOR_ON		0x0A	//Display off, cursor on
#define	DISP_ON_CURSOR_OFF		0x0C	//Display on, cursor off
#define	DISP_ON_CURSOR_NO_BLINK	0x0E	//Display on, cursor not blinking
#define	DISP_ON_CURSOR_BLINK	0x0F	//Display on, cursor blinking
#define	SHIFT_CURSOR_LEFT		0x10	//Shift cursor position to left
#define	SHIFT_CURSOR_RIGHT		0x14	//Shift the cursor position to the right
#define	SHIFT_DISP_LEFT			0x18	//Shift the entire display to the left
#define	SHIFT_DISP_RIGHT		0x1C	//Shift the entire display to the right
#define	DISP_2_X_5x7			0x28	//2 lines and 5×7 matrix

#define SEL_FIRST_LINE				0x80 //Force cursor to the beginning of 1st line
#define SEL_SECOND_LINE				0xC0 //Force cursor to the beginning of 2nd line


void LCD1602_send_char(unsigned char);

void LCD1602_send_command(uint8_t);

void LCD1602_send_UB_command(uint8_t);

void LCD1602_send_string(char *);

void LCD_init(void);

extern char TEXTBUFF[16];
extern char *textbuff;


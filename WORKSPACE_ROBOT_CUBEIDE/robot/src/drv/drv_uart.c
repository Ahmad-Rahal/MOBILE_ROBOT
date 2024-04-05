/*
 * drv_uart.c
 */
#include "drv_uart.h"
#include "math.h"
#include "util.h"

int put_char(char);
int putchar_stlink(char);
int puchar_zigbee(char);

uint8_t rec_buf2[NB_CAR_TO_RECEIVE+1];
uint8_t rec_buf6[NB_CAR_TO_RECEIVE+1];
// uint8_t rec_buf[NB_CAR_TO_RECEIVE+1]="";

//=================================================================
//	UART 2 INIT (STLINK UART)
//=================================================================

void uart2_Init()
{
	  Uart2Handle.Instance          = USART2;

	  Uart2Handle.Init.BaudRate     = USART2_BAUDRATE;
	  Uart2Handle.Init.WordLength   = UART_WORDLENGTH_8B;
	  Uart2Handle.Init.StopBits     = UART_STOPBITS_1;
	  Uart2Handle.Init.Parity       = UART_PARITY_NONE;
	  Uart2Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	  Uart2Handle.Init.Mode         = UART_MODE_TX_RX;
	  Uart2Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	  HAL_UART_Init(&Uart2Handle);

	  HAL_UART_Receive_IT(&Uart2Handle, (uint8_t *)rec_buf2, NB_CAR_TO_RECEIVE);
}
//=================================================================
//	UART 6 INIT (ZIGBEE UART)
//=================================================================

void uart6_Init()
{
	  Uart6Handle.Instance          = USART6;

	  Uart6Handle.Init.BaudRate     = USART6_BAUDRATE;
	  Uart6Handle.Init.WordLength   = UART_WORDLENGTH_8B;
	  Uart6Handle.Init.StopBits     = UART_STOPBITS_1;
	  Uart6Handle.Init.Parity       = UART_PARITY_NONE;
	  Uart6Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	  Uart6Handle.Init.Mode         = UART_MODE_TX_RX;
	  Uart6Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	  HAL_UART_Init(&Uart6Handle);

	  HAL_UART_Receive_IT(&Uart6Handle, (uint8_t *)rec_buf6, NB_CAR_TO_RECEIVE);
}

//=================================================================
//	UART WRITE
//=================================================================

void uart_Write(char* toprint, int size)
{
#ifdef USE_USART_STLINK
	HAL_UART_Transmit(&Uart2Handle, (uint8_t *)toprint, size, 0xFFFF);
#else
	HAL_UART_Transmit(&Uart6Handle, (uint8_t *)toprint, size, 0xFFFF);
#endif
}

//=================================================================
//	PUCHAR PROTOTYPE (USED BY PRINTF FUNCTIONS)
//=================================================================
int put_char(char ch)
{
#if USE_USART_STLINK
	HAL_UART_Transmit(&Uart2Handle, (uint8_t *)&ch, 1, 0xFFFF);
#else
	HAL_UART_Transmit(&Uart6Handle, (uint8_t *)&ch, 1, 0xFFFF);
#endif
  return 0;
}

int putchar_stlink(char ch)
{
	HAL_UART_Transmit(&Uart2Handle, (uint8_t *)&ch, 1, 0xFFFF);
	return 0;
}

int putchar_zigbee(char ch)
{
	HAL_UART_Transmit(&Uart6Handle, (uint8_t *)&ch, 1, 0xFFFF);
	return 0;
}

//================================================================
//				PUT STRING
//================================================================
void put_string(char* s)
{
	while(*s != '\0')
	{
		put_char(*s);
		s++;
	}
}

void put_string_stlink(char* s)
{
	while(*s != '\0')
	{
		putchar_stlink(*s);
		s++;
	}
}

void put_string_zigbee(char* s)
{
	while(*s != '\0')
	{
		putchar_zigbee(*s);
		s++;
	}
}
//================================================================
//				TERM_PRINTF
//================================================================

//char string[64];

void term_printf(const char* fmt, ...)
{
	__gnuc_va_list         ap;
	char          *p;
	char           ch;
	unsigned long  ul;
	unsigned long long ull;
	unsigned long  size;
	unsigned int   sp;
	char           s[60];
	int first=0;

	va_start(ap, fmt);

	while (*fmt != '\0') {
		if (*fmt =='%') {
			size=0; sp=1;
			if (*++fmt=='0') {fmt++; sp=0;}	// parse %04d --> sp=0
			ch=*fmt;
			if ((ch>'0') && (ch<='9')) {	// parse %4d --> size=4
				char tmp[10];
				int i=0;
				while ((ch>='0') && (ch<='9')) {
					tmp[i++]=ch;
					ch=*++fmt;
				}
				tmp[i]='\0';
				size=str2num(tmp,10);
			}
			switch (ch) {
				case '%':
					put_char('%');
					break;
				case 'c':
					ch = va_arg(ap, int);
					put_char(ch);
					break;
				case 's':
					p = va_arg(ap, char *);
					put_string(p);
					break;
				case 'd':
					ul = va_arg(ap, long);
					if ((long)ul < 0) {
						put_char('-');
						ul = -(long)ul;
						//size--;
					}
					num2str(s, ul, 10, size, sp);
					put_string(s);
					break;
				case 'u':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 10, size, sp);
					put_string(s);
					break;
				case 'o':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 8, size, sp);
					put_string(s);
					break;
				case 'p':
					put_char('0');
					put_char('x');
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16, size, sp);
					put_string(s);
					break;
				case 'x':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16, size, sp);
					put_string(s);
					break;
				case 'f':
					if(first==0){ ull = va_arg(ap, long long unsigned int); first = 1;}
					ull = va_arg(ap, long long unsigned int);
					int sign = ( ull & 0x80000000 ) >> 31;
					int m = (ull & 0x000FFFFF) ; // should be 0x007FFFFF
					float mf = (float)m ;
					mf = mf / pow(2.0,20.0);
					mf = mf + 1.0;
					int e = ( ull & 0x78000000 ) >> 23 ; // should be int e = ( ul & 0x7F800000 ) >> 23;
					e = e | (( ull & 0x000F00000 ) >> 20);
					e = e - 127;
					float f = mf*myPow(2.0,e);
					if(sign==1){ put_char('-'); }
					float2str((char*)s, f, 5);
					put_string((char*)s);
					break;

				default:
					put_char(*fmt);
			}
		} else put_char(*fmt);
		fmt++;
	}
	va_end(ap);
}
//================================================================
//				TERM_PRINTF STLINK
//================================================================

void term_printf_stlink(const char* fmt, ...)
{
	va_list        ap;
	char          *p;
	char           ch;
	unsigned long  ul;
	unsigned long  size;
	unsigned int   sp;
	char           s[34];

	va_start(ap, fmt);
	while (*fmt != '\0') {
		if (*fmt =='%') {
			size=0; sp=1;
			if (*++fmt=='0') {fmt++; sp=0;}	// parse %04d --> sp=0
			ch=*fmt;
			if ((ch>'0') && (ch<='9')) {	// parse %4d --> size=4
				char tmp[10];
				int i=0;
				while ((ch>='0') && (ch<='9')) {
					tmp[i++]=ch;
					ch=*++fmt;
				}
				tmp[i]='\0';
				size=str2num(tmp,10);
			}
			switch (ch) {
				case '%':
					 putchar_stlink('%');
					break;
				case 'c':
					ch = va_arg(ap, int);
					putchar_stlink(ch);
					break;
				case 's':
					p = va_arg(ap, char *);
					put_string_stlink(p);
					break;
				case 'd':
					ul = va_arg(ap, long);
					if ((long)ul < 0) {
						putchar_stlink('-');
						ul = -(long)ul;
						//size--;
					}
					num2str(s, ul, 10, size, sp);
					put_string_stlink(s);
					break;
				case 'u':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 10, size, sp);
					put_string_stlink(s);
					break;
				case 'o':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 8, size, sp);
					put_string_stlink(s);
					break;
				case 'p':
					putchar_stlink('0');
					putchar_stlink('x');
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16, size, sp);
					put_string_stlink(s);
					break;
				case 'x':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16, size, sp);
					put_string_stlink(s);
					break;
				case 'f':
					ul = va_arg(ap, long);
					int s = ( ul & 0x80000000 ) >> 31;
					int m = (ul & 0x000FFFFF) ; // should be 0x007FFFFF
					float mf = (float)m ;
					mf = mf / pow(2.0,20.0);
					mf = mf + 1.0;
					int e = ( ul & 0x78000000 ) >> 23 ; // should be int e = ( ul & 0x7F800000 ) >> 23;
					e = e | (( ul & 0x000F00000 ) >> 20);
					e = e - 127;
					float f = mf*myPow(2.0,e);
					if(s==1){ put_char('-'); }
					float2str((char*)s, f, 5);
					put_string_stlink((char*)s);
					break;
				default:
					putchar_stlink(*fmt);
			}
		} else putchar_stlink(*fmt);
		fmt++;
	}
	va_end(ap);
}
//=====================================================
void term_printf_zigbee(const char* fmt, ...)
{
	va_list        ap;
	char          *p;
	char           ch;
	unsigned long  ul;
	unsigned long  size;
	unsigned int   sp;
	char           s[34];

	va_start(ap, fmt);
	while (*fmt != '\0') {
		if (*fmt =='%') {
			size=0; sp=1;
			if (*++fmt=='0') {fmt++; sp=0;}	// parse %04d --> sp=0
			ch=*fmt;
			if ((ch>'0') && (ch<='9')) {	// parse %4d --> size=4
				char tmp[10];
				int i=0;
				while ((ch>='0') && (ch<='9')) {
					tmp[i++]=ch;
					ch=*++fmt;
				}
				tmp[i]='\0';
				size=str2num(tmp,10);
			}
			switch (ch) {
				case '%':
					 putchar_zigbee('%');
					break;
				case 'c':
					ch = va_arg(ap, int);
					putchar_zigbee(ch);
					break;
				case 's':
					p = va_arg(ap, char *);
					put_string_zigbee(p);
					break;
				case 'd':
					ul = va_arg(ap, long);
					if ((long)ul < 0) {
						putchar_zigbee('-');
						ul = -(long)ul;
						//size--;
					}
					num2str(s, ul, 10, size, sp);
					put_string_zigbee(s);
					break;
				case 'u':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 10, size, sp);
					put_string_zigbee(s);
					break;
				case 'o':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 8, size, sp);
					put_string_zigbee(s);
					break;
				case 'p':
					putchar_zigbee('0');
					putchar_zigbee('x');
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16, size, sp);
					put_string_zigbee(s);
					break;
				case 'x':
					ul = va_arg(ap, unsigned int);
					num2str(s, ul, 16, size, sp);
					put_string_zigbee(s);
					break;
				case 'f':
					ul = va_arg(ap, long);
					int s = ( ul & 0x80000000 ) >> 31;
					int m = (ul & 0x000FFFFF) ; // should be 0x007FFFFF
					float mf = (float)m ;
					mf = mf / pow(2.0,20.0);
					mf = mf + 1.0;
					int e = ( ul & 0x78000000 ) >> 23 ; // should be int e = ( ul & 0x7F800000 ) >> 23;
					e = e | (( ul & 0x000F00000 ) >> 20);
					e = e - 127;
					float f = mf*myPow(2.0,e);
					if(s==1){ put_char('-'); }
					float2str((char*)s, f, 5);
					put_string_zigbee((char*)s);
					break;
				default:
					putchar_zigbee(*fmt);
			}
		} else putchar_zigbee(*fmt);
		fmt++;
	}
	va_end(ap);
}

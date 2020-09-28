#include <LPC214x.h>
#include "LCD.h"
#define back_light_timeout 1000	   //IOODIR IO0CLR IO0SET

#define d4 (1<<18)	   // IO0DIR IO0CLR IO0SET
#define d5 (1<<19)
#define d6 (1<<20)
#define d7 (1<<21)

#define rs (1<<15)	   // IO0DIR IO1CLR IO1SET
#define rw (1<<16)
#define en (1<<17)

#define data_mask (d4 | d5 | d6 | d7)
#define flag d7

//#define lcd_IO0PIN    IO0PIN   

void wait_lcd (void);
void command_write(unsigned char command);
void set_lcd_port_output(void);
void lcd_clear(void);
void data_write (unsigned char data);
void putchar(int c);
void putstring(char *string);
void init_lcd(void);
int lcdgotoxy( unsigned int x, unsigned int y);
void delay(int count);
void test_cmd(void);


void wait_lcd (void)
{
		IO0DIR &= ~flag;
		IO0CLR |= rs;
		IO0SET |= rw | en;
		while(IO0PIN & flag);
		IO0CLR |= en | rw;
	    IO0DIR |= data_mask;
		delay(100);
	}
//void wait_lcd( void )
//{
//  LCD_DATA_DIR &= ~LCD_BUSY_FLAG;
//  LCD_CTRL_CLR |=  LCDRS;    
//  LCD_CTRL_SET |=  LCDRW |LCDEN;
//  while(LCD_IO0PIN & LCD_BUSY_FLAG);        /* wait for busy flag to become low */
//  
//  LCD_CTRL_CLR |= LCDEN | LCDRW;
//  LCD_DATA_DIR |= LCD_DATA_MASK;
//  
//  delay_1(100);  
// }


void command_write(unsigned char command)
{
	unsigned char temp=0;
	unsigned int temp1=0;

	temp=command;
	temp=(temp>>4)&0x0F;
	temp1=(temp<<18)&data_mask;

	IO0CLR = rs;
	IO0SET = en;
	IO0CLR = data_mask;
	IO0SET = temp1;
	delay(100000);
	IO0CLR = en;

	temp=command;
	temp&=0x0F;
	temp1=(temp<<18)& data_mask;
	delay(100*2);

	IO0CLR |= rs;
	IO0SET |= en;
	IO0CLR = data_mask;
	IO0SET = temp1;
	delay(100000);
	IO0CLR |= en;
	
	wait_lcd();
}

//void lcd_command_write( unsigned char command )
//{
//  unsigned char temp=0;
//  unsigned int temp1=0;
//
//  temp=command;
//  temp=(temp>>4)&0x0F;
//  temp1=(temp<<18)&LCD_DATA_MASK;
//
//  LCD_CTRL_CLR = LCDRS;
//  LCD_CTRL_SET = LCDEN;
//  LCD_DATA_CLR = LCD_DATA_MASK;
//  LCD_DATA_SET = temp1;
//  delay_1(10000);
//  LCD_CTRL_CLR = LCDEN;
//
//  temp=command;
//  temp&=0x0F;
//  temp1=(temp<<18)&LCD_DATA_MASK;
//  delay_1(100*2);
//
//  LCD_CTRL_CLR |= LCDRS;
//  LCD_CTRL_SET |= LCDEN;
//  LCD_DATA_CLR = LCD_DATA_MASK;
//  LCD_DATA_SET = temp1;
//  delay_1(10000);    
//  LCD_CTRL_CLR |= LCDEN;
//  wait_lcd();
//}

void set_lcd_port_output(void)
{
	IO0DIR |= (en | rs | rw);
	IO0CLR |= (en | rs | rw);
	IO0DIR |= data_mask;
}

//void set_lcd_port_output( void )
//{
//  LCD_CTRL_DIR |= ( LCDEN | LCDRS | LCDRW );
//  LCD_CTRL_CLR |= ( LCDEN | LCDRS | LCDRW );    
//  LCD_DATA_DIR |= LCD_DATA_MASK;
//}

void lcd_clear(void)
{
	command_write(0x01);
}

//void lcd_clear( void)
//{
//  lcd_command_write( 0x01 );
//}

void data_write (unsigned char data)
{
	unsigned char temp=0;
	unsigned int temp1=0;

	temp=data;
	temp=(temp>>4)&0x0F;
	temp1=(temp<<18)& data_mask;

	IO0SET |= en|rs;
	IO0CLR = data_mask;
	IO0SET = temp1;
	IO0CLR |= en;

	temp=data;
	temp&=0x0F;
	temp1=(temp<<18)& data_mask;

	IO0SET |= en | rs;
	IO0CLR=data_mask;
	IO0SET=temp1;
	IO0CLR |= en;

	wait_lcd();
}

//void lcd_data_write( unsigned char data )
//{
//  unsigned char temp=0;
//  unsigned int temp1=0;
//
//  temp=data;
//  temp=(temp>>4)&0x0F;
//  temp1=(temp<<18)&LCD_DATA_MASK;
//
//  LCD_CTRL_SET |= LCDEN|LCDRS;
//  LCD_DATA_CLR = LCD_DATA_MASK;
//  LCD_DATA_SET = temp1;
//  LCD_CTRL_CLR |= LCDEN;
//
//  temp=data;
//  temp&=0x0F;
//  temp1=(temp<<18)&LCD_DATA_MASK;
//
//  LCD_CTRL_SET |= LCDEN|LCDRS;
//  LCD_DATA_CLR = LCD_DATA_MASK;
//  LCD_DATA_SET = temp1;
//  LCD_CTRL_CLR |= LCDEN;
//  wait_lcd();
//}

void putchar(int c)
{
	data_write(c);
}

void putstring(char *string)
{
	while ( *string != '\0' )
	{
	putchar( *string );
	string++;
	}
}

void init_lcd(void)
{
	set_lcd_port_output();
//	delay(100);
	command_write(0x28);
	command_write(0x02);
	command_write(0x06);
	command_write(0x0F);
}

int lcdgotoxy( unsigned int x, unsigned int y)
{
	int retval=0;
	if ((x > 1) && (y > 15))
	{
	retval=-1;
	}	else {
	if( x==0 )
	{
	command_write(0x80 + y);
	}	else   
	if (x==1)
	{
	command_write(0xC0 + y);
	}
	}
	return retval;
}

//int lcd_gotoxy( unsigned int x, unsigned int y)
//{
//  int retval = 0;
//  
//  if( (x > 1) && (y > 15) )
//  {
//    retval = -1;
//  } else {
//  if( x == 0 )
//  {
//    lcd_command_write( 0x80 + y );        /* command - position cursor at 0x00 (0x80 + 0x00 ) */
//  } else if( x==1 ){
//    lcd_command_write( 0xC0 + y );        /* command - position cursor at 0x40 (0x80 + 0x00 ) */
//    }
//   }
//   return retval;
//}

void delay(int count)
{
	int j=0,i=0;

	for(j=0;j<count;j++)
	{
	for(i=0;i<65;i++);
	}
}

//int main(void)
//{
//init_lcd();
//	command_write(1);
//	command_write(0x80);
//	putstring("pulse");
//	delay(200000);
//	command_write(0xC0);
//    putstring("Bengaluru");
//    while(1)
//    {
//    command_write(0x1C);
//     delay(200000);
//    }
//}

//int main(void)
//{
//init_lcd();
////    init_adc0();
//    //uart1_init();
//    lcd_command_write(1);
////    lcd_putchar("A");
////lcd_command_write(0x18);
//lcd_command_write(0x80);
//    lcd_putstring("Shilpa");
// delay_1(200000);
//    lcd_command_write(0xC0);
//    lcd_putstring("Bengaluru");
//        while(1)
//    {
//    lcd_command_write(0x1C);
//        delay_1(2000000);
//    }

void test_cmd(void)
{
//    lcd_command_write(0x38);   // Interface length 8-bit 2 line
    command_write(0x08);   //Display off,cursor off
    command_write(0x18);
    command_write(0x0C);   //Display on cursor off
    command_write(0x10);   //Move cursor left by 1 char
    command_write(0x14);   //Move cursor right by 1 char
    command_write(0x01);   //Clear display

}	


#include<lpc214x.h>
#include"TIMER.H"
#include"ULTRASONIC.H"
#include"LCD.h"
#include "Serial.h"
#include "string.h"
 #include "adc.h"
//#include "dcmotor.h"
#define count 50

short adc_value = 0; 	 //PIN 29 ADC 
unsigned char WIFI_D[20];
unsigned char buff[60];
/**************************/
 unsigned int time=0,speed1=0;
 int d1,d2;
#define delay for(i=0;i<65000;i++);
#define switch1 (1<<25)
#define switch2 (1<<21)
 #define switch3 (1<<20)
 #define ir (1<<16)
 #define vibration (1<<18)
#define buzzer (1<<21)
//#define LED (1<<16)
#define IN_1 (1<<19)
#define IN_2 (1<<20)
#define IN_3 (1<<17)
#define IN_4 (1<<16)
#define RADAR (1<<15)
unsigned int range=0,i;
unsigned char temp[50];
void ultasonic(void);
void forward(void);
void reverse(void);
void right(void);
void left(void);
int speed=0;
void pwm_init(void);
void accident(void);
void stop(void);
void get_location();
/******************************************/
void sendsms(char *string);
/*****************************************/

void delay_for(unsigned int c)
{
int i,j;
for(i=0;i<c;i++)
for(j=0;j<1000;j++);
}
int check=0;
int main()
{

IO0DIR &=~ switch1;
IO0DIR &=~ RADAR;
IO0DIR &=~ switch2;
IO0DIR &=~ switch3;
IO1DIR &=~ ir;
IO0DIR &=~ vibration;
IO1DIR|=buzzer;
IO0DIR|=IN_1;								
IO0DIR|=IN_2;
IO0DIR|=IN_3;								
IO0DIR|=IN_4;
IO1CLR|=buzzer;
IO0CLR|=IN_1;
IO0CLR|=IN_2;
IO0CLR|=IN_3;
IO0CLR|=IN_4;
uart1_init();
//init_adc0();
sendsms("AI ROBO");
ultrasonic_init();
pwm_init();
 //sendsms("check me");
	while(1)
	{
	
	accident();
	
	ultasonic();
	if(!(IO0PIN & switch1))
	{
		//	uart1_puts("Accelerator");
	forward();
	ultasonic();
//	accident();
	}
	if(!(IO0PIN & switch3))
	{
	//uart1_puts("Reverse");
	reverse();
	//ultasonic();
	}
	if(!(IO0PIN & switch2))
	{
		//	uart1_puts("Brake");
	stop();
	PWMMR1=0;			//PWMMR4 and pwmr1 for po.0 for p0.8PWM CLEAR	 DUTY CYCLE VARY FOR THE DUTY CYCLE TO VARY THE VOLTAGE BY PULSE	ITS .02 MS TOFF SO CAN USE 
	PWMLER = (1<<0)|(1<<1); // update MR0 and MR4
	IO1SET|=buzzer;
	delay_for(2000);
	IO1CLR|=buzzer;
	delay_for(2000);
	}
}
}

void ultasonic(void)
{
int dis=0;
char tmp[10];
//	VPBDIV=0x00;
//uart1_init();
//uart1_puts("ULTRA");
range=get_range();

sprintf(tmp,"get:%d",range);
 //accident();
if((IO1PIN & ir))
	{
	pwm_init();
	PWMMR1=0;			//PWMMR4 and pwmr1 for po.0 for p0.8PWM CLEAR	 DUTY CYCLE VARY FOR THE DUTY CYCLE TO VARY THE VOLTAGE BY PULSE	ITS .02 MS TOFF SO CAN USE 
	PWMLER = (1<<0)|(1<<1); // update MR0 and MR4
	IO1SET|=buzzer;
	delay_for(500);
	IO1CLR|=buzzer;
	delay_for(500);
	}
//uart1_puts(tmp);
//delay_for(10000);
//			sprintf(temp,"DIS:%d",range);
if(((IO0PIN & RADAR)) && (range < 5))	   //((IO0PIN & RADAR)) && 
{
pwm_init();
stop();
PWMMR1=speed;			//PWMMR4 and pwmr1 for po.0 for p0.8PWM CLEAR	 DUTY CYCLE VARY FOR THE DUTY CYCLE TO VARY THE VOLTAGE BY PULSE	ITS .02 MS TOFF SO CAN USE 
PWMLER = (1<<0)|(1<<1); // update MR0 and MR4
IO1SET|=buzzer;
delay_for(500);
IO1CLR|=buzzer;
delay_for(2000);
//uart1_puts("Radar");
//uart0_puts("stop");
}
if((range > 5) && (range < 15))
{
pwm_init();
speed = (range -5 )*10;
PWMMR1=speed;			//PWMMR4 and pwmr1 for po.0 for p0.8PWM CLEAR	 DUTY CYCLE VARY FOR THE DUTY CYCLE TO VARY THE VOLTAGE BY PULSE	ITS .02 MS TOFF SO CAN USE 
PWMLER = (1<<0)|(1<<1); // update MR0 and MR4
//forward();
reverse();	
}
else if((range > 15))
{
T0CTCR=0;
T0PR=15-1;
T0TC=T0PC=0;
T0TCR=0x01;
d1=range;
delay_for(5000);
time=T0TC;
T0TCR=0;
d2=get_range();
time=time+(d2*59);
d2=(d1+d2)/2;
speed1=((d2*1000000)/time);
uart1_init();
sprintf(tmp,"Distance:%d",d2);
uart1_puts(tmp);
sprintf(tmp,"Time:%u",time);
uart1_puts(tmp);
sprintf(tmp,"speed:%u\n",speed1);
uart1_puts(tmp);
//check=1;
pwm_init();
PWMMR1=speed1;			//PWMMR4 and pwmr1 for po.0 for p0.8PWM CLEAR	 DUTY CYCLE VARY FOR THE DUTY CYCLE TO VARY THE VOLTAGE BY PULSE	ITS .02 MS TOFF SO CAN USE 
PWMLER = (1<<0)|(1<<1); // update MR0 and MR4
reverse();	
//forward();
if(speed1>90)
{
pwm_init();
uart1_puts("Over Speed");
PWMMR1=90;			//PWMMR4 and pwmr1 for po.0 for p0.8PWM CLEAR	 DUTY CYCLE VARY FOR THE DUTY CYCLE TO VARY THE VOLTAGE BY PULSE	ITS .02 MS TOFF SO CAN USE 
PWMLER = (1<<0)|(1<<1); // update MR0 and MR4
forward();
}
}
else 
{
PWMMR1=99;			//PWMMR4 and pwmr1 for po.0 for p0.8PWM CLEAR	 DUTY CYCLE VARY FOR THE DUTY CYCLE TO VARY THE VOLTAGE BY PULSE	ITS .02 MS TOFF SO CAN USE 
PWMLER = (1<<0)|(1<<1); // update MR0 and MR4
}
}
void forward()
{
//	IO1SET|=EN_1;
IO0SET|=IN_1;
IO0CLR|=IN_2;
IO0SET|=IN_3;
IO0CLR|=IN_4;
//	uart0_puts("forward");
}

void reverse()
{
//	IO1SET|=EN_1;
IO0CLR|=IN_1;
IO0SET|=IN_2;
IO0CLR|=IN_3;
IO0SET|=IN_4;
	
//	uart0_puts("reverse");
}
void right()
{
//	IO1SET|=EN_1;
IO0CLR|=IN_2;
IO0SET|=IN_1;
IO0CLR|=IN_3;
IO0CLR|=IN_4;
//	uart0_puts("right");
}
void left()
{
//	IO1SET|=EN_1;
IO0CLR|=IN_4;
IO0SET|=IN_3;
IO0CLR|=IN_1;
IO0CLR|=IN_2;
//	uart0_puts("left");
}

void stop()
{
//	IO1SET|=EN_1;
IO0SET|=IN_1;
IO0SET|=IN_2;
IO0SET|=IN_3;
IO0SET|=IN_4;
//	uart0_puts("stop");
}
void pwm_init(void)	//1ms  at 60Mhz			   PWMINT(INT A); PWMMR4=A; DELAY(1000);  PWMINT(10);
{

PINSEL0=0x00000002;//0x00020000;	//P0.0 as PWM /	 pin p0.8 is 0x00020000
PWMPR=59;		 // 1 ms resolution
PWMMR0=100;			//PWM RATE	   DUTY CYCLE	 CALCULATE ACCORDING TO    TON FOR .1MS  CAN INCRESASE THIS TIME FOR TON 
PWMMR1=80;			//PWMMR4 and pwmr1 for po.0 for p0.8PWM CLEAR	 DUTY CYCLE VARY FOR THE DUTY CYCLE TO VARY THE VOLTAGE BY PULSE	ITS .02 MS TOFF SO CAN USE 
//	PWMLER = (1<<0)|(1<<4); // update MR0 and MR4
PWMMCR=0x00000400;	//PWM4 RESET	   CAN RESET PWMMCR=(1<<1);
PWMTCR=0X00000002;        //COUNTER RESET
PWMPCR=0x0200;		//PWM4 ENABLED	  	PWMPCR=0x1000;
PWMTCR=0X09; 

}

void accident(void)
{
if((IO0PIN & vibration))
	{
//	sendsms("Accident Occured");
	IO1SET|=buzzer;
	delay_for(500);
	IO1CLR|=buzzer;
	delay_for(500);
	get_location();
//	uart1_puts("Accident Detected");
//	sendsms("Accident Occured");
	}


}

void sendsms(char *string)
{
	uart1_init();
    uart1_puts("AT");
	uart1_puts("\r");
	delay_for(2000);

	uart1_puts("AT+CMGF=1");
	uart1_puts("\r");
	delay_for(2000);
							
	uart1_puts("AT+CMGS=\"8792592135\"");
	uart1_puts("\r");
	delay_for(2000);

	uart1_puts(string);
	uart1_putc(26);
	delay_for(2000);
}

void get_location()
{
	unsigned char ch[100];
	char tmp[300];
	uart1_init();
//	uart0_init();
//	uart0_puts("AT");
//	uart0_puts("\r");
//	delay_for(2000);


    uart1_puts("AT");
	uart1_puts("\r");
	delay_for(2000);

	uart1_puts("AT+CGPSPWR=1");
	uart1_puts("\r");
	delay_for(2000);
							
	uart1_puts("AT+CGPSRST=0");
	uart1_puts("\r");
	delay_for(2000);

	uart1_puts("AT+CGSINF=0 ");
	uart1_puts("\r");
	delay_for(2000);
//
	uart1_puts("AT+CGPSSTATUS?");
	uart1_puts("\r");
	delay_for(2000);

	uart1_puts("AT+CGPSINF=0");
	uart1_puts("\r");
	uart1_puts("INFO GOT.....");
//	for(i=0;i<32;i++)
//	{
//	ch[i]=uart1_getkey();
//	uart1_putc(ch[i]);
//	//sprintf(tmp,"%c",ch);
//	}
	uart1_puts("ch");
	uart1_puts(ch);
//	delay_for(2000);
//	uart1_puts(ch);
//	uart1_puts("AT+CGPSINF=32");
//	uart1_puts("\r");
	
//	uart1_puts(tmp);
	delay_for(2000);
	//strcat(ch,"Accident Occured @");
//	sendsms(ch);
//	uart1_puts(ch);
	sendsms("Accident Occured @ https://www.google.com/maps/?q=12.8794,77.5443");
//	strcat(ch,"Accident Occured @");
//	sendsms(ch);
	delay_for(2000);

}


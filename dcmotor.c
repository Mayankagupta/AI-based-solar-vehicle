#include<LPC214x.h>
#include "dcmotor.h"

//#define EN_1 (1<<16)

//#define count 5000

void forward()
{
//	IO1SET|=EN_1;
	IO1SET|=IN_1;
	IO1CLR|=IN_2;
	IO1SET|=IN_3;
	IO1CLR|=IN_4;
//	uart0_puts("forward");
}

void reverse()
{
//	IO1SET|=EN_1;
	IO1CLR|=IN_1;
	IO1SET|=IN_2;
	IO1CLR|=IN_3;
	IO1SET|=IN_4;
//	uart0_puts("reverse");
}
void right()
{
//	IO1SET|=EN_1;
	IO1CLR|=IN_2;
	IO1SET|=IN_1;
	IO1CLR|=IN_3;
	IO1CLR|=IN_4;
//	uart0_puts("right");
}
void left()
{
//	IO1SET|=EN_1;
	IO1CLR|=IN_4;
	IO1SET|=IN_3;
	IO1CLR|=IN_1;
	IO1CLR|=IN_2;
//	uart0_puts("left");
}

void stop()
{
//	IO1SET|=EN_1;
	IO1CLR|=IN_1;
	IO1CLR|=IN_2;
	IO1CLR|=IN_3;
	IO1CLR|=IN_4;
//	uart0_puts("stop");
}


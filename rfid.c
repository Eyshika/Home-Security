#include<regx52.h>

#define rs P1_0

#define en P1_1

#define valid P2_3
#define nvalid P2_4
 



//DELAY INITIALIZATIONS
void delay(unsigned int );
//LCD INTIALIZATIONS
void lcd_init();
void enable();
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_str(unsigned char *);
void lcd_reset();
//SERIAL INITIALIZATION
void serial_init();
void serial_tx(unsigned char);
unsigned char serial_rx(void);

void compare();

unsigned char tag[12];
unsigned char tags[4][12]={"4D00166D6E58","4D0016530800","4D0016524F46","4D00163F2A4E"};//{"TAG1,"TAG2","TAG3","TAG4"}
unsigned int j,k,l,t,a;
unsigned char rec;
bit c,f,d;

void main()
{

serial_init();
lcd_init();
c=0;j=0;
d=0;t=5;
valid=nvalid=1;
//buzz=1;
lcd_str("PLACE YOUR CARD");
P0=0xff;

while(1)
{
 if(c==1)
 {	lcd_reset();
   for(l=0;l<12;l++)
   {
   lcd_data(tag[l]);
   }
   compare();
   if(d==1)
   {
	 lcd_reset();
	 lcd_str("WELCOME ");
	 //lcd_data(t+48);
	if(t==0){lcd_str("EYSHIKA");}//Put the name1
	if(t==1){lcd_str("NISHA");}//Put the name2
    if(t==2){lcd_str("TARUN");}//Put the name2
	if(t==3){lcd_str("RANJAI");}//Put the name2
	lcd_cmd(0xc0);
	lcd_str("ACSESS  GRANTED");
	valid=0;//buzz=0;delay(100);//buzz=1;
	delay(2000);
	valid=1;
	 }
	 else
	 {
	 lcd_reset();
	 nvalid=0;//buzz=0;
	 lcd_str("GET OUT YOU ARE");
	 lcd_cmd(0xc0);
	 lcd_str("NOT AUTHORIZED");
	 delay(3000);
	 nvalid=1;//buzz=1;
	  } d=0;
 	
c=0;
delay(200);
lcd_reset();
lcd_str("PLACE YOUR CARD");
}



}
}
 


void compare()
{t=5;
for(k=0;k<4;k++)	   
	{
	for(l=0;l<12;l++)
		{
		 if(tag[l]==tags[k][l])
		 	{
			//lcd_data('1');
			 f=1;
			}
			else
			{
			//lcd_data('0');
			f=0;break;
			}
			
		}
		if(f==1)
		{d=1;t=k;}
	}
}



void serial_isr() interrupt 4
{
if(RI==1)
{ 
rec=serial_rx();
serial_tx(rec);
tag[j]=rec;
j++;

 if(j==12)
 {c=1;j=0;
 }
}							  
}



void serial_init()
{
TMOD|=0x20;
SCON=0x50;
TH1=0xFD;
TR1=1;
IE=0x90;
}

void serial_tx(unsigned char x)
{
SBUF=x;
while(TI==0);
TI=0;
}


unsigned char serial_rx(void)
{
unsigned char a;
while(RI==0);
a=SBUF;
RI=0;
return a;
}

void delay(unsigned int msec)
{
unsigned int i;
for(i=0;i<msec;i++)
{
TMOD|=0x01;
TH0=0xFC;
TL0=0x066;
TR0=1;
while(TF0==0);
TF0=0;
}
}

void lcd_str(unsigned char *str)
{
while(*str!='\0')
{
lcd_data(*str);
str++;
}
}


void enable()
{
en=1;
delay(2);
en=0;
}

void lcd_init()
{
lcd_cmd(0x38);
lcd_cmd(0x0e);
lcd_cmd(0x01);
lcd_cmd(0x80);
delay(256);
}

void lcd_cmd(unsigned char x)
{
rs=0;//rw=0;
P2=x;
enable();
}

void lcd_data(unsigned char x)
{
rs=1;//rw=0;
P2=x;
enable();
}

void lcd_reset()
{
lcd_cmd(0x01);
lcd_cmd(0x80);
}
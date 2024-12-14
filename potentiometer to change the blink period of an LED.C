//Use a potentiometer to change the blink period of an LED
#include<p18f4580.h>
#define RS PORTCbits.RC0
#define EN PORTCbits.RC1


void Data(char Value);             
void Cmd(char Value);            
void delay();
void init();					
long map(long x,long in_min,long in_max,long out_min,long out_max);


void main()
{
	int value,i,convert_value;
	char ADC_str[5];
	
	TRISC=0X00;  // PORTC as output
	TRISD=0X00;  // PORTD as output
	TRISAbits.RA0=1;  // PORTA RC0 Pin as Input
	
	//lcd initialization
	init();
	
	//TIMER2 use for PWM
	T2CON=0X00;
	CCP1CON=0X0C;
	TMR2=0X00;
	PR2=199;
	CCPR1L=1;
	
	//ADC configure
	ADCON0=0X03;
	ADCON1=0X0D;
	ADCON2=0X88;
	
	T2CONbits.TMR2ON=1;
	while(1)
	{
		ADCON0bits.GO=1;
		while(ADCON0bits.GO==1);  // wait for conversion
		value=ADRESH;
		value=value<<8;
		value=value|ADRESL; // store ADC Value 
		
		convert_value= map(value,0,1023,0,199);  //map ADC value to PWM duty cycle(0–199)
		CCPR1L=convert_value;			       	//udate PWM duty cylce
		
		//Debug: Display mapped PWM value on the LCD
		for(i=0;i<=3;i++)
    	{
			ADC_str[i]=convert_value%10+'0';   //Convert the result into ASCII
			convert_value=convert_value/10;
		}
		Cmd(0x80);
		for(i=3;i>=0;i--)
	    {
			Data(ADC_str[i]);            //Display the result on LCD
		}
	}
}


long map(long x,long in_min,long in_max,long out_min,long out_max)  
{
	return (x - in_min)*(out_max-out_min+1)/(in_max-in_min+1)+out_min;
}


void delay()
{
	int i,j;
	for(i=0;i<100;i++)
	for(j=0;j<100;j++);
}



void Cmd(char Value){
     PORTD=Value;
     RS=0;             // Select command Registr
     EN=1;             // Enable Line ON
     delay();          
     EN=0;             // Enable Line OFF
}	
 
void Data(char Value){
     PORTD=Value;
     RS=1;                   //Select Data Registr
     EN=1;                   //[Enable Line ON]
     delay();                
     EN=0;                   //Enable Line OFF
}

void init()        // lcd initialization funtion
{
	Cmd(0x01);
	Cmd(0x38);
	Cmd(0x0E);
	Cmd(0x80);
}
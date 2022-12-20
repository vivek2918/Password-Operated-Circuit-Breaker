#include<reg51.h>

// P1 connected to LCD data pins.
#define lcd P1

// pins P3.0 and P3.2 to rs and e pins of LCD display.
sbit rs=P3^0;
sbit e=P3^2;

//// pins P0.0, P0.1, P0.2 and P0.3 for controlling 4 seperate relays
sbit load1=P0^0;
sbit load2=P0^1;
sbit load3=P0^2;
sbit load4=P0^3;

// initialize P2.0 to P2.3 to row 0 to row 3, 
// and P2.4 to P2.7 to col 0 to col 3 of the keypad.
sbit r0=P2^0;   
sbit r1=P2^1;
sbit r2=P2^2;
sbit r3=P2^3;
sbit c0=P2^4;   
sbit c1=P2^5;
sbit c2=P2^6;
sbit c3=P2^7;

//variable key is the 2 dimentional array which hold the data from 0 to 9.
unsigned char key[4][3]={0,1,2,3,4,5,6,7,8,9};

// initializing all used function globally.
void delay (int);
void cmd (unsigned char);
void display (unsigned char);
void string (char *);
void init (void);
unsigned char keypress (void);


// delay function which produce approx of 1ms delay if d is 1.
// so, if d is 1000 it produce approx 1sec delay.
void delay (int d)
{
	unsigned char i=0;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--);
		for(i=248;i>0;i--);
	}
}

// here, cmd function is used to give commands to the lcd.
// like, cursor location, clear display, selectings modes, memory locations (DDRAM & CGRAM) etc....
void cmd (unsigned char c)
{
	lcd=c;
	rs=0;
	e=1;
	delay(5);
	e=0;
}

// display function is used to give data to the lcd, which is to be displayed.
// usually a single character.
void display (unsigned char c)
{
	lcd=c;
	rs=1;
	e=1;
	delay(5);
	e=0;
}

// string function is used display the string characters (words).
// this function will work by keep on calling the display function to display the individual character in a word,
// untill it detect '/0'. usually an end of string (word).
void string (char *p)
{
	while(*p)
	{
		display(*p++);
	}
}

//the function will holds a set of primary commands to display which is used to initialize it.
void init (void)
{
	cmd(0x38);
	cmd(0x01);
	cmd(0x0c);
	cmd(0x80);
}

// this function will return the exact number of the corresponding key pressed 
// in the keypad.
unsigned char keypress (void)
{
	unsigned char row=-1,col=-1;
  r0=r1=r2=r3=0;// first make all row as 0 (low)
	c0=c1=c2=1;// and all coloumn as 1 (high).
	while(c0&c1&c2); // this statement will wait until any of the coloumn becomes low.
	                    // usually used for keypress detection.
	                    // as all row are 0, if any key was pressed the corresponding coloumn become low from high, this makes the condition false.
	                     
	// NOTE : usually a human takes a few milli seconds to release the key once he pressed, 
	//        but the below statement will complete in micro seconds.            
	
	
	
	r0=0;// now make one row to 0 (low)
	r1=r2=r3=1; // and the remaining to 1 (high).
	
	if((c0&c1&c2)==0)// now check if any of the coloum becomes zero.
		                  // if yes then we find the row, ie) row 0.
	{
		row=0;            
		goto check;       // now call the check function to find the coloumn
	}
	r1=0;
	r0=r2=r3=1;
	if((c0&c1&c2)==0)
	{
		row=1;
		goto check;
	}
	r2=0;
	r1=r0=r3=1;
	if((c0&c1&c2)==0)
	{
		row=2;
		goto check;
	}
	r3=0;
	r1=r2=r0=1;
	if((c0&c1&c2)==0)
	{
		row=3;
		goto check;
	}
	
	// this function will go through each coloumn individually, by checking which coloumn become 0 (low)
	check: 
	if(c0==0)
		col=1;
	else if(c1==0)
		col=2;
	else if(c2==0)
		col=3;
	// untill here it takes only few micro seconds, at this time we still releasing the key..
	delay(20);
	
	while(!(c0&c1&c2)); // this statement will wait until the key completly released. 
	return key[row][col]; // here this function will return the corresponding key value to the called function.
	                      // for eg, row=0 col=1. then key[0][1]=1. 
}
void main()
{
	
	char i=0;
  char a=1,b=1,c=1,d=1;
	
	// by changing the temp1, temp2....,temp4 value, the default password can be changed.
	// here [4] is the length of the password.
	int temp1[4]={1,3,2,4};
	int temp2[4]={2,3,6,5};
	int temp3[4]={1,2,3,4};
	int temp4[4]={8,9,7,1};
	int temp[4];
	
	P0=0x00;// make PORT 0 pins to LOW. 
	         // which avoid the loads should turn on, when the program starts. because this PORT is connected to load.
	
	init();// call init function to to initialize the display.
	delay(1500);
	string("    Wel Come    ");
	cmd(0xc0);
	string("                ");
	delay(1500);
	cmd(0x01);
	cmd(0x80);
	string(" Password Based ");
	cmd(0xc0);
	string(" Circuit Breaker");
	delay(1500);
	cmd(0x01);
  while(1)
	{
		a=~a;
		b=~b;
		c=~c;
		d=~d;
		
		bala:
		cmd(0x80);
		string(" Enter Password ");
		cmd(0xc0);
	  
		for(i=0;i<4;i++) // the number 4 nothing but the length of password, here the length is 4. ie, temp1[4].
		{
	 		temp[i]=keypress();// here we calling the function keypress to get the key value, which we going to press.
	                       // to know how this function works, go to the function written below. 
		  display(temp[i]+48);// for each keypress the corresponding value will be displayed.
	                        // to print on lcd we need to send ascii value, for eg, if temp1[1] is 2 means, we need to send 2+48 ie, 49 to the lcd.
			                    // ascii value of 49 is nnumber 2, same as for 50 is 3 and so on.	
		}
		
		
		for(i=0;i<4;i++) // the number 4 nothing but the length of password, here the length is 4. ie, temp1[4].
		{
			if(temp1[i]!=temp[i])// if this condition fails, LOAD 1 will ON, if it was first
				                   // time and OFF for the second time. This will repeat again and again.
			                     
                           // if the condition is false it will compare to the next next password for LOAD 2, LOAD 3  and LOAD 4.
			{
				for(i=0;i<4;i++)
		     {
			     if(temp2[i]!=temp[i])
			      {
							for(i=0;i<4;i++)
		           {  
			           if(temp3[i]!=temp[i])
			            {
										for(i=0;i<4;i++)
		                 {  
			                 if(temp4[i]!=temp[i])
			                  {
													 cmd(0x80);
				                   string(" Wrong Password ");
													 cmd(0xc0);
													 string("                ");
													 delay(1000);
													 goto bala;
												}
												
											}
										       a=~a;
													 if(a==1)
													 {
													 cmd(0x80);
													 string("   LOAD 4 ON    ");
													 cmd(0xc0);
													 string("                ");
				                   load4=1;
													 delay(1000);
													 goto bala;// this statement helps to go back to the starting point of the program.
													 }
													 else
													 {
													 cmd(0x80);
													 string("   LOAD 4 OFF   ");
													 cmd(0xc0);
													 string("                ");
				                   load4=0;
													 delay(1000);
													  goto bala;
													 }
					
										}
									
									}
							              b=~b;
													 if(b==1)
													 {
													 cmd(0x80);
													 string("   LOAD 3 ON    ");
													 cmd(0xc0);
													 string("                ");
				                   load3=1;
													 delay(1000);
														  goto bala;
													 }
													 else
													 {
													 cmd(0x80);
													 string("   LOAD 3 OFF   ");
													 cmd(0xc0);
													 string("                ");
				                   load3=0;
													 delay(1000);
														  goto bala;
													 }
					
								}
						
							}
				                    c=~c;
													 if(c==1)
													 {
													 cmd(0x80);
													 string("   LOAD 2 ON    ");
													 cmd(0xc0);
													 string("                ");
				                   load2=1;
													 delay(1000);
														  goto bala;
													 }
													 else
													 {
													 cmd(0x80);
													 string("   LOAD 2 OFF   ");
													 cmd(0xc0);
													 string("                ");
				                   load2=0;
													 delay(1000);
														  goto bala;
													 }
					
						}
			
					}
		                        d=~d;
													 if(d==1)
													 {
													 cmd(0x80);
													 string("   LOAD 1 ON    ");
													 cmd(0xc0);
													 string("                ");
				                   load1=1;
													 delay(1000);
														  goto bala;
													 }
													 else
													 {
													 cmd(0x80);
													 string("   LOAD 1 OFF   ");
													 cmd(0xc0);
													 string("                ");
				                   load1=0;
													 delay(1000);
														  goto bala;
													 }
					
		
}			
}													
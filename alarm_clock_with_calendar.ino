#include <Wire.h>
#include<EEPROM.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2); 
RTC_DS1307 RTC;
int tmp,Inc,hor,mIn,add=11; 
int set=8;  
int cge=9;   
int mod=10; 
int off=0;
float temp;
int tempPin = 0;
#define buz 7 
int Hor,Min,Sec; 
///////////////////////////////////////Function to adjust the time//////////////////////////////////
void time()                             
{
    int tmp=1,mins=0,hors=0,secs=0;
    while(tmp==1)
    {
     off=0;
     if(digitalRead(cge)==0)
     {
      Hor++;
      if(Hor==24)
      {
       Hor=0;
      }
     }
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Set Alarm Time ");

    lcd.setCursor(0,1);
    if(Hor<=9)
    lcd.print("0");
    lcd.print(Hor);
    lcd.print(":");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    delay(200);
    lcd.setCursor(0,1);
    lcd.print("  ");
    lcd.print(":");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    delay(200);
    if(digitalRead(set)==0)
    {
      hor=Hor;
      EEPROM.write(add++,hor);
     tmp=2;
     while(digitalRead(set)==0);
    }
    }
    
    while(tmp==2)
    {
     if(digitalRead(cge)==0)
     {
      Min++;
      if(Min==60)
      {Min=0;}
     }
    lcd.setCursor(0,1);
    lcd.print(Hor);
    lcd.print(":");
     if(Min<=9)
    lcd.print("0");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    lcd.print("  ");
    delay(200);
     lcd.setCursor(0,1);
    lcd.print(Hor);
    lcd.print(":");
    lcd.print("  ");
    lcd.print(":");
    lcd.print(Sec);
    lcd.print("  ");
    delay(200);
      if(digitalRead(set)==0)
      {
       mIn=Min;
       EEPROM.write(add++, mIn);
       tmp=0;
       while(digitalRead(set)==0);
      }
    }
    off=1;
    delay(10);
}
///////////////////////////////////////////function to sound the buzzer//////////////////////////////////
void Buz()
{
  if(digitalRead(set)==0)
  off=0;
  if(off==1)
  {
   digitalWrite(buz,HIGH);
   delay(500);
  
   digitalWrite(buz, LOW);
   delay(500);
  }
}
//////////////////////////////////////////function to compare the alarm time with current RTC time//////////////////
void TimeCheck()
{
  int tem[17];
  for(int i=11;i<17;i++)
  {
    tem[i]=EEPROM.read(i);
  }
  if(Hor == tem[11] && Min == tem[12] && off==1) 
  {
   add=11;
   Buz();
   Buz();
   lcd.clear();
   lcd.print("alarm...........");
   lcd.setCursor(0,1);
   lcd.print("...........alarm");
   Buz();
   Buz();
    }
}
////////////////////////////////////////////////////////////setup///////////////////////////
void setup()
{
 Wire.begin();
 RTC.begin();
 lcd.begin(16,2);
 pinMode(cge, INPUT);
 pinMode(set, INPUT);
 pinMode(mod, INPUT);
 pinMode(buz, OUTPUT);
 digitalWrite(set, HIGH);
 digitalWrite(mod, HIGH);
 digitalWrite(cge, HIGH);
 
   lcd.setCursor(0,0);
   lcd.print("Shikhar Jain");
   lcd.setCursor(0,1);
   lcd.print("  Alarm Clock  ");
   delay(2000);
    
 if(!RTC.isrunning())
 {
 RTC.adjust(DateTime(__DATE__,__TIME__));
 }
}
////////////////////////////////////////////////////////////loop/////////////////////////////////////
void loop()
{
   DateTime now = RTC.now();
   if(digitalRead(mod) == 0)      
   { 
    current();
    time();
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Alarm On");
    delay(2000);
   }
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(" ");
 Hor=now.hour(),DEC; 
 if(Hor<=9)
 {
  lcd.print("0");
  lcd.print(Hor=now.hour(),DEC);
 }
 else
 lcd.print(Hor=now.hour(),DEC);
 lcd.print(":");
 Min=now.minute(),DEC;
 if(Min<=9)
 {
 lcd.print("0");
 lcd.print(Min=now.minute(),DEC);
 }
 else
 lcd.print(Min=now.minute(),DEC);
 
 lcd.print(":");
 Sec=now.second(),DEC;
 if(Sec<=9)
 {
  lcd.print("0");
  lcd.print(Sec=now.second(),DEC);
 }
 else
 lcd.print(Sec=now.second(),DEC);

temp = analogRead(tempPin);

temp = temp * 0.48828125;

lcd.setCursor(12, 0);
lcd.print((int)temp);
lcd.print((char)223);
lcd.print("C"); 
 
 lcd.setCursor(0,1);
 lcd.print(" ");
 lcd.print(now.day(),DEC);
 lcd.print("/");
 lcd.print(now.month(),DEC);
 lcd.print("/");
 lcd.print(now.year()%100,DEC);

 int da=now.day();
 int m=now.month();
 int C=now.year();
 lcd.print(" ");

int leap;
 if ((C % 100 == 0) && (C % 400 != 0))
      leap = 0;
   else if (C % 4 == 0)
      leap = 1;
   else
      leap = 0;
 
   int fmonth = 3 + (2 - leap) * ((m + 2) / (2 * m))
         + (5 * m + m / 9) / 2;
 
   //bring it in range of 0 to 6
   fmonth = fmonth % 7;

   int dayOfWeek;
   int YY = C % 100;
   int century = C / 100;
 
   dayOfWeek = 1.25 * YY + fmonth + da - 2 * (century % 4);
 
   //remainder on division by 7
   dayOfWeek = dayOfWeek % 7;
 
   switch (dayOfWeek) {
      case 0:
         lcd.print("Sat");
         break;
      case 1:
         lcd.print("Sun");
         break;
      case 2:
         lcd.print("Mon");
         break;
      case 3:
         lcd.print("Tues");
         break;
      case 4:
         lcd.print("Wed");
         break;
      case 5:
         lcd.print("Thurs");
         break;
      case 6:
         lcd.print("Fri");
         break;
   }
 
 TimeCheck();
 delay(200);
}
///////////////////////////////////////////////////////function to get current RTC time//////////////////////////////
void current()
{
  lcd.setCursor(0,1);
  lcd.print(Hor);
  lcd.print(":");
  lcd.print(Min);
  lcd.print(":");
  lcd.print(Sec);
}

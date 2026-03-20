//*******************************************************************
#include <stdio.h>
#include "stm32l1xx.h"

//-------------------------------------------------------------------
#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include "adc.h"
#include "dac.h"
#include "port.h"
#include "pwm.h"
#include "math.h"

//*******************************************************************
//Structures, Globale Variablen und demos
typedef struct
{
	/**
	 * 0 Dc
	 * 1 Sinus
	 * 2 Sägezahn
	 * 3 Rechteck Testverhältnis 1:1
	 */
	int charak;
	/**
	 * 0 bis 1000 in Hz
	 */
	int frequenz;
	/**
	 * 0 bis 1500
	 * 1500=1,5V
	 */
	int amplitude;

	float zeitpunkt;

}aufbau;
typedef struct{
	float a0;
	float a1;
	float a2;
	float b1;
	float b2;
} Filterkoef;

typedef struct
{
	/**
	 * 0 Off
	 * 1 Tiefpass
	 * 2 Hochpass
	 * 3 Bandsperre
	 */
	int charak;
	/**
	 * 0 bis 100 in Hz
	 */
	float frequenz;
	/**
	 * 0 bis 10 Hz
	 *
	 */
	float Bandbreite;
	/**
	 * alle gemerkten Variablen
	 */
	Filterkoef koef;

	float x1;
	float x2;
	float y1;
	float y2;

}filter;




filter f1;
filter f2;
filter f3;
float faktor;
float offset;
int CYCLE_TIME=1000;
float PI=3.1415629;
float EULER= 2.71828;
float DeltaT=0.001;


//*******************************************************************
//Functions


float calculateFilterValue(filter *f, float x0)
{
	float y = f->koef.a0 * x0
			+ f->koef.a1 * f->x1
			+ f->koef.a2 * f->x2
			+ f -> koef.b1 * f->y1
			+ f -> koef.b2 * f->y2;

	f->x2 = f->x1;
		f->x1 = x0;
		f->y2 = f->y1;
		f->y1 = y;
		if(f->charak==0)
		{
			return x0;
		}
		return y;

}



float filter_berechnen(filter *f)
{
	Filterkoef fk= {0.0,0.0,0.0,0.0,0.0};

	float f0 = 0.0;
	float r = 0.0;
	float k = 0.0;
	switch(f->charak )
	{
	case 0: return 0;
	case 1:
		f0 = sqrt(2*PI*f->frequenz*DeltaT);
		r = exp(-f0);
		k = (1-(2*r) * cos(f0) + pow(r,2)) / 4;
		fk.a0 = k;
		fk.a1 = 2*k;
		fk.a2 = k;
		fk.b1 = 2*r*cos(f0);
		fk.b2 = -pow(r,2);
		f->koef = fk;

		return 0;
	case 2:
		f0 = sqrt(2*PI*f->frequenz*DeltaT);
		r = exp(-f0);
		k = (1+(2*r) * cos(f0) + pow(r,2)) / 4;
		fk.a0 = k;
		fk.a1 = -2*k;
		fk.a2 = k;
		fk.b1 = 2*r*cos(f0);
		fk.b2 = -pow(r,2);
		f->koef = fk;
		return 0;
	case 3:
		f0=2*PI*f->frequenz*DeltaT;
		r=1.0-3.0*f->Bandbreite*DeltaT;
		k=(1.0-2.0*r*cos(f0)+r*r)/(2.0-2.0*cos(f0));
		fk.a0 = k;
		fk.a1 = -2*k*cos(f0);
		fk.a2 = k;
		fk.b1 = 2.0*r*cos(f0);
		fk.b2 = -pow(r,2);
		f->koef = fk;
		return 0;


	}
	return -1;
}


int myIsr()
{

	//Offset für das Signal berechnen
	static const float gain=(float)0xFFF/3000;


	//Signal einlesen
	float signal_get=adcGet(3);
	dacSet(1,signal_get);
	signal_get=signal_get/gain-1500;


	//filtern
	float signal_get1=calculateFilterValue(&f1,signal_get);
	float signal_get2=calculateFilterValue(&f3,signal_get1);
	float signal_set=calculateFilterValue(&f2,signal_get2);
	signal_set=faktor*signal_set+offset;

	signal_set=gain*(signal_set+1500);
	//filter ausgeben
	if(signal_set>=0xFFF)
	{dacSet(2,0xFFF);}
	else if(signal_set<=0)
	{dacSet(2,0);}
	{dacSet(2,signal_set);}
}




int main(void)
{

  uartInit();
  lcdInit();
  dacInit(1);
  dacInit(2);
  adcInit(3);
  timerInit(1e6*DeltaT,myIsr);


  lcdPrintf( 0, 0, 20, __DATE__ "," __TIME__ );
  lcdPrintf( 1, 0, 20, "Hello test!" );
  int vorher;


  while( 1 )
  {
	  char *str = uartGetString();

	  vorher=f1.charak;
	  if(str)
	  {
		  switch(str[0])
		  {
		  case 'F':sscanf(&str[1],":%d;%f;%f",&f1.charak,&f1.frequenz,&f1.Bandbreite);
		  filter_berechnen(&f1);
		  break;
		  case 'G':sscanf(&str[1],":%d;%f;%f",&f2.charak,&f2.frequenz,&f2.Bandbreite);
		  filter_berechnen(&f2);
		  break;
		  case 'H':sscanf(&str[1],":%d;%f;%f",&f3.charak,&f3.frequenz,&f3.Bandbreite);
			  filter_berechnen(&f3);
			  break;
		  case 'V': sscanf(&str[1],":%f;%f",&faktor,&offset);

		  }
		  uartPrintf("->%s<-\r\n",str);
		  lcdPrintf( 1, 0, 20,str);
	  }
  }
}

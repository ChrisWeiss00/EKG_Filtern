//********************************************
#include <stdio.h>
#include <stdarg.h>
#include "stm32l1xx.h"

//--------------------------------------------
#include "lcd.h"

//********************************************
#define LCD_BUFFER_SIZE  31

//********************************************
#define BYTE unsigned char

//********************************************
// 
// Controller-spezifische Funktionen
// Implementierung des SPI-Busprotokolls
//
//********************************************
//--------------------------------------------
static void PortSetMode( BYTE pinId )
{
  GPIOB->MODER         |=  ((unsigned)(0x02) << (2* pinId   ));
  GPIOB->AFR[pinId>>3] |=  ((unsigned)(   5) << (4*(pinId%8)));
}

//--------------------------------------------
// Schreiben eine Bytes auf SPI-Device
//--------------------------------------------
static void SPItransceive( BYTE *data, BYTE size )
{
  unsigned i;
 
  for( i = 0; i < size; i++ )
  {
    // wait until transmission buffer empty 
    while( !(SPI2->SR & SPI_SR_TXE) );

    // start transmission
    SPI2->DR = data[i];

    // wait until transmission completed
    while( (SPI2->SR & SPI_SR_BSY) );
 
    // wait until data received 
    while( (SPI2->SR & SPI_SR_RXNE) )
    {
      data[i] = SPI2->DR;
    }
  }
}

//--------------------------------------------
static void SPIinit(void)
{
  BYTE data[1] = {0}; 

  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;     

  PortSetMode(  12 ); // CS
  PortSetMode(  13 ); // SCK
  PortSetMode(  14 ); // MISO
  PortSetMode(  15 ); // MOSI

  SPI2->CR2 |=  !SPI_CR2_FRF   // Frame format: Motorola
               | SPI_CR2_SSOE; // Slave Select output enable

  SPI2->CR1  =   !SPI_CR1_DFF       // Data frame format: 8-bit
               | !SPI_CR1_SSM       // Slave management: HW
               |  SPI_CR1_SSI       // 
               |  SPI_CR1_LSBFIRST  // LSB first: disable
               | !SPI_CR1_SPE       // SPI enable
               |  (3<<3)             // Baud rate: fpclk/16
               |  SPI_CR1_MSTR       // Master mode
               |  SPI_CR1_CPOL
               |  SPI_CR1_CPHA;
                        
  SPI2->CR1 |= SPI_CR1_SPE;
  
  // Dummy-Read to push SCK to high
  SPItransceive( data,1 );
}

//********************************************
// 
// LCD-spezifische Funktionen
// Implementiert LCD-Protokoll (4-Bit)
//
//********************************************
//--------------------------------------------
static void writeCmd(unsigned char cmd) 
{
  BYTE data[3];
  
  data[0] =    (0x1F)  // START
              |(0<<6)  // RS
              |(0<<5); // RW
  data[1] = (cmd   ) & 0x0F;
  data[2] = (cmd>>4) & 0x0F;
  
  SPItransceive( data, 3);
}

//--------------------------------------------
static void writeDat(unsigned char dat) 
{
  BYTE data[3];
  
  data[0] =    (0x1F)  // START
              |(1<<6)  // RS
              |(0<<5); // RW
  data[1] = (dat    ) & 0x0F;
  data[2] = (dat>>4 ) & 0x0F;
  
  SPItransceive( data, 3);
}

//********************************************
//
// Exportierte LCD-Funktionen
// 
//********************************************
//--------------------------------------------
void lcdInit(void)
{
  SPIinit();
  
  writeCmd(  (1<<5)  // Function set (RE=0)
            |(1<<4)   //   DL:  4-bit,
            |(0<<3)  //   N :  1-line,
            |(1<<2)  //   RE:  1
            |(0<<1)  //   DH:  dot scroll
            |(0<<0));//   REV: normal

  writeCmd(  (1<<3)  // extended Function set (RE=1)
            |(0<<2)  //  FW:  5-dot font width
            |(1<<1)  //  BW:  inverting cursor
            |(1<<0));//  NW:  4-line display

  writeCmd(  (1<<5)  // Function set (RE=1)
            |(1<<4)   //   DL:  4-bit,
            |(0<<3)  //   N :  1-line,
            |(0<<2)  //   RE:  0
            |(0<<1)  //   BE:  CG/SEG-RAM blink disable
            |(0<<0));//   LP:  normal mode

  writeCmd(  (1<<0));// Clear Display

  writeCmd(  (1<<3)  // Display On/Off Control
            |(1<<2)  //  D:  display on
            |(0<<1)  //  C:  cursor off
            |(0<<0));//  B:  blink off
}

//--------------------------------------------
void lcdPrintf(BYTE row, BYTE colum, BYTE minLen, const char *format,...)
{
  BYTE i;
  char str[LCD_BUFFER_SIZE+1]; // ein paar Zeichen als Reserve

  va_list argzeiger;
  va_start(argzeiger,format);
  vsprintf(str,format, argzeiger);
  va_end(argzeiger);
  
  // Startadresse fuer Display-Speicher schreiben
  writeCmd(0x80 | ((row<<5)+(colum&0x1F)) ); // Format: rrrccccc

  // Zeichenweise an Display-Speicher uebertragen
  for (i=0; colum<20 && str[i]!=0; colum++,i++) 
  {
    writeDat(str[i]);
  }
  
  // Rest mit Leerzeichen ueberschreiben
  for (;  colum<20 && i<minLen; colum++,i++) 
  {
    writeDat(' ');
  }  
}
// EOF

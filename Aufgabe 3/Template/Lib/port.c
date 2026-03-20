//*******************************************************************
#include "stm32l1xx.h"

//-------------------------------------------------------------------
#include "port.h"

//-------------------------------------------------------------------
void portpinInit( GPIO_TypeDef *ptr, unsigned pin, unsigned mode, unsigned af)
{
  unsigned pin2 = 2*pin;
  unsigned pin3 = pin>>3;

  unsigned bitmask1 = ~((unsigned)0x01 <<     pin    );
  unsigned bitmask2 = ~((unsigned)0x03 <<     pin2   );
  unsigned bitmask3 = ~((unsigned)0x0F << (4*(pin%8)));

  unsigned moder   = (unsigned)((mode    )&0x03) << pin2;
  unsigned pupdr   = (unsigned)((mode>> 8)&0x03) << pin2;
  unsigned otyper  = (unsigned)((mode>> 4)&0x01) << pin;
  unsigned ospeedr = (unsigned)((mode>>12)&0x03) << pin2;
  unsigned afr     = (unsigned)((af      )&0x0F) << (4*(pin%8));
  
  RCC->AHBENR |= 0x0F;
  
  ptr->MODER     = ( ptr->MODER     & bitmask2 ) | moder;
  ptr->PUPDR     = ( ptr->PUPDR     & bitmask2 ) | pupdr;
  ptr->OTYPER    = ( ptr->OTYPER    & bitmask1 ) | otyper;
  ptr->OSPEEDR   = ( ptr->OSPEEDR   & bitmask2 ) | ospeedr;
  ptr->AFR[pin3] = ( ptr->AFR[pin3] & bitmask3 ) | afr;  
}

//-------------------------------------------------------------------
void portpinSet( GPIO_TypeDef *ptr, unsigned pin, unsigned value )
{
  if( value )    
  {
    ptr->BSRRL = (0x01<<pin);
  }
  else
  {
    ptr->BSRRH = (0x01<<pin);
  }
}

//-------------------------------------------------------------------
unsigned portpinGet( GPIO_TypeDef *ptr, unsigned pin )
{
  return( (ptr->IDR&(1<<pin)) != 0 );
}

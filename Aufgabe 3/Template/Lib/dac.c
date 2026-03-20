//*******************************************************************
#include "stm32l1xx.h"

//-------------------------------------------------------------------
#include "dac.h"
#include "port.h"

//-------------------------------------------------------------------
void dacInit( unsigned ch )
{
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  RCC->AHBENR  |= (1 << 0);
  
  switch( ch )
  {
    case 1: 
      portpinInit( GPIOA, 4, ANALOG, 0 );
      DAC->CR |=   DAC_CR_EN1
                 | DAC_CR_BOFF1;
      break;
    
    case 2:
      portpinInit( GPIOA, 5, ANALOG, 0 );
      DAC->CR |=   DAC_CR_EN2
                 | DAC_CR_BOFF2; 
      break;
  }
}

//-------------------------------------------------------------------
void dacSet( unsigned ch, unsigned value )
{
  switch( ch )
  {
    case 1: DAC->DHR12R1 = value & 0x0FFF; break;
    case 2: DAC->DHR12R2 = value & 0x0FFF; break;
  }
}

//*******************************************************************
#include "stm32l1xx.h"

//-------------------------------------------------------------------
#include "adc.h"
#include "port.h"

//-------------------------------------------------------------------
void adcInit( unsigned ch )
{
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->AHBENR |= (1 << 0);
  
  ADC1->CR2 |=    ADC_CR2_ADON  // A/D Converter: ON
               | !ADC_CR2_CONT  // Continuous Conversion: OFF
               | !ADC_CR2_ALIGN;// Data alignment: right

  while( !(ADC1->SR & ADC_SR_ADONS) );

  switch( ch )
  {
    case  2: portpinInit( GPIOA,  2, ANALOG, 0 ); break;
    case  3: portpinInit( GPIOA,  3, ANALOG, 0 ); break;
  }
}

//-------------------------------------------------------------------
unsigned adcGet( unsigned ch )
{
  ADC1->SQR5 = ch & 0x1F;            // select channel

  ADC1->CR2 |= ADC_CR2_SWSTART;      // start conversion
  
  while( !(ADC1->SR & ADC_SR_EOC) ); // wait for end of conversion
  
  return( ADC1->DR & 0xFFFF );
}

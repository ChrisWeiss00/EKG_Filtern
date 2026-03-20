//*******************************************************************
#include "stm32l1xx.h"

//-------------------------------------------------------------------
#include "timer.h"

//*******************************************************************
void (*isr)(void) = 0;

//-------------------------------------------------------------------
void timerInit( unsigned cycleTime,  void (*isrIn)(void))
{
  float    tics = 32.0 /*MHz*/ * cycleTime /* us */;
  unsigned ps   = 1;

  // calculate tics and prescaler
  while( tics > 0xFFFF ) // tics must be < 16Bits
  {
    ps   *= 10;
    tics /= 10;
  }

  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  TIM2->CR1  = TIM_CR1_CEN;  // counter enable
  TIM2->DIER = TIM_DIER_UIE; // update interrupt enable
  TIM2->EGR  = TIM_EGR_UG;   // re-initialize timer
  TIM2->PSC  = ps - 1;
  TIM2->ARR  = (unsigned)tics;
  
  isr = isrIn;
  
  NVIC_EnableIRQ( TIM2_IRQn );
}

//-------------------------------------------------------------------
void TIM2_IRQHandler(void)
{
  TIM2->SR &= ~TIM_SR_UIF;
  if( isr )
  {
    isr();
  }
}

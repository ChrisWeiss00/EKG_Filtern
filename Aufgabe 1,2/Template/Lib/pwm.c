//*******************************************************************
#include "stm32l1xx.h"

//-------------------------------------------------------------------
#include "port.h"
#include "pwm.h"

//-------------------------------------------------------------------
void pwmInit()
{
  unsigned  ccmr =  (7<<4)  // OCxM: PWM-mode 2
                   |(0<<3); // OCxPE: preload enable

  unsigned ccerNorm =  (1<<0)     // CCxE: enable
                       |(1<<1);   // CCxP: polarity 
                                  //       0 = invers: active high
                                  //       1 = normal: active low

  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  
  TIM3->CR1  = TIM_CR1_CEN;  // counter enable
  TIM3->DIER = TIM_DIER_UIE; // update interrupt enable
  TIM3->EGR  = TIM_EGR_UG;   // re-initialize timer
  TIM3->PSC  = 1;
  TIM3->ARR  = 0x0FFF;
  
  portpinInit( GPIOB, 4, ALTERNATE_FUNC, 2);
  portpinInit( GPIOB, 5, ALTERNATE_FUNC, 2);
  
  portpinInit( GPIOD, 2, OUTPUT       , 0 );

  
  TIM3->CCMR1 |= ccmr;
  TIM3->CCER  |= ccerNorm;

  TIM3->CCMR1 |= ccmr<<8;
  TIM3->CCER  |= ccerNorm<<4;    
}

//-------------------------------------------------------------------
void pwmSet( float pwr )
{
  TIM3->CCR1 = (pwr>=0)?+pwr*0xFFF:0;
  TIM3->CCR2 = (pwr<=0)?-pwr*0xFFF:0;
}

//-------------------------------------------------------------------
void pwmEnable( unsigned e )
{
   portpinSet(GPIOD, 2, e );
}
    


//EOF

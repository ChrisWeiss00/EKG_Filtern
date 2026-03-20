//*******************************************************************
#include <stdio.h>
#include <stdarg.h>
#include "stm32l1xx.h"

//-------------------------------------------------------------------
#include "uart.h"
#include "port.h"

//-------------------------------------------------------------------
#define UART_BUFFER_SIZE  256

//-------------------------------------------------------------------
static char rxBuffer[ UART_BUFFER_SIZE ];
static char txBuffer[ UART_BUFFER_SIZE ];

static unsigned char txReadIdx  = 0;
static unsigned char txWriteIdx = 0;

static unsigned char rxReadIdx  = 0;
static unsigned char rxWriteIdx = 0;

static unsigned char txRunning = 0;

//-------------------------------------------------------------------
void uartInit( void )
{
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;  // enable USART clock
  RCC->AHBENR  |= (1 << 0);
  portpinInit( GPIOA,  9, ALTERNATE_FUNC, 7 ); 
  portpinInit( GPIOA, 10, ALTERNATE_FUNC, 7 );

  NVIC_EnableIRQ( USART1_IRQn );

  USART1->CR2 |=  (0<<12);           // 0: 1 stop bit

  USART1->CR1 |=  (  !USART_CR1_M    // 8 Bit
              | !USART_CR1_PCE       // no parity
              |  USART_CR1_RE        // RX mode
              |  USART_CR1_TE );     // TX mode

  USART1->CR3 |=  (  !USART_CR3_RTSE
              | !USART_CR3_CTSE );   // no hardware flow control

  USART1->BRR  = (unsigned)(32000000L / 9600);

  USART1->CR1 |= (  USART_CR1_UE     // Enable USART
              | USART_CR1_TCIE       // Transm. complete interr. enable
              | USART_CR1_RXNEIE );  // Receive reg. not empty interr. enable
}

//-------------------------------------------------------------------
void uartPrintf( const char *format, ... )
{
  unsigned  i;
  char str[ UART_BUFFER_SIZE+2 ]; // ein paar Zeichen als Reserve

  va_list argzeiger;
  va_start( argzeiger, format );
  vsnprintf( str, UART_BUFFER_SIZE, format, argzeiger );
  va_end( argzeiger);
  
  // Zeichenweise an Display-Speicher uebertragen
  for (i=0; str[i]!=0 && i<UART_BUFFER_SIZE; i++) 
  {    
    txBuffer[txWriteIdx] = str[i];
    txWriteIdx++;

    if( !txRunning ) // Interrupt ausloesen, falls erforderlich
    {
      txRunning = 1;  
      
      USART1->DR = txBuffer[txReadIdx];
      txReadIdx++;
    }
    while( txReadIdx == (unsigned char)(txWriteIdx+1) ); // warten bis FIFO nicht voll
  }
}

//-------------------------------------------------------------------
char *uartGetString( void )
{
  static char str[ UART_BUFFER_SIZE+2 ]; // ein paar Zeichen als Reserve
  static unsigned pos = 0;
  
  // lese aus FIFO bis String voll, FIFO leer oder '\n' erreicht
  while( rxReadIdx != rxWriteIdx )
  {
    char c = rxBuffer[rxReadIdx];
    rxReadIdx++;
    
    if(c == '\n' || c == '\r' || pos >= UART_BUFFER_SIZE-1)
    {
      str[pos] = 0;
      pos = 0;
      return( str );
    }
    else
    {
      str[pos++] = c;
    }
  }
  return( NULL );
}

//-------------------------------------------------------------------
void USART1_IRQHandler( void )
{  
  if( USART1->SR  & USART_SR_TC ) // transmission complete?
  {
    if( txReadIdx != txWriteIdx ) // FIFO not empty
    {
      USART1->DR = txBuffer[txReadIdx];
      txReadIdx++;
    }
    else
    {
      txRunning = 0;
      USART1->SR = ~(USART_SR_TC);
    }
  }
  
  if( USART1->SR  & USART_SR_RXNE ) // receive data register not empty
  {
    if( rxReadIdx != (unsigned char)(rxWriteIdx+1) ) // FIFO not full
    {
      rxBuffer[rxWriteIdx] = USART1->DR;
      rxWriteIdx++;
    }
    else
    {
      USART1->SR = ~(USART_SR_RXNE);
    }
  }
}

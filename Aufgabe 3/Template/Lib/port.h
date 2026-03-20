//--------------------------------------------
#include "stm32l1xx.h"

//--------------------------------------------
// Definition von Konstanten zur Konfiguration
// eines Port-Pins
//--------------------------------------------
typedef enum
{
  // MODER
  INPUT          = 0x0000,
  OUTPUT         = 0x0001,
  ALTERNATE_FUNC = 0x0002,
  ANALOG         = 0x0003,
  
  // OTYPER
  PUSH_PULL      = 0x0000,
  OPEN_DRAIN     = 0x0010,
  
  //PUPDR
  NO_PUPD        = 0x0000,
  PULL_UP        = 0x0100,
  PULL_DOWN      = 0x0200,

  //OSPEEDR
  LOW_SPEED      = 0x0000,
  MEDIUM_SPEED   = 0x1000,
  FAST_SPEED     = 0x2000,
  HIGH_SPEED     = 0x3000 
  
} MODE;
    
//--------------------------------------------
// Initialisierung eine Port-Pins
//
// Muss vor erster Verwendung 
// aufgerufen werden
//--------------------------------------------
void portpinInit( GPIO_TypeDef *ptr, // Zeiger auf HW-Register
                  unsigned      pin, // Pin-Nummer
                  unsigned      mode,// Mode gem. oben definierter Konstanten
                  unsigned      af); // Nummer der Alternate-Function

//--------------------------------------------
// Setzt Port-Pin auf angegeben Wert
//--------------------------------------------
void portpinSet( GPIO_TypeDef *ptr,     // Zeiger auf HW-Register
                 unsigned      pin,     // Pin-Nummer
                 unsigned      value ); // Wert (0,1)

//--------------------------------------------
// Liest Port-Pin und gibt den zustand zurueck
//
// Rueckgabe: 0 oder 1                 
//--------------------------------------------
unsigned portpinGet( GPIO_TypeDef *ptr,  // Zeiger auf HW-Register
                     unsigned      pin );// Pin-Nummer

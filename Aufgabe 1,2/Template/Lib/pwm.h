//--------------------------------------------
// Initialisiert den PWM-Ausgabg des Timer 3
// PB4: positiv
// PB5: negativ
//--------------------------------------------
void pwmInit( void );

//--------------------------------------------
// Setzt PWM-Ausgangswert für PB4 und PB5
// pwr: -1.0 (max neg.) bis +1.0 (max pos.)
//--------------------------------------------
void pwmSet( float pwr );

//--------------------------------------------
// Setzt Enable (PD2) der H-Brücke
// enable = true: H-Brücke freigeschaltet
//--------------------------------------------
void pwmEnable( unsigned enable );

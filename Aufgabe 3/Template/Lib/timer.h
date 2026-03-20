//--------------------------------------------
// Initialisiert den Timer 2
// 
// Die mit dem Funktionspointer angegebene 
// Callback-Funktion wird zyklisch durch den 
// Timer-Interrupt aufgerufen
//--------------------------------------------
void timerInit( unsigned cycleTime,  // Zykluszeit
                void (*isr)(void) ); // Funktionspointer der Callback-Funktion

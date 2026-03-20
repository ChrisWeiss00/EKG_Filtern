//--------------------------------------------
// Initialisierung des DAC
//
// Muss vor erster Verwendung fuer jeden Kanal 
// aufgerufen werden
// Es werden nur die Kanaele 1 und 2 unterstuetzt
//--------------------------------------------
void dacInit( unsigned ch );

//--------------------------------------------
// Setzt den 12-Bit Ausgabewert eines Kanals 
//--------------------------------------------
void dacSet( unsigned ch,      // Kanal-Nummer
             unsigned value ); // Ausgabewert

//--------------------------------------------
// Initialisierung des ADC
// 
// Muss vor erster Verwendung  fuer jeden Kanal
// aufgerufen werden
// Es werden nur die Kanaele 2 und 3 unterstuetzt
//--------------------------------------------
void adcInit( unsigned ch ); // Kanal-Nummer

//--------------------------------------------
// Startet Abtastvorgang, wartet auf dessen 
// Ende und liefert Abtastwert zurueck
//
// Rueckgabe: 12-Bit Abtastwert
//--------------------------------------------
unsigned adcGet( unsigned ch ); // Kanal-Nummer

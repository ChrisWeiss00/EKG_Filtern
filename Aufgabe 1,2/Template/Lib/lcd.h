//--------------------------------------------
// Initialisierung des LCD-Displays
// 
// Muss vor erster Verwendung 
// aufgerufen werden
//--------------------------------------------
void lcdInit(void);

//--------------------------------------------
// Schreibt einen String auf das Display
// 
// Syntax ab 'format' entsprechend printf()
//
// ACHTUNG: Die Zeichenkette darf nicht laenger 
//          als 20 Zeichen sein!!!
//
//--------------------------------------------
void lcdPrintf(unsigned char row,        // Zeile
               unsigned char column,     // Spalte
               unsigned char minLen,     // Anzahl Zeichen, die immer geloescht werden
               const    char *format,    // Format der Zeichenkette
               ...                    ); // Optionale Parameter


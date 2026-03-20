//--------------------------------------------
// Initialisierung der seriellen Schnittstelle 
// USART 1
// 
// Muss vor erster Verwendung 
// aufgerufen werden
//--------------------------------------------
void uartInit(void);

//--------------------------------------------
// Schreibt einen String auf die Schnittstelle
// 
// Syntax entsprechend printf()
//
// ACHTUNG: Die Zeichenkette darf nicht laenger 
//          als 256 Zeichen sein!!!
//
//--------------------------------------------
void uartPrintf(const    char *format,    // Format der Zeichenkette
                ...                    ); // Optionale Parameter

//--------------------------------------------
// Prüft, ob auf der seriellen Schnittstelle 
// ein mit '\n' abgeschlossener String oder 
// mehr als 255 Zeichen empfangen wurden
// 
// Rueckgabe:
//   0: String nicht vollstaendig
//   sonst Zeiger auf String
//
//--------------------------------------------
char *uartGetString( void ); 

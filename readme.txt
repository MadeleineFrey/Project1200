Ändringarna som jag har gjort:
initierat BTN_RETURN. Det är knappen längst till höger som vi kan använda som en tillbaka knapp

Flyttade define BTN till h-filen så vi kommer åt dem i alla filer

Jag ändrade så att jag satte menyn under labwork i en funktion. Detta är för att kunna hoppa tillbaka till menyn från highscore - listan. 
Jag vet dock inte om man vill använda knappen längst till vänster för return ellet till höger??

Jag har lagt in en wait-funktion i menyn så att den inte ska tro att man vill spela direkt då man vill tillbaka från highscore menyn. 

Efter att spelet är slut, kalla på funktionen new_highscore och skicka med score. Den kollar först om score kvalar sig in på highscore och därefter får man mata in sitt namn.

Jag har testat timer 2-5 och de fungerar!
Timer 2: wait-funktioner! Använd dessa funktioner, ej timern!
Timer 3:Pipes och highscore
Timer 4: Används till i2c protokollet
Timer 5: This one is for the accelerometer during game


Jag har gjort så att om du vill använda accelerometern kan du använda den globala variabeln ADXL_VALUE. Jag har gjort så att den variabeln 
hela tiden bör ha information om åt vilket håll som sensorn är vinklad.




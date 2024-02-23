Ändringarna som jag har gjort:
initierat BTN_RETURN. Det är knappen längst till höger som vi kan använda som en tillbaka knapp

Flyttade define BTN till h-filen

Jag ändrade så att jag satte menyn under labwork i en funktion. Detta är för att kunna hoppa tillbaka till menyn från highscore - listan. 
Jag vet dock inte om man vill använda knappen längst till vänster för return ellet till höger??

Vi har ett problem i menyn då vi inte använder interrupts för knapparna. Om man vill använda button select för att komma tillbaka till menyn så 
klickar den sedan automatiskt på "play" direkt så man inte hinner komma till menyn. Just nu är det BTN_RETURN som är tillbaka-knappen.

Efter att spelet är slut, kalla på funktionen new_highscore och skicka med score. Den kollar först om score kvalar sig in på highscore och därefter får man mata in sitt namn.

Jag har testat timer 2-5 och de fungerar!
Timer 2: wait-funktioner! Använd dessa!
Timer 3:Pipes och highscore
Timer 4: 
Timer 5: fågeln i menyn

Sätt in timers i highscore
fixa så att man kan skriva in sitt namn, fel bokstav kommer nu
Sorteringen

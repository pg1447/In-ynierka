#include <Stepper.h> //dodajemy do szkicu bibliotekę obsługującą silniki krokowe

#define STEPS_NUMBER 32 //definiujemy stałą gdzie podajemy ilość kroków zależną od użytego silnika w naszym przypadku 32

Stepper mojSilnik1(STEPS_NUMBER, 22, 23, 24, 25); //tutaj podajemy piny w Arduino, którymi sterujemy silnikiem
Stepper mojSilnik2(STEPS_NUMBER, 26, 27, 28, 29);
Stepper mojSilnik3(STEPS_NUMBER, 30, 31, 32, 33);
Stepper mojSilnik4(STEPS_NUMBER, 34, 35, 36, 37 );

void setup()

{



}

void loop ()

{

mojSilnik1.setSpeed(700); //podajemy prędkość obrotu wyrażona w rpm 
mojSilnik2.setSpeed(700);
mojSilnik3.setSpeed(700);
mojSilnik4.setSpeed(700);

mojSilnik1.step(2048); //linijka powoduje obrót ośki silnika 1 o 360 stopni
delay(2000);
mojSilnik2.step(2048);
delay(2000);
mojSilnik3.step(2048);
delay(2000);
mojSilnik4.step(2048);

delay(5000); // linijka powoduje odczekanie 5 sekund (wartosc w milisekundach 1000 = 1s)

}

// ===== PRZYCISKI =====
#define PRZYCISK_A A0
 
#define PRZYCISK_B A1
 
#define PRZYCISK_C A2
 
#define PRZYCISK_D A3
 
#include <Wire.h>  // Biblioteka pozwalająca na komunikację z urządzeniami I2C 
#include <LiquidCrystal_I2C.h> //Biblioteka pozwalająca na sterowanie wyświetlaczami I2C
#include <Stepper.h> //Podstawowa biblioteka obsługująca silniki krokowe



//Piny Arduino, które sterują silnikami krokowymi

#define STEPS_NUMBER 32 //definiujemy stałą gdzie podajemy ilość kroków zależną od użytego silnika w naszym przypadku 32

Stepper mojSilnik1(STEPS_NUMBER, 22, 23, 24, 25); 

Stepper mojSilnik2(STEPS_NUMBER, 26, 27, 28, 29);

Stepper mojSilnik3(STEPS_NUMBER, 30, 31, 32, 33);

Stepper mojSilnik4(STEPS_NUMBER, 34, 35, 36, 37 );

LiquidCrystal_I2C lcd(0x3F,16,2); // Nadanie adresu ekranowi LCD

// ===== ZMIENNE =====
String haslo;
String opcja;
boolean potwierdzenie =0;
boolean uzupelnienie;
const int coinpin = 2;
const int targetcents = 10;
volatile int cents = 0;
int credits = 0;
boolean wybor =0;
int produkty_a = 1;
int produkty_b = 1;
int produkty_c = 1;
int produkty_d = 1;
int stan_konta;
 
 
void setup()//Pętla ekranu startowego
{
 Serial.begin(9600); // Ustawienie szybkości transmisji danych dla monitora portu szeregowego
  delay(2000);
  
  // Konfiguracja pinów
  pinMode(coinpin, INPUT);
  pinMode(PRZYCISK_A, INPUT);
  pinMode(PRZYCISK_B, INPUT);
  pinMode(PRZYCISK_C, INPUT);
  pinMode(PRZYCISK_D, INPUT);
 attachInterrupt(digitalPinToInterrupt(coinpin), coinInterrupt, FALLING); // Przerwanie
Serial.println("Tryb serwisowy");
Serial.println("Wprowadz haslo:");
  lcd.init();                     
  lcd.init(); // inicjalizacja wyświetlacza LCD
  lcd.backlight(); // podświetlenie ekranu
  lcd.setCursor(5,0);
  lcd.print("Maszyna");
  lcd.setCursor(3,1);
  lcd.print("Vendingowa");
  delay(1000);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Piotr");
  lcd.setCursor(3,1);
  lcd.print("Galwiaczek");
  delay(1000);
  lcd.clear();
}

// ===== GŁÓWNA PĘTLA PROGRAMU

void loop()
{
  trybSerwisowy();
 
  if (cents >= targetcents) {
    credits = credits + 1;
    cents = cents - targetcents;
    wybor =1;
  }

  if(cents==0&&cents<10&&wybor==0)
  {
    waitingForCredit();
  }
  
  if(cents>0&&cents<10&&wybor==0)
  {
    updateCredits();
  }
 
  if(wybor==1)
  {
    wyborProduktu();
    cents = 0;
    delay(600);
  }
 
  delay(500);
  lcd.clear();
}
 
void trybSerwisowy() // Funkcja trybu serwisowego
{
    if(Serial.available() > 0){
      if(potwierdzenie == 0)
      {
          haslo = Serial.readStringUntil('\n');
 
      if(haslo.equals("1234")){
          potwierdzenie=1;
          Serial.println("****");
          Serial.println("Haslo wpisane poprawnie!\n");
          Serial.println("Opcje:\n");   
          Serial.println("1.Stan konta:\n");  
          Serial.println("2.Stan produktow:\n");  
          Serial.println("3.Uzupelnienie:\n");       
      }
      }
      if(haslo!="1234"&&potwierdzenie>0)
      {
          Serial.print("Bledne haslo");
      }
      if(potwierdzenie==1)
      {
          opcja = Serial.readStringUntil('\n');
      if(opcja.equals("1")){
          Serial.println("1.Stan konta:\n");
          Serial.println(stan_konta);
      }
      if(opcja.equals("2"))
      {
          Serial.println("2.Stan produktow:\n");
          Serial.println("Produkt A:\n");
          Serial.println(produkty_a);
          Serial.println("Produkt B:\n");
          Serial.println(produkty_b);
          Serial.println("Produkt C:\n");
          Serial.println(produkty_c);
          Serial.println("Produkt D:\n");
          Serial.println(produkty_d);
        }
      if(opcja.equals("3"))
      {
          Serial.println("Stan produktow zostal uzupelniony\n");
        uzupelnienie = 1;
      if(uzupelnienie=1)
      {
          produkty_a = 7;
          produkty_b = 7;
          produkty_c = 7;
          produkty_d = 7;
          delay(200);
          uzupelnienie = 0;
          }
        }  
      }
  }
}
 
 
void coinInterrupt(){ // Funkcja inkrementująca punkty po każdym przerwaniu(Efektem przerwania jest inkrementacja)
  if(wybor==0&&cents<=10)
  {
    cents++;
    stan_konta++;
  }
 
}
 
void updateCredits(){ // Funkcja updatująca punkty i wyświetlająca je na ekranie
  lcd.clear();
  if(cents == 0)
  {
    lcd.setCursor(0,0);
    lcd.print("kredyt:");
    lcd.setCursor(9,1);
    lcd.print("0.00zl");
  }
 
  if(cents>0&&cents<10){
    lcd.setCursor(0,0);
    lcd.print("kredyt:");
    lcd.setCursor(8,0);
    lcd.print(cents);
    lcd.setCursor(11,0);
    lcd.print("zl");  
  }
 
  if(cents>=10)
  {
    wybor=1;
    wyborProduktu();
  }
}
 
 
 
 
void waitingForCredit(){ // Stan oczekiwania na monety
  lcd.setCursor(1,0);
  lcd.print("Wrzuc 5, 2, 1zl");
}
 
void wyborProduktu()// Funkcja z ekranem wyboru produktu i startem silników
{ 
  if(produkty_a > 0){
  lcd.setCursor(0,0);
  lcd.print("A");
} 
  if (produkty_b >0){
  lcd.setCursor(7,0);
  lcd.print("B");  
}
  if(produkty_c>0){
  lcd.setCursor(0,1);
  lcd.print("C");  
}
  if(produkty_d>0){
  lcd.setCursor(7,1);
  lcd.print("D");
  }
  if(produkty_a + produkty_b + produkty_c + produkty_d <= 0){
  lcd.setCursor(2,0);
  lcd.print("Brak produktow"); 
  delay(3000);
  lcd.setCursor(0,0);
  lcd.print("Obsluga klienta");
  lcd.setCursor(0,1);
  lcd.print("tel 511-***-834");
}  
  if(digitalRead(PRZYCISK_A)==HIGH && produkty_a>0) //stan wysoki zainicjowany na przycisku A powoduje czyszczenie ekranu, wyświetlanie komunikatu o wyborze produktu oraz odejmuje produkt ze stanu i włącza silnik 1
  {
    
    lcd.clear();
    delay(100);
    lcd.print("Wybrano: A");
    delay(500);
    produkty_a--;
    Silnik1();
    wybor = 0; //flaga
}
 
if(digitalRead(PRZYCISK_B)==HIGH && produkty_b>0)
{
    lcd.clear();
    delay(100);
    lcd.print("Wybrano: B");
    delay(500);
    produkty_b--;
    Silnik2();
    wybor = 0;
}

if(digitalRead(PRZYCISK_C)==HIGH && produkty_c>0)
{
    lcd.clear();
    delay(100);
    lcd.print("Wybrano: C");
    delay(500);
     produkty_c--;
     Silnik3();
     wybor = 0;
}

if(digitalRead(PRZYCISK_D)==HIGH && produkty_d>0)
{
    lcd.clear();
    delay(100);
    lcd.print("Wybrano: D");
    delay(500);
    produkty_d--;
     Silnik4();
    wybor = 0;
}
 
}


// Aktywuj silnik1

void Silnik1(){
  mojSilnik1.setSpeed(700);
  mojSilnik1.step(2048); //linijka powoduje obrót ośki silnika 1 o 360 stopni
  delay(2000);  

}

// Aktywuj silnik2

void Silnik2(){
  mojSilnik2.setSpeed(700);
  mojSilnik2.step(2048); //linijka powoduje obrót ośki silnika 2 o 360 stopni
  delay(2000);  

}

// Aktywuj silnik3

void Silnik3(){
  mojSilnik3.setSpeed(700);
  mojSilnik3.step(2048); //linijka powoduje obrót ośki silnika 3 o 360 stopni
  delay(2000);  

}

// Aktywuj silnik4

void Silnik4(){
  mojSilnik4.setSpeed(700);
  mojSilnik4.step(2048); //linijka powoduje obrót ośki silnika 4 o 360 stopni
  delay(2000);  

}

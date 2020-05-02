#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int led = 13;

byte ENQ = 0x05;  //ASCII 0x05 == ENQUIRY
byte STX = 0x02; //STX=02h
byte ETX = 0x03; //ETX=03h
byte CR = 0x0D;  //CR=0Dh
byte LF = 0x0A;  //LF=0A

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);

  lcd.begin (16, 2);

  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("TESTE");
  lcd.setCursor(0, 1);
  lcd.print("BALANCA");
  delay(1000);
  lcd.setBacklight(LOW);
  delay(1000);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("...");
}

String leStringSerial() {
  String conteudo = "";
  char caractere;

  // Enquanto receber algo pela serial
  while (Serial.available() > 0) {
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n') {
      // Concatena valores
      conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }

  //Serial.print("Recebi: ");
  //Serial.println(conteudo);

  return conteudo;
}

long randNumber;
float num;

int analogPin = A0;

void loop() {

  delay(100);

  randNumber = analogRead(analogPin);

  randNumber = (randNumber - 23);
  randNumber = (randNumber * 10);
  num = (float)randNumber / 1000;
  String AComando = (String) randNumber;

  //PESO_INSTAVEL   = 'IIIII';
  //PESO_NEGATIVO   = 'NNNNN';
  //PESO_SOBRECARGA = 'SSSSS';

  if (num >= 10) {
    AComando = "SSSSS";
    lcd.setCursor(0, 0);
    lcd.print(" SOBRECARGA BAL ");
  }
  else if (num >= 9) {
    AComando = "IIIII";
    lcd.setCursor(0, 0);
    lcd.print(" PESO INSTAVEL BAL ");
  }
  else if (num < 0) {
    AComando = "NNNNN";
    lcd.setCursor(0, 0);
    lcd.print(" PESO NEGATIVO BAL ");
  }
  else if (num == 0.0) {
    AComando = "00000";
    lcd.setCursor(0, 0);
    lcd.print(" *** BAL ZERADA *** ");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("LEITURA BALANCA.....");
  }

  // Se receber algo pela serial
  if (Serial.available() > 0) {

    // Lê toda string recebida
    String recebido = ""; //leStringSerial();

    if (Serial.read() == ENQ) {
      Serial.write(STX);
      Serial.print(AComando);
      Serial.write(ETX);
    }

    if (recebido == "LED1:1") {
      digitalWrite(led, HIGH);
    }

    if (recebido == "LED1:0") {
      digitalWrite(led, LOW);
    }
  }

  lcd.setCursor(0, 1);
  String displayString = "Peso: " + String ( num , 3 );
  lcd.print(displayString);
  delay(100);

}

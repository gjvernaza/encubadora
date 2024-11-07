#include <DHT_U.h>
#include <DHT.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// sensor de temperatura y humedad DHT11
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ledPin = 13;
const int pinMotor = 8;
const int pot = A0;
const int pinCalentador = 5;
const int pinVentilador = 6;
const int pulsador = 2;


unsigned long tiempo = 0;
unsigned long lastTime = 0;

int temperatura = 0;
int humedad = 0;
int adc = 0;

bool status_pulsador = false;

void setup()
{
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Encubadora");
    lcd.setCursor(0, 1);
    lcd.print("Automatica");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Iniciando...");
    delay(700);
    lcd.clear();
    lastTime = millis();
    dht.begin();
    pinMode(pinCalentador, OUTPUT);
    pinMode(pinVentilador, OUTPUT);
    pinMode(pulsador, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pulsador), activate_ventilador, RISING);
    digitalWrite(pinCalentador, HIGH);
    digitalWrite(pinVentilador, LOW);
    

}

void activate_ventilador(){
    
    status_pulsador = !status_pulsador;
    
}

void loop()
{
    if(millis() - lastTime > 200){
        
        // Lectura de temperatura y humedad
        humedad = dht.readHumidity();
        temperatura = dht.readTemperature();
        if (isnan(humedad) || isnan(temperatura))
        {
            Serial.println("Error al leer el sensor DHT11");
            return;
        }
        
        lastTime = millis();
    }
    if (temperatura >= 36)
    {
        // CONECTAR COMUN Y NORMALMENTE CERRADO
        digitalWrite(pinCalentador, LOW);
        digitalWrite(pinVentilador, HIGH);
    }
    else
    {
        digitalWrite(pinCalentador, HIGH);
        if (status_pulsador)
        {
            digitalWrite(pinVentilador, HIGH);
        }
        else
        {
            digitalWrite(pinVentilador, LOW);
        }
    }
    lcd.setCursor(0, 0);
    lcd.print("Temperatura:");
    lcd.print(temperatura);
    lcd.setCursor(15, 0);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humedad: ");
    lcd.print(humedad);
    lcd.setCursor(15, 1);
    lcd.print("%");

    
    adc = analogRead(pot);
    Serial.print("Potenciometro: ");
    Serial.println(adc);
    int pwm = map(adc, 0, 1023, 0, 255);
    analogWrite(pinMotor, pwm);

}
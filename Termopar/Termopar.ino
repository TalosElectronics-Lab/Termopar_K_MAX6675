#include <Wire.h>
#include <LiquidCrystal_I2C.h>      //If you don't have the LiquidCrystal_I2C library, download it and install it
LiquidCrystal_I2C lcd(0x27, 20, 4); //sometimes the adress is not 0x3f. Change to 0x27 if it dosn't work.

#include <SPI.h>

#define MAX6675_SO 2
#define MAX6675_CS 3
#define MAX6675_SCK 4

void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5,1);
    lcd.print("Bienvenido");
    delay(1500);
    lcd.clear();
}

void loop()
{
    float temperatura = leer_termopar();
    lcd.setCursor(2, 1);
    lcd.print("Termopar tipo K");
    lcd.setCursor(7, 2);
    lcd.print(temperatura, 2);
    delay(300);
}

double leer_termopar()
{

    uint16_t v;
    pinMode(MAX6675_CS, OUTPUT);
    pinMode(MAX6675_SO, INPUT);
    pinMode(MAX6675_SCK, OUTPUT);

    digitalWrite(MAX6675_CS, LOW);
    delay(1);

    // Read in 16 bits,
    //  15    = 0 always
    //  14..2 = 0.25 degree counts MSB First
    //  2     = 1 if thermocouple is open circuit
    //  1..0  = uninteresting status

    v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
    v <<= 8;
    v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);

    digitalWrite(MAX6675_CS, HIGH);
    if (v & 0x4)
    {
        // Bit 2 indicates if the thermocouple is disconnected
        return NAN;
    }

    // The lower three bits (0,1,2) are discarded status bits
    v >>= 3;

    // The remaining bits are the number of 0.25 degree (C) counts
    return v * 0.25;
}
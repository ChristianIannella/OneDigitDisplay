#include <Arduino.h>
#include <Wire.h>

byte address[4] = {1, 2, 3, 4};
byte command[2];
byte numbers[10][2] = {{48, 0},
                       {49, 0},
                       {50, 0},
                       {51, 0},
                       {52, 0},
                       {53, 0},
                       {54, 0},
                       {55, 0},
                       {56, 0},
                       {57, 0}};
byte erase[2] = {10, 0};

void print_number(int number_to_stamp, int cursor);
void print_string(char string_to_stamp[], int cursor);
void erase_screen();
void colori();
void bianco();
void rosso();
void rainbow();
void verde();
void print_gradi(int cursore);
void dp_blink(int speed, int cursore);
void dp_stop(int onoff, int cursore);
void animazione();

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello clock");
  Wire.begin();
  delay(100);

  command[0] = 39;
  command[1] = 0;
  for (uint8_t i = 0; i < sizeof address; i++)
  {
    Wire.beginTransmission(address[i]);
    Wire.write(command, sizeof command);
    Wire.endTransmission();
  }
}

void loop()
{
  print_string("CIAO", 0);
  colori();
  delay(2000);
  erase_screen();
  print_gradi(0);
  print_string("23", 1);
  rainbow();
  delay(1000);
  erase_screen();
  bianco();
  for (int i = 0; i < 20; i++)
  {
    print_number(i, 0);
    delay(200);
  }
  delay(200);
  rosso();
  for (int i = 20; i > 0; i--)
  {
    print_number(i, 0);
    delay(200);
  }
  delay(200);
  verde();
  print_string("----", 0);
  delay(1000);
  dp_blink(3, 2);
  print_string("1315", 0);
  rainbow();
  dp_stop(0, 2);
  erase_screen();
  bianco();
  dp_blink(0, 0);
  dp_blink(2, 1);
  dp_blink(4, 2);
  dp_blink(6, 3);
  delay(5000);
  dp_stop(0, 0);
  dp_stop(0, 1);
  dp_stop(0, 2);
  dp_stop(0, 3);
  animazione();
}

void print_number(int number_to_stamp, int cursor)
{

  char array_data[10];
  sprintf(array_data, "%d", number_to_stamp);

  if (number_to_stamp > 9999)
  {
    Serial.println("Overflow");
  }

  else if (number_to_stamp <= 9999)
  {
    erase_screen();
    if (number_to_stamp > 999)
    {
      int index = array_data[0];
      Serial.println(index - 48);
      Wire.beginTransmission(address[3 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();

      index = array_data[1];
      Serial.println(index - 48);
      Wire.beginTransmission(address[2 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();

      index = array_data[2];
      Serial.println(index - 48);
      Wire.beginTransmission(address[1 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();

      index = array_data[3];
      Serial.println(index - 48);
      Wire.beginTransmission(address[0 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();
    }
    else if (number_to_stamp > 99)
    {
      Serial.println("3 cifre");
      int index = array_data[0];
      Serial.print(index - 48);
      Wire.beginTransmission(address[2 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();

      index = array_data[1];
      Serial.print(index - 48);
      Wire.beginTransmission(address[1 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();

      index = array_data[2];
      Serial.print(index - 48);
      Wire.beginTransmission(address[0 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();
      Serial.println(" ");
    }

    else if (number_to_stamp > 9)
    {
      int index = array_data[0];
      Wire.beginTransmission(address[1 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();

      index = array_data[1];
      Wire.beginTransmission(address[0 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();
    }

    else
    {
      int index = array_data[0];
      Wire.beginTransmission(address[0 + cursor]);
      Wire.write(numbers[index - 48], sizeof numbers[index - 48]);
      Wire.endTransmission();
    }
  }
}

void print_string(char string_to_stamp[], int cursor)
{

  for (uint8_t i = 0; i < 4; i++)
  {
    if (string_to_stamp[i] == NULL)
    {
      break;
    }
    command[0] = string_to_stamp[i];
    command[1] = 0;
    Wire.beginTransmission(address[(3 - i) - cursor]);
    Wire.write(command, sizeof command);
    Wire.endTransmission();
  }
}

void erase_screen()
{
  for (uint8_t i = 0; i < sizeof address; i++)
  {
    Wire.beginTransmission(address[i]);
    Wire.write(erase, sizeof erase);
    Wire.endTransmission();
  }
}

void colori()
{
  command[0] = 36;
  command[1] = 255;
  Wire.beginTransmission(address[0]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  command[0] = 36;
  command[1] = 210;
  Wire.beginTransmission(address[1]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  command[0] = 36;
  command[1] = 120;
  Wire.beginTransmission(address[2]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  command[0] = 36;
  command[1] = 50;
  Wire.beginTransmission(address[3]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
}

void rainbow()
{
  command[0] = 37;
  command[1] = 200;
  for (uint8_t i = 0; i < sizeof address; i++)
  {
    Wire.beginTransmission(address[i]);
    Wire.write(command, sizeof command);
    Wire.endTransmission();
  }
  for (int e = 0; e < 6; e++)
  {
    command[0] = 38;
    command[1] = 1 + (10 * e);
    for (uint8_t i = 0; i < sizeof address; i++)
    {
      Wire.beginTransmission(address[i]);
      Wire.write(command, sizeof command);
      Wire.endTransmission();
    }
    delay(1000);
  }
}

void bianco()
{
  command[0] = 36;
  command[1] = 125;
  for (uint8_t i = 0; i < sizeof address; i++)
  {
    Wire.beginTransmission(address[i]);
    Wire.write(command, sizeof command);
    Wire.endTransmission();
  }
}

void rosso()
{
  command[0] = 36;
  command[1] = 1;
  for (uint8_t i = 0; i < sizeof address; i++)
  {
    Wire.beginTransmission(address[i]);
    Wire.write(command, sizeof command);
    Wire.endTransmission();
  }
}

void verde()
{
  command[0] = 36;
  command[1] = 50;
  for (uint8_t i = 0; i < sizeof address; i++)
  {
    Wire.beginTransmission(address[i]);
    Wire.write(command, sizeof command);
    Wire.endTransmission();
  }
}

void print_gradi(int cursore)
{
  command[0] = 20;
  command[1] = 0;
  Wire.beginTransmission(address[cursore]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
}

void dp_blink(int speed, int cursore)
{
  command[0] = 34;
  command[1] = speed;
  Wire.beginTransmission(address[cursore]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
}

void dp_stop(int onoff, int cursore)
{
  command[0] = 35;
  command[1] = onoff;
  Wire.beginTransmission(address[cursore]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
}

int pausa = 80;
void animazione()
{
  for (int i = 0; i < 10; i++)
  {
  command[0] = 27;
  command[1] = 0;
  Wire.beginTransmission(address[3]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 27;
  command[1] = 0;
  Wire.beginTransmission(address[2]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 27;
  command[1] = 0;
  Wire.beginTransmission(address[1]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 27;
  command[1] = 0;
  Wire.beginTransmission(address[0]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 28;
  command[1] = 0;
  Wire.beginTransmission(address[0]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 29;
  command[1] = 0;
  Wire.beginTransmission(address[0]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 30;
  command[1] = 0;
  Wire.beginTransmission(address[0]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 30;
  command[1] = 0;
  Wire.beginTransmission(address[1]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 30;
  command[1] = 0;
  Wire.beginTransmission(address[2]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 30;
  command[1] = 0;
  Wire.beginTransmission(address[3]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 31;
  command[1] = 0;
  Wire.beginTransmission(address[3]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
  command[0] = 40;
  command[1] = 0;
  Wire.beginTransmission(address[3]);
  Wire.write(command, sizeof command);
  Wire.endTransmission();
  
  delay(pausa);
  erase_screen();
}
}
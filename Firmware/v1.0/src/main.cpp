#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define RING_PIN 10
#define NUM_LED 4
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUM_LED, RING_PIN, NEO_GRB + NEO_KHZ800);

#define E 2
#define D 3
#define C 4
#define DP 5
#define G 6
#define B 7
#define A 8
#define F 9
#define COM A3
#define b_l 10
#define DS0 A2
#define DS1 A1
#define DS2 A0

int input_pin[3] = {DS0, DS1, DS2};
byte i2c_address_input[3] = {0};
int i2c_address = 0;

unsigned long previous = 0;
unsigned long previous_dp_blink = 0;

int b_l_value = 0;
bool state = false;
bool dp_blynk_state = false;
unsigned long dp_delay = 20;
bool dp = false;
int digit = 10;

// First byte  Second byte     Symbol
void print_zero();      // 0           (0 or 1)        0
void print_one();       // 1           (0 or 1)        1
void print_two();       // 2           (0 or 1)        2
void print_three();     // 3           (0 or 1)        3
void print_four();      // 4           (0 or 1)        4
void print_five();      // 5           (0 or 1)        5
void print_six();       // 6           (0 or 1)        6
void print_seven();     // 7           (0 or 1)        7
void print_eight();     // 8           (0 or 1)        8
void print_nine();      // 9           (0 or 1)        9
void erase();           // 10             x
void show_dp();         // 11             x            .
void print_dash();      // 12          (0 or 1)        -
void print_degrees();   // 13          (0 or 1)        ˚
void print_A();         // 14          (0 or 1)        A
void print_b();         // 15          (0 or 1)        b
void print_c();         // 16          (0 or 1)        c
void print_C();         // 17          (0 or 1)        C
void print_d();         // 18          (0 or 1)        d
void print_E();         // 19          (0 or 1)        E
void print_F();         // 20          (0 or 1)        F
void print_H();         // 21          (0 or 1)        H
void print_I();         // 22          (0 or 1)        I
void print_L();         // 23          (0 or 1)        L
void print_P();         // 24          (0 or 1)        P
void print_U();         // 25          (0 or 1)        U
void print_u();         // 26          (0 or 1)        u
void print_segment_a(); // 27          (0 or 1)
void print_segment_b(); // 28          (0 or 1)
void print_segment_c(); // 29          (0 or 1)
void print_segment_d(); // 30          (0 or 1)
void print_segment_e(); // 31          (0 or 1)
void print_segment_f(); // 32          (0 or 1)
void print_segment_g(); // 33          (0 or 1)
void dp_blink();        // 34          (1 to 7)
void dp_blink_stop();   // 35          (0 or 1)
void back_light();      // 36          (0 to 255)       Backlight color HSV
void brightness();      // 37          (0 to 255)       Backlight brightness

void receiveEvent(int data)
{
  if (Wire.available() == 2)
  {

    int first_byte = Wire.read();
    int second_byte = Wire.read();

    if (first_byte == 34)
    {
      if (digit == 11)
      {
        digit = 10;
      }
      dp_blynk_state = true;
      if (second_byte == 0)
      {
        dp_delay = 100;
      }
      else if (second_byte == 1)
      {
        dp_delay = 200;
      }
      else if (second_byte == 2)
      {
        dp_delay = 300;
      }
      else if (second_byte == 3)
      {
        dp_delay = 400;
      }
      else if (second_byte == 4)
      {
        dp_delay = 500;
      }
      else if (second_byte == 5)
      {
        dp_delay = 1000;
      }
      else if (second_byte == 6)
      {
        dp_delay = 1500;
      }
      else if (second_byte == 7)
      {
        dp_delay = 2000;
      }
    }
    else if (first_byte == 35)
    {
      dp_blynk_state = false;
      if (second_byte == 0)
      {
        dp = false;
      }
      else if (second_byte == 1)
      {
        dp = true;
      }
    }
    else if (first_byte == 36)
    {
      for (int i = 0; i <= NUM_LED; i++)
      {
        pixel.setPixelColor(i, pixel.gamma32(pixel.ColorHSV(second_byte * 254)));
      }
      pixel.show();
    }
    else if (first_byte == 37)
    {
      pixel.setBrightness(second_byte);
      pixel.show();
    }
    else
    {
      digit = first_byte;
      if (dp_blynk_state == 0)
      {
        dp = second_byte;
      }
    }
  }
}

void requestEvent()
{
}

void setup()
{
  pinMode(COM, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);
  pinMode(b_l, OUTPUT);
  pinMode(DS0, INPUT_PULLUP);
  pinMode(DS1, INPUT_PULLUP);
  pinMode(DS2, INPUT_PULLUP);

  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(DP, LOW);
  digitalWrite(COM, LOW);
  analogWrite(b_l, LOW);

  for (int a = 0; a < 3; a++)
  {
    i2c_address_input[a] = !digitalRead(input_pin[a]);
    i2c_address = i2c_address * 2 + i2c_address_input[a];
  }

  Wire.begin(i2c_address);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  pixel.begin();
  pixel.clear();
  pixel.show();
}

void loop()
{
  unsigned long current_dp_blink = millis();
  unsigned long current = millis();

  if (dp_blynk_state == true)
  {
    if (current_dp_blink - previous_dp_blink >= dp_delay)
    {
      previous_dp_blink = current_dp_blink;
      dp = !dp;
    }
  }

  if (current - previous >= 5)
  {
    previous = current;
    state = !state;
    digitalWrite(COM, state);

    if (digit == 0)
    {
      print_zero();
    }
    else if (digit == 1)
    {
      print_one();
    }
    else if (digit == 2)
    {
      print_two();
    }
    else if (digit == 3)
    {
      print_three();
    }
    else if (digit == 4)
    {
      print_four();
    }
    else if (digit == 5)
    {
      print_five();
    }
    else if (digit == 6)
    {
      print_six();
    }
    else if (digit == 7)
    {
      print_seven();
    }
    else if (digit == 8)
    {
      print_eight();
    }
    else if (digit == 9)
    {
      print_nine();
    }
    else if (digit == 10)
    {
      erase();
    }
    else if (digit == 11)
    {
      show_dp();
    }
    else if (digit == 12)
    {
      print_dash();
    }
    else if (digit == 13)
    {
      print_degrees();
    }
    else if (digit == 14)
    {
      print_A();
    }
    else if (digit == 15)
    {
      print_b();
    }
    else if (digit == 16)
    {
      print_c();
    }
    else if (digit == 17)
    {
      print_C();
    }
    else if (digit == 18)
    {
      print_d();
    }
    else if (digit == 19)
    {
      print_E();
    }
    else if (digit == 20)
    {
      print_F();
    }
    else if (digit == 21)
    {
      print_H();
    }
    else if (digit == 22)
    {
      print_I();
    }
    else if (digit == 23)
    {
      print_L();
    }
    else if (digit == 24)
    {
      print_P();
    }
    else if (digit == 25)
    {
      print_U();
    }
    else if (digit == 26)
    {
      print_u();
    }
    else if (digit == 27)
    {
      print_segment_a();
    }
    else if (digit == 28)
    {
      print_segment_b();
    }
    else if (digit == 29)
    {
      print_segment_c();
    }
    else if (digit == 30)
    {
      print_segment_d();
    }
    else if (digit == 31)
    {
      print_segment_e();
    }
    else if (digit == 32)
    {
      print_segment_f();
    }
    else if (digit == 33)
    {
      print_segment_g();
    }
  }
}

void print_zero()
{
  digitalWrite(A, !state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_one()
{
  digitalWrite(A, state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_two()
{
  digitalWrite(A, !state);
  digitalWrite(B, !state);
  digitalWrite(C, state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_three()
{
  digitalWrite(A, !state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_four()
{
  digitalWrite(A, state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_five()
{
  digitalWrite(A, !state);
  digitalWrite(B, state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_six()
{
  digitalWrite(A, !state);
  digitalWrite(B, state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_seven()
{
  digitalWrite(A, !state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_eight()
{
  digitalWrite(A, !state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_nine()
{
  digitalWrite(A, !state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void erase()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  digitalWrite(DP, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void show_dp()
{
  dp = true;
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_dash()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_degrees()
{
  digitalWrite(A, !state);
  digitalWrite(B, !state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_A()
{
  digitalWrite(A, !state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_b()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_c()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_C()
{
  digitalWrite(A, !state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_d()
{
  digitalWrite(A, state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_E()
{
  digitalWrite(A, !state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_F()
{
  digitalWrite(A, !state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_H()
{
  digitalWrite(A, state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_I()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_L()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_P()
{
  digitalWrite(A, !state);
  digitalWrite(B, !state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_U()
{
  digitalWrite(A, state);
  digitalWrite(B, !state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, !state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_u()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, !state);
  digitalWrite(D, !state);
  digitalWrite(E, !state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_segment_a()
{
  digitalWrite(A, !state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_segment_b()
{
  digitalWrite(A, state);
  digitalWrite(B, !state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_segment_c()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, !state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_segment_d()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, !state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_segment_e()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, !state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_segment_f()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, !state);
  digitalWrite(G, state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
void print_segment_g()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, !state);
  if (dp == true)
  {
    digitalWrite(DP, !state);
  }
  else
  {
    digitalWrite(DP, state);
  }
}
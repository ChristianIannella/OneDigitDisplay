#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 10
#define NUM_LED 4
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUM_LED, LED_PIN, NEO_GRB + NEO_KHZ800);

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
long firstPixelHue = 0;
unsigned long t = 0;

int b_l_value = 0;
bool state = false;
bool dp_blynk_state = false;
unsigned long dp_delay = 20;
bool dp = false;
bool rainbow_state = false;
int rainbow_speed = 0;
int digit = 10;

//                      First byte  Second byte     Symbol
void print_space();     // 32             x          space
void print_zero();      // 48          (0 or 1)        0
void print_one();       // 49          (0 or 1)        1
void print_two();       // 50          (0 or 1)        2
void print_three();     // 51          (0 or 1)        3
void print_four();      // 52          (0 or 1)        4
void print_five();      // 53          (0 or 1)        5
void print_six();       // 54          (0 or 1)        6
void print_seven();     // 55          (0 or 1)        7
void print_eight();     // 56          (0 or 1)        8
void print_nine();      // 57          (0 or 1)        9
void erase();           // 10             x
void show_dp();         // 46             x            .
void print_dash();      // 45          (0 or 1)        -
void print_degrees();   // 20          (0 or 1)        ˚
void print_A();         // 65          (0 or 1)        A
void print_b();         // 98          (0 or 1)        b
void print_c();         // 99          (0 or 1)        c
void print_C();         // 67          (0 or 1)        C
void print_d();         // 100         (0 or 1)        d
void print_E();         // 69          (0 or 1)        E
void print_F();         // 70          (0 or 1)        F
void print_H();         // 72          (0 or 1)        H
void print_I();         // 73          (0 or 1)        I
void print_L();         // 76          (0 or 1)        L
void print_O();         // 79          (0 or 1)        O
void print_P();         // 80          (0 or 1)        P
void print_S();         // 83          (0 or 1)        S
void print_U();         // 85          (0 or 1)        U
void print_u();         // 117         (0 or 1)        u
void print_segment_a(); // 27          (0 or 1)
void print_segment_b(); // 28          (0 or 1)
void print_segment_c(); // 29          (0 or 1)
void print_segment_d(); // 30          (0 or 1)
void print_segment_e(); // 31          (0 or 1)
void print_segment_f(); // 40          (0 or 1)
void print_segment_g(); // 33          (0 or 1)
                        // 34          (1 to 7)         dp blink
                        // 35          (0 or 1)         dp stop
                        // 37          (0 to 255)       Backlight brightness
                        // 38          (1 to 255)       Rainbow
                        // 39             x             erase all

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
      rainbow_state = false;
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
    else if (first_byte == 38)
    {
      rainbow_state = true;
      rainbow_speed = second_byte;
    }
    else if (first_byte == 39)
    {
      rainbow_state = false;
      dp_blynk_state = false;
      dp = false;
      for (int i = 0; i <= NUM_LED; i++)
      {
        pixel.setPixelColor(i,0,0,0);
      }
      pixel.show();
      digit = 10;
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

  raimbow();

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

    if (digit == 48)
    {
      print_zero();
    }
    else if (digit == 32)
    {
      print_space();
    }
    else if (digit == 49)
    {
      print_one();
    }
    else if (digit == 50)
    {
      print_two();
    }
    else if (digit == 51)
    {
      print_three();
    }
    else if (digit == 52)
    {
      print_four();
    }
    else if (digit == 53)
    {
      print_five();
    }
    else if (digit == 54)
    {
      print_six();
    }
    else if (digit == 55)
    {
      print_seven();
    }
    else if (digit == 56)
    {
      print_eight();
    }
    else if (digit == 57)
    {
      print_nine();
    }
    else if (digit == 10)
    {
      erase();
    }
    else if (digit == 46)
    {
      show_dp();
    }
    else if (digit == 45)
    {
      print_dash();
    }
    else if (digit == 20)
    {
      print_degrees();
    }
    else if (digit == 65)
    {
      print_A();
    }
    else if (digit == 98)
    {
      print_b();
    }
    else if (digit == 99)
    {
      print_c();
    }
    else if (digit == 67)
    {
      print_C();
    }
    else if (digit == 100)
    {
      print_d();
    }
    else if (digit == 69)
    {
      print_E();
    }
    else if (digit == 70)
    {
      print_F();
    }
    else if (digit == 72)
    {
      print_H();
    }
    else if (digit == 73)
    {
      print_I();
    }
    else if (digit == 76)
    {
      print_L();
    }
    else if (digit == 79)
    {
      print_O();
    }
    else if (digit == 80)
    {
      print_P();
    }
    else if (digit == 83)
    {
      print_S();
    }
    else if (digit == 85)
    {
      print_U();
    }
    else if (digit == 117)
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
    else if (digit == 40)
    {
      print_segment_f();
    }
    else if (digit == 33)
    {
      print_segment_g();
    }
  }
}
void print_space()
{
  digitalWrite(A, state);
  digitalWrite(B, state);
  digitalWrite(C, state);
  digitalWrite(D, state);
  digitalWrite(E, state);
  digitalWrite(F, state);
  digitalWrite(G, state);
  digitalWrite(DP, state);
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
void print_O()
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
void print_S()
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

void raimbow()
{
  if (rainbow_state == true)
  {
    if (millis() > t + 1+(1*rainbow_speed))
    {
      t = millis();
      for (int i = 0; i < pixel.numPixels(); i++)
      {
        int pixelHue = firstPixelHue  + (i * 65536L / pixel.numPixels());
        // int pixelHue = firstPixelHue + (65536L / ring.numPixels());
        pixel.setPixelColor(i, pixel.gamma32(pixel.ColorHSV(pixelHue)));
      }
      pixel.show();
      if (firstPixelHue > 5 * 65536)
      {
        firstPixelHue = 1000 * i2c_address;
      }
      firstPixelHue += 256;
    }
  }
}
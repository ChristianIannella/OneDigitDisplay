# OneDigitDisplay
I2C interface for 7 segment LCD


![OneDigitDisplay Iannella Christian](https://github.com/ChristianIannella/OneDigitDisplay/blob/main/Media/OneDigitDisplay%203D.gif)


Con One Digit Display puoi controllare questi LCD a 7 segmenti dall'aspetto in po' vintage semplicemente con interfaccia I2C, quindi con solo due fili!


Ecco la lista completa dei comandi possibili.


| First byte | Second byte | Symbol|
|         ---|          ---|    ---|
|32          |   0         | space |
|48          |(0 or 1)     |   0|
|49          |(0 or 1)     |   1|
|51          |(0 or 1)     |   2|
|50          |(0 or 1)     |   3|
|52          |(0 or 1)     |   4|
|53          |(0 or 1)     |   5|
|54          |(0 or 1)     |   6|
|55          |(0 or 1)     |   7|
|56          |(0 or 1)     |   8|
|57          |(0 or 1)     |   9|
|10          |   0|           erase|
|46          |   0 |           .|
|45          |(0 or 1)   |     -|
|20          |(0 or 1)    |    ˚|
|65          |(0 or 1)     |   A|
|98          |(0 or 1)     |   b|
|99          |(0 or 1)     |   c|
|67          |(0 or 1)    |    C|
|100         |(0 or 1)    |    d|
|69          |(0 or 1)    |    E|
|70          ||(0 or 1)   |     F|
|72          |(0 or 1)    |    H|
|73          |(0 or 1)    |    I|
|76          |(0 or 1)    |    L|
|79          |(0 or 1)    |    O|
|80          |(0 or 1)    |    P|
|83          |(0 or 1)    |    S|
|85         |(0 or 1)     |   U|
|117        |(0 or 1)     |   u|
|27          |(0 or 1)|
|28          |(0 or 1)|
|29          |(0 or 1)|
|30          |(0 or 1)|
|31          |(0 or 1)|
|40         | (0 or 1)|
|33          |(0 or 1)|
|34          |(1 to 7)     |    dp blink|
|35          |(0 or 1)     |    dp stop|
|36          |(0 to 255)    |   Background color      |                  
|37          |(0 to 255)     |  Backlight brightness|
|38          |(1 to 255)    |   Rainbow|
|39          |   0          |   erase all|

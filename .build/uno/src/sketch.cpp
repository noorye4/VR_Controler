#include <Arduino.h>

void setup();
void loop();
void blink(int dealy_time,int loop_time);
void get_flex_init();
#line 1 "src/sketch.ino"
#define fsr_pin_0 A0
#define fsr_pin_1 A1
#define fsr_pin_2 A2
#define fsr_pin_3 A3
#define flex_pin_4 A4 //L knee
#define flex_pin_5 A5 //R knee

int led = 13;
int flex_offset = 40;
int bend_L;
int bend_R;
int stand_L;
int stand_R;

int stand_L_min;
int stand_L_max;

int stand_R_max;
int stand_R_min;


int bend_L_min;
int bend_L_max;

int bend_R_min;
int bend_R_max;

bool stand = false;
bool crouch = false;
bool walk = false;
bool listen = false;
bool setting = false;




void setup()
{
    pinMode(led, OUTPUT);
    Serial1.begin(9600);
    blink(500,4);
}

void loop()
{

    unsigned char charreceived = Serial1.read();

    if (charreceived == 'q')
       listen = true;
    if (charreceived == 'w')
       listen = false;
    if (charreceived == 'e')
        setting = true;
    if (setting)
        get_flex_init();

    if (listen)
    {
        int fsr_value_0 = analogRead(fsr_pin_0);
        int fsr_value_1 = analogRead(fsr_pin_1);
        int fsr_value_2 = analogRead(fsr_pin_2);
        int fsr_value_3 = analogRead(fsr_pin_3);
        int flex_value_L = analogRead(flex_pin_4);
        int flex_value_R = analogRead(flex_pin_5);

        /*Serial1.println(fsr_value_3);*/

        if (flex_value_L > stand_L_min  and flex_value_L < stand_L_max and flex_value_R > stand_R_min  and flex_value_R < stand_R_max)
        {
            stand = true;
            crouch = false;
            walk = false;
        }

        if (flex_value_L > bend_L_min and flex_value_L < bend_L_max and flex_value_R > bend_R_min and flex_value_R < bend_R_max)
        {
            stand = false;
            crouch = true;
            walk = false;
        }

        if (flex_value_L > bend_L_min and flex_value_L < bend_L_max and flex_value_R > stand_R_min  and flex_value_R < stand_R_max)
        {
            walk = true;
            stand = false;
            crouch = false;
        }

        if (flex_value_R > bend_R_min and flex_value_R < bend_R_max and flex_value_L > stand_L_min  and flex_value_L < stand_L_max)
        {
            walk = true;
            stand = false;
            crouch = false;
        }

        if (stand)
            Serial1.println("stand");
        if (crouch)
            Serial1.println("crouch");
        if (walk)
            Serial1.println("walk");

        if (fsr_value_0 < 700 && fsr_value_1 < 700)
            Serial1.println("01");

        if (fsr_value_0 < 700 && fsr_value_2 < 700)
            Serial1.println("02");

        if (fsr_value_0 < 700 && fsr_value_3 < 700)
            Serial1.println("03");
    }

        Serial1.flush();



}


void blink(int dealy_time,int loop_time)
{
    for (int i = 0; i < loop_time; i++)
    {
        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(dealy_time);               // wait for a second
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        delay(dealy_time);               // wait for a second
    }
}

void get_flex_init()
{
    int flex_value_L;
    int flex_value_R;
    blink(3000,1);
    flex_value_L = analogRead(flex_pin_4);
    flex_value_R = analogRead(flex_pin_5);
    stand_L = flex_value_L;
    stand_R = flex_value_R;
    Serial1.print("L stand :");
    Serial1.print(stand_L);
    Serial1.print("    ");
    Serial1.print("R stand :");
    Serial1.print(stand_R);
    Serial1.println();
    blink(200,2);
    delay(2000);

    blink(3000,1);
    flex_value_R = analogRead(flex_pin_5);
    bend_R = flex_value_R;
    Serial1.print("R bend :");
    Serial1.print(bend_R);
    Serial1.println();
    blink(200,2);
    delay(2000);

    blink(3000,1);
    flex_value_L = analogRead(flex_pin_4);
    bend_L = flex_value_L;
    Serial1.print("L bend :");
    Serial1.print(bend_L);
    Serial1.println();
    blink(200,2);

    stand_L_min = stand_L - flex_offset;
    stand_L_max = stand_L + flex_offset;

    stand_R_min = stand_R - flex_offset;
    stand_R_max = stand_R + flex_offset;

    bend_L_min = bend_L - flex_offset;
    bend_L_max = bend_L + flex_offset;

    bend_R_min = bend_L - flex_offset;
    bend_R_max = bend_L + flex_offset;

    setting = false;

    Serial1.flush();
}

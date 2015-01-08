#include <Arduino.h>

void setup();
void loop();
int GetCurrState();
void SendKey(int curr_state);
void blink(int dealy_time,int loop_time);
void knee_init();
#line 1 "src/sketch.ino"
#define fsr_pin_0 A0 //thumb finger
#define fsr_pin_1 A1 //fore finger
#define fsr_pin_2 A2 //middle finger
#define fsr_pin_3 A3 //ring finger
#define flex_pin_4 A4 //L knee
#define flex_pin_5 A5 //R knee

const int led = 13;

/*sensor value */
int flex_value_L;
int flex_value_R;
int fsr_value_0;
int fsr_value_1;
int fsr_value_2;
int fsr_value_3;

/*people stance */
int curr_state = 0;
int last_state = 0;

/*process about knee flex variable*/
int flex_offset = 50;

int stand_L_min;
int stand_L_max;

int stand_R_max;
int stand_R_min;


int bend_L_min;
int bend_L_max;

int bend_R_min;
int bend_R_max;

/*start or stop listen stance event*/
bool listen = true;
bool setting = false;

unsigned long duration = 100;
unsigned long future_time;


void setup()
{
    pinMode(led, OUTPUT);
    Serial.begin(9600);
    blink(500,4);
    knee_init();
    future_time = millis() + duration;
    Keyboard.begin();
}

void loop()
{

    unsigned char charreceived = Serial.read();

    if (charreceived == 'q'){
        listen = true;
        Serial.println("Start Listen state...");
    }

    if (charreceived == 'w'){
        listen = false;
        Serial.println("Stop Listen state...");
    }

    if (charreceived == 'e'){
        setting = true;
        Serial.println("setting knee value");
    }

    if (setting)
        knee_init();

    if (listen){
        if (millis() >= future_time){
            int curr_state = GetCurrState();

            //if state chagne send keyboard event
            if (last_state != curr_state){
                SendKey(curr_state);
                last_state = curr_state;
                Serial.print("curr state  ");
                Serial.println(curr_state);
                future_time = millis() + duration;
            }
        }

    }

}

int GetCurrState(){

    fsr_value_0 = analogRead(fsr_pin_0);
    fsr_value_1 = analogRead(fsr_pin_1);
    fsr_value_2 = analogRead(fsr_pin_2);
    fsr_value_3 = analogRead(fsr_pin_3);
    flex_value_L = analogRead(flex_pin_4);
    flex_value_R = analogRead(flex_pin_5);

    //stand stance
    if (flex_value_L > stand_L_min  and flex_value_L < stand_L_max and flex_value_R > stand_R_min  and flex_value_R < stand_R_max)
        curr_state = 0;

    //crouch stance
    if (flex_value_L > bend_L_min and flex_value_L < bend_L_max and flex_value_R > bend_R_min and flex_value_R < bend_R_max)
        curr_state = 1;

    //walk state
    if (flex_value_L > bend_L_min and flex_value_L < bend_L_max and flex_value_R > stand_R_min  and flex_value_R < stand_R_max)
        curr_state = 2;

    //walk state
    if (flex_value_R > bend_R_min and flex_value_R < bend_R_max and flex_value_L > stand_L_min  and flex_value_L < stand_L_max)
        curr_state = 2;

    if (fsr_value_0 < 700 and fsr_value_1 < 700)
        curr_state = 3;

    if (fsr_value_0 < 700 and fsr_value_2 < 700)
        curr_state = 4;

    if (fsr_value_0 < 700 and fsr_value_3 < 700)
        curr_state = 5;

    if (fsr_value_1 < 500 and fsr_value_0 > 800 and fsr_value_2 > 800 and fsr_value_3 > 800)
        curr_state = 6;

    if (fsr_value_2 < 500 and fsr_value_1 > 800 and fsr_value_0 > 800 and fsr_value_3 > 800)
        curr_state = 7;

    if (fsr_value_3 < 500 and fsr_value_0 > 800 and fsr_value_1 > 800 and fsr_value_2 > 800)
        curr_state = 8;

    return curr_state;
}

/*follow the state to control what keyboard event to send*/
void SendKey(int curr_state){

    /*body state*/
    if (curr_state == 0)
        Keyboard.release('c');

    if (curr_state == 1)
        Keyboard.press('c');

    /*finger state*/
    if (curr_state == 3)
        Keyboard.press('1');
    else
        Keyboard.release('1');

    if (curr_state == 4)
        Keyboard.press('2');
    else
        Keyboard.release('2');

    if (curr_state == 5)
        Keyboard.press('3');
    else
        Keyboard.release('3');

    if (curr_state == 6)
        Keyboard.press('w');
    else
        Keyboard.release('w');

    if (curr_state == 7)
        Keyboard.press('s');
    else
        Keyboard.release('s');
}

void blink(int dealy_time,int loop_time)
{
    for (int i = 0; i < loop_time; i++){
        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(dealy_time);               // wait for a second
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        delay(dealy_time);               // wait for a second
    }
}

void knee_init()
{
    int bend_L;
    int bend_R;
    int stand_L;
    int stand_R;

    blink(3000,1);
    flex_value_L = analogRead(flex_pin_4);
    flex_value_R = analogRead(flex_pin_5);
    stand_L = flex_value_L;
    stand_R = flex_value_R;
    Serial.print("L stand :");
    Serial.print(stand_L);
    Serial.print("    ");
    Serial.print("R stand :");
    Serial.print(stand_R);
    Serial.println();
    blink(200,2);
    delay(2000);

    blink(3000,1);
    flex_value_R = analogRead(flex_pin_5);
    bend_R = flex_value_R;
    Serial.print("R bend :");
    Serial.print(bend_R);
    Serial.println();
    blink(200,2);
    delay(2000);

    blink(3000,1);
    flex_value_L = analogRead(flex_pin_4);
    bend_L = flex_value_L;
    Serial.print("L bend :");
    Serial.print(bend_L);
    Serial.println();
    blink(200,2);

    stand_L_min = stand_L - flex_offset;
    stand_L_max = stand_L + flex_offset;

    stand_R_min = stand_R - flex_offset;
    stand_R_max = stand_R + flex_offset;

    bend_L_min = bend_L - flex_offset;
    bend_L_max = bend_L + flex_offset;

    bend_R_min = bend_L - flex_offset;
    bend_R_max = bend_L + flex_offset;

}

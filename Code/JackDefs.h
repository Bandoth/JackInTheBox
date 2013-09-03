#ifndef _JackDefs
#define JackDefs

/*
 * SD Pins:
 *    SS = 53
 *    MOSI = 51
 *    MISO = 50
 *    SCK = 52
 * DAC Pins:
 *    CS = 22
 *    SCK = 24
 *    SDI = 26
 * Servos:
 *    LidServo1    = pin 5
 *    LidServo2    = pin 6
 *    NeckServo    = pin 7
 *    TorsoServo   = pin 44
 *    StabServo    = pin 45
 *    WaveServo    = pin 46
 *    TalkServo    = pin 8
 * LEDs:
 *    pins 30-39
 * Actuator:
 *    pin 40
 * Button Input
 *    pin 41
 */
 
#define SERVO_1_OPEN  20
#define SERVO_2_OPEN  160
#define SERVO_1_CLOSE  160
#define SERVO_2_CLOSE  20

#define PinLEDOptics1 30
#define PinLEDOptics2 31
#define PinLEDBox1    32
#define PinLEDBox2    33
#define PinLEDBox3    34
#define PinLEDBox4    35
#define PinLEDBox5    36
#define PinLEDBox6    37
#define PinLEDBox7    38
#define PinLEDBox8    39

#define PinActuator   40
#define PinButtonInp  41
 
 #endif

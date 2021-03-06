#ifndef _JackDefs
#define JackDefs 1

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
 *    LidServo1    = pin 5    LEFT looking at front of box
 *    LidServo2    = pin 6    RIGHT looking at front of box
 *    NeckServo    = pin 7
 *    FlowerServo   = pin 44
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
 
// #define TESTRUN                  1
 
#define SERVO_1_OPEN                0
#define SERVO_2_OPEN                180
#define SERVO_1_CLOSE               180
#define SERVO_2_CLOSE               0

#define THRESH_ButtonTimeout        60
#define THRESH_ButtonPeriodSlowdown 11

#define AUDIO_MULT_BASE             128
#define AUDIO_MULT_MIN              116
#define AUDIO_MULT_MAX              140
#define AUDIO_MULT_SHIFT            7

#define PinLidServo1                5
#define PinLidServo2                6
#define PinNeckServo                7
#define PinTorsoServo               44
#define PinStabServo                45
#define PinWaveServo                46
#define PinTalkServo                8

#define PinLEDOptics1               30
#define PinLEDOptics2               31
#define PinLEDBox1                  32
#define PinLEDBox2                  33
#define PinLEDBox3                  34
#define PinLEDBox4                  35
#define PinLEDBox5                  36
#define PinLEDBox6                  37
#define PinLEDBox7                  38
#define PinLEDBox8                  39

#define PinActuator                 40
#define PinButtonInp                41

#define ACTUATOR_Down               LOW
#define ACTUATOR_Up                 HIGH

#define BASETASKTIME                2    // BASETASKTIME * 10ms = rate we run = 20ms
#define BUTTONTASKDELAY             1    // ThisTaskTime * BASETASKTIME * 10ms = rate this is called = 20ms
#define AUDIORATETASKDELAY          10   // ThisTaskTIme * BASETASKTIME * 10ms = rate this is handled = 200ms

#define EEPROMAddr_FirstWrite       1
#define EEPROMAddr_ButtonIndex      2
#define EEPROMAddr_PopTimerIndex    3
#define EEPROMAddr_RoutineIndex     4

#define PSEUDO_MaxRoutineChooser    29
//#define PSEUDO_MaxRoutineChooser    1
#define PSEUDO_MaxPressChooser      29
#define PSEUDO_MaxTimerChooser      50

 #endif

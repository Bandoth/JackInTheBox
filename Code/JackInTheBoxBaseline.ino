#include <avr/eeprom.h>
#include <EEPROM.h>
#include "WaveUtil.h"
#include "WaveHC.h"
#include "Typedefs.h"
#include "JackDefs.h"

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file for a pi digit or period
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time
UINT_8 AudioPlaybackMultiplier = 64;
UINT_32 BaseAudioRate;

uint8_t dirLevel; // indent level for file/dir names    (for prettyprinting)
dir_t dirBuf;     // buffer for directory reads

BoxState JackBoxState = _Waiting;
JackRoutine SelectedRoutine = _Routine1;

/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

// Function definitions (we define them here, but the code is below)
void play(FatReader &dir);

/* Setup routine automatically run first */
void setup()
{
    JackServo myServo;

    DigitalPinsInit();
    
    PWMSetup();

    myServo = _LidServo1;
    WriteServo(myServo, SERVO_1_OPEN);

    myServo = _LidServo2;
    WriteServo(myServo, SERVO_2_OPEN);

    myServo = _NeckServo;
    WriteServo(myServo, 90);

    myServo = _TorsoServo;
    WriteServo(myServo, 90);

    myServo = _StabServo;
    WriteServo(myServo, 90);

    myServo = _WaveServo;
    WriteServo(myServo, 90);

    myServo = _TalkServo;
    WriteServo(myServo, 90);
    
    SDCardInit();
    
    ScheduleTimerSetup();
}

/* Loop routine runs continuously forever */
void loop()
{
    JackBoxState = _Playing;

    while(!OCF0A);
    TIFR0 |= 0b00000010;    // Clear flag
    
    JackStateHandler();
    MusicStateMachine();
    ServoStateMachine();
    LEDStateMachine();
}


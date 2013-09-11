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
UINT_16 AudioPlaybackMultiplier = AUDIO_MULT_BASE;
UINT_32 BaseAudioRate;
UINT_8 AudioRateSet = 0;

uint8_t dirLevel; // indent level for file/dir names    (for prettyprinting)
dir_t dirBuf;     // buffer for directory reads

BoxState JackBoxState = _Waiting;
JackRoutine SelectedRoutine = _Routine1;
UINT_8 THRESH_ButtonPop = 0;
UINT_16 THRESH_WeaselMusicTimer = 0;

UINT_32 ScheduleTimer = 0;
UINT_16 DelCounter = 0;

extern const JackRoutine RoutineChooser[30];
extern const UINT_8 ButtonPressChooser[30];
extern const UINT_16 ButtonTimerChooser[51];

/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

// Function definitions (we define them here, but the code is below)
void play(FatReader &dir);

/* Setup routine automatically run first */
void setup()
{
    UINT_8 index = 0;
    
    DigitalPinsInit();
    
    PWMSetup();

    WriteServo(_LidServo1, SERVO_1_OPEN);
    WriteServo(_LidServo2, SERVO_2_OPEN);
    WriteServo(_NeckServo, 90);
    WriteServo(_FlowerServo, 90);
    WriteServo(_StabServo, 90);
    WriteServo(_WaveServo, 90);
    WriteServo(_TalkServo, 90);
    
    SDCardInit();
    
    if (EEPROM.read(EEPROMAddr_FirstWrite) != 0xA5)
    {
        EEPROM.write(EEPROMAddr_FirstWrite, 0xA5);
        EEPROM.write(EEPROMAddr_ButtonIndex, 1);
        EEPROM.write(EEPROMAddr_PopTimerIndex, 1);
        EEPROM.write(EEPROMAddr_RoutineIndex, 1);
        
        SelectedRoutine = (JackRoutine)RoutineChooser[0];
        THRESH_ButtonPop = ButtonPressChooser[0];
        THRESH_WeaselMusicTimer = ButtonTimerChooser[0];
    }
    else
    {
        index = EEPROM.read(EEPROMAddr_ButtonIndex);
        THRESH_ButtonPop = ButtonPressChooser[index];
        index++;
        if (index > PSEUDO_MaxPressChooser)
        {
            index = 0;
        }
        EEPROM.write(EEPROMAddr_ButtonIndex, index);
        
        index = EEPROM.read(EEPROMAddr_PopTimerIndex);
        THRESH_WeaselMusicTimer = ButtonTimerChooser[index];
        index++;
        if (index > PSEUDO_MaxTimerChooser)
        {
            index = 0;
        }
        EEPROM.write(EEPROMAddr_PopTimerIndex, index);
        
        index = EEPROM.read(EEPROMAddr_RoutineIndex);
        SelectedRoutine = RoutineChooser[index];
        index++;
        if (index > PSEUDO_MaxRoutineChooser)
        {
            index = 0;
        }
        EEPROM.write(EEPROMAddr_RoutineIndex, index);
    }
    
    Serial.println("SelectedRoutine");
    Serial.println(SelectedRoutine);
    Serial.println("Button Presses");
    Serial.println(THRESH_ButtonPop);
    Serial.println("TimerThresh");
    Serial.println(THRESH_WeaselMusicTimer);
}

/* Loop routine runs continuously forever */
void loop()
{
    if((millis() - ScheduleTimer) >= 10)
    {
        DelCounter++;
        
        if (DelCounter >= BASETASKTIME)
        {
            DelCounter = 0;
            
            JackStateHandler();
            MusicStateMachine();
            ServoStateMachine();
            LEDStateMachine();
        }
        
        ScheduleTimer = millis();
    }
}


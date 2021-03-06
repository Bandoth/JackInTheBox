#include <avr/eeprom.h>
#include <EEPROM.h>
#include "WaveUtil.h"
#include "WaveHC.h"
#include "Typedefs.h"
#include "JackDefs.h"
#include "RoutineDefs.h"

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
    DigitalPinsInit();
    
    PWMSetup();

    JackSafeStartup();
    
    SDCardInit();
    
    NextRoutine();
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


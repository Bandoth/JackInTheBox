#include <avr/eeprom.h>
#include <EEPROM.h>
#include "WaveUtil.h"
#include "WaveHC.h"
#include "Typedefs.h"

/*
 * This example plays every .WAV file it finds on the SD card in a loop while playing with a servo
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
 */

#define SERVO_1_OPEN  20
#define SERVO_2_OPEN  160
#define SERVO_1_CLOSE  160
#define SERVO_2_CLOSE  20

const int LEDPin = 31;

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file for a pi digit or period
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time
UINT_8 AudioRateSelect = 4;
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

    pinMode(LEDPin, OUTPUT);
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

    Serial.begin(9600);  
    putstring_nl("\nWave test!");  // say we woke up!

    putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
    Serial.println(FreeRam());

    //    if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
    if (!card.init()) 
    {         //play with 8 MHz spi (default faster!)  
        error("Card init. failed!");  // Something went wrong, lets print out why
    }

    // enable optimize read - some cards may timeout. Disable if you're having problems
    card.partialBlockRead(true);

    // Now we will look for a FAT partition!
    uint8_t part;
    for (part = 0; part < 5; part++) {   // we have up to 5 slots to look in
        if (vol.init(card, part)) 
            break;                           // we found one, lets bail
    }
    if (part == 5) 
    {                     // if we ended up not finding one  :(
        error("No valid FAT partition!");  // Something went wrong, lets print out why
    }

    // Lets tell the user about what we found
    putstring("Using partition ");
    Serial.print(part, DEC);
    putstring(", type is FAT");
    Serial.println(vol.fatType(), DEC);     // FAT16 or FAT32?

    // Try to open the root directory
    if (!root.openRoot(vol)) 
    {
        error("Can't open root dir!");      // Something went wrong,
    }

    // Whew! We got past the tough parts.
    putstring_nl("Files found (* = fragmented):");

    // Print out all of the files in all the directories.
    root.ls(LS_R | LS_FLAG_FRAGMENTED);
    
    ScheduleTimerSetup();
}

/* Loop routine runs continuously forever */
void loop()
{
    JackBoxState = _Playing;
    // Do looping stuff, no need for while statement.
    while(!OCF0A);
    TIFR0 |= 0b00000010;    // Clear flag

    MusicStateMachine();
    ServoStateMachine();
    LEDStateMachine();
}


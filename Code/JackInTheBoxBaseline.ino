#include <avr/eeprom.h>
#include <EEPROM.h>
#include <Servo.h>
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
 *    Test Servo = 8
 *    Test Servo2 = 11
 */
 
#define SERVO_1_OPEN  20
#define SERVO_2_OPEN  160
#define SERVO_1_CLOSE  160
#define SERVO_2_CLOSE  20

const int LEDPin = 31;
 
const int LidPin1 = 8;      // Digital pin for Lid PWM
const int LidPin2 = 11;      // Digital pin for Lid PWM
const int ServoPin3 = 7;
const int ServoPin4 = 6;
const int ServoPin5 = 5;

const int ServoMin = 900;    // Corresponds to minimum pulse width in usecs for 0 degrees
const int ServoMax = 2100;   // Corresponds to maximum pulse width in usecs for 180 degrees

Servo LidServo1;
Servo LidServo2;
Servo TestServo1;
Servo TestServo2;
Servo TestServo3;

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file for a pi digit or period
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

uint8_t dirLevel; // indent level for file/dir names    (for prettyprinting)
dir_t dirBuf;     // buffer for directory reads

/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

// Function definitions (we define them here, but the code is below)
void play(FatReader &dir);

//#define LIDTEST 1

/* Setup routine automatically run first */
void setup()
{
  pinMode(LEDPin, OUTPUT);
  
  LidServo1.attach(LidPin1);
  LidServo2.attach(LidPin2);
  TestServo1.attach(ServoPin3);
  TestServo2.attach(ServoPin4);
  TestServo3.attach(ServoPin5);
  
#ifdef LIDTEST
  LidServo1.write(SERVO_1_OPEN);
  LidServo2.write(SERVO_2_OPEN);
#else
  LidServo1.write(90);
  LidServo2.write(90);
#endif
  TestServo1.write(90);
  TestServo2.write(90);
  TestServo3.write(90);

  Serial.begin(9600);  
  putstring_nl("\nWave test!");  // say we woke up!
  
  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(FreeRam());

//  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
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
  if (part == 5) {                     // if we ended up not finding one  :(
    error("No valid FAT partition!");  // Something went wrong, lets print out why
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(), DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    error("Can't open root dir!");      // Something went wrong,
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Files found (* = fragmented):");

  // Print out all of the files in all the directories.
  root.ls(LS_R | LS_FLAG_FRAGMENTED);
}

/* Loop routine runs continuously forever */
void loop()
{
  // Do looping stuff, no need for while statement.
//  root.rewind();
//  play(root);
  
  DoStuff();
  delay(500);
}

void DoStuff()
{
  char name[13];
  
  // copy flash string for 'period' to filename
  strcpy_P(name, PSTR("TEST.WAV"));
  
  if (wave.isplaying) {// already playing something, so stop it!
  wave.stop(); // stop it
  }
  if (!file.open(root, name)) {
    PgmPrint("Couldn't open file ");
    Serial.print(name);
    while(1); 
  }
  if (!wave.create(file)) {
    PgmPrintln("Not a valid WAV");
    while(1);
  }
  
  wave.volume = 0;  // Volume Control 5 = Very Soft, 0 = LOUD
  
  // ok time to play!
  wave.play();
  
  while (wave.isplaying) {
    //Add code for what to do while playing music
    ServoHandler();
    
    digitalWrite(LEDPin, LOW);
    delay(1000);
    digitalWrite(LEDPin, HIGH);
    delay(1000);
  }
  sdErrorCheck();                    // everything OK?
}

/////////////////////////////////// HELPERS
/*
 * print error message and halt
 */
void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
  while(1);
}
/*
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}
/*
 * play recursively - possible stack overflow if subdirectories too nested
 */
void play(FatReader &dir) {
  FatReader file;
  while (dir.readDir(dirBuf) > 0) {    // Read every file in the directory one at a time
  
    // Skip it if not a subdirectory and not a .WAV file
    if (!DIR_IS_SUBDIR(dirBuf)
         && strncmp_P((char *)&dirBuf.name[8], PSTR("WAV"), 3)) {
      continue;
    }

    Serial.println();            // clear out a new line
    
    for (uint8_t i = 0; i < dirLevel; i++) {
       Serial.write(' ');       // this is for prettyprinting, put spaces in front
    }
    if (!file.open(vol, dirBuf)) {        // open the file in the directory
      error("file.open failed");          // something went wrong
    }
    
    if (file.isDir()) {                   // check if we opened a new directory
      putstring("Subdir: ");
      printEntryName(dirBuf);
      Serial.println();
      dirLevel += 2;                      // add more spaces
      // play files in subdirectory
      play(file);                         // recursive!
      dirLevel -= 2;    
    }
    else {
      // Aha! we found a file that isnt a directory
      putstring("Playing ");
      printEntryName(dirBuf);              // print it out
      if (!wave.create(file)) {            // Figure out, is it a WAV proper?
        putstring(" Not a valid WAV");     // ok skip it
      } else {
        Serial.println();                  // Hooray it IS a WAV proper!
        wave.play();                       // make some noise!
        
        uint8_t n = 0;
//        while (wave.isplaying) {// playing occurs in interrupts, so we print dots in realtime
//          putstring(".");
//          if (!(++n % 32))Serial.println();
//          delay(100);
//        }       
//        sdErrorCheck();                    // everything OK?
        // if (wave.errors)Serial.println(wave.errors);     // wave decoding errors
      }
    }
  }
}

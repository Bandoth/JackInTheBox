#include "Typedefs.h"
#define SERVODEGREES_0 0x03E8

const byte myAddress = 0;
const byte myVal = 42;  // byte = unsigned 8 bit, char = signed 8 bit

byte servoState = 0;

void ServoHandler()
{
  servoState++;
  
  switch (servoState)
  {
    case 1:
    #ifdef LIDTEST
      LidServo1.write(SERVO_1_OPEN);
      LidServo2.write(SERVO_2_OPEN);
    #else
      LidServo1.write(65);
      LidServo2.write(65);
    #endif
      TestServo1.write(65);
      TestServo2.write(65);
      TestServo3.write(65);

      Serial.println("Open");
      break;
    
    case 2:
    #ifdef LIDTEST
      LidServo1.write(SERVO_1_CLOSE);
      LidServo2.write(SERVO_2_CLOSE);
    #else
      LidServo1.write(135);
      LidServo2.write(135);
    #endif
      TestServo1.write(135);
      TestServo2.write(135);
      TestServo3.write(135);

      Serial.println("Close");
      break;

    default:
      servoState = 0;
      Serial.println("Reset");
      delay(2000);
      break;
  }
  
  /*
  byte EEPROM_Write_Successful = false;
  
  EEPROM_Write_Successful = CustomEEPROMWrite(myAddress, myVal);
  if (EEPROM_Write_Successful)
  {
    myservo.write(90);
  }
  else
  {
    myservo.write(0);
  }
  */
}

void WriteServo(JackServo myServo, UINT_8 setDegrees)
{
    UINT_32 CalcVar;
    UINT_16 servoWriteValue;
    
    CalcVar = setDegrees * 1000;
    CalcVar = CalcVar / 1000;
    servoWriteValue = CalcVar + 1000;
    
    switch (myServo)
    {
        case _LidServo1:
            // OCRnx = servoWriteValue;
            break;
        case _LidServo2:
            // OCRnx = servoWriteValue;
            break;
        case _NeckServo:
            // OCRnx = servoWriteValue;
            break;
        case _TorsoServo:
            // OCRnx = servoWriteValue;
            break;
        case _StabServo:
            // OCRnx = servoWriteValue;
            break;
        case _WaveServo:
            // OCRnx = servoWriteValue;
            break;
        case _TalkServo:
            // OCRnx = servoWriteValue;
            break;
        default:
            break;
    }
}

/*
*    Use Timer3A, 4ABC, and 5ABC for PWMs (servos, only 3 PWMs per timer, Tmr1 used by music)
*    Phase and Frequency Correct mode
*    Fclk = 16MHz, so prescaler N = 8, ICRn = 20000
*    This yields 
*    0 degrees = OCRnx = 1000
*    90 degrees = OCRnx = 1500
*    180 degrees = OCRnx = 2000
*    OCRnx double buffered, so no need for interrupts
*    n = timer
*    x = pin/servo PWMing on that timer
*/
void PWMSetup()
{
    // Init ontime to closed box values
    OCR3A = SERVODEGREES_0;   
    OCR4A = SERVODEGREES_0;
    OCR4B = SERVODEGREES_0;
    OCR4C = SERVODEGREES_0;
    OCR5A = SERVODEGREES_0;
    OCR5B = SERVODEGREES_0;
    OCR5C = SERVODEGREES_0;
    
    // ICRn = 20000 (Freq of PWM = 50Hz)
    ICR3 = 0x4E20;
    ICR4 = 0x4E20;
    ICR5 = 0x4E20;
    
    // Tmr3 output on OC3A, Toggle on match, Setup for Phase and Freq Correct mode
    TCCR3A = 0b01000000;
    // Turn off peripherals, Toggle on match, Prescaler N = 8
    TCCR3B = 0b00010010;
    
    // Tmr4 output on OC4A, OC4B, OC4C, Toggle on match, Setup for Phase and Freq Correct mode
    TCCR4A = 0b01010100;
    // Turn off peripherals, Toggle on match, Prescaler N = 8
    TCCR4B = 0b00010010;
    
    // Tmr5 output on OC5A, OC5B, OC5C, Toggle on match, Setup for Phase and Freq Correct mode
    TCCR5A = 0b01010100;
    // Turn off peripherals, Toggle on match, Prescaler N = 8
    TCCR5B = 0b00010010;
    
    // Disable Timer 3, 4, and 5 interrupts
    TIMSK3 = 0;
    TIMSK4 = 0;
    TIMSK5 = 0;
}

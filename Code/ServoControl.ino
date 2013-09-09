#include "Typedefs.h"
#define SERVODEGREES_0 0x03E8

const byte myAddress = 0;
const byte myVal = 42;  // byte = unsigned 8 bit, char = signed 8 bit

byte servoState = 0;

void ServoStateMachine(void)
{
    ServoHandler();
}

void ServoHandler(void)
{
    servoState++;

    switch (servoState)
    {
    case 1:
        WriteServo(_LidServo1, SERVO_1_CLOSE);
        WriteServo(_LidServo2, SERVO_2_CLOSE);
        WriteServo(_NeckServo, 65);
        WriteServo(_TorsoServo, 65);
        WriteServo(_StabServo, 65);
        WriteServo(_WaveServo, 65);
        WriteServo(_TalkServo, 65);

//        Serial.println("Servos Close");
        break;

    case 2:
        WriteServo(_LidServo1, SERVO_1_OPEN);
        WriteServo(_LidServo2, SERVO_2_OPEN);
        WriteServo(_NeckServo, 135);
        WriteServo(_TorsoServo, 135);
        WriteServo(_StabServo, 135);
        WriteServo(_WaveServo, 135);
        WriteServo(_TalkServo, 135);

//        Serial.println("Servos Open");
        break;

    default:
        servoState = 0;
//        Serial.println("Servos Reset");
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
    UINT_16 servoWriteValue = 0;
    
    servoWriteValue = map(setDegrees, 0, 180, 1000, 10000);
    
    switch (myServo)
    {
    case _LidServo1:
        OCR3A = servoWriteValue;
        break;
    case _LidServo2:
        OCR4A = servoWriteValue;
        break;
    case _NeckServo:
        OCR4B = servoWriteValue;
        break;
    case _TorsoServo:
        OCR5C = servoWriteValue;
        break;
    case _StabServo:
        OCR5B = servoWriteValue;
        break;
    case _WaveServo:
        OCR5A = servoWriteValue;
        break;
    case _TalkServo:
        OCR4C = servoWriteValue;
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
    WriteServo(_LidServo1, SERVO_1_CLOSE);
    WriteServo(_LidServo2, SERVO_2_CLOSE);
    WriteServo(_NeckServo, 90);
    WriteServo(_TorsoServo, 90);
    WriteServo(_StabServo, 90);
    WriteServo(_WaveServo, 90);
    WriteServo(_TalkServo, 90);
    
    
    // ICRn = 20000 (Freq of PWM = 50Hz)
    ICR3 = 20000;
    ICR4 = 20000;
    ICR5 = 20000;

    TCCR3B = _BV(WGM33) | _BV(CS31); // phase and freq correct pwm mode, clk divider = 8
    TCCR3A = 0;                      // Clear settings
    TCCR3A |= _BV(COM3A1);           // Output on A only
    
    TCCR4B = _BV(WGM43) | _BV(CS41); // phase and freq correct pwm mode, clk divider = 8
    TCCR4A = 0;                      // Clear settings
    TCCR4A |= _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1); // Output on A B and C

    TCCR5B = _BV(WGM53) | _BV(CS51); // phase and freq correct pwm mode, clk divider = 8
    TCCR5A = 0;                      // Clear settings
    TCCR5A |= _BV(COM5A1) | _BV(COM5B1) | _BV(COM5C1); // Output on A B and C

    // Disable Timer 3, 4, and 5 interrupts
    TIMSK3 = 0;
    TIMSK4 = 0;
    TIMSK5 = 0;

    TCNT3 = 0; // Enable timers by clearing values
    TCNT4 = 0; // Enable timers by clearing values
    TCNT5 = 0; // Enable timers by clearing values
}


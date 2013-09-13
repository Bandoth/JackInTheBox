#include "Typedefs.h"
#define SERVODEGREES_0 0x03E8

byte servoState = 0;

UINT_16 ServoCounterNeck = 999;
UINT_16 ServoCounterFlower = 999;
UINT_16 ServoCounterStab = 999;
UINT_16 ServoCounterWave = 999;
UINT_16 ServoCounterTalk = 999;

UINT_16 ServoDelNeck = 0;
UINT_16 ServoDelFlower = 0;
UINT_16 ServoDelStab = 0;
UINT_16 ServoDelWave = 0;
UINT_16 ServoDelTalk = 0;

UINT_16 ActionIndexNeck = 0;
UINT_16 ActionIndexFlower = 0;
UINT_16 ActionIndexStab = 0;
UINT_16 ActionIndexWave = 0;
UINT_16 ActionIndexTalk = 0;

boolean FinishedNeck = false;
boolean FinishedFlower = false;
boolean FinishedStab = false;
boolean FinishedWave = false;
boolean FinishedTalk = false;

UINT_8 TableLengthNeck = 0;
UINT_8 TableLengthFlower = 0;
UINT_8 TableLengthStab = 0;
UINT_8 TableLengthWave = 0;
UINT_8 TableLengthTalk = 0;

void ServoStateMachine(void)
{
    switch (JackBoxState)
    {
    case _Popped:
        PlayRoutine();
        break;
    case _Waiting:
    case _Playing:
    default:
        break;
    }
}

void PlayRoutine(void)
{
    UINT_16 ServoDegreesNeck;
    UINT_16 ServoDegreesFlower;
    UINT_16 ServoDegreesStab;
    UINT_16 ServoDegreesWave;
    UINT_16 ServoDegreesTalk;

    ServoCounterNeck++;
    ServoCounterFlower++;
    ServoCounterStab++;
    ServoCounterWave++;
    ServoCounterTalk++;
    
    switch (SelectedRoutine)
    {
    case _Routine1:
        ServoDelNeck = ServoRoutineNeck_1[ActionIndexNeck][COLUMN_DELAY];
        ServoDelFlower = ServoRoutineFlower_1[ActionIndexNeck][COLUMN_DELAY];
        ServoDelStab = ServoRoutineStab_1[ActionIndexNeck][COLUMN_DELAY];
        ServoDelWave = ServoRoutineWave_1[ActionIndexNeck][COLUMN_DELAY];
        ServoDelTalk = ServoRoutineTalk_1[ActionIndexNeck][COLUMN_DELAY];
        
        ServoDegreesNeck = ServoRoutineNeck_1[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesFlower = ServoRoutineFlower_1[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesStab = ServoRoutineStab_1[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesWave = ServoRoutineWave_1[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesTalk = ServoRoutineTalk_1[ActionIndexNeck + 1][COLUMN_ACTION];
        
        TableLengthNeck = TABLELEN_Neck1;
        TableLengthFlower = TABLELEN_Flower1;
        TableLengthStab = TABLELEN_Stab1;
        TableLengthWave = TABLELEN_Wave1;
        TableLengthTalk = TABLELEN_Talk1;
        break;
    case _Routine2:
        ServoDelNeck = ServoRoutineNeck_2[ActionIndexNeck][COLUMN_DELAY];
        ServoDelFlower = ServoRoutineFlower_2[ActionIndexNeck][COLUMN_DELAY];
        ServoDelStab = ServoRoutineStab_2[ActionIndexNeck][COLUMN_DELAY];
        ServoDelWave = ServoRoutineWave_2[ActionIndexNeck][COLUMN_DELAY];
        ServoDelTalk = ServoRoutineTalk_2[ActionIndexNeck][COLUMN_DELAY];
        
        ServoDegreesNeck = ServoRoutineNeck_2[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesFlower = ServoRoutineFlower_2[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesStab = ServoRoutineStab_2[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesWave = ServoRoutineWave_2[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesTalk = ServoRoutineTalk_2[ActionIndexNeck + 1][COLUMN_ACTION];
        
        TableLengthNeck = TABLELEN_Neck2;
        TableLengthFlower = TABLELEN_Flower2;
        TableLengthStab = TABLELEN_Stab2;
        TableLengthWave = TABLELEN_Wave2;
        TableLengthTalk = TABLELEN_Talk2;
        break;
    case _Routine3:
        ServoDelNeck = ServoRoutineNeck_3[ActionIndexNeck][COLUMN_DELAY];
        ServoDelFlower = ServoRoutineFlower_3[ActionIndexNeck][COLUMN_DELAY];
        ServoDelStab = ServoRoutineStab_3[ActionIndexNeck][COLUMN_DELAY];
        ServoDelWave = ServoRoutineWave_3[ActionIndexNeck][COLUMN_DELAY];
        ServoDelTalk = ServoRoutineTalk_3[ActionIndexNeck][COLUMN_DELAY];
        
        ServoDegreesNeck = ServoRoutineNeck_3[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesFlower = ServoRoutineFlower_3[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesStab = ServoRoutineStab_3[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesWave = ServoRoutineWave_3[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesTalk = ServoRoutineTalk_3[ActionIndexNeck + 1][COLUMN_ACTION];
        
        TableLengthNeck = TABLELEN_Neck3;
        TableLengthFlower = TABLELEN_Flower3;
        TableLengthStab = TABLELEN_Stab3;
        TableLengthWave = TABLELEN_Wave3;
        TableLengthTalk = TABLELEN_Talk3;
        break;
    case _Routine4:
        ServoDelNeck = ServoRoutineNeck_4[ActionIndexNeck][COLUMN_DELAY];
        ServoDelFlower = ServoRoutineFlower_4[ActionIndexNeck][COLUMN_DELAY];
        ServoDelStab = ServoRoutineStab_4[ActionIndexNeck][COLUMN_DELAY];
        ServoDelWave = ServoRoutineWave_4[ActionIndexNeck][COLUMN_DELAY];
        ServoDelTalk = ServoRoutineTalk_4[ActionIndexNeck][COLUMN_DELAY];
        
        ServoDegreesNeck = ServoRoutineNeck_4[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesFlower = ServoRoutineFlower_4[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesStab = ServoRoutineStab_4[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesWave = ServoRoutineWave_4[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesTalk = ServoRoutineTalk_4[ActionIndexNeck + 1][COLUMN_ACTION];
        
        TableLengthNeck = TABLELEN_Neck4;
        TableLengthFlower = TABLELEN_Flower4;
        TableLengthStab = TABLELEN_Stab4;
        TableLengthWave = TABLELEN_Wave4;
        TableLengthTalk = TABLELEN_Talk4;
        break;
    case _Routine5:
        ServoDelNeck = ServoRoutineNeck_5[ActionIndexNeck][COLUMN_DELAY];
        ServoDelFlower = ServoRoutineFlower_5[ActionIndexNeck][COLUMN_DELAY];
        ServoDelStab = ServoRoutineStab_5[ActionIndexNeck][COLUMN_DELAY];
        ServoDelWave = ServoRoutineWave_5[ActionIndexNeck][COLUMN_DELAY];
        ServoDelTalk = ServoRoutineTalk_5[ActionIndexNeck][COLUMN_DELAY];
        
        ServoDegreesNeck = ServoRoutineNeck_5[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesFlower = ServoRoutineFlower_5[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesStab = ServoRoutineStab_5[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesWave = ServoRoutineWave_5[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesTalk = ServoRoutineTalk_5[ActionIndexNeck + 1][COLUMN_ACTION];
        
        TableLengthNeck = TABLELEN_Neck5;
        TableLengthFlower = TABLELEN_Flower5;
        TableLengthStab = TABLELEN_Stab5;
        TableLengthWave = TABLELEN_Wave5;
        TableLengthTalk = TABLELEN_Talk5;
        break;
    default:
        ServoDelNeck = ServoRoutineNeck_1[ActionIndexNeck][COLUMN_DELAY];
        ServoDelFlower = ServoRoutineFlower_1[ActionIndexNeck][COLUMN_DELAY];
        ServoDelStab = ServoRoutineStab_1[ActionIndexNeck][COLUMN_DELAY];
        ServoDelWave = ServoRoutineWave_1[ActionIndexNeck][COLUMN_DELAY];
        ServoDelTalk = ServoRoutineTalk_1[ActionIndexNeck][COLUMN_DELAY];
        
        ServoDegreesNeck = ServoRoutineNeck_1[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesFlower = ServoRoutineFlower_1[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesStab = ServoRoutineStab_1[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesWave = ServoRoutineWave_1[ActionIndexNeck + 1][COLUMN_ACTION];
        ServoDegreesTalk = ServoRoutineTalk_1[ActionIndexNeck + 1][COLUMN_ACTION];
        
        TableLengthNeck = TABLELEN_Neck1;
        TableLengthFlower = TABLELEN_Flower1;
        TableLengthStab = TABLELEN_Stab1;
        TableLengthWave = TABLELEN_Wave1;
        TableLengthTalk = TABLELEN_Talk1;
        break;
    }
    
    if (ServoCounterNeck >= ServoDelNeck)
    {
        ActionIndexNeck++;
        if (ActionIndexNeck < TableLengthNeck)
        {
            Serial.println("AcionIndexNeck");
            Serial.println(ActionIndexNeck);
            Serial.println("NeckServo");
            Serial.println(ServoDegreesNeck);
            WriteServo(_NeckServo, ServoDegreesNeck);
        }
        else
        {
            FinishedNeck = true;
        }
    }
    
    if (ServoCounterFlower >= ServoDelFlower)
    {
        ActionIndexFlower++;
        if (ActionIndexFlower < TableLengthFlower)
        {
            WriteServo(_FlowerServo, ServoDegreesFlower);
        }
        else
        {
            FinishedFlower = true;
        }
    }
    
    if (ServoCounterStab >= ServoDelStab)
    {
        ActionIndexStab++;
        if (ActionIndexStab < TableLengthStab)
        {
            WriteServo(_StabServo, ServoDegreesStab);
        }
        else
        {
            FinishedStab = true;
        }
    }
    
    if (ServoCounterWave >= ServoDelWave)
    {
        ActionIndexWave++;
        if (ActionIndexWave < TableLengthWave)
        {
            WriteServo(_WaveServo, ServoDegreesWave);
        }
        else
        {
            FinishedWave = true;
        }
    }
    
    if (ServoCounterTalk >= ServoDelTalk)
    {
        ActionIndexTalk++;
        if (ActionIndexTalk < TableLengthTalk)
        {
            WriteServo(_TalkServo, ServoDelTalk);
        }
        else
        {
            FinishedTalk = true;
        }
    }
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
    case _FlowerServo:
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

void JackSafeStartup(void)
{
    UINT_16 DelCounter_SafeStartup = 0;
    // Set body servos to safe spot
    // Lower actuator
    // Close lid
    WriteServo(_NeckServo, NECK_Neutral);
    WriteServo(_TalkServo, TALK_Neutral);
    WriteServo(_StabServo, STAB_Neutral);
    WriteServo(_WaveServo, WAVE_Neutral);
    WriteServo(_FlowerServo, FLOWER_Neutral);
    
    digitalWrite(PinActuator, ACTUATOR_Down);
    
    DelCounter_SafeStartup = millis();
    while ((millis() - DelCounter_SafeStartup) < 250);
    
    WriteServo(_LidServo1, SERVO_1_CLOSE);
    WriteServo(_LidServo2, SERVO_2_CLOSE);
}

void JackSafePop(void)
{
    UINT_16 DelCounter_SafePop = 0;
    // Servos should be in safe location
    // Pop servos
    // pause
    // Pop actuator
    
    WriteServo(_LidServo1, SERVO_1_OPEN);
    WriteServo(_LidServo2, SERVO_2_OPEN);
    
    DelCounter_SafePop = millis();
    while ((millis() - DelCounter_SafePop) < 100);
    
    digitalWrite(PinActuator, ACTUATOR_Up);
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
void PWMSetup(void)
{
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
    
    // Init ontime to closed box values
    /*    
    WriteServo(_LidServo1, SERVO_1_CLOSE);
    WriteServo(_LidServo2, SERVO_2_CLOSE);
    WriteServo(_NeckServo, 90);
    WriteServo(_FlowerServo, 90);
    WriteServo(_StabServo, 90);
    WriteServo(_WaveServo, 90);
    WriteServo(_TalkServo, 90);
    */
    JackSafeStartup();
}


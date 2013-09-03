#ifndef _TYPEDEF
#define _TYPEDEF

typedef unsigned char UINT_8;
typedef char SINT_8;
typedef unsigned int UINT_16;
typedef int SINT_16;
typedef unsigned long UINT_32;

typedef enum 
{ 
    _LidServo1, 
    _LidServo2,
    _NeckServo,
    _TorsoServo,
    _StabServo,
    _WaveServo,
    _TalkServo 
} JackServo;

typedef enum
{
    _Waiting,
    _Playing,
    _Popped
} BoxState;

typedef enum
{
    _Routine1 = 1,
    _Routine2,
    _Routine3,
    _Routine4,
    _Routine5
} JackRoutine;

typedef enum
{
    _AudioNone,
    _AudioPop,
    _AudioRoutine1,
    _AudioRoutine2,
    _AudioRoutine3,
    _AudioRoutine4,
    _AudioRoutine5
} SelectedAudio;

#endif


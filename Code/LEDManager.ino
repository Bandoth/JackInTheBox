

void LEDStateMachine(void)
{
    
}

void DigitalPinsInit(void)
{
    pinMode(PinLidServo1, OUTPUT);
    pinMode(PinLidServo2, OUTPUT);
    pinMode(PinNeckServo, OUTPUT);
    pinMode(PinTorsoServo, OUTPUT);
    pinMode(PinStabServo, OUTPUT);
    pinMode(PinWaveServo, OUTPUT);
    pinMode(PinTalkServo, OUTPUT);
    
    pinMode(PinLEDOptics1, OUTPUT);
    pinMode(PinLEDOptics2, OUTPUT);

    pinMode(PinLEDBox1,    OUTPUT);
    pinMode(PinLEDBox2,    OUTPUT);
    pinMode(PinLEDBox3,    OUTPUT);
    pinMode(PinLEDBox4,    OUTPUT);
    pinMode(PinLEDBox5,    OUTPUT);
    pinMode(PinLEDBox6,    OUTPUT);
    pinMode(PinLEDBox7,    OUTPUT);
    pinMode(PinLEDBox8,    OUTPUT);

    pinMode(PinActuator,   OUTPUT);

    pinMode(PinButtonInp,  INPUT);
}



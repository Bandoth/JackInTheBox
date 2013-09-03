SelectedAudio CurrentAudio = _AudioNone;

void MusicStateMachine(void)
{
    switch (JackBoxState)
    {
    case _Waiting:
        CurrentAudio = _AudioNone;
        if (wave.isplaying)
        {
            wave.stop();
        }
        sdErrorCheck();    // everything OK?
        break;
    case _Playing:
        if ((CurrentAudio == _AudioNone) || (CurrentAudio != _AudioPop))
        {
            PlayPopGoesTheWeasel();
        }
        AudioSpeedandTimeout();
        break;
    case _Popped:
        if ((CurrentAudio == _AudioNone) || (CurrentAudio == _AudioPop))
        {
            PlayRoutine(SelectedRoutine);
        }
        break;
    default:
        break;
    }
//    PlayPopGoesTheWeasel();
}

void PlayPopGoesTheWeasel(void)
{
    char name[13];

    // copy flash string for 'period' to filename
    strcpy_P(name, PSTR("WEASEL.WAV"));
    CurrentAudio = _AudioPop;
    
    if (wave.isplaying) // already playing something, so stop it!
    {
        wave.stop(); // stop it
    }
    if (!file.open(root, name)) 
    {
        PgmPrint("Couldn't open file ");
        Serial.print(name);
        while(1); 
    }
    if (!wave.create(file)) 
    {
        PgmPrintln("Not a valid WAV");
        while(1);
    }

    wave.volume = 0;  // Volume Control 5 = Very Soft, 0 = LOUD

    // ok time to play!
    wave.play();
}

void PlayRoutine(JackRoutine CurrentRoutine)
{
    char name[13];

    switch (CurrentRoutine)
    {
    case _Routine1:
        strcpy_P(name, PSTR("R1.WAV"));
        CurrentAudio = _AudioRoutine1;
        break;
    case _Routine2:
        strcpy_P(name, PSTR("R2.WAV"));
        CurrentAudio = _AudioRoutine2;
        break;
    case _Routine3:
        strcpy_P(name, PSTR("R3.WAV"));
        CurrentAudio = _AudioRoutine3;
        break;
    case _Routine4:
        strcpy_P(name, PSTR("R4.WAV"));
        CurrentAudio = _AudioRoutine4;
        break;
    case _Routine5:
        strcpy_P(name, PSTR("R5.WAV"));
        CurrentAudio = _AudioRoutine5;
        break;
    default:
        strcpy_P(name, PSTR("R1.WAV"));
        CurrentAudio = _AudioRoutine1;
        break;
    }

    if (wave.isplaying) // already playing something, so stop it!
    {
        wave.stop(); // stop it
    }
    if (!file.open(root, name)) 
    {
        PgmPrint("Couldn't open file ");
        Serial.print(name);
        while(1); 
    }
    if (!wave.create(file)) 
    {
        PgmPrintln("Not a valid WAV");
        while(1);
    }

    wave.volume = 0;  // Volume Control 5 = Very Soft, 0 = LOUD

    // ok time to play!
    wave.play();
}

void AudioSpeedandTimeout(void)
{
    
}

/////////////////////////////////// HELPERS
/*
 * print error message and halt
 */
void error_P(const char *str) 
{
    PgmPrint("Error: ");
    SerialPrint_P(str);
    sdErrorCheck();
    while(1);
}
/*
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) 
{
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
void play(FatReader &dir) 
{
    FatReader file;
    while (dir.readDir(dirBuf) > 0) {    // Read every file in the directory one at a time

        // Skip it if not a subdirectory and not a .WAV file
        if (!DIR_IS_SUBDIR(dirBuf) &&
            strncmp_P((char *)&dirBuf.name[8], PSTR("WAV"), 3)) 
        {
            continue;
        }

        Serial.println();            // clear out a new line

        for (uint8_t i = 0; i < dirLevel; i++) 
        {
            Serial.write(' ');       // this is for prettyprinting, put spaces in front
        }
        if (!file.open(vol, dirBuf)) // open the file in the directory
        {        
            error("file.open failed");          // something went wrong
        }

        if (file.isDir()) // check if we opened a new directory
        {                   
            putstring("Subdir: ");
            printEntryName(dirBuf);
            Serial.println();
            dirLevel += 2;                      // add more spaces
            // play files in subdirectory
            play(file);                         // recursive!
            dirLevel -= 2;    
        }
        else 
        {
            // Aha! we found a file that isnt a directory
            putstring("Playing ");
            printEntryName(dirBuf);              // print it out
            if (!wave.create(file)) // Figure out, is it a WAV proper?
            {            
                putstring(" Not a valid WAV");     // ok skip it
            } 
            else 
            {
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

/*
 *EXAMPLE OF PLAYBACK SPEED CONTROL
 */
UINT_32 AudioRate;
UINT_8 AudioRateSet = 0;

void SpeedControlExample(char *name) 
{
    if (!AudioRateSet)
    {
        BaseAudioRate = wave.dwSamplesPerSec;
        AudioRateSet = 1;
    }
//    counter++;
    AudioRate = BaseAudioRate;
//    AudioRate = AudioRate * 50;  
//    AudioRate = AudioRate * 78;
//    AudioRate = AudioRate >> 6;
//    wave.setSampleRate(AudioRate);

    if (AudioRateSelect == 0)
    {
        wave.setSampleRate(AudioRate);
    }
    else if (AudioRateSelect == 1)
    {
        AudioRate = AudioRate * 78;
        AudioRate = AudioRate >> 6;
        wave.setSampleRate(AudioRate);
    }
    else if (AudioRateSelect == 2)
    {
        AudioRate = AudioRate * 50;
        AudioRate = AudioRate >> 6;
        wave.setSampleRate(AudioRate);
    }

//    if (counter >= 3000)
//        counter = 0;
}


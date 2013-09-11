SelectedAudio CurrentAudio = _AudioNone;
UINT_16 AudioRateDelay = 0;

void MusicStateMachine(void)
{
    switch (JackBoxState)
    {
    case _Waiting:
        CurrentAudio = _AudioNone;
        if (wave.isplaying)
        {
            Serial.println("Audio Stopping for Wait");
            wave.stop();
        }
        sdErrorCheck();    // everything OK?
        break;
    case _Playing:
        if ((CurrentAudio == _AudioNone) || (CurrentAudio != _AudioPop))
        {
            Serial.println("Audio Starting Weasel");
            PlayRoutine(_RoutineWeasel);
        }

        AudioSpeedHandler();
        break;
    case _Popped:
        if ((CurrentAudio == _AudioNone) || (CurrentAudio == _AudioPop))
        {
            Serial.println("Audio Starting Pop");
            PlayRoutine(_RoutineTest);
            //            PlayRoutine(SelectedRoutine);
        }
        break;
    default:
        break;
    }
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
    case _RoutineWeasel:
        strcpy_P(name, PSTR("WEASEL.WAV"));
        CurrentAudio = _AudioPop;
        break;
    case _RoutineTest:
        strcpy_P(name, PSTR("TEST.WAV"));
        CurrentAudio = _AudioTest;
        break;
    default:
        strcpy_P(name, PSTR("R1.WAV"));
        CurrentAudio = _AudioRoutine1;
        break;
    }

    if (wave.isplaying) // already playing something, so stop it!
    {
        Serial.println("AudioStop to play Routine");
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

    if (!AudioRateSet)
    {
        Serial.println("Audio Setting base rate");
        BaseAudioRate = wave.dwSamplesPerSec;
        Serial.println(BaseAudioRate);
        AudioRateSet = 1;
    }
}

void AudioSpeedHandler(void)
{
    UINT_32 AudioRate = BaseAudioRate;
    AudioRateDelay++;

    if (AudioRateDelay >= AUDIORATETASKDELAY)
    {
        AudioRateDelay = 0;
        if ((JackBoxState == _Playing) && (wave.isplaying))
        {
            Serial.println("Audio Changing rate");
            Serial.println(AudioRate);
            Serial.println("Multiplier");
            Serial.println(AudioPlaybackMultiplier);
            AudioRate = AudioRate * ((UINT_32)AudioPlaybackMultiplier);
            Serial.println(AudioRate);
            AudioRate = AudioRate >> AUDIO_MULT_SHIFT;
            Serial.println(AudioRate);

            wave.setSampleRate(AudioRate);
            Serial.println("Audio Rate Set");
        }
        else if (wave.isplaying)
        {
            wave.setSampleRate(AudioRate);
        }
    }
}

void SDCardInit(void)
{
    Serial.begin(9600);  
//    putstring_nl("\nWave test!");  // say we woke up!

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
/*
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
 */





/*  EEPROM.write() will automatically wait for the previous write to complete.
 This is unacceptable.  Use eeprom_is_ready() to check if you can call
 EEPROM.write() 
 */
byte CustomEEPROMWrite(byte Address, byte Val)
{
    if (eeprom_is_ready())  // Use state handler to control writes.
    {
        EEPROM.write(Address, Val);
        return true;
    }
    else
    {
        return false;
    }
}

void NextRoutine(void)
{
    UINT_8 index = 0;
    
    if (EEPROM.read(EEPROMAddr_FirstWrite) != 0xA5)
    {
        EEPROM.write(EEPROMAddr_FirstWrite, 0xA5);
        EEPROM.write(EEPROMAddr_ButtonIndex, 1);
        EEPROM.write(EEPROMAddr_PopTimerIndex, 1);
        EEPROM.write(EEPROMAddr_RoutineIndex, 1);
        
        SelectedRoutine = (JackRoutine)RoutineChooser[0];
        THRESH_ButtonPop = ButtonPressChooser[0];
        THRESH_WeaselMusicTimer = ButtonTimerChooser[0];
    }
    else
    {
        index = EEPROM.read(EEPROMAddr_ButtonIndex);
        THRESH_ButtonPop = ButtonPressChooser[index];
        index++;
        if (index > PSEUDO_MaxPressChooser)
        {
            index = 0;
        }
        EEPROM.write(EEPROMAddr_ButtonIndex, index);
        
        index = EEPROM.read(EEPROMAddr_PopTimerIndex);
        THRESH_WeaselMusicTimer = ButtonTimerChooser[index];
        index++;
        if (index > PSEUDO_MaxTimerChooser)
        {
            index = 0;
        }
        EEPROM.write(EEPROMAddr_PopTimerIndex, index);
        
        index = EEPROM.read(EEPROMAddr_RoutineIndex);
        SelectedRoutine = RoutineChooser[index];
        index++;
        if (index > PSEUDO_MaxRoutineChooser)
        {
            index = 0;
        }
        EEPROM.write(EEPROMAddr_RoutineIndex, index);
    }
    
    #ifdef TESTRUN
    SelectedRoutine = _Routine1;
    THRESH_ButtonPop = 2;
    #endif
    
    Serial.println("SelectedRoutine");
    Serial.println(SelectedRoutine);
    Serial.println("Button Presses");
    Serial.println(THRESH_ButtonPop);
    Serial.println("TimerThresh");
    Serial.println(THRESH_WeaselMusicTimer);
}

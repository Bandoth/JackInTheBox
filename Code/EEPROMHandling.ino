
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

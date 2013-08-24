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

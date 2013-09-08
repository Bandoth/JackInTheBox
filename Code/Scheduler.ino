

UINT_8 DelayCounter = 0;
ButtonStates ButtonState = _ButtonWaiting;

UINT_16 ButtonPeriodCounter = 0;
UINT_16 ButtonTimeoutCounter = 0;
UINT_8 ButtonHoldDetector = 0;
UINT_8 ButtonPressCounter = 0;

void JackStateHandler(void)
{
    switch (JackBoxState)
    {
    case _Waiting:
        AudioPlaybackMultiplier = 64;
        
        DelayCounter++;
        if (DelayCounter >= 5)
        {
            DelayCounter = 0;
            ButtonState = ButtonHandler();
            
            if ((ButtonState != _ButtonWaiting) && (ButtonState != _ButtonTimeout))
            {
                JackBoxState = _Playing;
            }
        }
        break;
    case _Playing:
        DelayCounter++;
        if (DelayCounter >= 5)
        {
            DelayCounter = 0;
            ButtonState = ButtonHandler();
            
            if (ButtonState == _ButtonTimeout)
            {
                if (wave.isplaying)
                {
                    wave.stop();
                }
                JackBoxState = _Waiting;
            }
            else if (ButtonState == _ButtonPopThreshReached)
            {
                //PopStartup();
                JackBoxState = _Popped;
            }
        }
        break;
    case _Popped:
        if (!wave.isplaying)
        {
            DelayCounter++;
            if (DelayCounter >= 150)
            {
                //PopShutdown();
                JackBoxState = _Waiting;
            }
        }
        else
        {
            DelayCounter = 0;
        }
        break;
    default:
        JackBoxState = _Waiting;
        break;
    }
}

ButtonStates ButtonHandler(void)
{
    UINT_8 ButtonVal;
    
    switch (JackBoxState)
    {
    case _Waiting:
        ButtonVal = digitalRead(PinButtonInp);
        
        if (ButtonVal == 1)
        {
            ButtonHoldDetector = ButtonVal;
            ButtonPressCounter++;
            return _ButtonPressesDetected;
        }
        break;
    case _Playing:
        ButtonVal = digitalRead(PinButtonInp);
        
        if ((ButtonVal == 1) && (ButtonHoldDetector == 1))
        {
            // Holding button, count period, count to timeout
            ButtonPeriodCounter++;
            
            if (ButtonPeriodCounter >= THRESH_ButtonPeriodSlowdown)
            {
                AudioPlaybackMultiplier--;
                if (AudioPlaybackMultiplier < AUDIO_MULT_MIN)
                {
                    AudioPlaybackMultiplier = AUDIO_MULT_MIN;
                }
            }
            
            ButtonTimeoutCounter++;
            
            if (ButtonTimeoutCounter >= THRESH_ButtonTimeout)
            {
                ButtonTimeoutCounter = 0;
                ButtonPeriodCounter = 0;
                return _ButtonTimeout;
            }
        }
        else if ((ButtonVal == 0) && (ButtonHoldDetector == 1))
        {
            // Button released, reset timeout, count period
            ButtonHoldDetector = ButtonVal;
            ButtonPeriodCounter++;
            
            if (ButtonPeriodCounter >= THRESH_ButtonPeriodSlowdown)
            {
                AudioPlaybackMultiplier--;
                if (AudioPlaybackMultiplier < AUDIO_MULT_MIN)
                {
                    AudioPlaybackMultiplier = AUDIO_MULT_MIN;
                }
            }
            
            ButtonTimeoutCounter = 0;
            
            return _ButtonPressesDetected;
        }
        else if (ButtonVal == 1)
        {
            // Button pressed, reset timeout, update global period
            ButtonHoldDetector = ButtonVal;
            ButtonTimeoutCounter = 0;
            
            if (ButtonPeriodCounter <= 10)
            {
                AudioPlaybackMultiplier += 3;
                if (AudioPlaybackMultiplier > AUDIO_MULT_MAX)
                {
                    AudioPlaybackMultiplier = AUDIO_MULT_MAX;
                }
            }
            else if (ButtonPeriodCounter >= 11)
            {
                AudioPlaybackMultiplier += 2;
                if (AudioPlaybackMultiplier > AUDIO_MULT_MAX)
                {
                    AudioPlaybackMultiplier = AUDIO_MULT_MAX;
                }
            }
            else
            {
                AudioPlaybackMultiplier++;
                if (AudioPlaybackMultiplier > AUDIO_MULT_MAX)
                {
                    AudioPlaybackMultiplier = AUDIO_MULT_MAX;
                }
            }
            
            ButtonPeriodCounter = 0;
            
            ButtonPressCounter++;
            if (ButtonPressCounter >= 12)
            {
                AudioPlaybackMultiplier = AUDIO_MULT_BASE;
                return _ButtonPopThreshReached;
            }
            
            return _ButtonPressesDetected;
        }
        else
        {
            // Button not pressed, count timeout and period
            ButtonHoldDetector = ButtonVal;
            ButtonPeriodCounter++;
            
            if (ButtonPeriodCounter >= THRESH_ButtonPeriodSlowdown)
            {
                AudioPlaybackMultiplier--;
                if (AudioPlaybackMultiplier < AUDIO_MULT_MIN)
                {
                    AudioPlaybackMultiplier = AUDIO_MULT_MIN;
                }
            }
            
            ButtonTimeoutCounter++;
            
            if (ButtonTimeoutCounter >= THRESH_ButtonTimeout)
            {
                ButtonTimeoutCounter = 0;
                ButtonPeriodCounter = 0;
                return _ButtonTimeout;
            }
        }
        break;
    case _Popped:
        break;
    default:
        break;
    }
    
    return _ButtonTimeout;
}

const UINT_8 RoutineChooser[30] =
{
    5,
    1,
    3,
    4,
    2,
    3,
    1,
    5,
    2,
    4,
    5,
    3,
    4,
    2,
    1,
    4,
    1,
    2,
    5,
    3,
    5,
    1,
    4,
    2,
    3,
    4,
    5,
    1,
    3,
    2
};

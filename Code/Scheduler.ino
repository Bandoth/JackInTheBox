/*
 *    Use 8 bit Timer0
 *    10ms tick wouldn't be bad
 *    Poll
 *    Run tasks   
 */

/*
 *    Timer 0 running 10ms tick, no interrupts
 */
void ScheduleTimerSetup()
{
    OCR0A = 0x270F; // 9999 with N = 8 for 10ms tick
    
    // Tmr0 No outputs, Setup for CTC mode
    TCCR0A = 0b00000010;
    // Turn off peripherals, CTC mode Prescaler N = 8
    TCCR0B = 0b00000010;
    
    TCNT0 = 0;
    TIFR0 |= 0b00000010;    // Clear flag
}

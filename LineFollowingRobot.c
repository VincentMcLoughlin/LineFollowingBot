#include "mte220.c"  // C functions needed for labs

#define WAIT_TIME 100

void stopCar()
{
	
	//Motors Off
	SetLeft(2600); // 3512 left stop value, 2500 full speed
	SetRight(4800); // 3505 right stop value, 2600 full speed
	Delay(WAIT_TIME);

	SetLeft(2800);
	SetRight(4400);
	Delay(WAIT_TIME);

	SetLeft(3000);
	SetRight(4000);
	Delay(WAIT_TIME);

	SetLeft(3200);
	SetRight(3800);
	Delay(WAIT_TIME); 	

	BothServosOff

}

void startCar()
{
	
	SetLeft(3200); //2600 full speed
	SetRight(3800);//2500 full speed
	Delay(WAIT_TIME);

	SetLeft(3000);
	SetRight(4000);
	Delay(WAIT_TIME);

	SetLeft(2800);
	SetRight(4400);	
	Delay(WAIT_TIME);
	
	SetLeft(2600);
	SetRight(4800);
	Delay(WAIT_TIME); 

	BothServosOn

}

void followLine()
{
		uns8 analog_value;

		analog_value = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp

        if ( analog_value < 0x66 )  // 0x66 is 2V for 10-bit ADC with 2 LSB dropped
        {
            // left servo only
            LeftServoOn
            RightServoOff
        }
        else if ( analog_value > 0x99 )  // 0x99 is 3V for 10-bit ADC with 2 LSB dropped
        {
            // right servo only
            RightServoOn
            LeftServoOff
        }
        else
        {
            // both servos on
            BothServosOn
			
        }
}


void main(void)
{
    uns8 analog_value;  // current ADC value
	uns8 hallAnalog_value;

    Initialization();

	uns8 gaussThreshold =  179; //0xA8; // Base Value is at 3.2V for some reason, was 2.5 during the lab, will build around this value instead.
	uns8 gaussThresholdHigh = 230; //0xC2;
	uns8 gaussThresholdLow = 130; // 0x89; 
	uns8 hallFlag = 0;

	WaitForButton();

    // initially both servos are on
    UseServos         // (syntax of "call" is correct without () or ;)
    startCar();


    while (1)  // loop forever
    {
		

		followLine();

		// get analog value from the Hall effect sensor
        hallAnalog_value = AnalogConvert(ADC_HALL_EFFECT);

		if(hallAnalog_value < gaussThresholdLow ) //gaussLowThreshold
		{
			
			LongDelay(1);

			// Blink the LED if less than gauss threshold value
			if(hallAnalog_value < gaussThresholdLow )
			{

				hallFlag = 1;

				stopCar();

				int i = 0;

				while(i < 15)
				{
					OnLED
				
					// wait for0.25 second
					LongDelay(2);
				
					OffLED
					LongDelay(2);

					i++;

				}

				startCar();

			}
		}

	

		if(hallAnalog_value > gaussThresholdHigh) // 3.8V x 33
		{
			LongDelay(1);
		
			if(hallAnalog_value > gaussThresholdHigh)
			{

				hallFlag =  1;
				stopCar();
			

				OnLED
				LongDelay(56); //16*3.5, original value of 16 corresponded to a 2s delay.

				OffLED

				startCar();

			}
		}	
	
		if(hallAnalog_value > gaussThresholdHigh || hallAnalog_value < gaussThresholdLow)
		{	
			Delay(100);
			
			if(hallAnalog_value > gaussThresholdHigh || hallAnalog_value < gaussThresholdLow)
			{
				while(hallAnalog_value > gaussThresholdHigh || hallAnalog_value < gaussThresholdLow){
						hallAnalog_value = AnalogConvert(ADC_HALL_EFFECT); 					
						followLine();
					}
			}
		
		}
			
	}

}


// when recives a value steps up to it over time ~ 

int X = 0;  // value of light, recived from server or set by IO.
int Y = 0;  // actual value of the LED.

{
if(X <Y)
{
	for(X,X++,Y)
		{
			anologritWe(LED,Y);
			// chck state mahine.

			// sleep or jump thread for 10ms;
		}
}
	else
	{
		for(Y,X--,X)
			{
				anologWrite(LED,y);
				// check state machine.

				// sleep or jump thread for 10ms;
			}
	}

}

	// posibly turn this into a function.

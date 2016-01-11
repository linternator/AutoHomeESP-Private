// 		PushButton-pulse		//


// arguments to feed the  function
int TimeConstant  = 15;
int NhNL = TRUE; //Normaly higth / Normaly low = True or false
int pin = 3; 	// chosen GPIO to toggle

void button-pulse( TimeConstnat, NhNL, pin);
{
	digitalWrite(pin,!NhNL);
		delay(TimeConstant); // substitue with interupt some how ? 
	digitalWrite(pin,NhNL);
}

// later add feature for duble tap ?  maybe ? 


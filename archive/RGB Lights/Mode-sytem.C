//		Mode-system.C		//
/////////////////////////////////////////
//

void setup()
{

	// delare IO pins
	// delare device id 
	// delare device name and group

	// connect to wifi
	// report to server ? 

	// send ready signal, be it blink led or message server
}


int common verables // like LED status, ect. global verables


void main()
}

switch(STATE)
{
	case:MANUAL
		{
			while( STATE == MANUAL)
				{ // only manual mode like a dumb device
					Button();
				}}
					break;

	case:AUTOonly
		{
			while( STATE == AUTOonly)
				{
					auto_bit();
				}}
					break;

	case:NORMAL
		{
			while( STATE == NORMAL)
				{	// normal mode manual and auto
					Buttons();
					WiFi();
					auto_bit();
				}}
					break;

	case:WIFIonly
		{
			while( STATE == WIFIonly)
				{	// only remote controled
					wifi();
				}}
					break;
}


void BUTTONS();
{	// service interupt for button contoles

// set up timer 1.  timer value about 20ms

if button pushed 
	record witch one wait timer (posibly timer interupt driven)
if button pushed 
	record in new space wait for timer
if button pushed 
	record in new space.

switch()
{
	case:1,0,0
		{	// just a tap
			// set int out
		} break;

	case:1,0,1
		{	// duble tap (pay rolitys to  BT :P )
			// set int out
		} break;

	case:1,1,1
		{	// hold down
			//  set int out
		} break;

	case:0,0,0
		{	// massive error proplem, PANIC!!
			//  set int out, and  serial print an error message
		} break;
}

else
	{ 
		write-state();
	}
}}



void WIFI();
{
	// Dave stuff
	
	// Listen for UDP pakets adressed to device
	//  set int out.  based on info from server
	
	write-state();
}

void AUTO_bit
{	// dependant on the device its on, maybe wifi set 

	// PIR sensor initolise 
	// timer for deactiveation something ...
		// to be done later
}



void write-state();
{
	switch( int out)
		{
			case:A
				{
					// set output apropriately
				   // LEDfade(); 	// make it do the thing.
				} break;

			case:B
				{
					// set output apropreatly
				} break;
		// defult no change
		}
}
}

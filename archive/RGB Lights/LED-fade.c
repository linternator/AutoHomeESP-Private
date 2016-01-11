////////	LED-fade.c	////////

array OLDled(index) = (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);		// array of all the pins, could be limited to just LED decleared, or use for othere things as well
array NEWled(index) = (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);		// array of all the pins / LEDs.	new values for the pins, kinda acting as a buffer.

// int mode = 0; 	// set mode, 0 = standby ? or normal ? 

atach inturpts(timer0);

int delay-time = 10;	// delay of brightness change, i want it to be optinal

void Inturput-function();
{

if(NEWled(index) == OLDled(index)
	{
		OLDled(index) = NEWled(index);
		detach inturput;	// only detach this inturupt ? maybe have intuprt fuction (index) ???
	}
		else
{
	if(NEWled(index) < OLDled(index))	// if the new value is bigger than the current (lighitng up more)
		{
		OLDled(index) ++;
		anolog.Write((index),OLDled(index));
		}

	if(NEWled(index) > OLDled(index))	// if the new value is smaller than the current (dimming the light)
		{
		OLDled(index) --;
		anologWrite((index),OLDled(index);
		}
}}


void main ()
{	// main code thing. dont care for this

	// wifi code
		// wifi sets moed ?
	// inbuilt button code
		// button push changes mode ? 

	// switch(mode)
	//	{
	//		case 0:
	//			{
	//				set NEWled(index);	// put a new value in NEWled at pin index (index)
	//				atach inturupt timer;
	//			}
	//	}

}


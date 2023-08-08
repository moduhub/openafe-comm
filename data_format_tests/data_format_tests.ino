#include "openAFEComm.h"

void setup() 
{
	Serial.begin(115200);
	openAFEComm_waitForCommands();
}

void loop() 
{
	// woah, such empty :O
}

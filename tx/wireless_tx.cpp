#include <VirtualWire.h>
#include "WProgram.h"
#include "rxtx_macros.h"

void setup();
void loop();
void updatePir();
void motionStart();
void motionStop();
void sendState();
void checkTimeout();
int darkEnough();

int txPin = 2;
int pirPin = 8;
int ledPin = 9;

int pirState = LOW;
int relayState = TURN_OFF;
int timeoutPending = 0;
unsigned long time;

const unsigned long dur = 30 * 1000; // 30 second duration

void setup() {
  vw_set_tx_pin( txPin );
  vw_setup( BAUD );
}

void loop() {
    updatePir();
    checkTimeout();
    sendState();
    delay( 1000 );
}

void updatePir() {
    int pirReading = digitalRead( pirPin );

    if ( pirReading == pirState ) {
        return;
    }

    if ( pirReading > pirState ) {
        motionStart();
    } else {
        motionStop();
    }
}

void motionStart() {
    pirState = HIGH;
    if ( darkEnough() || relayState == TURN_ON ) {
        timeoutPending = 0;
        relayState = TURN_ON;

    }
}

void motionStop() {
    pirState = LOW;
    if ( relayState == TURN_ON ) {
        timeoutPending = 1;
        time = millis();
    }
}

void checkTimeout() {
    if ( timeoutPending && ( millis() - time > dur ) ) {
        relayState = TURN_OFF;
    }
}

int darkEnough() {
    // TODO: rig up light sensor.
    return 1;
}

void sendState() {
    uint8_t msg[MSG_LEN];
    msg[ACTN_INDEX] = relayState;
    vw_send( msg, MSG_LEN );
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


#include "rxtx_macros.h"

int relayPin = 2; // arduino pin wired to relay control 
int ledPin   = 9; // non-essential led
int rxPin    = 8; // arduino pin wired to receiver data pin

int state    = 0;

void setup();
void loop();

void setup() {
    // initialize VirtualWire
    vw_set_rx_pin( rxPin );
    vw_setup( BAUD );
    vw_rx_start();
    
    // initialize relay and led pins
    pinMode( relayPin, OUTPUT );
    pinMode( ledPin, OUTPUT );

    digitalWrite( ledPin, HIGH );
    delay( 100 );
    digitalWrite( ledPin, LOW );
    delay( 100 );
    digitalWrite( ledPin, HIGH );
    delay( 100 );
    digitalWrite( ledPin, LOW );
}

void loop() {
    uint8_t msg[MSG_LEN];
    uint8_t msglen = MSG_LEN;
    if ( vw_get_message( msg, &msglen ) ) {
        if ( msglen != MSG_LEN ) {
            // msglen has actuall number of bytes read. if less
            // than expected, we received either an incomplete message
            // or noise. make blinky error and ignore.
            digitalWrite( ledPin, HIGH );
            delay( 100 );
            digitalWrite( ledPin, LOW );
            delay( 100 );
            digitalWrite( ledPin, HIGH );
            delay( 100 );
            digitalWrite( ledPin, LOW );
            digitalWrite( ledPin, state );
            return;
        }
        // I went a little overboard in my message 'protocol'. don't
        // really need addr or message type, so ignore.
        uint8_t action = msg[ACTN_INDEX];
        state = ( action == TURN_ON ? HIGH : LOW );
        digitalWrite( relayPin, state );
        digitalWrite( ledPin, state );
    }
}

int main(void) {
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

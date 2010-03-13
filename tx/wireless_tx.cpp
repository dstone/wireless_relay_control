#include <VirtualWire.h>
#include "WProgram.h"
#include "rxtx_macros.h" // shared info: baud, signal codes

void setup();
void loop();
void updatePir();
void motionStart();
void motionStop();
void sendState();
void checkTimeout();
int darkEnough();

int txPin = 2;  // pin connected to data pin on transmitter
int pirPin = 8; // pin connected to PIR sensor
int cdsPin = 5; // pin connected to photosensor
int ledPin = 9;

int pirState = LOW;         // current state of PIR sensor
int relayState = TURN_OFF;  // signal to be sent to receiver
int timeoutPending = 0;     
unsigned long time;

const unsigned long dur = 30 * 1000;    // 30 second duration
int cdsThreshold = 10;                  // if cds reading is above this, don't 
                                        // send TURN_ON signal

void setup() {
    vw_set_tx_pin( txPin );
    vw_setup( BAUD );
    pinMode( ledPin, OUTPUT );

    // ready for business
    digitalWrite( ledPin, HIGH );
    delay( 500 );
    digitalWrite( ledPin, LOW );
}

void loop() {
    // main loop:
    // read PIR, check if timeout reached, transmit relay state, 
    // all once per second
    updatePir();
    checkTimeout();
    sendState();
    delay( 1000 );
}

void updatePir() {
    int pirReading = digitalRead( pirPin );

    if ( pirReading == pirState ) {
        // ignore if pir state hasn't changed
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
    return analogRead( cdsPin ) <= cdsThreshold;
}

void sendState() {
    uint8_t msg[MSG_LEN];
    msg[ACTN_INDEX] = relayState;
    vw_send( msg, MSG_LEN );

    // some debug: blink if sending TURN_ON
    if ( relayState == TURN_ON ) {
        digitalWrite( ledPin, HIGH );
        delay( 10 );
        digitalWrite( ledPin, LOW );
        delay( 100 );
    }
}

int main(void)
{
    init();

    setup();
    
    for (;;)
        loop();
        
    return 0;
}


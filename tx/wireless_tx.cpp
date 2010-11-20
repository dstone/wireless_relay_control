#include <VirtualWire.h>
#include "WProgram.h"
#include "rxtx_macros.h" // shared info: baud, signal codes

void setup();
void loop();
void updatePir();
void sendState();
int darkEnough();

int txPin = 2;  // pin connected to data pin on transmitter
int pirPin = 8; // pin connected to PIR sensor
int cdsPin = 5; // pin connected to photosensor
int ledPin = 9;

int pirState = LOW;         // current state of PIR sensor

int cdsThreshold = 100;     // if cds reading is above this, don't 
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
    // read PIR, transmit PIR state
    updatePir();
    sendState();
    delay( 100 );
}

void updatePir() {
    int pirReading = digitalRead( pirPin );

    if ( pirReading == pirState ) {
        // ignore if pir state hasn't changed
        return;
    }

    if ( pirReading > pirState ) {
        pirState = HIGH;
    } else {
        pirState = LOW;
    }
}

int darkEnough() {
    return analogRead( cdsPin ) <= cdsThreshold;
}

void sendState() {
    uint8_t msg[MSG_LEN];
    msg[CDS_STATE_INDEX] = darkEnough() ? DARK : LIGHT;
    msg[ACTN_INDEX] = pirState ? TURN_ON : TURN_OFF;
    vw_send( msg, MSG_LEN );

    // some debug: blink if sending TURN_ON
    if ( pirState ) {
        digitalWrite( ledPin, HIGH );
        delay( 10 );
        digitalWrite( ledPin, LOW );
    }
}


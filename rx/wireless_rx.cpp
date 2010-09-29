#include "rxtx_macros.h"

int relayPin = 2; // arduino pin wired to relay control 
int ledPin   = 3; // non-essential led
int rxPin    = 8; // arduino pin wired to receiver data pin

int state = HIGH;
long time = 0;
int timeoutPending = true;
long duration = 5000; // timeout of 10 seconds
double timeScale = 2.0 * 2.71828183;

void setup();
void loop();
void checkRX();
void motionStart();
void motionStop();
void blinkyError();
void checkTimeout();
void updateRelay();
int ledSine();
long timeLeft();
void blink(int count);

void setup() {
    // initialize VirtualWire
    vw_set_rx_pin( rxPin );
    vw_setup( BAUD );
    vw_rx_start();
    
    // initialize relay and led pins
    pinMode( relayPin, OUTPUT );
    pinMode( ledPin, OUTPUT );
    blink(5);
}

void loop() {
    checkRX();
    checkTimeout();
    updateRelay();
}

void blink(int count) {
    while ( count-- ) {
        digitalWrite( ledPin, HIGH );
        delay(10);
        digitalWrite( ledPin, LOW );
        delay(10);
    }
}

void checkRX() {
    uint8_t msg[MSG_LEN];
    uint8_t msglen = MSG_LEN;
    if ( vw_get_message( msg, &msglen ) ) {
        if ( msglen != MSG_LEN ) {
            // msglen has actuall number of bytes read. if less
            // than expected, we received either an incomplete message
            // or noise. make blinky error and ignore.
            //blinkyError();
            return;
        }
        // I went a little overboard in my message 'protocol'. don't
        // really need addr or message type, so ignore.
        uint8_t action = msg[ACTN_INDEX];
        if ( action == TURN_ON ) {
            motionStart();
        } else {
            motionStop();
            blinkyError();
        }
    } else {
        // nothing received. start timeout
        //motionStop();
    }
}

void motionStart() {
    state = HIGH;
    timeoutPending = false;
}

void motionStop() {
    if ( state == LOW || timeoutPending ) {
        return; // don't reset clock if timeout already pending
    }
    timeoutPending = true;
    time = millis();
}

void checkTimeout() {
    if ( timeoutPending && timeLeft() <= 0 ) {
        state = LOW;
        timeoutPending = false;
    }
}

void updateRelay() {
    digitalWrite( relayPin, state );
    //digitalWrite( ledPin, state );
    /*
    if ( timeoutPending ) {
        digitalWrite( ledPin, ledSine() );
    } else {
        digitalWrite( ledPin, state );
    }
    */
}

int ledSine() {
    double t = (millis() - time)/(duration/timeScale);

    /*
    int val = (int)((sin(exp(t)) + 1) * 125);
    int s = max( val, 0 );
    return min( 255, s );
    */
    return sin(exp(t)) >= 0;
}

long timeLeft() {
    return duration - (millis() - time);
}

void blinkyError() {
    digitalWrite( ledPin, HIGH );
    delay( 100 );
    digitalWrite( ledPin, LOW );
    delay( 100 );
    digitalWrite( ledPin, HIGH );
    delay( 100 );
    digitalWrite( ledPin, LOW );
    digitalWrite( ledPin, state );
}


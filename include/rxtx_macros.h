#ifndef ST_RXTX
#define ST_RXTX

#include <VirtualWire.h>
#include "WProgram.h"

#define BAUD 2400

#define MSG_LEN 3
#define ADDR_INDEX 0
#define TYPE_INDEX 1
#define ACTN_INDEX 2

// Type of message
#define STATE_CHANGE 0xAA
#define PING 0x33

// Action to take
#define TURN_ON 0xAA
#define TURN_OFF 0xF0

#endif

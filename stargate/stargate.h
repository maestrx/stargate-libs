// ----------------------------------------
// COMMON
// ----------------------------------------

// indicate if usign real or fake gate
#define FAKE_GATE 1

// https://forum.arduino.cc/t/sending-struct-over-i2c/886392/30
// https://github.com/EinarArnason/ArduinoQueue
struct i2c_message {
    // action:
    //   --- DHD => GATE
    //   1 -> chevron 1 ID
    //   2 -> chevron 2 ID
    //   3 -> chevron 3 ID
    //   4 -> chevron 4 ID
    //   5 -> chevron 5 ID
    //   6 -> chevron 6 ID
    //   7 -> chevron 7 ID
    //   20 -> RED button pressed, valid address
    //   21 -> RED button pressed, INVALID address, reset dial
    //   22 -> reset dial/close gate (RED button pressed to close gate)
    //   99 -> No Operation
    //   --- DHD => MP3
    //   X -> Play sound X (1-14)
    //   50 -> Stop sounds
    //   99 -> No Operation
    //   --- GATE => DHD
    //   10 -> chevron dialing started
    //   11 -> chevron dialing done
    //   99 -> No Operation
    uint8_t action;
    // chevron:
    //   chevron ID -> chevron 1 dialing done
    uint8_t chevron;
};

// I2C command list
#define ACTION_DIAL_START 10
#define ACTION_DIAL_END 11
#define ACTION_ADDR_VALID 20
#define ACTION_ADDR_INVALID 21
#define ACTION_GATE_RESET 22
#define ACTION_SOUND_STOP 50
#define ACTION_NOOP 99
#define ACTION_NODATA 255  // indicates that no data were recieved over the I2C bus

// ----------------------------------------
// GATE definitions
// ----------------------------------------

// debug options
#define DEBUG_I2C_MP3 false
#define DEBUG_I2C_MP3_DEV if(DEBUG_I2C_MP3)Serial
#define DEBUG_I2C_GATE false
#define DEBUG_I2C_GATE_DEV if(DEBUG_I2C_GATE)Serial

// set motor speed, 1000 is max speed
#define SPEED_STEPS_PER_SECOND 1000

// set the number of motor steps for the gate and chevron
#ifdef FAKE_GATE
  #define GATE_SYMBOLS 8
  #define GATE_CHEVRON_STEPS 810
#else
  #define GATE_SYMBOLS 39
  #define GATE_CHEVRON_STEPS 246
#endif
#define GATE_CHEVRON_OPEN_STEPS 8

// define LED PINs
#ifdef FAKE_GATE
const int Calibrate_LED = 15;
const int Calibrate_Resistor = A8;
const int Ramp_LED = 16;
const int Chevron_LED[] = {40,41,42,43,44,45,46,47};     // TBD!!!
#else
const int Calibrate_LED = 16;
const int Calibrate_Resistor = A8;
const int Ramp_LED = 51;
const int Chevron_LED[] = {47,45,43,41,39,37,35,33,31};     // TBD!!!

#endif

// ----------------------------------------
// MP3 definitions
// ----------------------------------------

#define MP3_GATE_DIALING 1
#define MP3_CHEVRON_SEAL 2
#define MP3_WORMHOLE_START 3
#define MP3_WORMHOLE_STOP 4
#define MP3_WORMHOLE_RUNNING 5
#define MP3_CHEVRON_1 6
#define MP3_CHEVRON_2 7
#define MP3_CHEVRON_3 8
#define MP3_CHEVRON_4 9
#define MP3_CHEVRON_5 10
#define MP3_CHEVRON_6 11
#define MP3_CHEVRON_7 12
#define MP3_UNKNOWN 13

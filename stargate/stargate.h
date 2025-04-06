// version 0.1, 1.4.2025
// version 0.2, 6.4.2025


// MP3 -> COM9
// DHD -> COM18 (old BL)
// GATE -> COM16

// ----------------------------------------
// COMMON
// ----------------------------------------

// indicate if usign real or fake gate
#define FAKE_GATE 1

// I2C debug on Gate&MP3
#define DEBUG_I2C false
#define DEBUG_I2C_DEV if(DEBUG_I2C)Serial

// I2C debug on DHD
#define DEBUG_I2C_MP3 false
#define DEBUG_I2C_MP3_DEV if(DEBUG_I2C_MP3)Serial
#define DEBUG_I2C_GATE false
#define DEBUG_I2C_GATE_DEV if(DEBUG_I2C_GATE)Serial

#ifdef FAKE_GATE
  #define MP3_VOLUME 15 //Set volume value. From 0 to 30
#else
  #define MP3_VOLUME 30 //Set volume value. From 0 to 30
#endif

// https://forum.arduino.cc/t/sending-struct-over-i2c/886392/30
// https://github.com/EinarArnason/ArduinoQueue
struct i2c_message {
    // action:
    //   --- DHD => GATE
    //   (1) -> dial chevron #1
    //   (2) -> dial chevron #2
    //   (3) -> dial chevron #3
    //   (4) -> dial chevron #4
    //   (5) -> dial chevron #5
    //   (6) -> dial chevron #6
    //   (7) -> dial chevron #7
    //   ACTION_ADDR_VALID(20) -> RED button pressed, valid address
    //   ACTION_ADDR_INVALID(21) -> RED button pressed, INVALID address, reset dial
    //   ACTION_GATE_RESET(22) -> reset dial/close gate (RED button pressed to close gate)
    //   ACTION_NOOP(99) -> No Operation, keepalive ping
    //   --- DHD => MP3
    //   (X)-> Play sound X (1-14)
    //   ACTION_SOUND_STOP(50) -> Stop sounds
    //   ACTION_NOOP(99) -> No Operation, keepalive pong
    //   --- GATE => DHD
    //   ACTION_DIAL_START(10) -> chevron dialing started, chevron # in chevron field
    //   ACTION_DIAL_END(11) -> chevron dialing done, chevron # in chevron field
    //   ACTION_WORMHOLE_ESTABLISHED(12) -> wormhole established
    //   ACTION_NOOP(99) -> No Operation, keepalive pong
    uint8_t action;
    // chevron:
    //   chevron ID -> chevron 1 dialing done
    uint8_t chevron;
};

/*
Typical message workflow:
DHD -> Gate: [1, chevron ID] Dial chevron 1
Gate -> DHD: [ACTION_DIAL_START, 1] Chevron dialing started, chevron contains chevron sequence #
Gate -> DHD: [ACTION_DIAL_END, 1] Chevron dialing done, chevron contains chevron sequence #
DHD -> Gate: [2, chevron ID] Dial chevron 2
Gate -> DHD: [ACTION_DIAL_START, 2] Chevron dialing started, chevron contains chevron sequence #
Gate -> DHD: [ACTION_DIAL_END, 2] Chevron dialing done, chevron contains chevron sequence #
DHD -> Gate: [3, chevron ID] Dial chevron 3
Gate -> DHD: [ACTION_DIAL_START, 3] Chevron dialing started, chevron contains chevron sequence #
Gate -> DHD: [ACTION_DIAL_END, 3] Chevron dialing done, chevron contains chevron sequence #
DHD -> Gate: [4, chevron ID] Dial chevron 4
Gate -> DHD: [ACTION_DIAL_START, 4] Chevron dialing started, chevron contains chevron sequence #
Gate -> DHD: [ACTION_DIAL_END, 4] Chevron dialing done, chevron contains chevron sequence #
DHD -> Gate: [5, chevron ID] Dial chevron 5
Gate -> DHD: [ACTION_DIAL_START, 5] Chevron dialing started, chevron contains chevron sequence #
Gate -> DHD: [ACTION_DIAL_END, 5] Chevron dialing done, chevron contains chevron sequence #
DHD -> Gate: [6, chevron ID] Dial chevron 6
Gate -> DHD: [ACTION_DIAL_START, 6] Chevron dialing started, chevron contains chevron sequence #
Gate -> DHD: [ACTION_DIAL_END, 6] Chevron dialing done, chevron contains chevron sequence #
DHD -> Gate: [7, chevron ID] Dial chevron 7
Gate -> DHD: [ACTION_DIAL_START, 7] Chevron dialing started, chevron contains chevron sequence #
Gate -> DHD: [ACTION_DIAL_END, 7] Chevron dialing done, chevron contains chevron sequence #
DHD -> Gate: [ACTION_ADDR_VALID, 0] RED button pressed, valid address
DHD -> Gate: [ACTION_GATE_RESET, 0] reset dial/close gate
*/

// I2C command list
#define ACTION_DIAL_START 10
#define ACTION_DIAL_END 11
#define ACTION_WORMHOLE_ESTABLISHED 12
#define ACTION_ADDR_VALID 20
#define ACTION_ADDR_INVALID 21
#define ACTION_GATE_RESET 22
#define ACTION_SOUND_STOP 50
#define ACTION_NOOP 99
#define ACTION_NODATA 255  // indicates that no data were recieved over the I2C bus

// ----------------------------------------
// DHD definitions
// ----------------------------------------

#define KEYPAD_INPUT A0
#define KEYPRESS_TIMEOUT 60000 // timeout for keypress action
int DHD_Chevron_LED[] = {2,3,4,5,6,7,8,9,};  // LED pin array
#ifdef FAKE_GATE
  #define KEYPRESS_RAW_THRESHOLD 900
#else
  #define KEYPRESS_RAW_THRESHOLD 950
#endif


// ----------------------------------------
// GATE definitions
// ----------------------------------------

#define GATE_ACTION_TIMEOUT 60000 // timeout for gate actions

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
#if defined(ARDUINO_AVR_MEGA2560)
  #ifdef FAKE_GATE
    const int Calibrate_LED = 15;
    const int Calibrate_Resistor = A8;
    const int Ramp_LED = 16;
    const int Gate_Chevron_LED[] = {40,41,42,43,44,45,46,47};
  #else
    const int Calibrate_LED = 16;
    const int Calibrate_Resistor = A8;
    const int Ramp_LED = 51;
    const int Gate_Chevron_LED[] = {47,45,43,41,39,37,35,33,31};
  #endif
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

#ifndef ADB_h
#define ADB_h

#include <Arduino.h>

#define ADB_DATA_PIN        PB4
#define ADB_DATA_PORT       GPIOB
#define ADB_DATA_PIN_NO     4
#define ADB_WRITE(bit)      digitalWrite(ADB_DATA_PIN, bit)
#define ADB_READ()          (digitalRead(ADB_DATA_PIN))

#define ADB_ADDRESS(addr)   (addr << 4)
#define ADB_REGISTER(reg)   (reg)
#define ADB_CMD_TALK        (0b11 << 2)
#define ADB_CMD_LISTEN      (0b10 << 2)
#define ADB_CMD_FLUSH       (0b01 << 2)

#define ADB_BIT_ERROR       0xFF

// Reset: signal low for 3 ms.
static void adb_reset() {
    ADB_WRITE(LOW);
    delayMicroseconds(3000);
    ADB_WRITE(HIGH);
}

// Attention: signal low for 800 μs.
static void adb_attention() {
    ADB_WRITE(LOW);
    delayMicroseconds(800);
    ADB_WRITE(HIGH);
}

// Sync: signal high for 70 μs.
static void adb_sync() {
    ADB_WRITE(HIGH);
    delayMicroseconds(70);
    ADB_WRITE(LOW);
}

// Send a single on ADB_DATA_PIN.
// '0' if bit == 0x0: write 65 μs LOW, 35 μs HIGH.
// '1' if bit >= 0x1: write 35 μs LOW, 65 μs HIGH.
static void adb_write_bit(uint16_t bit) {
    if (bit) { // '1' bit
        ADB_WRITE(LOW);
        delayMicroseconds(35);
        
        ADB_WRITE(HIGH);
        delayMicroseconds(65);
    }
    else { // '0' bit
        ADB_WRITE(LOW);
        delayMicroseconds(65);
        
        ADB_WRITE(HIGH);
        delayMicroseconds(35);
    }
}

// Send `length` least significant bits from `bits`,
// starting with the most significant bit.
static void adb_write_bits(uint16_t bits, uint8_t length) {
    uint16_t mask = 1 << (length - 1);
    while (mask) {
        adb_write_bit(bits & mask);
        mask >>= 1;
    }
}

// Like adb_write_bits, but add start and stop bits
static void adb_write_data_packet(uint16_t bits, uint8_t length) {
    adb_write_bit(1);
    adb_write_bits(bits, length);
    adb_write_bit(0);
}

// Send the '0' stop bit, and listen for an SRQ.
// ``If a device in need of service issues a service request,
// it must do so within the 65 μs of the Stop Bit’s low time
// and maintain the line low for a total of 300 μs.''
// Returns true if there was an SRQ.
static bool adb_stop_bit_srq_listen() {
    adb_write_bit(0);
    // TODO: Properly handle SRQ, currently waits it out:
    auto time_end_stop_bit = micros();
    while (ADB_READ() == LOW && micros() - time_end_stop_bit < 300 - 35) { // TODO: does it make sense to timeout?
        delayMicroseconds(5);
    }
    return false;
}

// Write a command to the bus.
void adb_write_command(uint8_t command_byte) {
    adb_attention();
    adb_sync();
    adb_write_bits((uint16_t)command_byte, 8);
    adb_stop_bit_srq_listen(); // TODO: Handle the SRQ
}

// Stop-to-start time: period of 140 - 260 μs before device's
// response when the bus is held high.
// Returns: true if the response is starting, false if timeout
static bool adb_wait_tlt(bool response_expected) {
    ADB_WRITE(HIGH);
    delayMicroseconds(140);
    uint8_t i = 0;
    while (ADB_READ() == HIGH && i < 240 && response_expected) {
        delayMicroseconds(1);
        i++;
    }
    
    return true;
}

// Read a single bit from the bus.
static uint8_t adb_read_bit() {
    auto time_start = micros();
    
    while (ADB_READ() == LOW) {
        // devices need to stick to 30% precision, 65 * 1.3 = 85 μs
        // if this time is exceeded assume timeout
        if (micros() - time_start > 85)
            return ADB_BIT_ERROR;
    }
    auto low_time = micros() - time_start;

    while (ADB_READ() == HIGH) {
        // devices need to stick to 30% precision, 65 * 1.3 = 85 μs
        // if this time is exceeded assume timeout
        if (micros() - time_start - low_time > 85)
            return ADB_BIT_ERROR;
    }
    auto high_time = micros() - time_start - low_time;
    
    return (low_time < high_time) ? 0x1 : 0x0;
}

// Read `length` bits from the bus into `buffer`.
bool adb_read_data_packet(uint16_t* buffer, uint8_t length)
{   
    if (adb_read_bit() != 0x1) { // start bit should equal to '1'
        return false;
    }

    *buffer = 0;
    for (uint8_t i = 0; i < length; i++)
    {
        uint8_t current_bit = adb_read_bit();
        if (current_bit == ADB_BIT_ERROR) {
            return false;
        }
        *buffer <<= 1;
        *buffer |= current_bit;
    }

    /* uint8_t stop_bit = */ adb_read_bit(); // should equal to '0'
    return true;
}

void adb_init() {
    pinMode(ADB_DATA_PIN, OUTPUT_OPEN_DRAIN);
    ADB_WRITE(HIGH);

    while (ADB_READ() == LOW); // wait for the bus

    adb_reset();
}

#endif
#include <Arduino.h>
#include <SPI.h>

#define RFM69_CS    8
#define RFM69_DIO0  3
#define RFM69_DIO1  15
#define RFM69_RST   4

#define RFM69_SPI_CLK_SPEED                     10000000

#define RFM69_WRITE_MODE                        0x80

#define RFM69_REG00_FIFO                        0x00
#define RFM69_REG01_OP_MODE                     0x01
#define RFM69_REG02_DATA_MODULATION             0x02
#define RFM69_REG03_BITRATE_MSB                 0x03
#define RFM69_REG04_BITRATE_LSB                 0x04
#define RFM69_REG05_FREQ_DEV_MSB                0x05
#define RFM69_REG06_FREQ_DEV_LSB                0x06
#define RFM69_REG07_RF_FREQ_MSB                 0x07
#define RFM69_REG08_RF_FREQ_MID                 0x08
#define RFM69_REG09_RF_FREQ_LSB                 0x09
#define RFM69_REG18_LNA                         0x18
#define RFM69_REG19_RX_BANDWIDTH                0x19
#define RFM69_REG23_RSSI_CONFIG                 0x23
#define RFM69_REG24_RSSI_VALUE                  0x24
#define RFM69_REG25_DIO_MAPPING_1               0x25
#define RFM69_REG26_DIO_MAPPING_2               0x26
#define RFM69_REG27_IRQ_FLAGS_1                 0x27
#define RFM69_REG28_IRQ_FLAGS_2                 0x28
#define RFM69_REG2C_PREAMBLE_SIZE_MSB           0x2C
#define RFM69_REG2D_PREAMBLE_SIZE_LSB           0x2D
#define RFM69_REG2E_SYNC_CONFIG                 0x2E
#define RFM69_REG2F_SYNC_VALUE_1                0x2F
#define RFM69_REG30_SYNC_VALUE_2                0x30
#define RFM69_REG31_SYNC_VALUE_3                0x31
#define RFM69_REG32_SYNC_VALUE_4                0x32
#define RFM69_REG33_SYNC_VALUE_5                0x33
#define RFM69_REG34_SYNC_VALUE_6                0x34
#define RFM69_REG35_SYNC_VALUE_7                0x35
#define RFM69_REG36_SYNC_VALUE_8                0x36
#define RFM69_REG37_PACKET_CONFIG_1             0x37
#define RFM69_REG38_PAYLOAD_LENGTH              0x38
#define RFM69_REG3C_FIFO_THRESHOLD              0x3C
#define RFM69_REG58_TEST_LNA                    0x58

//RFM69_REG01_OP_MODE
#define RFM69_OP_MODE_SLEEP                     0x00
#define RFM69_OP_MODE_STANDBY                   0x04
#define RFM69_OP_MODE_FREQ_SYNTH                0x08
#define RFM69_OP_MODE_TX                        0x0C
#define RFM69_OP_MODE_RX                        0x10

//RFM69_REG02_DATA_MODULATION
#define RFM69_DATA_MODE_PACKET                  0x00
#define RFM69_DATA_MODE_CONTINUOUS_WITH_BITSYNC 0x40
#define RFM69_DATA_MODE_CONTINUOUS              0x60

#define RFM69_DATA_MODULATION_TYPE_FSK          0x00
#define RFM69_DATA_MODULATION_TYPE_OOK          0x08

#define RFM69_DATA_MODULATION_SHAPING_NONE      0x00
#define RFM69_DATA_MODULATION_SHAPING_GFSK_1_0  0x01
#define RFM69_DATA_MODULATION_SHAPING_GFSK_0_5  0x02
#define RFM69_DATA_MODULATION_SHAPING_GFSK_0_3  0x04

//RFM69_REG18_LNA
#define RFM69_LNA_50_OHM                        0x00
#define RFM69_LNA_200_OHM                       0x80

#define RFM69_LNA_GAIN_AGC                      0x00
#define RFM69_LNA_GAIN_HIGH                     0x08

//RFM69_REG23_RSSI_CONFIG
#define RFM69_RSSI_CONFIG_START                 0x01
#define RFM69_RSSI_CONFIG_DONE                  0x02

//RFM69_REG25_DIO_MAPPING_1
#define RFM69_DIO0_MAPPING_RX_CRC_OK            0x00
#define RFM69_DIO0_MAPPING_RX_PAYLOAD_READY     0x40
#define RFM69_DIO0_MAPPING_RX_SYNC_ADDRESS      0x80
#define RFM69_DIO0_MAPPING_RX_RSSI              0xC0

#define RFM69_DIO1_MAPPING_FIFO_LEVEL           0x00
#define RFM69_DIO1_MAPPING_FIFO_FULL            0x10
#define RFM69_DIO1_MAPPING_FIFO_NOT_EMPTY       0x20
#define RFM69_DIO1_MAPPING_FIFO_TIMEOUT         0x30

//RFM69_REG27_IRQ_FLAGS_1
#define IRQ_FLAGS_1_SYNC_ADDRESS_MATCH          0x01
#define IRQ_FLAGS_1_AUTO_MODE                   0x02
#define IRQ_FLAGS_1_TIMEOUT                     0x04
#define IRQ_FLAGS_1_RSSI                        0x08
#define IRQ_FLAGS_1_PLL_LOCK                    0x10
#define IRQ_FLAGS_1_TX_READY                    0x20
#define IRQ_FLAGS_1_RX_READY                    0x40
#define IRQ_FLAGS_1_MODE_READY                  0x80

//RFM69_REG28_IRQ_FLAGS_2
#define IRQ_FLAGS_2_CRC_OK                      0x02
#define IRQ_FLAGS_2_PAYLOAD_READY               0x04
#define IRQ_FLAGS_2_PACKET_SENT                 0x08
#define IRQ_FLAGS_2_FIFO_OVERRUN                0x10
#define IRQ_FLAGS_2_FIFO_LEVEL                  0x20
#define IRQ_FLAGS_2_FIFO_NOT_EMPTY              0x40
#define IRQ_FLAGS_2_FIFO_FULL                   0x80

//RFM69_REG2E_SYNC_CONFIG
#define SYNC_CONFIG_SYNC_ERROR_TOLERATE_0       0x00
#define SYNC_CONFIG_SYNC_ERROR_TOLERATE_1       0x01
#define SYNC_CONFIG_SYNC_ERROR_TOLERATE_2       0x02
#define SYNC_CONFIG_SYNC_ERROR_TOLERATE_3       0x03
#define SYNC_CONFIG_SYNC_ERROR_TOLERATE_4       0x04
#define SYNC_CONFIG_SYNC_ERROR_TOLERATE_5       0x05
#define SYNC_CONFIG_SYNC_ERROR_TOLERATE_6       0x06
#define SYNC_CONFIG_SYNC_ERROR_TOLERATE_7       0x07

// (size + 1)
#define SYNC_CONFIG_SYNC_WORD_SIZE_1            0
#define SYNC_CONFIG_SYNC_WORD_SIZE_2            (1 << 3)
#define SYNC_CONFIG_SYNC_WORD_SIZE_3            (2 << 3)
#define SYNC_CONFIG_SYNC_WORD_SIZE_4            (3 << 3)
#define SYNC_CONFIG_SYNC_WORD_SIZE_5            (4 << 3)
#define SYNC_CONFIG_SYNC_WORD_SIZE_6            (5 << 3)
#define SYNC_CONFIG_SYNC_WORD_SIZE_7            (6 << 3)
#define SYNC_CONFIG_SYNC_WORD_SIZE_8            (7 << 3)

#define SYNC_CONFIG_SYNC_ON                     0x80

//RFM69_REG37_PACKET_CONFIG_1
#define RFM69_PACKET_CONFIG_1_PACKET_FORMAT_FIXED       0x00
#define RFM69_PACKET_CONFIG_1_PACKET_FORMAT_VARIABLE    0x80

#define RFM69_PACKET_CONFIG_1_DC_FREE_NONE              0x00
#define RFM69_PACKET_CONFIG_1_DC_FREE_MANCHESTER        0x20
#define RFM69_PACKET_CONFIG_1_DC_FREE_WHITENING         0x40

#define RFM69_PACKET_CONFIG_1_CRC_ON                    0x10

//RFM69_REG3C_FIFO_THRESHOLD
#define FIFO_THRESHOLD_TX_START_CONDITION_FIFO_LEVEL        0x00
#define FIFO_THRESHOLD_TX_START_CONDITION_FIFO_NOT_EMTPY    0x80

volatile bool msgReceived = false;
volatile uint8_t irqFlags1;
volatile uint8_t irqFlags2;
volatile uint8_t msgHopIndex;
volatile uint8_t msgLen;
volatile int8_t msgRSSI;
volatile uint8_t writeOffset;
volatile uint8_t msg[255];

uint8_t readReg(uint8_t reg) {
    SPI.beginTransaction(SPISettings(RFM69_SPI_CLK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(RFM69_CS, LOW);
    SPI.transfer(reg & ~RFM69_WRITE_MODE);
    uint8_t value = SPI.transfer(0);
    digitalWrite(RFM69_CS, HIGH);
    SPI.endTransaction();
    return value;
}

void readRegBurst(uint8_t reg, uint8_t *buffer, uint8_t len) {
    SPI.beginTransaction(SPISettings(RFM69_SPI_CLK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(RFM69_CS, LOW);
    SPI.transfer(reg);
    for (uint8_t i = 0; i < len; i++) {
        buffer[i] = SPI.transfer(0);
    }
    digitalWrite(RFM69_CS, HIGH);
    SPI.endTransaction();
}

uint8_t writeReg(uint8_t reg, uint8_t value) {
    SPI.beginTransaction(SPISettings(RFM69_SPI_CLK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(RFM69_CS, LOW);
    SPI.transfer(reg | RFM69_WRITE_MODE);
    uint8_t oldValue = SPI.transfer(value);
    digitalWrite(RFM69_CS, HIGH);
    SPI.endTransaction();
    return oldValue;
}

int8_t readRSSI(bool start) {
    if (start) {
        writeReg(RFM69_REG23_RSSI_CONFIG, RFM69_RSSI_CONFIG_START);
        while (!(readReg(RFM69_REG23_RSSI_CONFIG) & RFM69_RSSI_CONFIG_DONE));
    }
    int16_t rssi = readReg(RFM69_REG24_RSSI_VALUE);
    rssi >>= 1;
    rssi = -rssi;
    return (int8_t)rssi;
}

uint16_t crc_ccitt(uint8_t *msg, uint8_t len) {
    uint16_t crc = 0xFFFF;
    uint16_t poly = 0x8408;
    uint16_t i, j, k;
  
    for (i = 0; i < len; i++) {
      for (j = 0; j < 8; j++) {
        k = crc & 1;
        crc >>= 1;
        if (msg[i] & (1 << j)) {
          k ^= 1;
        }
        if (k) {
          crc ^= poly;
        }
      }
    }
  
    crc ^= 0xFFFF;
    return crc;
}

bool validateCrc(uint8_t *msg, uint8_t len) {
    uint16_t calculated_crc = crc_ccitt(msg, len-2);
    uint16_t expected_crc = msg[len-2] | (msg[len-1] << 8);
    return (calculated_crc == expected_crc ? true : false);
}

inline void SerialPrintHex(uint8_t value) {
    char buf[3];

    sprintf(buf, "%02x", value);
    Serial.print(buf);
}

const uint8_t frequencyTable[][3] = {
    0xE1, 0x99, 0x99, // 902.4MHz 00 //Not Used
    0xE1, 0xB3, 0x33, // 902.8MHz 01
    0xE1, 0xCC, 0xCC, // 903.2MHz 02 //Not Used
    0xE1, 0xE6, 0x66, // 903.6MHz 03
    0xE2, 0x00, 0x00, // 904.0MHz 04
    0xE2, 0x19, 0x99, // 904.4MHz 05
    0xE2, 0x33, 0x33, // 904.8MHz 06
    0xE2, 0x4C, 0xCC, // 905.2MHz 07
    0xE2, 0x66, 0x66, // 905.6MHz 08
    0xE2, 0x80, 0x00, // 906.0MHz 09
    0xE2, 0x99, 0x99, // 906.4MHz 10 //Not Used
    0xE2, 0xB3, 0x33, // 906.8MHz 11
    0xE2, 0xCC, 0xCC, // 907.2MHz 12
    0xE2, 0xE6, 0x66, // 907.6MHz 13
    0xE3, 0x00, 0x00, // 908.0MHz 14
    0xE3, 0x19, 0x19, // 908.4MHz 15
    0xE3, 0x33, 0x33, // 908.8MHz 16
    0xE3, 0x4C, 0xCC, // 909.2MHz 17 //Not Used
    0xE3, 0x66, 0x66, // 909.6MHz 18
    0xE3, 0x80, 0x00, // 910.0MHz 19
    0xE3, 0x99, 0x99, // 910.4MHz 20
    0xE3, 0xB3, 0x33, // 910.8MHz 21
    0xE3, 0xCC, 0xCC, // 911.2MHz 22
    0xE3, 0xE6, 0x66, // 911.6MHz 23
    0xE4, 0x00, 0x00, // 912.0MHz 24 //Not Used
    0xE4, 0x19, 0x19, // 912.4MHz 25
    0xE4, 0x33, 0x33, // 912.8MHz 26
    0xE4, 0x4C, 0xCC, // 913.2MHz 27
    0xE4, 0x66, 0x66, // 913.6MHz 28
    0xE4, 0x80, 0x00, // 914.0MHz 29
    0xE4, 0x99, 0x99, // 914.4MHz 30
    0xE4, 0xB3, 0x33  // 914.8MHz 31
};

uint8_t dwellCount = 0;
uint8_t hopIndex = 0;
void hop() {
    dwellCount = 0;
    hopIndex++;

    // For now, skipping channels with no activity so that the channel numbers
    // in the log are consistent.
    switch (hopIndex) {
      case 2:
        hopIndex = 3;
        break;

      case 10:
        hopIndex = 11;
        break;

      case 17:
        hopIndex = 18;
        break;

      case 24:
        hopIndex = 25;
        break;

      case 32:
        hopIndex = 1;
        break;

      default:
        break;
    }

    writeReg(RFM69_REG01_OP_MODE, RFM69_OP_MODE_FREQ_SYNTH); 
    SPI.beginTransaction(SPISettings(RFM69_SPI_CLK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(RFM69_CS, LOW);
    SPI.transfer(RFM69_REG07_RF_FREQ_MSB | RFM69_WRITE_MODE);
    SPI.transfer(frequencyTable[hopIndex][0]);
    SPI.transfer(frequencyTable[hopIndex][1]);
    SPI.transfer(frequencyTable[hopIndex][2]);
    digitalWrite(RFM69_CS, HIGH);
    SPI.endTransaction();
    writeReg(RFM69_REG01_OP_MODE, RFM69_OP_MODE_RX);
}

void onFifoLevel() {
    irqFlags2 = readReg(RFM69_REG28_IRQ_FLAGS_2);

    while (irqFlags2 & IRQ_FLAGS_2_FIFO_NOT_EMPTY && !(irqFlags2 & IRQ_FLAGS_2_PAYLOAD_READY)) {
        msg[writeOffset++] = readReg(RFM69_REG00_FIFO) ^ 0xaa;
        irqFlags2 = readReg(RFM69_REG28_IRQ_FLAGS_2);
    }
}

void onPayloadReady() {
    irqFlags2 = readReg(RFM69_REG28_IRQ_FLAGS_2);

    if (irqFlags2 & IRQ_FLAGS_2_PAYLOAD_READY) {
        digitalWrite(LED_BUILTIN, HIGH);
        writeReg(RFM69_REG01_OP_MODE, RFM69_OP_MODE_STANDBY);
        msgHopIndex = hopIndex;
        msgRSSI = readRSSI(false);
        msgLen = msg[1] + 2; //+2 for CRC
        msgReceived = true;
    }
}

void setup() {
    digitalWrite(LED_BUILTIN, HIGH);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(921600);
    //while (!Serial) delay(1);

    digitalWrite(RFM69_RST, LOW);
    pinMode(RFM69_RST, OUTPUT);

    digitalWrite(RFM69_CS, HIGH);
    pinMode(RFM69_CS, OUTPUT);

    SPI.begin();

    digitalWrite(RFM69_RST, HIGH);
    delay(1);
    digitalWrite(RFM69_RST, LOW);
    delay(10);

    writeReg(RFM69_REG01_OP_MODE, RFM69_OP_MODE_STANDBY);
    writeReg(RFM69_REG02_DATA_MODULATION, RFM69_DATA_MODE_PACKET | RFM69_DATA_MODULATION_TYPE_FSK | RFM69_DATA_MODULATION_SHAPING_NONE);

    writeReg(RFM69_REG03_BITRATE_MSB, 0x00); //142.2222kBaud
    writeReg(RFM69_REG04_BITRATE_LSB, 0xE1);

    writeReg(RFM69_REG05_FREQ_DEV_MSB, 0x08); //125kHz (seemed to perform better)
    //writeReg(RFM69_REG05_FREQ_DEV_MSB, 0x04); //62.5kHz
    writeReg(RFM69_REG06_FREQ_DEV_LSB, 0x00);

    writeReg(RFM69_REG19_RX_BANDWIDTH, 0x51); //200kHz
    
    writeReg(RFM69_REG2E_SYNC_CONFIG, SYNC_CONFIG_SYNC_ON | SYNC_CONFIG_SYNC_WORD_SIZE_8);
    writeReg(RFM69_REG2F_SYNC_VALUE_1, 0xf0); //4x oversampled A6 56
    writeReg(RFM69_REG30_SYNC_VALUE_2, 0xf0);
    writeReg(RFM69_REG31_SYNC_VALUE_3, 0x0f);
    writeReg(RFM69_REG32_SYNC_VALUE_4, 0xf0);
    writeReg(RFM69_REG33_SYNC_VALUE_5, 0x0f);
    writeReg(RFM69_REG34_SYNC_VALUE_6, 0x0f);
    writeReg(RFM69_REG35_SYNC_VALUE_7, 0x0f);
    writeReg(RFM69_REG36_SYNC_VALUE_8, 0xf0);

    writeReg(RFM69_REG37_PACKET_CONFIG_1, RFM69_PACKET_CONFIG_1_PACKET_FORMAT_FIXED);
    writeReg(RFM69_REG38_PAYLOAD_LENGTH, 128);
    writeReg(RFM69_REG3C_FIFO_THRESHOLD, FIFO_THRESHOLD_TX_START_CONDITION_FIFO_LEVEL | 16);

    writeReg(RFM69_REG58_TEST_LNA, 0x2D); // Increase sensitivity

    pinMode(RFM69_DIO0, INPUT);
    uint32_t payloadReadyInterruptNumber = digitalPinToInterrupt(RFM69_DIO0);
    attachInterrupt(payloadReadyInterruptNumber, onPayloadReady, RISING);
    SPI.usingInterrupt(payloadReadyInterruptNumber);

    pinMode(RFM69_DIO1, INPUT);
    uint32_t fifoInterruptNumber = digitalPinToInterrupt(RFM69_DIO1);
    attachInterrupt(fifoInterruptNumber, onFifoLevel, RISING);
    SPI.usingInterrupt(fifoInterruptNumber);

    writeReg(RFM69_REG25_DIO_MAPPING_1, RFM69_DIO0_MAPPING_RX_PAYLOAD_READY | RFM69_DIO1_MAPPING_FIFO_LEVEL);

    Serial.println("Ready.");
    digitalWrite(LED_BUILTIN, LOW);
    
    hop();
}

void dumpIrqFlags() {
    irqFlags1 = readReg(RFM69_REG27_IRQ_FLAGS_1);
    irqFlags2 = readReg(RFM69_REG28_IRQ_FLAGS_2);

    if (irqFlags1 & IRQ_FLAGS_1_MODE_READY)         Serial.print("ModeReady ");
    if (irqFlags1 & IRQ_FLAGS_1_RX_READY)           Serial.print("RxReady ");
    if (irqFlags1 & IRQ_FLAGS_1_TX_READY)           Serial.print("TxReady ");
    if (irqFlags1 & IRQ_FLAGS_1_PLL_LOCK)           Serial.print("PLLLock ");
    if (irqFlags1 & IRQ_FLAGS_1_RSSI)               Serial.print("RSSI ");
    if (irqFlags1 & IRQ_FLAGS_1_TIMEOUT)            Serial.print("Timeout ");
    if (irqFlags1 & IRQ_FLAGS_1_AUTO_MODE)          Serial.print("AutoMode ");
    if (irqFlags1 & IRQ_FLAGS_1_SYNC_ADDRESS_MATCH) Serial.print("SyncAddressMatch ");

    if (irqFlags2 & IRQ_FLAGS_2_FIFO_FULL)          Serial.print("FIFOFull ");
    if (irqFlags2 & IRQ_FLAGS_2_FIFO_NOT_EMPTY)     Serial.print("FIFONotEmpty ");
    if (irqFlags2 & IRQ_FLAGS_2_FIFO_LEVEL)         Serial.print("FIFOLevel ");
    if (irqFlags2 & IRQ_FLAGS_2_FIFO_OVERRUN)       Serial.print("FIFOOverrun ");
    if (irqFlags2 & IRQ_FLAGS_2_PACKET_SENT)        Serial.print("PacketSent ");
    if (irqFlags2 & IRQ_FLAGS_2_PAYLOAD_READY)      Serial.print("PayloadReady ");
    if (irqFlags2 & IRQ_FLAGS_2_CRC_OK)             Serial.print("CRC OK ");
}

int8_t currentRSSI;
unsigned long lastMessageTimestamp; 
uint32_t goodCrcCount, badCrcCount;

void loop() {
    static uint8_t counter;

    ///////////////
    noInterrupts();
    ///////////////

    if (msgReceived) {
        
        Serial.print("[CH");
        if (msgHopIndex < 10) {
            Serial.print("0");
        }
        Serial.print(msgHopIndex);
        Serial.print("] ");
        for (uint8_t i = 0; i < msgLen; i++) {
            SerialPrintHex(msg[i]);
        }
        Serial.print(" (RSSI: ");
        Serial.print(msgRSSI);
        
        if (validateCrc((uint8_t *)msg, msgLen)) {
            Serial.println(") CRC OK");
            goodCrcCount++;
        } else {
            Serial.println(") CRC BAD");
            badCrcCount++;
        }

        msgReceived = false;
        writeOffset = 0;
        writeReg(RFM69_REG01_OP_MODE, RFM69_OP_MODE_RX);
        digitalWrite(LED_BUILTIN, LOW);
    }
    
    // If we're not currently receiving a packet...
    if (!writeOffset) {
        currentRSSI = readRSSI(true);
        if (currentRSSI < -85) {
            hop();
        } 
    }

    /////////////
    interrupts();
    /////////////

    if (counter == 0) {
          
        ///////////////
        noInterrupts();
        ///////////////  
        
        dwellCount++;
          
        //Hop if we're stuck on a channel (noise), but not receiving messages
        if (dwellCount == 5) {
            Serial.println("stuck on channel, force reset");
            writeOffset = 0;
            writeReg(RFM69_REG01_OP_MODE, RFM69_OP_MODE_STANDBY);
            hop();
        }
        
        /////////////
        interrupts();
        /////////////

        if (Serial.available()) {
            int key = Serial.read();
            if (key == '?') {
                Serial.print("\rRSSI: ");
                Serial.print(currentRSSI);
                Serial.print(", CH: ");
                if (hopIndex < 10) {
                    Serial.print("0");
                }
                Serial.print(hopIndex);
                Serial.print(", writeOffset = ");
                Serial.print(writeOffset);
                Serial.print(", goodCrcCount = ");
                Serial.print(goodCrcCount);
                Serial.print(", badCrcCount = ");
                Serial.print(badCrcCount);
                Serial.print(" ");
                dumpIrqFlags();
                Serial.println();
            } else if (key == 'r') {
                Serial.print("Entering RX Mode...");
                writeReg(RFM69_REG01_OP_MODE, RFM69_OP_MODE_RX);
                Serial.println("Done.");
            } else if (key == 's') {
                Serial.print("Entering Standby Mode...");
                writeReg(RFM69_REG01_OP_MODE, RFM69_OP_MODE_STANDBY);
                Serial.println("Done.");
            } else if (key == 'x') {
                Serial.print("Resetting writeOffset...");
                writeOffset = 0;
                Serial.println("Done.");
            }
        }
    }

    counter++;
    delayMicroseconds(200);
}

// command map
// #define R_REGISTER // Read command and status registers. AAAAA = 5 bit Register Map Address
/* Write command and status registers. 
Executable in power down or standby modes only.
*/
#define W_REGISTER 0x20 

/* Read RX-payload: 1 – 32 bytes. A read operation
always starts at byte 0. Payload is deleted from
FIFO after it is read.
*/
#define R_RX_PAYLOAD 0x61 

/* Write TX-payload: 1 – 32 bytes. A write operation
always starts at byte 0 used in TX payload.
*/
#define W_TX_PAYLOAD 0xA0

/* Flush TX FIFO, used in TX mode */
#define FLUSH_TX 0xE1

/* Flush RX FIFO, used in RX mode */
#define FLUSH_RX 0xE2

/* used for reading the STATUS register */
#define NOP 0xFF

// register map
#define CONFIG 0 // configuration register
/* Enable Auto Acknowledgment function. Disable 
this functionality to be compatible with nRF2401*/
#define EN_AA 1 
#define EN_RXADDR 2 //Enabled RX Addresses
#define SETUP_AW 3 // Setup of Address Widths (common for all data pipes)
#define SETUP_RETR 4 // Setup of Automatic Retransmission
#define RF_CH 5 // RF Channel. RX and TX must be on the same frequency channel to communicate
#define RF_SETUP 6 // RF Setup Register
#define STATUS 7
#define OBSERVE_TX 8 // Transmit observe register
#define RPD 9 // Reserved power detector
#define RX_ADDR_P0 10
#define RX_ADDR_P1 11
#define RX_ADDR_P2 12
#define RX_ADDR_P3 13
#define RX_ADDR_P4 14
#define RX_ADDR_P5 15
#define TX_ADDR 16
#define RX_PW_P0 17
#define RX_PW_P1 18
#define RX_PW_P2 19
#define RX_PW_P3 20
#define RX_PW_P4 21
#define RX_PW_P5 22
#define FIFO_STATUS 23

// library specific globals
#define RX_DR 6
#define TX_DS 5
#define MAX_RT 4

#define MIN_PWR 0
#define LOW_PWR 2
#define HIGH_PWR 4
#define MAX_PWR 6
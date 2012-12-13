#include "nrf24_map.h"
#include "nrf24.h"

#define NRF_PORT   PORTA
#define NRF_PORT_PIN PINA

#define NRF_SCK  4 //Output
#define NRF_MISO 5 //Input
#define NRF_MOSI 6 //Output

#define NRF_CE 1 //Output
#define NRF_CSN 2 //Output

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

// reads RX data into array and forces a flush on everything
void receive_data(uint8_t * data, uint8_t pipe)
{
  cbi(PORTB, NRF_CSN); //Stand by mode

  tx_spi_byte(R_RX_PAYLOAD | pipe); //Read RX Payload

  for (int i = 0; i < sizeof(data); i++) {
    data[i] = tx_spi_byte(NOP);
  }

  sbi(PORTB, NRF_CSN);
  
  //Flush and reset everything
  tx_send_byte(FLUSH_RX);
  tx_send_command(W_REGISTER | STATUS, sbi(RX_DR) | sbi(TX_DS) | sbi(MAX_RT));

  // enable again
  sbi(PORTB, NRF_CE);
}

// sets rx address pipes
void set_rx_addr(uint8_t pipe, uint8_t address)
{
  cbi(PORTB, NRF_CE); //Go into standby mode
  tx_send_payload(W_REGISTER | RX_ADDR_P0 | pipe, sizeof(address), address);
  sbi(PORTB, NRF_CE);
}

// configures the receiver
void config_rx(uint8_t * address, uint8_t rf_channel, 
  uint8_t power_level, uint8_t recieve_payload_size)
{
  cbi(PORTB, NRF_CE); //Go into standby mode

  //Enable RX IRQ, CRC Enabled, be a receiver
  tx_send_command(W_REGISTER, 0x39);

  //Disable auto-acknowledge
  tx_send_command(W_REGISTER | EN_AA, 0x00);

  //Set address width to 5bytes (default, not really needed)
  tx_send_command(W_REGISTER | SETUP_AW, sizeof(address));

  //RF Channel
  tx_send_command(W_REGISTER | RF_CH, rf_channel);

  //Air data rate 1Mbit, 0dBm, Setup LNA
  tx_send_command(W_REGISTER | RF_SETUP, power_level);

  //Set RX pipe 0 address
  tx_send_payload(W_REGISTER | RX_ADDR_P0, sizeof(address), address);

  // receive payload setup
  tx_send_command(W_REGISTER | RX_PW_P0, recieve_payload_size);

  //RX interrupt, power up, be a receiver
  tx_send_command(W_REGISTER, 0x3B);

  sbi(PORTB, NRF_CE);
} 

// transmits a data array
void transmit_data(uint8_t * data)
{
  tx_send_command(W_REGISTER | STATUS, sbi(RX_DR) | sbi(TX_DS) | sbi(MAX_RT));
  
  tx_send_byte(FLUSH_TX); //Clear TX Fifo
  
  tx_send_payload(W_TX_PAYLOAD, sizeof(data), data);

  sbi(PORTB, NRF_CE); //Pulse CE to start transmission
  delay_ms(3);
  cbi(PORTB, NRF_CE);
}

// configures a transmitter
uint8_t config_tx(uint8_t * address, uint8_t rf_channel, uint8_t power_level)
{
  cbi(PORTB, NRF_CE);
  
  //CRC enabled, be a transmitter
  tx_send_command(W_REGISTER, 0x78);

  //Disable auto acknowledge on all pipes
  tx_send_command(W_REGISTER | EN_AA, 0x00);

  //Set address width
  tx_send_command(W_REGISTER | SETUP_AW, sizeof(address));

  //Disable auto-retransmit
  tx_send_command(W_REGISTER | SETUP_RETR, 0x00);

  // set the RF channel
  tx_send_command(W_REGISTER | RF_CH, rf_channel);

  //Air data rate 1Mbit, 0dBm, Setup LNA
  tx_send_command(W_REGISTER | RF_SETUP, power_level);

  //Set TX address
  tx_send_payload(W_REGISTER | TX_ADDR, sizeof(address), address);
  
  //Power up, be a transmitter
  tx_send_command(W_REGISTER, 0x7A);

  sbi(PORTB, NRF_CE);

  return(tx_send_byte(NOP));
}

//Sends x bytes of payload
void tx_send_payload(uint8_t cmd, uint8_t size, uint8_t * data)
{
  uint8_t i;

  cbi(PORTB, NRF_CSN); //Select chip
  tx_spi_byte(cmd);
  
  for(i = 0 ; i < size ; i++)
    tx_spi_byte(data[i]);

  sbi(PORTB, NRF_CSN); //Deselect chip
}


//Sends command to nRF
uint8_t tx_send_command(uint8_t cmd, uint8_t data)
{
  uint8_t status;

  cbi(PORTB, NRF_CSN); //Select chip
  tx_spi_byte(cmd);
  status = tx_spi_byte(data);
  sbi(PORTB, NRF_CSN); //Deselect chip

  return(status);
}

//Sends one byte to nRF
uint8_t tx_send_byte(uint8_t cmd)
{
  uint8_t status;
  
  cbi(PORTB, NRF_CSN); //Select chip
  status = tx_spi_byte(cmd);
  sbi(PORTB, NRF_CSN); //Deselect chip
  
  return(status);
}

//Basic SPI to nRF
uint8_t tx_spi_byte(uint8_t outgoing)
{
  uint8_t i, incoming;
  incoming = 0;

  //Send outgoing byte
  for(i = 0 ; i < 8 ; i++)
  {
    if(outgoing & 0b10000000)
      sbi(NRF_PORT, NRF_MOSI);
    else
      cbi(NRF_PORT, NRF_MOSI);
    
    sbi(NRF_PORT, NRF_SCK);
    _delay_us(120);

    //MISO bit is valid after clock goes going high
    incoming <<= 1;
    if( NRF_PORT_PIN & (1<<NRF_MISO) ) incoming |= 0x01;

    cbi(NRF_PORT, NRF_SCK);
    _delay_us(120);
    
    outgoing <<= 1;
  }

  return(incoming);
}
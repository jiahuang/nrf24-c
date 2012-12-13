// tx functions
uint8_t config_tx(uint8_t * address, uint8_t rf_channel, uint8_t power_level);
bool transmit_data(uint8_t * data);

// rx functions
uint8_t config_rx(uint8_t * address, uint8_t rf_channel, uint8_t power_level);
void set_rx_addr(uint8_t pipe, uint8_t address);
void receive_data(uint8_t * data, uint8_t pipe);

// helper functions by sparkfun https://www.sparkfun.com/products/8602
//Sends command to nRF
uint8_t nrf_send_byte(uint8_t cmd);
//Basic SPI to nRF
uint8_t nrf_send_command(uint8_t cmd, uint8_t data);
//Sends the 4 bytes of payload
void nrf_send_payload(uint8_t cmd, uint8_t size, uint8_t * data);
//Basic SPI to nRF
uint8_t nrf_spi_byte(uint8_t outgoing);
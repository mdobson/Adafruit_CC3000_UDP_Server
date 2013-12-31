#ifndef ADAFRUIT_CC3000_UDP_SERVER_H
#define ADAFRUIT_CC3000_UDP_SERVER_H

#include "Adafruit_CC3000.h"
#include "Server.h"
#include "utility/socket.h"

#define RXBUFFERSIZE 512 

class Adafruit_CC3000_UDP_Server : public Server {
  public:
    Adafruit_CC3000_UDP_Server(uint16_t port);
    
    size_t write(uint8_t c);

    int16_t write(const void *buf, uint16_t len, uint32_t flags = 0);

    int16_t write(const void *buf, uint16_t len, sockaddr *address);

    int16_t read(void *buf, uint16_t len, uint32_t flags = 0);

    int16_t read(void *buf, uint16_t len, sockaddr *address);

    uint8_t read(void);

    int32_t close(void);

    uint8_t available(void);
    
    void begin(void);

    uint8_t _rx_buf[RXBUFFERSIZE], _rx_buf_idx;
    int16_t bufsiz;

  private:
    int16_t _socket;
    uint16_t _port;
    
};

#endif

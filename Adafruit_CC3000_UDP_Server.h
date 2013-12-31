#ifndef ADAFRUIT_CC3000_UDP_SERVER_H
#define ADAFRUIT_CC3000_UDP_SERVER_H

#include "Adafruit_CC3000.h"
#include "Server.h"
#include "utility/socket.h"

//Internal buffer size.
#define RXBUFFERSIZE 512 

class Adafruit_CC3000_UDP_Server : public Server {
  public:
    /****************************************
     * Constructor
     * Params:
     *  uint16_t: port number for server to listen on.
    ****************************************/
    Adafruit_CC3000_UDP_Server(uint16_t port);
    
    /****************************************
     * write
     * Params:
     *  uint8_t c: character to write on socket
     * Returns:
     *  size_t: The size of the write result
    ****************************************/
    size_t write(uint8_t c);

    /****************************************
     * write
     * Params:
     *  const void *buf: Data for writing on the socket
     *  uint16_t len: Length of data needing to be written
     *  uint32_t flags: Flags for writing on the socket
     * Returns:
     *  int16_t: Size of the write result.
     * Notes:
     *  Uses the send syscall to write data on the socket.
    ****************************************/
    int16_t write(const void *buf, uint16_t len, uint32_t flags = 0);

    /****************************************
     * write
     * Params:
     *  const void *buf: Data for writing on the socket
     *  uint16_t len: Length of data needing to be written
     *  sockaddr *address: Address of UDP socket to write to
     * Returns:
     *  int16_t: Size of the write result.
     * Notes:
     *  Uses the sendto syscall to send data to a specific address
    ****************************************/
    int16_t write(const void *buf, uint16_t len, sockaddr *address);

    /****************************************
     * read
     * Params:
     *  const void *buf: Reference to buffer that data will be placed in.
     *  uint16_t len: Amount of data to read from
     *  uint32_t flags: Flags to read
     * Returns:
     *  int16_t: Size of the read result
     * Notes:
     *  Uses the recv syscall. No address data returned.
    ****************************************/
    int16_t read(void *buf, uint16_t len, uint32_t flags = 0);

    /****************************************
     * read
     * Params:
     *  const void *buf: Reference to buffer that data will be placed in.
     *  uint16_t len: Amount of data to read from
     *  sockaddr *address: Rederence to sockaddr struct that will hold sender info.
     * Returns:
     *  int16_t: Size of the read result
     * Notes:
     *  Uses the recvfrom syscall. No address data returned.
    ****************************************/
    int16_t read(void *buf, uint16_t len, sockaddr *address);

    /****************************************
     * read
     * Params:
     *  None
     * Returns:
     *  uint8_t: Single byte from internal buffer
     * Notes:
     *  Read single byte from server buffer
    ****************************************/
    uint8_t read(void);

    /****************************************
     * close
     * Params:
     *  None
     * Returns:
     *  int32_t: Result of closesocket call
     * Notes:
     *  Closes socket on server
    ****************************************/
    int32_t close(void);

    /****************************************
     * available
     * Params:
     *  None
     * Returns:
     *  uint8_t: Size of data received from socket.
    ****************************************/
    uint8_t available(void);
    
    /****************************************
     * begin
     * Params:
     *  None
     * Returns:
     *  None
     * Notes:
     *  This sets up the listening socket.
    ****************************************/
    void begin(void);

    /****************************************
     * _rx_buf
     *  Buffer of received data
     * _rx_buf_idx
     *  Current index of received data buffer
    ****************************************/
    uint8_t _rx_buf[RXBUFFERSIZE], _rx_buf_idx;

    /****************************************
     * bufsiz
     *  Current amound of data stored in internal buffer
    ****************************************/
    int16_t bufsiz;

  private:
    /****************************************
     * Reference to setup UDP Socket
    ****************************************/
    int16_t _socket;

    /****************************************
     * Port that server is listening on
    ****************************************/
    uint16_t _port;
    
};

#endif

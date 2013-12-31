#include "Adafruit_CC3000_UDP_Server.h"
#include "Adafruit_CC3000.h"
#include "utility/socket.h"

#define CC3K_PRINTLN_F(text) { if(CC3KPrinter != NULL) { CC3KPrinter->println(F(text)); }}

Adafruit_CC3000_UDP_Server::Adafruit_CC3000_UDP_Server(uint16_t port) : _port(port) {
  bufsiz = 0;
  _rx_buf_idx = 0;
};

size_t Adafruit_CC3000_UDP_Server::write(uint8_t c) {
  int32_t r;
  r = send(_socket, &c, 1, 0);
  if(r < 0) return 0;
  return r;
};

int16_t Adafruit_CC3000_UDP_Server::write(const void *buf, uint16_t len, uint32_t flags) {
  return send(_socket, buf, len, flags);
};

int16_t Adafruit_CC3000_UDP_Server::write(const void *buf, uint16_t len, sockaddr *address) {
  socklen_t slen = sizeof(sockaddr);
  int16_t ret = sendto(_socket, buf, len, 0, address, slen);
  return ret;
};

int16_t Adafruit_CC3000_UDP_Server::read(void *buf, uint16_t len, uint32_t flags ) {
  return recv(_socket, buf, len, flags);
};

int16_t Adafruit_CC3000_UDP_Server::read(void *buf, uint16_t len, sockaddr *address) {
  socklen_t slen = sizeof(sockaddr);
  int16_t ret = recvfrom(_socket, buf, len, 0, address, &slen);
  return ret;
};

uint8_t Adafruit_CC3000_UDP_Server::read(void) {
  while((bufsiz <= 0) || (bufsiz == _rx_buf_idx)) {
    cc3k_int_poll();
    
    bufsiz = recv(_socket, _rx_buf, sizeof(_rx_buf), 0);
    if(bufsiz == -57) {
      close();
      return 0;
    }

    _rx_buf_idx = 0;
  }
  uint8_t ret = _rx_buf[_rx_buf_idx];
  _rx_buf_idx++;
  return ret;
};

int32_t Adafruit_CC3000_UDP_Server::close(void) {
  int32_t x = closesocket(_socket);
  _socket = -1;
  return x;
};

uint8_t Adafruit_CC3000_UDP_Server::available(void) {

  if (_socket < 0) return 0;
  if ((bufsiz > 0) && (_rx_buf_idx < bufsiz)) {
    return (bufsiz - _rx_buf_idx);
  }

  timeval timeout;
  fd_set fd_read;

  memset(&fd_read, 0, sizeof(fd_read));
  FD_SET(_socket, &fd_read);

  timeout.tv_sec = 0;
  timeout.tv_usec = 5000;

  int16_t s = select(_socket+1, &fd_read, NULL, NULL, &timeout);
  if (s == 1) {
    return 1;
  } 
  else {
    return 0;
  }
};

void Adafruit_CC3000_UDP_Server::begin(void) {

  unsigned long aucDHCP = 14400;
  unsigned long aucARP = 3600;
  unsigned long aucKeepalive = 30;
  unsigned long aucInactivity = 0;

  cc3k_int_poll();
  if (netapp_timeout_values(&aucDHCP, &aucARP, &aucKeepalive, &aucInactivity) != 0 ) {
    CC3K_PRINTLN_F("Error setting inactivity timeouts");
    return;
  }

  cc3k_int_poll();
  int16_t sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    CC3K_PRINTLN_F("Couldn't create listening socket");
    return;
  }

  cc3k_int_poll();
  char arg = SOCK_ON;
  if( setsockopt(sock, SOL_SOCKET, SOCKOPT_ACCEPT_NONBLOCK, &arg, sizeof(arg)) < 0 ) {
    CC3K_PRINTLN_F("Couldn't set socket as non-blocking");
    return;
  }

  sockaddr_in home;
  home.sin_family = AF_INET;
  home.sin_addr.s_addr = htonl(0);
  home.sin_port = htons(_port);

  cc3k_int_poll();
  if( bind(sock, (sockaddr *)&home, sizeof(home)) < 0) {
    CC3K_PRINTLN_F("Error binding listening socket to address");
    return;
  }

  _socket = sock;
};


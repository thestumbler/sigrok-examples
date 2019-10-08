#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "libserialport.h"
 
#define DEFBAUD 115200
#define MAXPORTNAME (128)
char portname[MAXPORTNAME];

int main(int argc, char *argv[]) {
  struct sp_port *port;
  int err;
  int baud=DEFBAUD;

// We need a serial port name
  if (argc<2) {
    printf("Usage:\n");
    printf("  rdpoem serial-port [baud:def=115200]\n");
    exit(-901);
  }

  int iarg=1;
  strncpy( portname, argv[iarg], MAXPORTNAME );

// optional baudrate
  iarg++;
  if (argc>iarg) {
    errno=0;
    baud = (int)strtol( argv[iarg], NULL, 10 );
    if(errno) {
      fprintf(stderr,"error reading baudrate: %s\n", argv[iarg] );
      exit(-902);
    }
  }

  printf("Requested serial port %s at %d baud\n", portname, baud );
 
// Open serial port
  err=sp_get_port_by_name(portname, &port );
  if (err!=SP_OK) {
    fprintf(stderr,"Can't locate port name %s\n", portname );
    exit(err);
  }
  err=sp_open(port,SP_MODE_READ_WRITE);
  if (err!=SP_OK) {
    fprintf(stderr,"Can't open port %s\n", portname );
    exit(err);
  }
// set Baud rate
  sp_set_baudrate(port,baud);
  printf("Serial port %s opened at %d baud\n", portname, baud );

// flush any queued data
  sp_flush( port, SP_BUF_BOTH );

// max timeout is one minute,
// normal timeout is one second
#define MAX_TIMEOUT (60000)
#define DEF_TIMEOUT (1000)

// read data 
  int nrb=0;
  int count=0;
  uint16_t cksum=0;
  int timeout = MAX_TIMEOUT;
  int ch=0;
  while(1) {
    ch=0;
    nrb = sp_blocking_read( port, &ch, 1, timeout );
    if(nrb==0) break;
    cksum += (uint8_t)ch; // checksum
    count++;
    putchar(ch);
    timeout=DEF_TIMEOUT;
  }
  sp_close(port);

  printf("Received summary:\n");
  printf("Recd: %d bytes\n", count);
  printf("Checksum is 0x%04x\n", cksum);

  return 0;
}

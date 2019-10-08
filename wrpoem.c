#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "libserialport.h"
 
#define MAXPORTNAME (128)
char portname[MAXPORTNAME];

// Random text, a poem entitled "Azaleas" 
// by a famous poet here in South Korea, Kim Sowol
// https://en.wikipedia.org/wiki/Kim_Sowol
char *lines[] = {
  "*** Azaleas ***\r\n",
  "The day will come when you loathe me and leave me;\r\n",
  "Goodbye, that's all, it's over.\r\n",
  "I'll have them strew your road with azaleas from Yaksan in Yongbyon.\r\n",
  "Then be off with you marching briskly over those withered petals.\r\n",
  "The day will come when you loathe me and leave me;\r\n",
  "You think I'll cry? Not on your life. I won't!\r\n",
  "-- Kim Sowol, 1925\r\n",
  NULL
};

int main(int argc, char *argv[]) {
  int i;
  struct sp_port *port;
  int err;

// how long is the poem?
  int nlines = (sizeof( lines ) / sizeof( char *)) -1;

// print help and exit if no port specified
  if (argc<2) {
    printf("Usage:\n");
    printf("  wrpoem serial-port [baud:def=115200] [numrepeats:def=5]\n");
    exit(-901);
  }

// get port name
  int iarg=1;
  strncpy( portname, argv[iarg], MAXPORTNAME );

// optional baudrate
#define DEFBAUD (115200)
  int baud=DEFBAUD;
  iarg++;
  if (argc>iarg) {
    errno=0;
    baud = (int)strtol( argv[iarg], NULL, 10 );
    if(errno) {
      fprintf(stderr,"error reading baudrate: %s\n", argv[iarg] );
      exit(-902);
    }
  }

// optional number of repeats
#define DEFLOOPS (3)
  int nloops=DEFLOOPS;
  iarg++;
  if (argc>iarg) {
    errno=0;
    nloops = (int)strtol( argv[iarg], NULL, 10 );
    if(errno) {
      fprintf(stderr,"error reading number of repeats: %s\n", argv[iarg] );
      exit(-903);
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

// send the text
  uint16_t cksum=0;
  int count=0;
  int xlines=0;
  int timeout = 500; 
  int udelay = 500*1000; // 1/4 second delay between lines
  int size;
  printf("Sending the text %d times...\n", nloops );
  for(int loop=0;loop<nloops;loop++) {
    for(int iline=0;iline<nlines; iline++) {
      size = strlen(lines[iline]);
      sp_blocking_write( port, (void *)lines[iline], size, timeout );
      for(i=0;i<size; i++) cksum += lines[iline][i]; // checksum
      count+=size; // total bytes tally
      xlines++; // sent line counter
      printf("Loop %d, line %d: %s", loop, iline, lines[iline]);
      usleep(udelay);
    }
  }
  sp_close(port);

  printf("Sent %d lines\n", xlines );
  printf("Sent %d bytes\n", count );
  printf("Checksum is 0x%04x\n", cksum );
  return 0;
}


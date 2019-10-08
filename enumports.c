#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h> // for sleep function

#include <libserialport.h> // cross platform serial port lib

struct sp_port *port;

void main_list_ports() {
  int i;
  struct sp_port **ports;

  enum sp_return error = sp_list_ports(&ports);
  if (error == SP_OK) {
    for (i = 0; ports[i]; i++) {
      printf("Found port: '%s'\n", sp_get_port_name(ports[i]));
    }
    sp_free_port_list(ports);
  } else {
    printf("No serial devices detected\n");
  }
  printf("\n");
}

int main() {
  main_list_ports();
  return 0;
}  

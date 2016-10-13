#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int ac, char* av[]) {
  int socket_handle;
  struct sockaddr_in socket_details;
  char* input_buffer;
  char httpget[BUFSIZ];
  int buffer_size = 2000000;

  strcpy(httpget, "GET ");
  strcat(httpget, av[2]);
  strcat(httpget, " HTTP/1.0\r\n\r\n");
  printf("httpget: %s\n", httpget);

  struct addrinfo *result;
  getaddrinfo(av[1], NULL, NULL, &result);
  struct in_addr ip = ((struct sockaddr_in *)(result->ai_addr))->sin_addr;

  input_buffer = malloc (buffer_size);

  socket_handle = socket (AF_INET, SOCK_STREAM, 0);
  socket_details.sin_family = AF_INET;
  socket_details.sin_addr.s_addr = inet_addr (inet_ntoa(ip));
  socket_details.sin_port = htons(80);

  int cresult = connect (socket_handle,(struct sockaddr*)&socket_details, sizeof ( struct sockaddr));
  if (cresult != 0) {
    fprintf(stderr, "Error: connect\n");
    exit(1);
  }
  int nbytes = strlen(httpget);
  int sresult = send (socket_handle , httpget, nbytes, 0 );
  if (nbytes != sresult) {
    fprintf(stderr, "Error: send\n");
    exit(1);
  }
  int total_receive = 0;
  int n = recv (socket_handle , input_buffer , buffer_size, 0 );
  while(n > 0) {
    total_receive += n;
    printf("received data : %d\n", total_receive);
    n = recv (socket_handle , input_buffer + total_receive , buffer_size - total_receive, 0 );
  }
  input_buffer[n] = total_receive;
  printf ("input: %s\n", input_buffer );
  close(socket_handle);

  return 0 ;
}

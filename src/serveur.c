#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "chatbot.h"

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char **argv)
{
  /********************/
  char line[LINELENGTH];
  char *word;
  printf("$ this is the tcp-ip-chatbot\n");

  hashtable_t *hashtable = ht_create(65536);
  ht_set(hashtable, "hi", "hello");
  ht_set(hashtable, "hey", "hello");
  ht_set(hashtable, "hear", "What you heard is right");
  ht_set(hashtable, "python", "Yo, I love Python");
  ht_set(hashtable, "light", "I like light");
  ht_set(hashtable, "What", "It is clear, ain't it?");
  /*********************************/
  int parentfd;                  /* parent socket */
  int childfd;                   /* child socket */
  int portno;                    /* port to listen on */
  int clientlen;                 /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp;         /* client host info */
  char buf[BUFSIZE];             /* message buffer */
  char *hostaddrp;               /* dotted decimal host addr string */
  int optval;                    /* flag value for setsockopt */
  int n;                         /* message byte size */

  portno = 3250;

  /* 
   * socket: create the parent socket 
   */
  parentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parentfd < 0)
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
             (const void *)&optval, sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *)&serveraddr, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(parentfd, (struct sockaddr *)&serveraddr,
           sizeof(serveraddr)) < 0)
    error("ERROR on binding");

  /* 
   * listen: make this socket ready to accept connection requests 
   */
  if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */
    error("ERROR on listen");

  /* 
   * main loop: wait for a connection request, echo input line, 
   * then close connection.
   */
  clientlen = sizeof(clientaddr);
  while (1)
  {

    /* 
     * accept: wait for a connection request 
     */
    childfd = accept(parentfd, (struct sockaddr *)&clientaddr, &clientlen);
    if (childfd < 0)
      error("ERROR on accept");

    /* 
     * gethostbyaddr: determine who sent the message 
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                          sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server established connection with %s (%s)\n",
           hostp->h_name, hostaddrp);
    while (1)
    {
      /* 
     * read: read input string from the client
     */
      bzero(buf, BUFSIZE);
      n = read(childfd, buf, BUFSIZE);
      if (n < 0)
        error("ERROR reading from socket");

      if (n > 0)
        printf("server received from client %d bytes: %s", n, buf);

      /* 
     * write: echo the input string back to the client 
     */
      if (strlen(buf) <= 1)
        break;                      /*exit program*/
      word = strtok(buf, SEPCHARS); /*Find first word */
      while (word != NULL)
      {
        if (strncasecmp(word, "exit", 150) == 0)
        {
          exit(0);
        }
        // Some responses based on the keywords
        if (ht_get(hashtable, word) != NULL)
        {
          n = write(childfd, ht_get(hashtable, word), strlen(ht_get(hashtable, word)));
          if (n < 0)
        error("ERROR writing to socket");
        }
        else 
        {
          n = write(childfd, "Sorry, I don't know what to say about that", strlen("Sorry, I don't know what to say about that"));
          if (n < 0)
        error("ERROR writing to socket");
        }
        word = strtok(NULL, SEPCHARS);
      }
    }
    close(childfd);
  }
}

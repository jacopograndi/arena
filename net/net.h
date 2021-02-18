#ifndef NET_H
#define NET_H

#include <plibsys.h>

#define SERVER_PORT 9999

void itob (char *buf, int n);
int btoi (char *buf);

void net_init ();

typedef struct {
    PSocket *socket;
    PSocketAddress * addr_server;
} net_client;

void net_client_open(net_client *c);
int net_client_connect (net_client *c, char ip[], int port);
int net_client_recv(net_client *c, char buffer[]);
void net_client_send (net_client *c, char data[], int sizeofdata);
void net_client_close (net_client *c);

typedef struct {
    PSocket *socket;
    PSocket *sock_client;
} net_server;

void net_server_open(net_server *s, int port);
void net_server_accept(net_server *s);
int net_server_recv(net_server *s, char buffer[]);
void net_server_send (net_server *s, char data[], int sizeofdata);
void net_server_close (net_server *s);

#endif
#include <net.h>

void itob (char *buf, int n) {
    buf[0] = (n >> 24) & 0xFF;
    buf[1] = (n >> 16) & 0xFF;
    buf[2] = (n >> 8) & 0xFF;
    buf[3] = n & 0xFF;
}

int btoi (char *buf) {
    int n = 0;
    n += buf[0]*256*256*256;
    n += buf[1]*256*256;
    n += buf[2]*256;
    n += buf[3];
    return n;
}


void net_init () {
    p_libsys_init ();
}

int net_open_socket(PSocket **socket, char ip[], int port) {
    *socket = p_socket_new (P_SOCKET_FAMILY_INET,
        P_SOCKET_TYPE_STREAM,
        P_SOCKET_PROTOCOL_TCP,
        NULL);
    p_socket_set_blocking (*socket, FALSE);
    p_socket_set_timeout (*socket, 10);
    PSocketAddress *sock_addr = p_socket_address_new(ip, port);
    int sender_port = 0;
    if (p_socket_bind (*socket, sock_addr, FALSE, NULL) == FALSE) {
        p_socket_free (*socket);
        p_socket_address_free (sock_addr);
        return 1;
    } else {
        p_socket_address_free (sock_addr);
        PSocketAddress *local_addr = p_socket_get_local_address(*socket, NULL);
        if (local_addr == NULL) {
            p_socket_free (*socket);
            p_uthread_exit (-1);
            return 2;
        }
        sender_port = p_socket_address_get_port (local_addr);
        p_socket_address_free (local_addr);
    }
    return 0;
}

int net_recv (PSocket *socket, char buffer[]) {
    int len = 0;
    for (int i=0; ; i++) {
        psize recv_now = p_socket_receive (socket, 
            buffer+i*1024, 1024, NULL);
        if (recv_now == -1) {
            if (len > 0) { 
                printf("received (%d)\n", len);
                return len; 
            }
            printf("error recv\n");
            return -1;
        } else {
            len += recv_now;
            if (recv_now < 1024) { 
                printf("received (%d)\n", len);
                return len; 
            }
        }
    }
    printf("received (%d)\n", len);
    return len;
}


void net_client_open(net_client *c) {
    int err = net_open_socket(&c->socket, "127.0.0.1", 0);
    if (err > 0) printf("error opening client socket: %d\n", err);
}

int net_client_connect (net_client *c, char ip[], int port) {
    c->addr_server = p_socket_address_new(ip, port);
    if (c->addr_server == NULL) printf("error client making server addr\n");
    int is_connected = p_socket_connect(c->socket, c->addr_server, NULL);
    if (is_connected == FALSE) {
        if (p_socket_io_condition_wait 
            (c->socket, P_SOCKET_IO_CONDITION_POLLOUT, NULL) == TRUE &&
            p_socket_check_connect_result (c->socket, NULL) == FALSE)
        {
            p_socket_address_free (c->addr_server);
            p_socket_free (c->socket);
            printf("error client socket freed\n");
            return 1;
        }
    }

    is_connected = p_socket_is_connected (c->socket);
    if (is_connected == FALSE) {
        printf("error client can't connect to %s:%d\n", ip, port);
        return 1;
    }
    return 0;
}

void net_client_send (net_client *c, char data[], int sizeofdata) {
    if(p_socket_is_connected (c->socket) == FALSE) { 
        printf("error client send lost connection\n");
    }
    PError *err;
    int send_now = p_socket_send (c->socket, data, sizeofdata, &err);
    if (send_now == -1) printf("error: %d\n", p_error_get_code(err));
}

int net_client_recv (net_client *c, char buffer[]) {
    net_recv(c->socket, buffer);
}

void net_client_close (net_client *c) {
    if (c->socket != NULL) p_socket_close (c->socket, NULL);
    p_socket_address_free (c->addr_server);
	p_socket_free (c->socket);
}



void net_server_open(net_server *s, char ip[], int port) {
    int err = net_open_socket(&s->socket, ip, port);
    if (err > 0) printf("error opening server socket: %d\n", err);
    if (p_socket_listen (s->socket, NULL) == FALSE) {
        printf("error server socket listen, nobody in.\n");
        return;
    }
}

void net_server_accept (net_server *s) {
    pboolean poll = p_socket_io_condition_wait (s->socket, 
        P_SOCKET_IO_CONDITION_POLLIN, NULL);
    if (poll == FALSE) {
        printf("server socket, no one in queue\n");
        return;
    }
    s->sock_client = p_socket_accept (s->socket, NULL);
    if (s->sock_client == NULL) {
        printf("server socket, no accept\n");
    }
    p_socket_set_blocking (s->sock_client, FALSE);
    printf("connected\n");
}

int net_server_recv (net_server *s, char buffer[]) {
    net_recv(s->sock_client, buffer);
}

void net_server_send (net_server *s, char data[], int sizeofdata) {
    if(p_socket_is_connected (s->sock_client) == FALSE) { 
        printf("error client send lost connection\n");
        return;
    }
    PError *err;
    int send_now = p_socket_send (s->sock_client, data, sizeofdata, &err);
    if (send_now == -1) printf("error: %d\n", p_error_get_code(err));
}

void net_server_close (net_server *s) {
    if (s->socket != NULL) p_socket_close (s->socket, NULL);
    if (s->sock_client != NULL) p_socket_free (s->sock_client);
	if (s->socket != NULL) p_socket_free(s->socket);
}

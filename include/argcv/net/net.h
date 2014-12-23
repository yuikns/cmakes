// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef INCLUDE_ARGCV_NET_NET_H_
#define INCLUDE_ARGCV_NET_NET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/socket.h> // for socklen_t

#ifndef LISTENQ
#define LISTENQ 1024 // 
#endif // LISTENQ

typedef struct sockaddr SA;

// Make this header file easier to include in C++ code
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *
 * TCP 
 *
 ******************************************************************************/

/**
 * const char * host : host name  or IP
 * const char * serv : port or service 
 */
int tcp_connect(const char * host , const char * serv);


/**
 * const char * hostname : host name  or IP
 * const char * service : port or service
 * socklen_t * addrlenp : size of protocol address
 */
int tcp_listen(const char * hostname , const char * service , socklen_t * addrlenp);



/*******************************************************************************
 *
 * UDP 
 *
 ******************************************************************************/

int udp_client(const char * host, const char * serv,SA ** saptr,socklen_t * addrlenp); 
int udp_connect(const char * host, const char * serv);
int udp_server(const char * host,const char * serv,socklen_t *addrlenp);


/*******************************************************************************
 *
 * SCTP 
 *
 ******************************************************************************/






/*******************************************************************************
 *
 * TOOLS IN UNPV13E
 *
 ******************************************************************************/
char * sock_ntop_host(const struct sockaddr *sa, socklen_t salen); // check : HAVE_HEADER_NET_IF_DL_H


#ifdef __cplusplus
}
#endif


#endif  // INCLUDE_ARGCV_NET_NET_H_
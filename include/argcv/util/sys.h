// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef INCLUDE_ARGCV_UTIL_SYS_H_
#define INCLUDE_ARGCV_UTIL_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/socket.h> // for socklen_t

/*******************************************************************************
*
* SYSTEM 
*
******************************************************************************/
// defined in P. 105 , unix network programming
typedef void sigfunc(int);
sigfunc * _signal(int signo, sigfunc * func);
void sig_chld(int signo);


/*******************************************************************************
*
* DAEMON 
*
******************************************************************************/

//#ifndef MAXFD
//#define MAXFD 64
//#endif // MAXFD

//#ifndef PROJ_WORK_DIR
//#define PROJ_WORK_DIR "/"
//#endif //PROJ_WORK_DIR
extern int max_fd;
extern char * proj_work_dir;
extern int daemon_proc;  // defined in error.c ?

int daemon_init(const char * ident);
void daemon_stop();




#ifdef __cplusplus
}
#endif


#endif  // INCLUDE_ARGCV_UTIL_SYS_H_

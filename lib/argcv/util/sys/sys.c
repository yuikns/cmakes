#include "argcv/util/sys.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h> // setsid
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // malloc

#include <syslog.h> // syslog
#include <signal.h> // signal
#include <sys/stat.h> // open O_RDONL
#include <fcntl.h> // open

#include <sys/wait.h> // wait

#include <netdb.h>
#include <arpa/inet.h>


// default project work dir
char * proj_work_dir = "/";
int max_fd = 64;


/*******************************************************************************
 *
 * SYSTEM PART
 *
 ******************************************************************************/
sigfunc * _signal(int signo,sigfunc * func)
{
    struct sigaction act; // signal.h
    struct sigaction oact;
    printf("signal called  ...\n");

    act.sa_handler = func;
    sigemptyset(&act.sa_mask); // signal.h
    act.sa_flags = 0;

    if(signo == SIGALRM) //
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT; // SunOS 4.x
#endif //SA_INTERRUPT
    }else
    {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART; // SVR4, 4.4BSD
#endif //SA_RESTART
    }
    if(sigaction(signo,&act,&oact) < 0 )
        return SIG_ERR;
    return oact.sa_handler;
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while( (pid = waitpid(-1,&stat,WNOHANG)) > 0 )
    {
#ifdef ARGCVNW_IN_DEBUG
        if(daemon_proc)
        {
            syslog(LOG_DEBUG,"child %d terminated \n",pid);
        }else
        {
            fprintf(stdout,"child %d terminated \n",pid);
        }
#endif // ARGCVNW_IN_DEBUG
    }
    return;
}




/*******************************************************************************
 *
 * DAEMON PART
 *
 ******************************************************************************/
int daemon_proc = 0; // for err_XXX() functions

int daemon_init(const char * ident)
{
    openlog(ident,LOG_PID,LOG_USER);
    //openlog(ident,LOG_CONS|LOG_PID,LOG_USER);
    syslog(LOG_INFO,"daemon starting");
    int i;
    pid_t pid;

    if((pid = fork()) < 0 )
    {
        return -1;
    }
    else if(pid)
    {
        _exit(0); // parent terminates
    }

    // child 1 continues ...
    //
    if(setsid() < 0) // become session leader
    {
        return -1;
    }

    signal(SIGHUP,SIG_IGN);

    if((pid = fork()) < 0)
    {
        return -1;
    }
    else if(pid)
    {
        _exit(0);
    }
    // child 2 continues ...
    daemon_proc = 1; // for err_XXX() functions

    chdir(proj_work_dir);

    // close off file descriptors
    for(i = 0 ; i < max_fd ; i ++)
    {
        close(i);
    }

    open("/dev/null",O_RDONLY);
    open("/dev/null",O_RDWR);
    open("/dev/null",O_RDWR);

    syslog(LOG_INFO,"daemon started");
    return 0;
}

void daemon_stop()
{
    syslog(LOG_INFO,"daemon stopped");
    closelog();
}
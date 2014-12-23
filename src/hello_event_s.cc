/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For fcntl */
#include <fcntl.h>

#include <event2/event.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


#include <string>

#include <mutex> // for std::lock_guard
#include <queue>


// #define MAX_LINE 16384
#define MAX_LINE 3

namespace acvse{
namespace rpc{



template<class T>
class Queue{
public:
    Queue(){}
    ~Queue(){}
    void push(const T& t)  { std::lock_guard<std::mutex> l(m_mutex); m_queue.push(t); }
    void pop()             { std::lock_guard<std::mutex> l(m_mutex); m_queue.pop(); }
    bool empty()           { std::lock_guard<std::mutex> l(m_mutex); return m_queue.empty(); }

    T& front()             { std::lock_guard<std::mutex> l(m_mutex); return m_queue.front(); }
    const T& front() const { std::lock_guard<std::mutex> l(m_mutex); return m_queue.front(); }

    // void push(const T& t)  { m_queue.push(t); }
    // void pop()             { m_queue.pop(); }
    // bool empty()           { return m_queue.empty(); }

    // T& front()             { return m_queue.front(); }
    // const T& front() const { return m_queue.front(); }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};//endof Queue



void do_read(evutil_socket_t fd, short events, void *rpc);
void do_write(evutil_socket_t fd, short events, void *rpc);
//void do_accept(evutil_socket_t listener, short event, void *rpc);


class RpcState
{
    public :
        RpcState(struct event_base *base, evutil_socket_t & fd);
        ~RpcState();

        std::string buf; // buf.length() < size

        Queue<std::string> wq;

        struct event *r_evn;
        struct event *w_evn;

        bool sflag;

        evutil_socket_t m_fd;
};

RpcState::RpcState(struct event_base *base, evutil_socket_t & fd)
{
    fprintf(stdout, "RpcState start\n");
    sflag = true;
    if (!(r_evn = event_new(base, fd, EV_READ|EV_PERSIST, do_read,this)))
    {
        sflag = false;
        return;
    }
    if(!(w_evn =  event_new(base, fd, EV_WRITE|EV_PERSIST, do_write,this)))
    {
        event_free(r_evn);
        sflag = false;
        return;
    }
    m_fd = fd;
    fprintf(stdout, "RpcState end\n");
}

RpcState::~RpcState()
{
    printf("~RpcState start\n");
    if(sflag)
    {
        event_free(r_evn);
        event_free(w_evn);

        close(m_fd);
    }
    printf("~RpcState end\n");
}


void do_read(evutil_socket_t fd, short events, void * _state)
{
    printf("do_read start\n");

    if ( _state == NULL)
    {
        fprintf(stderr, "[ERR] do_write , not a right state\n");
        return ;
    }

    RpcState & state = * ((RpcState *)_state);
    ssize_t r_sz = -1;
    char buf[MAX_LINE];


    while(true)
    {
        printf ("(%d)fd : %d \n",__LINE__,fd);
        r_sz = recv(fd, buf, MAX_LINE , 0);
        if (r_sz <= 0 ){
            printf("BREAK : %d\n",__LINE__);
            break;
        }
        printf("GET(%d) : %s ",(int)r_sz,buf);
        for(int i = 0 ; i < r_sz ; i ++)
        {
            if ( buf[i] == EOF )
            {
                printf("EOF\n");
                if (state.buf.length() > 0 )
                {
                    //printf("plus : %s \n",state.buf.c_str());
                    state.wq.push(state.buf);
                    state.buf.clear();
                    //char mbuf[20] = "vvvvvv";
                    //printf ("(%d)fd : %d \n",__LINE__,fd);
                    //ssize_t r_sz = send(fd,mbuf,sizeof(mbuf),0);
                    assert(state.w_evn);
                    event_add(state.w_evn,NULL);
                }
                break;
            } else
            {
                printf("%c => %c \n",buf[i],
                                    (buf[i] == '\n' ? '\n' : (char)(buf[i] + 1)));
                //state.buf += buf[i] == '\n' ? '\n' : (char)(buf[i] + 1);
                state.buf += buf[i] == '\n' ? '\n' : (char)(buf[i]);
            }
        }
    }

    if (r_sz == 0)
    {
        printf("free state ...%d\n",__LINE__);
        delete &state;
    } else if (r_sz < 0 )
    {
        if (errno == EAGAIN )
            return;
        fprintf(stderr, "[ERR] recv\n");
        delete &state;
    }

    printf("do_read end\n");
}

void do_write(evutil_socket_t fd, short events,void * _state)
{
    printf("do_write start\n");

    if ( _state == NULL)
    {
        fprintf(stderr, "[ERR] do_write , not a right state\n");
        return ;
    }

    RpcState & state = * ((RpcState *)_state);

    while(!state.wq.empty())
    {
        std::string buf = state.wq.front();
        state.wq.pop();
        //for( int z =0 ; z < 10 ; z ++ )
        {
            fprintf(stdout,"[W](%lu) [%s]",buf.length(),buf.c_str());
            //char mbuf[20] = "cccccccc";
            buf += EOF;
            ssize_t r_sz = send(fd,buf.data(),sizeof(char) * buf.length(),0);
            //printf ("(%d)fd : %d \n",__LINE__,fd);
            //ssize_t r_sz = send(fd,mbuf,sizeof(mbuf),0);
            printf("[W sz] %lu\n", r_sz);
            if( r_sz < 0 )
            {
                if (errno == EAGAIN ) // XXX use evutil macro
                    return ;
                delete & state ;
            }
        }
    }

    event_del(state.w_evn);
    printf("do_write end\n");

}

void do_accept(evutil_socket_t listener, short event, void *arg)
{
    printf("do_accept start\n");
    struct event_base *base = (struct event_base *)arg;
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    evutil_socket_t fd = accept(listener, (struct sockaddr*)&ss, &slen);
    // int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    if (fd < 0) { // XXXX eagain??
        perror("accept");
    } else if (fd > FD_SETSIZE) {
        printf("XXX replace all closes with EVUTIL_CLOSESOCKET\n");
        close(fd); // XXX replace all closes with EVUTIL_CLOSESOCKET
    } else {
        printf("new visit start\n");
        evutil_make_socket_nonblocking(fd);
        printf ("(%d)fd : %d \n",__LINE__,fd);
        RpcState & state = * (new RpcState(base, fd));
        if (state.sflag)
        {
            event_add(state.r_evn,NULL);
        }else
        {
            delete & state;
            fprintf(stderr,"state sflag is false ... \n");
        }

        printf("new visit end\n");
    }
    printf("do_accept end\n");
}

void run(void)
{
    printf("run start ...\n");
    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base *base;
    struct event *listener_event;

    base = event_base_new();
    if (!base)
        return; /*XXXerr*/

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(9527);

    listener = socket(AF_INET, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(listener);

    #ifndef WIN32 // in fact , i'll never use it is windows service
    {
        int one = 1;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
    #endif

    if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return;
    }

    if (listen(listener, 16)<0) {
        perror("listen");
        return;
    }

    listener_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);
    /*XXX check it */
    event_add(listener_event, NULL);

    event_base_dispatch(base);
    printf("run end\n");
}


}
}

int main(int argc, char *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    acvse::rpc::run();
    return 0;
}



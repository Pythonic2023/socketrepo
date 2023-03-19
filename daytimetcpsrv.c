#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */


// Defines
#define MAXLINE 4096
#define SA struct sockaddr
#define BACKLOG 5


int main(int argc, char *argv[]){
    int listenfd, connfd;
    struct sockaddr_in servaddr, peeraddr;
    socklen_t addr_size;
    char buff[MAXLINE];
    time_t ticks;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        fprintf(stderr, "Socket error\n");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13); // Daytime server

    if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) == -1){
        printf("Bind error: %s\n", strerror(errno));
    }

    if(listen(listenfd, BACKLOG) == -1){
        fprintf(stderr, "Listen error\n");
    }  

    for( ; ; ){
        addr_size = sizeof(peeraddr);
        connfd = accept(listenfd, (SA *) &peeraddr, &addr_size);
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));

        close(connfd);
    }

}
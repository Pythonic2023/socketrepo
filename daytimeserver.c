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

// DEFINES 
#define SA struct sockaddr
#define MAXLINE 4096

int main(int argc, char *argv[]){
    int sockfd, n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;

    if(argc != 2){
        fprintf(stderr, "Usage: PROGRAM ARGUMENT\n");
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "socket error\n");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13); // daytime server
    
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
    }

    if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
        fprintf(stderr, "connect error\n");

        while((n = read(sockfd, recvline, MAXLINE)) > 0){
            recvline[n] = 0; // null terminate
            if(fputs(recvline, stdout) == EOF)
            fprintf(stderr, "fputs error\n");
        }
        if(n < 0){
            fprintf(stderr, "read error\n");
        }
        return 0;
    }
}
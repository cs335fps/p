#ifndef _WEB_
#define _WEB_

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <stdlib.h>

//added to make close() work
#include <unistd.h>
#define PORT 80
#define USERAGENT "HTMLGET 1.0"
class Web
{
    int create_tcp_socket();
    char *get_ip(const char *host);
    char *build_get_query(const char *host, const char *page);
    int HttpConnect(const char*,const char*);
    public:
    int Score(char* name, int score, int shots, int hits, int ticks);

};


#endif


// Edited by: Nicholas Gardner
// cs335
// lab3
// source:
// http://coding.debuntu.org/c-linux-socket-programming-tcp-simple-http-client
#include "web.h"
using namespace std;

string Web::Score(char* name, 
    int score, 
    int shots, 
    int hits, 
    int ticks, 
    int streak)
{
    char buf[5][16];
    sprintf(buf[0],"%d",score);
    sprintf(buf[1],"%d",shots);
    sprintf(buf[2],"%d",hits);
    sprintf(buf[3],"%d",ticks);
    sprintf(buf[4],"%d",streak);
    string host = "www.cs.csubak.edu";
    string page = "~ngardner/fps/score.php?name=" + string(name) + 
        "&kills=" + string(buf[0]) + 
        "&shots=" + string(buf[1]) + 
        "&hits=" + string(buf[2]) + 
        "&ticks=" + string(buf[3]) +
        "&streak=" + string(buf[4]);
    string ret = HttpConnect(host.c_str(), page.c_str());
    
    if (ret.compare("na") == 0) {
        ret = "Leaderboards not available.,Kills: " + string(buf[0]); 
    }
    return ret;
}

string Web::HttpConnect(const char *host, const char *page)
{
    string s = "";
    string na = "na";
    struct sockaddr_in *remote;
    int sock;
    int tmpres;
    char *ip;
    char *get;
    char buf[BUFSIZ+1];
    
    sock = create_tcp_socket();
    ip = get_ip(host);
    if (ip == NULL)
        return na;
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
    
    if ( tmpres < 0) {
        return na;
        //perror("Can't set remote->sin_addr.s_addr");
        //exit(1);
    } else if (tmpres == 0) {
        return na;
        //fprintf(stderr, "%s is not a valid IP address\n", ip);
        //exit(1);
    }
    remote->sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0) {
        return na;
        //perror("Could not connect");
        //exit(1);
    }
    get = build_get_query(host, page);
    //fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);

    //Send the query to the server
    unsigned int sent = 0;
    while (sent < strlen(get)) {
        tmpres = send(sock, get+sent, strlen(get)-sent, 0);
        if (tmpres == -1) {
            return na;
            //perror("Can't send query");
            //exit(1);
        }
        sent += tmpres;
    }
    //now it is time to receive the page
    
    memset(buf, 0, sizeof(buf));
    int htmlstart = 0;
    char * htmlcontent;
    while ((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0) {
        if (htmlstart == 0) {
            /* Under certain conditions this will not work.
             * If the \r\n\r\n part is splitted into two messages
             * it will fail to detect the beginning of HTML content
             */
            htmlcontent = strstr(buf, "\r\n\r\n");
            if(htmlcontent != NULL){
                htmlstart = 1;
                htmlcontent += 4;
            }
        } else {
            htmlcontent = buf;
        }
        if (htmlstart) {
            // changed fprintf to printf
            //printf("%s", htmlcontent);
            s = htmlcontent;
        }

        memset(buf, 0, tmpres);
    }
    if (tmpres < 0) {
        return na;
        //perror("Error receiving data");
    }
    free(get);
    free(remote);
    free(ip);
    close(sock);
    return s;
}


int Web::create_tcp_socket()
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        //perror("Can't create TCP socket");
        //exit(1);
    }
    return sock;
}


char *Web::get_ip(const char *host)
{
    struct hostent *hent;
    int iplen = 20; //XXX.XXX.XXX.XXX
    char *ip = (char *)malloc(iplen+1);
    memset(ip, 0, iplen+1);
    if ((hent = gethostbyname(host)) == NULL) {
        //herror("Can't get IP");
        //exit(1);
        return NULL;
    }
    if (inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL) {
        //perror("Can't resolve host!");
        //exit(1);
        return NULL;
    }
    return ip;
}

//const char
char *Web::build_get_query(const char *host, const char *page)
{
    char *query;
    // const char
    const char *getpage = page;
    const char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
    if (getpage[0] == '/') {
        getpage = getpage + 1;
        //fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
    }
    // -5 is to consider the %s %s %s in tpl and the ending \0
    query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
    sprintf(query, tpl, getpage, host, USERAGENT);
    return query;
}



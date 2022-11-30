
        char rer[2048];

#ifndef _HTTPD_H___
#define _HTTPD_H___

#include <string.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
//Server control functions

#define CONNMAX 1000

static int listenfd, clients[CONNMAX];
static void error(char *);
static void startServer(const char *);
static void respond(int);

typedef struct { char *name, *value; } header_t;
static header_t reqhdr[17] = { {"\0", "\0"} };
static int clientfd;

static char *buf;

void serve_forever(const char *PORT);

// Client request

char    *method,    // "GET" or "POST"
        *uri,       // "/index.html" things before '?'
        *qs,        // "a=1&b=2"     things after  '?'
        *prot;      // "HTTP/1.1"

char    *payload;     // for POST
int      payload_size;

char *request_header(const char* name);

// user shall implement this function

void route();

// some interesting macro for `route()`
#define ROUTE_START()       if (0) {
#define ROUTE(METHOD,URI)   } else if (strcmp(URI,uri)==0&&strcmp(METHOD,method)==0) {
#define ROUTE_GET(URI)      ROUTE("GET", URI) 
#define ROUTE_POST(URI)     ROUTE("POST", URI) 
#define ROUTE_END()         } else printf(\
                                "HTTP/1.1 500 Not Handled\r\n\r\n" \
                                "The server has no handler to the request.\r\n" \
                            );

#endif


#include <fastdb.h>


void route()
{
    ROUTE_START()

    ROUTE_GET("/")
    {
        char rert[2048];
        memcpy(&rert,&rer,204);
        printf("HTTP/1.1 200 OK\r\n\r\n");
        char * prot = rert;
        for(int i = 0;i<strlen(prot);i++){
            if(prot[i]=='?')
            {
                prot+=i+1;
                break;
            }
        }
        char *path = prot;
         for(int i = 0;i<strlen(path );i++){
            if(prot[i]=='=')
            {
                prot[i] = 0;
                path = prot+i+1;
                break;
            }
        }
        char * content = path;
        
        for(int i = 0;i<strlen(path);i++){
            if(path[i]=='&')
            {
                path[i] = 0;
                content = path+i+1;
                break;
            }
        }
        

        if(strcmp(prot,"get")==0){                
            node_t * node = openNode(path,READ);
            if(node!=NULL){
                char * content = readContent(node,db_file);
                if(content!=NULL&&node->hasChild==false)
                printf("{\"response\": \"200\",\"data\": \"%s\"}",content);
                else{
                printf("{\"response\": \"602\"}");
                }
                free(content);
            }
            else{
                printf("{\"response\": \"601\"}");
            }
            free(node);

        }
        else if(strcmp(prot,"write")==0){                
            node_t * node = openNode(path,READ_WRITE);
            if(node==NULL){
                printf("{\"response\": \"601\"}");
            }
            int res = writeContent(node,content,db_file);
            if(res == 0){
                printf("{\"response\": \"200\"}");
            }else{
                printf("{\"response\": \"603\"}");
            }
             free(node);
            
           
        }
        else{
                node_t * node = openNode("index",READ);
                char * content = readContent(node,db_file);
                printf(content);

        }
        fclose(db_file);
        db_file = fopen(input_file_path,fprotocol);
        fseek(db_file,0,SEEK_SET);
        

    }

    ROUTE_POST("/")
    {
     
      printf("HTTP/1.1 200 OK\r\n\r\n");
      printf("fuck you!");
    
    }
  
    ROUTE_END()
}






void serve_forever(const char *PORT)
{
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;    
    
    int slot=0;
    
    printf(
            "Server started %shttp://127.0.0.1:%s%s\n",
            "\033[92m",PORT,"\033[0m"
            );

    // Setting all elements to -1: signifies there is no client connected
    int i;
    for (i=0; i<CONNMAX; i++)
        clients[i]=-1;
    startServer(PORT);
    
    // Ignore SIGCHLD to avoid zombie threads
    signal(SIGCHLD,SIG_IGN);

    // ACCEPT connections
    while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[slot]<0)
        {
            perror("accept() error");
        }
        else
        {
            if ( fork()==0 )
            {
                respond(slot);
                exit(0);
            }
        }

        while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;
    }
}

//start server
void startServer(const char *port)
{
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port, &hints, &res) != 0)
    {
        perror ("getaddrinfo() error");
        exit(1);
    }
    // socket and bind
    for (p = res; p!=NULL; p=p->ai_next)
    {
        int option = 1;
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p==NULL)
    {
        perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if ( listen (listenfd, 1000000) != 0 )
    {
        perror("listen() error");
        exit(1);
    }
}


// get request header
char *request_header(const char* name)
{
    header_t *h = reqhdr;
    while(h->name) {
        if (strcmp(h->name, name) == 0) return h->value;
        h++;
    }
    return NULL;
}

//client connection
void respond(int n)
{
    int rcvd, fd, bytes_read;
    char *ptr;

    buf = malloc(65535);
    rcvd=recv(clients[n], buf, 65535, 0);

    if (rcvd<0)    // receive error
        fprintf(stderr,("recv() error\n"));
    else if (rcvd==0)    // receive socket closed
        fprintf(stderr,"Client disconnected upexpectedly.\n");
    else    // message received
    {
        buf[rcvd] = '\0';

        method = strtok(buf,  " \t\r\n");
        uri    = strtok(NULL, " \t");
        strcpy(rer,uri);
        prot   = strtok(NULL, " \t\r\n"); 
        
        fprintf(stderr, "\x1b[32m + [%s] %s\x1b[0m\n", method, uri);
        
        if (qs = strchr(uri, '?'))
        {
            *qs++ = '\0'; //split URI
        } else {
            qs = uri - 1; //use an empty string
        }

        header_t *h = reqhdr;
        char *t, *t2;
        while(h < reqhdr+16) {
            char *k,*v,*t;
            k = strtok(NULL, "\r\n: \t"); if (!k) break;
            v = strtok(NULL, "\r\n");     while(*v && *v==' ') v++;
            h->name  = k;
            h->value = v;
            h++;
            fprintf(stderr, "[H] %s: %s\n", k, v);
            t = v + 1 + strlen(v);
            if (t[1] == '\r' && t[2] == '\n') break;
        }
        t++; // now the *t shall be the beginning of user payload
        t2 = request_header("Content-Length"); // and the related header if there is  
        payload = t;
        payload_size = t2 ? atol(t2) : (rcvd-(t-buf));

        // bind clientfd to stdout, making it easier to write
        clientfd = clients[n];
        dup2(clientfd, STDOUT_FILENO);
        close(clientfd);

        // call router
        route();

        // tidy up
        fflush(stdout);
        shutdown(STDOUT_FILENO, SHUT_WR);
        close(STDOUT_FILENO);
    }

    //Closing SOCKET
    shutdown(clientfd, SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(clientfd);
    clients[n]=-1;
}
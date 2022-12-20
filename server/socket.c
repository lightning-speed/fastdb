

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <fastdb.h>
// Server control functions

#define CONNMAX 1000

FILE *logFile;

static int listenfd, clients[CONNMAX];
static void error(char *);
static void startServer(const char *);
static void respond(int);

typedef struct
{
    char *name, *value;
} header_t;
static header_t reqhdr[17] = {{"\0", "\0"}};
static int clientfd;

static char *buf;

void serve_forever(const char *PORT);

// Client request

char *method, // "GET" or "POST"
    *uri,     // "/index.html" things before '?'
    *qs,      // "a=1&b=2"     things after  '?'
    *prot;    // "HTTP/1.1"

char *payload; // for POST
int payload_size;

char *request_header(const char *name);

// user shall implement this function

char *filePath;
char rer[2048];
char *indPath;
void route();

// some interesting macro for `route()`
#define ROUTE_START() \
    if (0)            \
    {
#define ROUTE(METHOD, URI)                                         \
    }                                                              \
    else if (strcmp(URI, uri) == 0 && strcmp(METHOD, method) == 0) \
    {
#define ROUTE_GET(URI) ROUTE("GET", URI)
#define ROUTE_POST(URI) ROUTE("POST", URI)
#define ROUTE_END(URI) \
    ROUTE("END", URI)  \
    }

void route()
{
    bool responded = false;
    printf("HTTP/1.1 200 OK\r\n\r\n");

    ROUTE_START()

    ROUTE_GET("/")
    {

        char *prot = NULL;
        char *tokenKey = NULL;
        char *path = NULL;
        char *content = NULL;
        strtok(rer, "&");
        prot = strtok(NULL, "&");
        if (prot != NULL)
            path = strtok(NULL, "&");
        if (path != NULL)
            tokenKey = strtok(NULL, "&");
        if (tokenKey != NULL)
            content = strtok(NULL, "&");
        fprintf(stderr, "\nprot: %s\npath: %s\ncontent: %s\ntokenKey %s\n", prot, path, content, tokenKey);
        fprintf(logFile, "\nprot: %s\npath: %s\ncontent: %s\ntokenKey %s\n", prot, path, content, tokenKey);
        if (isValidTokenKey((uint64_t)strtoull(tokenKey, NULL, 10)))
        {

            // if (isValidTokenKey(tokenKey))
            if (prot != NULL)
            {
                if (strcmp(prot, "get") == 0)
                {
                    char *args[4] = {"", filePath, "-read", path};
                    startDB(4, args);
                    responded = true;
                }
                else if (strcmp(prot, "write") == 0)
                {
                    char *args[5] = {"", filePath, "-write", path, content};
                    startDB(5, args);
                    responded = true;
                }
                else if (strcmp(prot, "info") == 0)
                {
                    char *args[4] = {"", filePath, "-info", path};
                    startDB(4, args);
                    responded = true;
                }
                else if (strcmp(prot, "delete") == 0)
                {
                    char *args[4] = {"", filePath, "-delete", path};
                    startDB(4, args);
                    responded = true;
                }
                else if (strcmp(prot, "rename") == 0)
                {
                    char *args[5] = {"", filePath, "-rename", path, content};
                    startDB(5, args);
                    responded = true;
                }

                else
                {
                }
            }
        }
        else if (strlen(prot) != 0 && strcmp(prot, "aus") == 0)
        {
            uint64_t key = loginUser(path, tokenKey);
            fprintf(stderr, "%lli", (uint64_t)key);
            char dat[256];
            sprintf(dat, "%lli", key);
            if (key != (uint64_t)1)
                sendSucessResponse(dat);
            else
            {
                sendUnAuthResponse("Invalid loing credentials");
            }
            responded = true;
        }
        else
            sendUnAuthResponse("Invalid Seq Key");
    }

    ROUTE_POST("/")
    {

        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("fuck you!");
    }

    ROUTE_END("/")
    {
        if (!responded)
        {

            char *path = rer;
            for (int i = 0; i < strlen(path); i++)
            {
                if (path[i] == '/')
                {
                    path += ++i;
                    break;
                }
            }
            if (strlen(path) == 0)
            {
                path = "index.html";
            }
            wchar_t *buff = malloc(1024);
            sprintf(buff, "%s/%s", indPath, path);
            FILE *file = fopen(buff, "r");
            if (file != NULL)
            {
                while (fgets(buff, 500, file) != NULL)
                {
                    printf("%s", buff);
                }
                fclose(file);
            }
            free(buff);
        }
    }
}

void serve_forever(const char *PORT)
{

    logFile = fopen("./server.log", "wb+");
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;

    int slot = 0;

    printf(
        "Server started %shttp://127.0.0.1:%s%s\n",
        "\033[92m", PORT, "\033[0m");

    // Setting all elements to -1: signifies there is no client connected
    int i;
    for (i = 0; i < CONNMAX; i++)
        clients[i] = -1;
    startServer(PORT);

    // Ignore SIGCHLD to avoid zombie threads
    signal(SIGCHLD, SIG_IGN);

    // ACCEPT connections
    while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept(listenfd, (struct sockaddr *)&clientaddr, &addrlen);

        if (clients[slot] < 0)
        {
            perror("accept() error");
        }
        else
        {
            if (fork() == 0)
            {
                respond(slot);
                exit(0);
            }
        }

        while (clients[slot] != -1)
            slot = (slot + 1) % CONNMAX;
    }
    fclose(logFile);
}

// start server
void startServer(const char *port)
{
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port, &hints, &res) != 0)
    {
        perror("getaddrinfo() error");
        exit(1);
    }
    // socket and bind
    for (p = res; p != NULL; p = p->ai_next)
    {
        int option = 1;
        listenfd = socket(p->ai_family, p->ai_socktype, 0);
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (listenfd == -1)
            continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
    }
    if (p == NULL)
    {
        perror("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if (listen(listenfd, 1000000) != 0)
    {
        perror("listen() error");
        exit(1);
    }
}

// get request header
char *request_header(const char *name)
{
    header_t *h = reqhdr;
    while (h->name)
    {
        if (strcmp(h->name, name) == 0)
            return h->value;
        h++;
    }
    return NULL;
}

// client connection
void respond(int n)
{
    int rcvd, fd, bytes_read;
    char *ptr;
    buf = malloc(65535);
    rcvd = recv(clients[n], buf, 65535, 0);

    if (rcvd < 0) // receive error
        fprintf(logFile, ("recv() error\n"));
    else if (rcvd == 0) // receive socket closed
        fprintf(logFile, "Client disconnected upexpectedly.\n");
    else // message received
    {
        buf[rcvd] = '\0';

        method = strtok(buf, " \t\r\n");
        uri = strtok(NULL, " \t");
        strcpy(rer, uri);
        prot = strtok(NULL, " \t\r\n");

        //  fprintf(logFile, "\x1b[32m + [%s] %s\x1b[0m\n", method, uri);

        if (qs = strchr(uri, '?'))
        {
            *qs++ = '\0'; // split URI
        }
        else
        {
            qs = uri - 1; // use an empty string
        }

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
    free(buf);
    // Closing SOCKET
    shutdown(clientfd, SHUT_RDWR); // All further send and recieve operations are DISABLED...
    close(clientfd);
    clients[n] = -1;
}

int smain(int argc, char **argv)
{
    if (argc > 1)
    {
        filePath = argv[1];
    }
    else
    {
        printf("DB file required\n");
        return -1;
    }
    if (argc > 3)
    {
        indPath = argv[3];
    }
    serve_forever("9001");
}
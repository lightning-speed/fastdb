#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
extern char *input_file_path;
extern char *index_file_path;

#define MAX_CHILDREN 128
#define MAX_DEPTH 10
#define fprotocol "rb+"

#define SUCESSFUL_RESPONSE 200
#define NOT_FOUND_RESPONSE 404
#define INCORRECT_REQUEST_FORMAT 403
#define UNAUTH_REQUEST_FORMAT 401
#define OTHER_ERROR_RESPONSE 500

enum access_t
{
  READ = 0,
  WRITE = 1,
  READ_WRITE = 2,
  NONE = 3
};
enum RUN_TYPE
{
  CMD_LINE,
  DATA_LISTNER,
  SINGLE_EXEC
};

static char *perms[] = {
    "READ",
    "WRITE",
    "READ_WRITE",
    "NONE"};

typedef struct
{
  long long uid;
  char name[64];
  uintptr_t *parent;
  uintptr_t *children;
  uintptr_t ca;
  int cap;
  uint16_t size;
  uintptr_t addr;
  bool linked;
  enum access_t access;
  char *content;
  bool hasChild;
  bool isPointer;
  bool saved;

} node_t;

typedef struct
{
  char username[32];
  char passhash[256];
} user_t;

extern FILE *db_file;
extern node_t *db_node;
extern enum RUN_TYPE runType;

node_t *createDB();
int startDB(int argc, char **argv);
node_t *createNode();
node_t *createRNode(char *name);
uintptr_t writeNode(node_t *node, FILE *db);
void saveNode(node_t *node, FILE *db);
void linkNode(node_t *parent, node_t *child, FILE *db);
node_t *createDB(FILE *db_file);
node_t *openNode(char *path, enum access_t access);
node_t *getNodeFromAddr(uintptr_t addr, FILE *db);
uintptr_t findChildAddr(node_t *node, char *name, FILE *db);
node_t *getNode(node_t *parent, char *name, FILE *db);

long long hashName(char *name);
int writeContent(node_t *node, char *content, FILE *db);

char *readContent(node_t *node, FILE *db);
void defragment();

node_t *addDEFNode(node_t *node, FILE *db, int depth);

void printTree(node_t *node, int depth, FILE *db);
void ptree(node_t *db);
void deleteNode(node_t *node, FILE *db);

void initArgs();

void sendNotFoundResponse();
void sendIncorrectFormatResponse();
void sendUnAuthResponse(char *data);
void sendSucessResponse(char *data);
void sendOtherErroResponse(char *res);

void setChild(node_t *node, int at, uintptr_t addr, FILE *db);
uintptr_t getAddrOfChildAt(node_t *node, int at, FILE *db);

uint64_t createKey();
bool isValidTokenKey(uint64_t keyL);
uint64_t loginUser(const char *username,const char *phash);
uint64_t registerUser(const char *username,const char *phash);
char* SHA256(char* data);

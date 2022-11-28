#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
extern char * input_file_path;

#define MAX_CHILDREN 128
#define MAX_DEPTH 10





enum access_t{
  READ  = 0,
  WRITE  = 1,
  READ_WRITE  = 2,
  NONE =  3
};

typedef struct{
 long long uid;
 char name[64];
 uintptr_t * parent;
 uintptr_t children[32];
 uint16_t size;
 uintptr_t addr;
 bool linked;
 enum access_t access;
 char * content;
 bool hasChild;
}node_t;

extern FILE * db_file;
extern node_t * db_node;

node_t * createDB();
int startDB(int argc,char ** argv);
node_t *createNode();
node_t *createRNode(char * name);
uintptr_t writeNode(node_t * node,FILE * db);
void saveNode(node_t * node,FILE * db);
void link(node_t * parent,node_t * child,FILE * db);
node_t * createDB(FILE * db_file);
node_t * openNode(char * path,enum access_t access);
node_t *getNodeFromAddr(uintptr_t addr,FILE * db);
uintptr_t findChildAddr(node_t *node,char * name,FILE *db);
node_t * getNode(node_t * parent,char * name,FILE * db);

long long hashName(char * name);
void writeContent(node_t * node,char * content,FILE * db);

char * readContent(node_t * node,FILE * db);
void defragment();
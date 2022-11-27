#include <fastdb.h>
#include <fastdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


node_t *createNode(){
	return (node_t *)malloc(sizeof(node_t));
}
node_t *createRNode(char * name){
	node_t * node = createNode();
	node->size =0;
	node->uid = hashName(name);
	node->linked = false;
	node->hasChild = false;
	strcpy(node->name,name);
	return node;
}
uintptr_t writeNode(node_t * node,FILE * db){
	fseek(db,0,SEEK_END);
	node->addr = ftell(db);
	saveNode(node,db);
	return node->addr;
}
void saveNode(node_t * node,FILE * db){
	fseek(db,node->addr,SEEK_SET);
	fwrite(node,1,sizeof(node_t),db);
}
void link(node_t * parent,node_t * child,FILE * db){
	if(findChildAddr(parent,child->name,db)!=1){
		printf("%s%s%s%s%s","Child '",child->name,"'' already exists in '",parent->name,"'");
		return;
	}
	if(child->linked==false)
	writeNode(child,db);
	parent->children[parent->size++] = child->addr;
	parent->hasChild = true;
	child->parent = parent->addr;
	child->linked = true;
	saveNode(parent,db);
	saveNode(child,db);
}
node_t * createDB(FILE * db_file){
 node_t * db = createRNode("root");
 writeNode(db,db_file);
 return db;
}
void writeContent(node_t * node,char * content,FILE * db){
	if(node->hasChild==true){
		printf("%s%s%s","Node '",node->name,"' cannot hold values since its a parent node");
		return;
	}
	fseek(db,0,SEEK_END);
	node->content = (char *)ftell(db);
	int n = strlen(content);
	node->size = n;
	fwrite(content,1,n+1,db);
	saveNode(node,db);
}
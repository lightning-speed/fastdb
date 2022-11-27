#include <stdint.h>
#include <fastdb.h>
#include <stdio.h>
#include <string.h>
FILE * db_file;
char * input_file_path = NULL;



int startDB(int argc,char ** argv){
  if(input_file_path==NULL){
  	printf("\nError: No DB file specified");
  	return 44;
  }
  db_file = fopen(input_file_path,"rwb+");
  fseek(db_file,0,SEEK_END);
  long long int size = ftell(db_file);
  fseek(db_file,0,SEEK_SET);
 
  node_t * db;
  if(size == 0){
   db = createDB(db_file);
  }else
   db  = getNodeFromAddr(0,db_file);
  if(argc>2)
  	if(!strcmp(argv[2],"-read")){
  		if(argc>3){
  		node_t * node  = openNode(argv[3],READ);
  		if(node!=NULL){
  			char * content = readContent(node,db_file);
  			printf(content);
  			free(content);
  			free(node);
  		}
  		}else{
  			printf("Require one more argument");
  		}
  	}
  	else if(!strcmp(argv[2],"-write")){
  		if(argc>3){
  		node_t * node  = openNode(argv[3],WRITE);
  		if(node!=NULL){
  			writeContent(node,argv[4],db_file);
  			free(node);
  		}
  		}else{
  			printf("Require more arguments");
  		}
  	}
  fseek(db_file,0,SEEK_END);
  fclose(db_file);
  return 0;
}

node_t * openNode(char * path,enum access_t access){
	char temp[MAX_DEPTH][64];
	int depth_index = 0;
	int str_index = 0;
	int n = strlen(path);
	for(int i = 0;i<n;i++){
		if(path[i]!='.'){
			temp[depth_index][str_index] = path[i];
			temp[depth_index][++str_index]  = 0;
		}
		else{
			depth_index++;
			str_index = 0;
		}
	}
	node_t * node = getNodeFromAddr(0,db_file);
	for(int i = 0;i<depth_index+1;i++){
		uintptr_t addr = findChildAddr(node,temp[i],db_file);
		if(addr!=1){
			free(node);
			node = getNodeFromAddr(addr,db_file);
		}
		else if(access==WRITE||access==READ_WRITE){
			node_t * cnode = createRNode(temp[i]);
			link(node,cnode,db_file);
			free(node);
			node = cnode;
		}else{
			return NULL;
		}
	}
	return node;
}
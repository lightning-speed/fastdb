#include <stdint.h>
#include <fastdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
FILE * db_file;
node_t * db_node;
char * input_file_path = NULL;



int startDB(int argc,char ** argv){
  if(input_file_path==NULL){
  	printf("\nError: No DB file specified");
  	return 44;
  }
  //OPEN
  db_file = fopen(input_file_path,"rwb+");
  if(db_file==NULL){
  	printf("%s%s%s","Error: file '",input_file_path,"' doesn't exist");
  	return 55;
  }

  fseek(db_file,0,SEEK_END);
  long long int size = ftell(db_file);
  fseek(db_file,0,SEEK_SET);
 
 //CREATE DATABASE
  node_t * db;
  if(size == 0){
   db = createDB(db_file);
  }else
   db  = getNodeFromAddr(0,db_file);
  db_node = db;

  //AGRUMENTS

  if(argc>2)

  	//READ

  	if(!strcmp(argv[2],"-read")){
  		if(argc>3){
  		node_t * node  = openNode(argv[3],READ);
  		if(node!=NULL){
  			char * content = readContent(node,db_file);
  			if(content==NULL){
  				printf("Node does not have read permission");
  				return 78;
  			}
  			else
  			printf(content);
  			free(content);
  			free(node);
  		}
  		else{
  			printf("node doesn't exist");
  			return 56;
  		}
  		}else{
  			printf("usage: -read [node_path]");
  			return 57;
  		}
  	}

  	//WRITE

  	else if(!strcmp(argv[2],"-write")){
  		if(argc>4){
  		node_t * node  = openNode(argv[3],WRITE);
  		if(node==1){
  			printf("Innode cannot be created");
  			return 61;
  		}
  		else if(node!=NULL){
  			int res = writeContent(node,argv[4],db_file);
  			if(res==-1){
  				printf("Node does not have write permission");
  				return 77;
  			}
  			free(node);
  		}
  		else{
  			printf("node doesn't exist");
  			return 56;

  		}
  		}else{
  			printf("usage: -write [node_path] [data]");
  			return 57;

  		}
  	}

  	//DEFRAGMENT

  	else if(!strcmp(argv[2],"-defragment")){
  		defragment();
  	}

  	//TREE

  	else if(!strcmp(argv[2],"-tree")){
  		ptree(db);
  	}
  	else if(!strcmp(argv[2],"-rename"))	{
  		if(argc>4){
  		node_t * node  = openNode(argv[3],READ);
  		if(node!=NULL){
  			strcpy(node->name,argv[4]);
  			saveNode(node,db_file);
  			free(node);
  		}else{
  			printf("node doesn't exist");
  			return 56;

  		}
  	}
  }
  	else if(!strcmp(argv[2],"-delete"))	{
  		if(argc>3){
  		node_t * node  = openNode(argv[3],READ);
  		if(node!=NULL){
  			deleteNode(node,db_file);
  			free(node);
  		}else{
  			printf("node doesn't exist");
  			return 56;

  		}
  	}else{
  			printf("usage: -delete [node_path]");
  			return 57;

  	}
  }
  else if(!strcmp(argv[2],"-setperms")){
  		if(argc>4){
  		node_t * node  = openNode(argv[3],READ);
  		if(node!=NULL){
  			node->access = atoi(argv[4])%4;
  			saveNode(node,db_file);
  			free(node);
  		}else{
  			printf("node doesn't exist");
  			return 56;

  		}
  	}else{
  			printf("usage: -setperms [node_path] [perms] //perms must be a value between 0-3 [read,write,read_write]");
  			return 57;
  	}
  }
  else{
  	printf("Unknow Option '%s'",argv[2]);
  			return 69;
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
			free((void *)node);
			node = getNodeFromAddr(addr,db_file);
		}
		else if(access==WRITE||access==READ_WRITE){
			if(node->content!=NULL){
				return 1;
			}
			node_t * cnode = createRNode(temp[i]);
			link(node,cnode,db_file);
			free((void *)node);
			node = cnode;
		}else{
			return NULL;
		}
	}
	return node;
}


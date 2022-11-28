#include <stdint.h>
#include <fastdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

node_t * addDEFNode(node_t * node,FILE * db,int depth){
	node_t * nn = createRNode(node->name);
	nn->size = 0;
	nn->linked = node->linked;
	writeNode(nn,db);
	if(node->hasChild==true&&!node->isPointer){
		for(int i = 0;i<node->size;i++){
			node_t * cnode  = getNodeFromAddr(node->children[i],db_file);
			node_t * cn = addDEFNode(cnode,db,depth+1);
			link(nn,cn,db);
			if(cnode->hasChild==false){
				char * content = readContent(cnode,db_file);
				writeContent(cn,content,db);
				free(content);
			}
			
		}
	}
	return nn;
}

void defragment(){
	printf("\nInfo: defragmenting...");
	remove("./temp.dat");
	system("touch ./temp.dat");
	FILE * tempsFile = fopen("./temp.dat","rwb+");
	fseek(tempsFile,0,SEEK_SET);
	addDEFNode(db_node,tempsFile,0);
	fclose(db_file);
	fclose(tempsFile);
	remove(input_file_path);
	rename("./temp.dat",input_file_path);
	printf("\nInfo: Success : defragmenting done");

}
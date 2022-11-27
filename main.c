#include <stdio.h>
#include <fastdb.h>


int main(int argc,char ** argv){
	if(argc>1)
		input_file_path = argv[1];
	int exit_code = startDB(argc,argv);
	printf("\n");
	return exit_code;
}
#include <fastdb.h>
#include <math.h>
#include <string.h>


long long hashName(char * name){
	long long out = 0;
	int n = strlen(name);
	for(int i = 0;i<n;i++){
		out+=name[i]*name[i]*name[i];
	}
	return out;
}
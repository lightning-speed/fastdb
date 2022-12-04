#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


uint64_t keys[65535];
uint32_t key_count = 0;

uint64_t createKey(){
	uint64_t key;
	((uint32_t *)&key)[0] = key_count;
	((uint32_t *)&key)[1] = (int)(time(0));
	keys[key_count++] = key;
	return key;
}




bool isValidTokenKey(uint64_t keyL){
	uint32_t keyID = ((uint32_t*)&keyL)[0];
	uint32_t keyPass = ((uint32_t*)&keyL)[1];
	if(keyID<key_count&&keys[key_count]==keyL){
		return true;
	}
	return false;

}
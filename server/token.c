#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <fastdb.h>

user_t users[10];
int user_count = 0;
uint64_t keys[600];

uint32_t key_count = 0;
void usbanrk(char *a, char *b)
{
	char c[2] = {a[0], b[0]};
	fprintf(stderr, a);
}
void readKey(int keyN)
{
	FILE *f = fopen(".akeys", "rb+");
	if (f != NULL)
	{
		fread(&key_count, 1, 4, f);
		if (keyN < key_count && keyN < 600)
		{
			fseek(f, (keyN * 8) + 4, SEEK_SET);
			fread((&keys[keyN]), 1, 8, f);
		}
		fclose(f);
	}
}
void writeKeys()
{
	FILE *f = fopen(".akeys", "wb+");
	fwrite(&key_count, 1, 4, f);
	fwrite(&keys, 600, 8, f);
	fclose(f);
}
uint64_t createKey()
{
	readKey(0);
	if (key_count >= 599)
	{
		// MAX KEYS REACHED
		return 0;
	}
	uint64_t t;
	time(&t);
	uint64_t key;
	((uint32_t *)&key)[0] = key_count;
	((uint32_t *)&key)[1] = (int)(t);
	keys[key_count] = key;
	key_count++;
	writeKeys();
	return key;
}

bool isValidTokenKey(uint64_t keyL)
{

	uint32_t keyID = ((uint32_t *)&keyL)[0];
	readKey(keyID);
	if (keyID < key_count)
	{
		uint64_t a = keys[keyID];
		usbanrk("%i", a);
		if (a == keyL)
			return true;
		else
			return false;
	}
	return false;
}
uint64_t loginUser(char *username, char *phash)
{
	for (int i = 0; i < user_count; i++)
	{
		if (strcmp(username, users[i].username) == 0 && strcmp(phash, users[i].passhash) == 0)
		{
			return (uint64_t)createKey();
		}
	}
	return 1;
}
void registerUser(char *username, char *phash)
{
	strcpy(users[user_count].username, username);
	strcpy(users[user_count++].passhash, phash);
}
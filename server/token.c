#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <fastdb.h>

user_t users[200];
int user_count = 0;
uint64_t keys[600];

uint32_t key_count = 0;
void saveUsers()
{

	FILE *f = fopen(".users", "wb+");
	if (f != NULL)
	{
		fwrite(&users, 200 * sizeof(user_t), 1, f);
		fclose(f);
	}
}
void loadUsers()
{
	FILE *f = fopen(".users", "rb+");
	if (f != NULL)
	{
		fread(&users, 200 * sizeof(user_t), 1, f);
		for (int i = 0; i < 200; i++)
		{
			if (users[i].username[0] == 0)
			{
				user_count = i;
				break;
			}
		}
		fclose(f);
	}
}
void usbanrk(char *a, char *b)
{
	char c[2] = {a[0], b[0]};
	fprintf(stderr, "%s", a);
}
void readKeys()
{
	FILE *f = fopen(".akeys", "rb+");
	if (f != NULL)
	{
		fread(&key_count, 1, 4, f);
		fread((&keys), 600, 8, f);
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
	readKeys();
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
	readKeys();
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
	loadUsers();

	for (int i = 0; i < 200; i++)
	{
		if (strcmp(username, users[i].username) == 0 && strcmp(phash, users[i].passhash) == 0)
		{
			return (uint64_t)createKey();
		}
	}
	return 1;
}

int registerUser(char *username, char *phash)
{
	loadUsers();
	for (int i = 0; i < 200; i++)
	{
		if (strcmp(username, users[i].username) == 0)
		{
			return -1;
		}
	}
	strcpy(users[user_count].username, username);
	strcpy(users[user_count++].passhash, phash);
	saveUsers();
	return 0;
}
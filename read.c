#include <fastdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

node_t *getNodeFromAddr(uintptr_t addr, FILE *db)
{
	node_t *node = createNode();
	fseek(db, addr, SEEK_SET);
	int result = fread((void *)node, 1, sizeof(node_t), db);
	node->children = malloc((node->cap + 5) * sizeof(uintptr_t));
	if (node->ca != NULL)
	{
		fseek(db, node->ca, SEEK_SET);
		fread((char *)(node->children), 1, node->cap * sizeof(uintptr_t), db);
	}
	return node;
}

uintptr_t findChildAddr(node_t *node, char *name, FILE *db)
{
	if (node == NULL)
	{
		printf("CANNOT FIND NULL NODE ADDR");
		return 1;
	}
	long long hash = hashName(name);
	for (int i = 0; i < node->size; i++)
	{
		long long tempHash;
		fseek(db, node->children[i], SEEK_SET);
		;
		int result = fread((void *)&tempHash, 1, 8, db);
		if (tempHash == hash)
		{
			return node->children[i];
		}
	}
	return 1;
}
node_t *getNode(node_t *parent, char *name, FILE *db)
{
	return getNodeFromAddr(findChildAddr(parent, name, db), db);
}
char *readContent(node_t *node, FILE *db)
{
	if (node->access != READ && node->access != READ_WRITE)
	{
		return NULL;
	}
	char *out = malloc(node->size);
	fseek(db, (uintptr_t)node->content, SEEK_SET);
	int result = fread(out, 1, node->size, db);
	return out;
}
uintptr_t getAddrOfChildAt(node_t *node, int at, FILE *db)
{
	int addrL = at * sizeof(uintptr_t);
	uintptr_t addr;
	fseek(db, node->children + addrL, SEEK_SET);
	fread(&addr, 1, sizeof(uintptr_t), db);
	return addr;
}
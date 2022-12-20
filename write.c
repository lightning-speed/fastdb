#include <fastdb.h>
#include <fastdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int a, int b)
{
	return a > b ? a : b;
}
node_t *createNode()
{
	return (node_t *)malloc(sizeof(node_t));
}
node_t *createRNode(char *name)
{
	node_t *node = createNode();
	node->size = 0;
	node->uid = hashName(name);
	node->linked = false;
	node->hasChild = false;
	node->access = READ_WRITE;
	node->isPointer = false;
	node->saved = false;
	node->cap = 0;
	node->ca = NULL;
	node->children = malloc(32 * sizeof(uintptr_t));
	strcpy(node->name, name);
	return node;
}
uintptr_t writeNode(node_t *node, FILE *db)
{

	if (node == NULL)
	{
		printf("CANNOT WRITE NULL NODE");
		return (uintptr_t)NULL;
	}
	node->saved = true;
	fseek(db, 0, SEEK_END);
	node->addr = ftell(db);
	saveNode(node, db);
	return node->addr;
}
void saveNode(node_t *node, FILE *db)
{
	if (node == NULL)
	{
		printf("CANNOT SAVE NULL NODE");
		return;
	}
	if (node->hasChild)
	{
		if (node->ca == NULL || node->size >= node->cap)
		{
			fseek(db, 0, SEEK_END);
			node->ca = ftell(db);
			node->cap += 20;
		}
		fseek(db, node->ca, SEEK_SET);
		fwrite((char *)(node->children), 1, node->cap * sizeof(uintptr_t), db);
	}
	fseek(db, node->addr, SEEK_SET);
	fwrite(node, 1, sizeof(node_t), db);
}
void linkNode(node_t *parent, node_t *child, FILE *db)
{
	if (parent == NULL || child == NULL)
	{
		printf("CANNOT LINK NULL NODE");
		return;
	}
	if (findChildAddr(parent, child->name, db) != 1)
	{
		printf("%s%s%s%s%s", "Child '", child->name, "'' already exists in '", parent->name, "'");
		return;
	}
	if (child->saved == false)
		writeNode(child, db);
	parent->children[parent->size++] = child->addr;
	parent->hasChild = true;
	child->parent = (uintptr_t *)parent->addr;
	child->linked = true;
	saveNode(parent, db);
	saveNode(child, db);
}
node_t *createDB(FILE *db_file)
{
	node_t *db = createRNode("root");
	writeNode(db, db_file);
	return db;
}
int writeContent(node_t *node, char *content, FILE *db)
{
	if (node->access != WRITE && node->access != READ_WRITE)
	{
		return -1;
	}
	if (node->hasChild == true)
	{
		printf("%s%s%s", "Node '", node->name, "' cannot hold values since its a parent node");
		return -2;
	}
	fseek(db, 0, SEEK_END);
	node->content = (char *)ftell(db);
	int n = strlen(content);
	node->size = n;
	fwrite(content, 1, n + 1, db);
	saveNode(node, db);
	return 0;
}

void deleteNode(node_t *node, FILE *db)
{
	if (node == NULL)
	{
		printf("CANNOT DELETE NULL NODE!");
		return;
	}
	node_t *parent = getNodeFromAddr((node_t *)(node->parent), db);
	for (int i = 0; i < parent->size; i++)
	{
		if (parent->children[i] == node->addr)
		{
			for (int j = i; j < parent->size; j++)
			{
				parent->children[j] = parent->children[j + 1];
			}
			parent->size--;
			break;
		}
	}
	saveNode(parent, db);
}

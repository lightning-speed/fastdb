#include <stdint.h>
#include <fastdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int metasize = 0;
int contentsize = 0;
void printTree(node_t *node, int depth, FILE *db)
{

	metasize += sizeof(node_t);
	for (int i = 0; i < 5; i++)
	{
		putchar('=');
	}
	printf("> %s", node->name);
	if (node->isPointer)
		printf("(pointer)");
	int nn = strlen(node->name);
	if (node->hasChild == false)
	{
		contentsize += node->size;
		for (int i = 0; i < 50 - depth - nn; i++)
			putchar(' ');
		printf("%i KB [ %s ]", node->size / 1024, perms[node->access]);
	}
	printf(" \n");

	if (node->hasChild && (!node->isPointer || depth == 1))
		for (int i = 0; i < node->size; i++)
		{
			for (int i = 0; i < depth; i++)
			{
				putchar(' ');
			}
			putchar('[');
			uintptr_t add = node->children[i];
			if (add != NULL)
			{
				node_t *nodes = getNodeFromAddr(add, db);
				printTree(nodes, depth + 2, db);
				free(nodes);
			}
		}
}
void ptree(node_t *db)
{
	printTree(db, 1, db_file);
	fseek(db_file, 0, SEEK_END);
	int totalsize = ftell(db_file) / 1024;
	metasize /= 1024;
	contentsize /= 1024;
	printf("\nMeta Size: %i KB\n", metasize);
	printf("Content Size Size: %i KB\n", contentsize);
	printf("Fragmented : %i KB\n", totalsize - (metasize + contentsize));
	printf("Total Size: %i KB\n", totalsize);
}
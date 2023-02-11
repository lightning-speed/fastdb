#include <stdint.h>
#include <fastdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
FILE *db_file;
node_t *db_node;
char *input_file_path = NULL;
enum RUN_TYPE runType;
char *index_file_path;
char *coms[] = {"-write", "-read", "-delete", "-rename", "-point", "-tree", "-server", "-defragment", "-setperms", "-info"};
char *WRITE_C;
char *READ_C;
char *DEL_C;
char *RENAME_C;
char *POINT_C;
char *TREE_C;
char *SERVER_C;
char *DEFRAGMENT_C;
char *SETP_C;
char *INFO_C;

void initArgs()
{
	if (runType == CMD_LINE)
	{
		for (int i = 0; coms[i] != NULL; i++)
		{
			coms[i]++;
		}
	}
	WRITE_C = coms[0];
	READ_C = coms[1];
	DEL_C = coms[2];
	RENAME_C = coms[3];
	POINT_C = coms[4];
	TREE_C = coms[5];
	SERVER_C = coms[6];
	DEFRAGMENT_C = coms[7];
	SETP_C = coms[8];
	INFO_C = coms[9];
}

int startDB(int argc, char **argv)
{
	if (input_file_path == NULL)
	{
		printf("\nError: No DB file specified");
		return 44;
	}
	// OPEN
	db_file = fopen(input_file_path, fprotocol);
	db_file = fopen(input_file_path, fprotocol);
	if (db_file == NULL)
	{
		db_file = fopen(input_file_path, "wb+");
		return 55;
	}

	fseek(db_file, 0, SEEK_END);
	long long int size = ftell(db_file);
	fseek(db_file, 0, SEEK_SET);

	// CREATE DATABASE
	node_t *db;
	if (size == 0)
	{
		db = createDB(db_file);
	}
	else
		db = getNodeFromAddr(0, db_file);
	db_node = db;

	// AGRUMENTS

	if (argc > 2)

		// READ

		if (!strcmp(argv[2], READ_C))
		{
			if (argc > 3)
			{
				node_t *node = openNode(argv[3], READ);
				if (node != NULL)
				{
					char *content = readContent(node, db_file);
					if (content == NULL)
					{
						printf("Node does not have read permission");
						return 78;
					}
					else if(content== -1){
							printf("Node is a directory");
						return 79;
					}
					else
						sendSucessResponse(content);
					free(content);
					free(node);
				}
				else
				{
					sendNotFoundResponse();
					return 56;
				}
			}
			else
			{
				sendIncorrectFormatResponse();

				return 57;
			}
		}
		else if (!strcmp(argv[2], INFO_C))
		{
			if (argc > 3)
			{
				node_t *node = openNode(argv[3], READ);
				if (node != NULL)
				{
					printf("{"
						   " \"response\": 200,"
						   " \"name\": \"%s\","
						   " \"uid\": %lli,"
						   " \"size\": %i,"
						   " \"access\": %i"
						   "}",
						   node->name,
						   node->uid,
						   node->size,
						   node->access);
					free(node);
				}
				else
				{
					sendNotFoundResponse();
					return 56;
				}
			}
			else
			{
				sendIncorrectFormatResponse();

				return 57;
			}
		}

		// WRITE

		else if (!strcmp(argv[2], WRITE_C))
		{
			if (argc > 4)
			{
				node_t *node = openNode(argv[3], WRITE);
				if (node == (node_t *)1)
				{
					printf("Innode cannot be created");
					return 61;
				}
				else if (node != NULL)
				{
					int res = writeContent(node, argv[4], db_file);
					if (res == -1)
					{
						printf("Node does not have write permission");
						return 77;
					}
					else
					{
						sendSucessResponse("written!");
					}
				}
				else
				{
					sendNotFoundResponse();
					return 56;
				}
			}
			else
			{
				sendIncorrectFormatResponse();

				return 57;
			}
		}

		// DEFRAGMENT

		else if (!strcmp(argv[2], DEFRAGMENT_C))
		{
			defragment();
		}

		// TREE

		else if (!strcmp(argv[2], TREE_C))
		{
			if (argc > 3)
			{
				node_t *node = openNode(argv[3], READ);
				if (node != NULL)
					ptree(node);
				else
				{
					sendNotFoundResponse();
					return 56;
				}
			}
			else if (argc > 2)
				ptree(db);
		}
		else if (!strcmp(argv[2], RENAME_C))
		{
			if (argc > 4)
			{
				node_t *node = openNode(argv[3], READ);
				node_t *rnc = openNode(argv[4], READ);
				if (node != NULL)
				{
					if (rnc == NULL)
					{
						strcpy(node->name, argv[4]);
						node->uid = hashName(node->name);
						saveNode(node, db_file);
						sendSucessResponse("renaming done!");
					}
					else
					{
						sendOtherErroResponse("THE RENAMING NODE ALREADY EXISTS");
						free(rnc);
					}
					free(node);
				}
				else
				{
					sendNotFoundResponse();
					return 56;
				}
			}
		}
		else if (!strcmp(argv[2], DEL_C))
		{
			if (argc > 3)
			{
				node_t *node = openNode(argv[3], READ);
				if (node != NULL)
				{
					deleteNode(node, db_file);
					sendSucessResponse("node deleted!");
					free(node);
				}
				else
				{
					sendNotFoundResponse();
					return 56;
				}
			}
			else
			{
				sendIncorrectFormatResponse();
				return 57;
			}
		}
		else if (!strcmp(argv[2], "-setperms"))
		{
			if (argc > 4)
			{
				node_t *node = openNode(argv[3], READ);
				if (node != NULL)
				{
					node->access = atoi(argv[4]) % 4;
					saveNode(node, db_file);
					free(node);
				}
				else
				{
					sendNotFoundResponse();
					return 56;
				}
			}
			else
			{


				printf("usage: -setperms [node_path] [perms] //perms must be a value between 0-3 [read,write,read_write]");
				return 57;
			}
		}
		else if (!strcmp(argv[2], POINT_C))
		{
			if (argc > 4)
			{
				node_t *node = openNode(argv[3], READ);
				node_t *noder = openNode(argv[4], WRITE);
				if (noder == (node_t *)1)
				{
					printf("Innode cannot be created");
					return 61;
				}
				if (node != NULL)
				{
					noder->isPointer = true;
					linkNode(noder, node, db_file);
					free(node);
				}
				else
				{
					sendNotFoundResponse();
					return 56;
				}
			}
			else
			{
				sendIncorrectFormatResponse();
				return 57;
			}
		}
		else
		{

			printf("Unknow Option '%s'", argv[2]);
			return 69;
		}
	fseek(db_file, 0, SEEK_END);
	fclose(db_file);
	return 0;
}

node_t *openNode(char *path, enum access_t access)
{
	char temp[MAX_DEPTH][64];
	int depth_index = 0;
	int str_index = 0;
	int n = strlen(path);
	for (int i = 0; i < n; i++)
	{
		if (path[i] != '.')
		{
			temp[depth_index][str_index] = path[i];
			temp[depth_index][++str_index] = 0;

		}
		else
		{

			depth_index++;
			str_index = 0;
		}
	}
		if(str_index==0){
			return NULL;
		}
	node_t *node = getNodeFromAddr(0, db_file);
	for (int i = 0; i < depth_index + 1; i++)
	{
		uintptr_t addr = findChildAddr(node, temp[i], db_file);
		if (addr != 1)
		{
			free((void *)node);
			node = getNodeFromAddr(addr, db_file);
		}
		else if (access == WRITE || access == READ_WRITE)
		{
			if (node->content != NULL)
			{
				return (node_t *)1;
			}
			node_t *cnode = createRNode(temp[i]);
			linkNode(node, cnode, db_file);
			free((void *)node);
			node = cnode;
		}
		else
		{
			return NULL;
		}
	}
	return node;
}

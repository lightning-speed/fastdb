#include <stdio.h>
#include <fastdb.h>
#include <string.h>

char *help = "\ncommands:"
			 "\n -read"
			 "\n -write"
			 "\n -delete"
			 "\n -point"
			 "\n -rename"
			 "\n -tree"
			 "\n -defragment"
			 "\n -setperms"
			 "\n -server"
			 "\n\n";
int main(int argct, char **arge)
{
	if (argct > 1 && (!strcmp(arge[1], "-help") || !strcmp(arge[1], "--help")))
	{
		printf("%s", help);
		return 0;
	}
	else
		input_file_path = arge[1];
	if (argct > 2 && !strcmp(arge[2], "-console"))
	{
		printf("FastDB Version 1.0.0\n");
		runType = CMD_LINE;
		initArgs();
		argct--;
		char buff[1024];
		char *argv[10];
		for (int i = 0; arge[i] != NULL; i++)
		{
			argv[i] = arge[i];
		}
		for (;;)
		{
			char *line = buff;
			printf("\nfastdb:> ");
			scanf("%[^\n]%*c", line);
			int argc = argct;
			argv[argc] = strtok(line, " ");
			while (argv[argc] != NULL)
			{
				argc++;
				argv[argc] = strtok(NULL, " ");
			}
			startDB(argc, argv);
		}
	}
	else if (argct > 2 && !strcmp(arge[2], "-server"))
	{
		runType = SINGLE_EXEC;
		initArgs();

		smain(argct, arge);
	}
	else
	{
		runType = SINGLE_EXEC;
		initArgs();
		startDB(argct, arge);
	}
	printf("\n");

	return 0;
}
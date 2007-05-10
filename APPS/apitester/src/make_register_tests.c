
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#define IO_BUFFER_SIZE 65535
#define TEXT_BUFFER_SIZE 65535
#define HEADER1 "#include \"api_tests.h\"\n\n"
#define HEADER2 "\nstruct test all_tests[] = {\n"
#define HEADER3 "};\nunsigned int number_of_tests = (sizeof(all_tests) / sizeof(all_tests[0]));\n"

static char io_buffer[IO_BUFFER_SIZE];
static char text_buffer[TEXT_BUFFER_SIZE];

char* read(FILE *stream) {
	char *text = text_buffer;
	*text = '\0';
	while(fgets(io_buffer, IO_BUFFER_SIZE, stream)) {
		strcat(text, io_buffer);
	}
	return text;
}


void make_file_register_tests()
{
	FILE *testfile, *registerfile;
	char *tests, *line;
	char header[100], declare[100];
	
	system("grep \"DEFINE\" test* > tests.txt");
	testfile = fopen("tests.txt", "r");
	if (!testfile) 
	{
		fprintf(stderr, "Cannot open file [%s] for writing: %s\n", "tests.txt", strerror(errno));
		exit(-1);
	}
	else
	{
		tests = read(testfile);
		fclose (testfile);
		registerfile = fopen("register.c","w");
		if (!registerfile) 
		{
			fprintf(stderr, "Cannot open file [%s] for writing: %s\n", "register.c", strerror(errno));
			exit(-1);
		}
		else
		{
			strcpy(header, HEADER1);
			fwrite(header, strlen(header),1,registerfile);
			line = strtok (tests, "(");
			while (line)
			{	
				line = strtok(NULL, ")");
				if (line)
				{
					declare[0] = '\0';
					strcat(declare,"DECLARE_APITEST(");
					strcat(declare,line);
					strcat(declare,");\n");
					fwrite(declare, strlen(declare), 1, registerfile);
					line = strtok(NULL, "(");
				}
				else
					break;	
			}
			testfile = NULL;
			testfile = fopen("tests.txt", "r");
			tests=NULL;
			tests = read(testfile);
			fclose (testfile);
			strcpy(header, HEADER2);
			fwrite(header, strlen(header),1,registerfile);
			line = strtok (tests, "(");
			while (line)
			{			
				line = strtok(NULL, ")");
				if (line)
				{
					declare[0] = '\0';
					strcat(declare,"\tREGISTER_APITEST(");
					strcat(declare,line);
					strcat(declare,"),\n");
					fwrite(declare, strlen(declare), 1, registerfile);
					line = strtok(NULL, "(");
				}
				else
					break;	
			}
			strcpy(header, HEADER3);
			fwrite(header, strlen(header),1,registerfile);
		}
	}
	
	fclose (registerfile);
}


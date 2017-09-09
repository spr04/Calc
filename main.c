#include "Header.h"
//#include <crtdbg.h> 
// TODO: Include local header files

// TODO: Use all unused parameters and remove this temporary definition
// Helps eliminating warning C4100: 'error': unreferenced formal parameter
#define UNUSED_PARAMETER(name) (void)name

//////////////////////////////////////////////////////////////////////////////
// Dummy calc module

unsigned char s[4] = "kek";
int F_END = 0;

// TODO: Move to a separate module 'calc'
unsigned char const* GetErrorString(error_t error)
{
	// TODO: Find the corresponding error description
	UNUSED_PARAMETER(error);
	return s;
}

// TODO: Move to a separate module 'calc'
double Calculate(unsigned char const* expression, error_t* error)
{
	double result;

	// TODO: Replace with a computational algorithm subdivided into modules/functions
	UNUSED_PARAMETER(expression);

	result = am1_calculate(expression, error);
	return result;
}

//////////////////////////////////////////////////////////////////////////////
// UI functions
unsigned char* ReadLine(error_t * err, FILE* in)
{
	unsigned char* line = NULL;
	unsigned char * p = NULL;  
	int i = 0, max = 10;
	int c;

	if (feof(in))
	{
		*err = ERR_EOF1;
		return NULL;
	}
	else if (ferror(in))
	{
		*err = ERR_EOF1_ERR;
		return line;
	}
	// TODO: Read a line of text into a dynamic memory block
	if ((line = (unsigned char *)malloc((max + 1) * sizeof(unsigned char))) == NULL)
	{
		int d;
		do { d = fgetc(in); }while ( d != '\n' && d != EOF1 && d != EOF);
		*err = ERR_NOT_ENOUGH_MEMORY;
		if (d == EOF1 || d == EOF)
			*err = ERR_EOF1_ERR;
		return 0;
	}
	c = fgetc(in);
	if (c == EOF1 || c == EOF)
		if (i == 0)
		{
			if(line!=NULL)
			free(line);
			*err = ERR_EOF1;
			return 0;
		}
		else
		{
			line[i] = 0;
			return line;
		}
	line[0] = (unsigned char)c;
	while (line[i] != '\n') 
	{
		i++;
		c = fgetc(in);
		if (c == EOF1 || c == EOF)
			if (i == 0)
			{
				if (line != NULL)
				free(line);
				*err = ERR_EOF1;
				return 0;
			}
			else
			{
				line[i] = 0;
				return line;
			}
		line[i] = (unsigned char)c;

		if (ferror(in))
		{
			*err = ERR_UNNAMED;
			if (line != NULL)
			free(line);
			line = NULL;
			return line;
		}
		if (i >= max)
		{
			max *= 2;
			
			if ((p = realloc(line, (max + 1) * sizeof(unsigned char))) == NULL)
			{
				line = p;
				*err = ERR_NOT_ENOUGH_MEMORY;
				int d = 0;
				do { d = fgetc(in); } while (d != '\n' && d != EOF1 && d != EOF);
				if (d == EOF1 || d == EOF)
					*err = ERR_EOF1_ERR;
				if (line != NULL)
				free(line);
				line = NULL;
				return line;
			}
			//free(line);
			line = p;
		}
	}

	if (ferror(in))
	{
		*err = ERR_UNNAMED;
		if (line != NULL)
		free(line);
		line = NULL;
		return line;
	}
	if (line[0] == EOF || line[0] == EOF1)
	{
		*err = ERR_EOF1;
		if (line != NULL)
		free(line);
		return NULL;
	}
	line[i] = 0;
	max = 10;
	UNUSED_PARAMETER(in);/// гдеяэ асдер ньхайю оюлъ
	return line;
}

int NeedCalculate(const unsigned char* line)
{
	int i;
	int d = 1;

	// TODO: Determine if the line contains an expression

	for (i = 0; i < (int)strlen((const char *)line) - 1; i++)
	{
		if (line[i] != ' ' && d && line[i] != '/')
			d = 0;
		if (line[i] == '/' && line[i + 1] == '/' && d)
		{
			return 2;
			break;
		}
	}
	for (i = 0; i < (int)strlen((const char *)line); i++)
		if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r' && line[i] != '\t')
		{
			return 0;
		}
	return 2;
	UNUSED_PARAMETER(line);
}

error_t ReportError(error_t error)
{
	printf("ERROR: %s\n", GetErrorString(error));
	return error;
}

void ProcessLine(unsigned char const* line)
{
	error_t lastError = ERR_OK;
	double result;

	switch (NeedCalculate(line))
	{
	case 2:
		puts((const char *)line);
		break;
	case 0:
		printf("%s == ", line);
		result = (double)Calculate(line, &lastError);
		if (lastError == ERR_OK)
			printf("%g\n", result);
		else
			ReportError(lastError);
		break;
	}
}

int main(int argc, unsigned char const* argv[])
{
	FILE* in = stdin;
	//argv[1] = "22.txt";
	//argc = 2;
	unsigned char* line = NULL;
	error_t err = ERR_OK; 
	//_CrtMemState _ms;
	//_CrtMemCheckpoint(&_ms);
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

	//argc = 2;
	//argv[1] = "l.txt";
	// Choose an input source
	if (argc == 2 && (fopen_s(&in, (char *)argv[1], "r")) == EINVAL)
	{
		printf("ERROR: Cannot open file\n");
		if (in != stdin)
			fclose(in);
		return 0;
	}
	else if (argc > 2)
	{
		printf("ERROR: too many arguments\n");
		if (in != stdin)
			fclose(in);
		return 0;
	}

	// Process the data line by line
	while (((line = ReadLine(&err, in)) != NULL || err != ERR_EOF1) && err != ERR_EOF1_ERR)
	{
		if (line != NULL && err == ERR_EOF1)
		{
			ProcessLine(line);
			err = ERR_OK;
			break;
		}
		else if (err != ERR_OK)
		{
			printf("ERROR: kek\n");
			err = ERR_OK;
		}
		else
			ProcessLine(line);
		if (line != NULL)
		{
			free(line);
			line = NULL;
		}
		//_CrtDumpMemoryLeaks();
		//_CrtMemDumpAllObjectsSince(&_ms);
	}

	if (line != NULL)
		free(line);
	if (err == ERR_EOF1)
	{
		if (in != stdin)
			fclose(in);
		return 0;
	}
	if (err != ERR_OK)
	{
		printf("ERROR: kek\n");
		if (in != stdin)
			fclose(in);
		return 0;
	}
	//_CrtDumpMemoryLeaks();
	//_CrtMemDumpAllObjectsSince(&_ms);

	// Clean up
	if (in != stdin)
		fclose(in);
	//_CrtDumpMemoryLeaks();
	//system("pause");
	return 0;
}

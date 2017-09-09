//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*#ifdef _DEBUG 
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h> 
#endif*/
#define MAX 20
#define EOF1 255

// TODO: Move to a separate module 'calc'
typedef enum
{
	ERR_OK,
	ERR_NOT_ENOUGH_MEMORY,
	ERR_UNNAMED,
	ERR_EOF1,
	ERR_EOF1_ERR
} error_t;


double am1_calculate(unsigned char const* expression, error_t * error);
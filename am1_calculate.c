#include "Header.h"
#include <math.h>

#define c_E        2.71828182845904523536  
#define c_PI       3.14159265358979323846

struct node;
struct outnode;

struct node
{
	unsigned char c;
	struct node *next;
};

struct	outnode
{
	int mode;
	unsigned char c;
	double digit;
	struct outnode *next;
	struct outnode *prev;
};

typedef struct node node;
typedef struct outnode outnode;

struct read
{
	node * Opers;
	outnode * Out;
};

typedef struct read read;

outnode *outpush(error_t *err, outnode *head, outnode n)
{
	outnode *Newhead;
	if ((Newhead = (outnode *)malloc(sizeof(outnode))) == NULL)
	{
		//printf("ERROR: memory\n");
		*err = ERR_UNNAMED;
		return NULL;
	}
	Newhead->mode = n.mode;
	Newhead->digit = n.digit;
	Newhead->c = n.c;
	Newhead->next = head;
	return Newhead;
}

node *push(error_t *err, node *head, unsigned char a)
{
	node *Newhead;
	if ((Newhead = malloc(sizeof(node))) == NULL)
	{
		//printf("ERROR: memory\n");
		*err = ERR_UNNAMED;
		return NULL;
	}
	Newhead->c = a;
	Newhead->next = head;
	return Newhead;
}

unsigned char pop(node **head)
{
	node *NewHead;
	unsigned char c;

	if (*head == NULL)
		return 0;
	NewHead = *head;
	c = NewHead->c;
	*head = NewHead->next;
	if(NewHead!=NULL)
	free(NewHead);
	return c;
}

outnode outpop(outnode **head)
{
	outnode *NewHead;
	outnode c = {-1, 0, 0, NULL};

	if (*head == NULL)
		return c;
	NewHead = *head;
	c.digit = NewHead->c;
	c.mode = NewHead->mode;
	*head = NewHead->next;
	if(NewHead!=NULL)
	free(NewHead);
	return c;
}

int priority(unsigned char a)
{
	switch (a)
	{
	case 's':
	case 'c':
	case 'S':
	case 'C':
	case 't':
	case 'T':
	case 'l':
	case 'g':
	case 'f':
	case 'i':
	case 'q':
	case 'm':
	case '^':
		return 4;
	case '*':
		return 3;
	case '/':
		return 3;
	case '-':
		return 2;
	case '+':
		return 2;
	case '(':
		return 1;
	default:
		return -1;
	}
}

/*double digit(const unsigned char * expr, int *i, unsigned char c1)
{
	double result = 0;
	unsigned char a[64] = { 0 };
	int j = 1, k = 1;
	double l = 1;
	int f = 0;

	a[0] = c1;
	(*i)++;
	while (expr[*i] < '9' && expr[*i] > '0' && expr[*i] != '.' && expr[*i] != '\n' && expr[*i] != EOF1)
	{
		a[j] = expr[*i];
		(*i)++;
		j++;
	}
	j--;
	if (expr[*i] == '.')
	{
		f = 1;
		(*i)++;
		while (expr[*i] < '9' && expr[*i] > '0' && expr[*i] != '\n' && expr[*i] != EOF1)
		{
			a[j + k] = expr[*i];
			(*i)++;
			k++;
		}
	}
	k--;
	int oldj = j;
	for (;j != -1; j--)
	{
		result += (a[j] - '0') * l;
		l *= 10;
	}
	j = oldj;
	l = 10;
	oldj++;
	if (f)
	for (; oldj != k + j + 1; oldj++)
	{
		result += (double)(a[oldj] - '0') / l;
		l *= 10;
	}
	int i1;
	for (i1 = 0; i1 < 64; i1++)
		a[i1] = 0;
	*i--;
	return result;
}
*/

void delMem( error_t * err, node * Opers, outnode * Out )
{
	while (Opers != NULL)
	{
		int f = 0;
		node * d2 = Opers;
		node * d1 = NULL;
		while (d2->next != NULL)
		{
			f = 1;
			d1 = d2;
			d2 = d2->next;
		}
		if(d2!=NULL)
		free(d2);
		if (d1 != NULL)
			d1->next = NULL;
		if (!f)
			Opers = NULL;
	}
	while (Out != NULL)
	{
		int f = 0;
		outnode * d2 = Out;
		outnode * d1 = NULL;
		while (d2->next != NULL)
		{
			f = 1;
			d1 = d2;
			d2 = d2->next;
		} 
		if (d2 != NULL)
		free(d2);
		if (d1 != NULL)
			d1->next = NULL;
		if (!f)
			Out = NULL;
	}
	*err = ERR_UNNAMED;
	Opers = NULL;
	Out = NULL;
}

double read_expression(unsigned char const* expression, error_t * err)
{
	read re;
	int k;

	for (k = 0; expression[k] != 0;)
	{
		if (expression[k] == 'e' || expression[k] == 'E')
		{
			outnode n;
			n.digit = c_E;
			n.c = 0;
			n.mode = 1;
			re.Out = outpush(err, re.Out, n);
			if (*err != ERR_OK)
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
			k++;
			continue;
		}
		else if (expression[k] == 'p' || expression[k] == 'P')
			if (expression[k + 1] == 'i' || expression[k + 1] == 'I')
			{
				outnode n;
				n.digit = c_PI;
				n.c = 0;
				n.mode = 1;
				re.Out = outpush(err, re.Out, n);
				if (*err != ERR_OK)
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
				k += 2;
				continue;
			}
			else
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
		else if (expression[k] == 'l')
			if (expression[k + 1] == 'n')
			{
				if (expression[k + 2] != '(')
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
				re.Opers = push(err, re.Opers, 'l');
				if (*err != ERR_OK)
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
				k += 2;
				continue;
			}
			else
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
		else if (expression[k] == 's')
		{
			if (expression[k + 1] == 'i')
				if (expression[k + 2] == 'n')
				{
					if (expression[k + 3] != '(')
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
					re.Opers = push(err, re.Opers, 's');
					if (*err != ERR_OK)
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
					k += 3;
					continue;
				}
				else
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
			else if (expression[k + 1] == 'q')
				if (expression[k + 2] == 'r')
					if (expression[k + 3] == 't')
					{
						if (expression[k + 4] != '(')
						{
							delMem(err, re.Opers, re.Out);
							return 0;
						}
						re.Opers = push(err, re.Opers, 'q');
						if (*err != ERR_OK)
						{
							delMem(err, re.Opers, re.Out);
							return 0;
						}
						k += 4;
						continue;
					}
					else
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
				else
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
			else
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
		}

		else if (expression[k] == 'f')
		{
			if (expression[k + 1] == 'l')
				if (expression[k + 2] == 'o')
					if (expression[k + 3] == 'o')
						if (expression[k + 4] == 'r')
						{
							if (expression[k + 5] != '(')
							{
								delMem(err, re.Opers, re.Out);
								return 0;
							}
							re.Opers = push(err, re.Opers, 'f');
							if (*err != ERR_OK)
							{
								delMem(err, re.Opers, re.Out);
								return 0;
							}
							k += 5;
							continue;
						}
						else
						{
							delMem(err, re.Opers, re.Out);
							return 0;
						}
					else
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
				else
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
			else
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
		}
		else if (expression[k] == 't')
			if (expression[k + 1] == 'g')
			{
				if (expression[k + 2] != '(')
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
				re.Opers = push(err, re.Opers, 't');
				if (*err != ERR_OK)
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
				k += 2;
				continue;
			}
			else
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
		else if (expression[k] == 'c')
		{
			if (expression[k + 1] == 'e')
				if (expression[k + 2] == 'i')
					if (expression[k + 3] == 'l')
					{
						if (expression[k + 4] != '(')
						{
							delMem(err, re.Opers, re.Out);
							return 0;
						}
						re.Opers = push(err, re.Opers, 'i');
						if (*err != ERR_OK)
						{
							delMem(err, re.Opers, re.Out);
							return 0;
						}
						k += 4;
						continue;
					}
					else
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
				else
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
			else if (expression[k + 1] == 'o')
				if (expression[k + 2] == 's')
				{
					if (expression[k + 3] != '(')
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
					re.Opers = push(err, re.Opers, 'c');
					if (*err != ERR_OK)
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
					k += 3;
					continue;
				}
				else
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
			else if (expression[k + 1] == 't')
				if (expression[k + 2] == 'g')
				{
					if (expression[k + 3] != '(')
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
					re.Opers = push(err, re.Opers, 'g');
					if (*err != ERR_OK)
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
					k += 3;
					continue;
				}
				else
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
			else
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
		}

		else if (expression[k] == 'a')
		{
			if (expression[k + 1] != 'r')
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
			if (expression[k + 2] != 'c')
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
			k += 2;
			if (expression[k + 1] == 's')
			{
				if (expression[k + 2] == 'i')
					if (expression[k + 3] == 'n')
					{
						if (expression[k + 4] != '(')
						{
							delMem(err, re.Opers, re.Out);
							return 0;
						}
						re.Opers = push(err, re.Opers, 'S');
						if (*err != ERR_OK)
						{
							delMem(err, re.Opers, re.Out);
							return 0;
						}
						k += 4;
						continue;
					}
					else
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
				else
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
			}
			else if (expression[k + 1] == 't')
				if (expression[k + 2] == 'g')
				{
					if (expression[k + 3] != '(')
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
					re.Opers = push(err, re.Opers, 'T');
					if (*err != ERR_OK)
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
					k += 3;
					continue;
				}
				else
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
			else if (expression[k + 1] == 'c')
			{
				if (expression[k + 2] == 'o')
					if (expression[k + 3] == 's')
					{
						if (expression[k + 4] != '(')
						{
							delMem(err, re.Opers, re.Out);
							return 0;
						}
						re.Opers = push(err, re.Opers, 'C');
						if (*err != ERR_OK)
						{
							delMem(err, re.Opers, re.Out);
							return 0;
						}
						k += 4;
						continue;
					}
					else
					{
						delMem(err, re.Opers, re.Out);
						return 0;
					}
				else
				{
					delMem(err, re.Opers, re.Out);
					return 0;
				}
			}
			else
			{
				delMem(err, re.Opers, re.Out);
				return 0;
			}
		}
	}
	else if (expression[k] == '(')
	{
		int k5;
		for (k5 = k + 1; isspace(expression[k5]); k5++);
		if (!(isdigit(expression[k5]) || expression[k5] == '('

			|| expression[k5] == 'a'
			|| expression[k5] == 's'
			|| expression[k5] == 'c'
			|| expression[k5] == 'f'
			|| expression[k5] == 's'
			|| expression[k5] == 't'
			|| expression[k5] == 'l'
			|| expression[k5] == '-'
			|| expression[k5] == 'e' || expression[k5] == 'E' || ((expression[k5 + 1] == 'i' || expression[k5 + 1] == 'I') && (expression[k5] == 'p' || expression[k5] == 'P'))))
		{
			delMem(err, Opers, Out);
			*err = ERR_UNNAMED;
			return 0;
		}
		Opers = push(err, Opers, '(');
		if (*err != ERR_OK)
		{
			delMem(err, Opers, Out);
			return 0;
		}
		k++;
		continue;
	}

	if (expression[k] == ')')
	{
		int k4;
		for (k4 = k - 1; isspace(expression[k4]); k4--);
		if (Opers == NULL || expression[k4] == '(')
		{
			delMem(err, Opers, Out);
			*err = ERR_UNNAMED;
			return 0;
		}
		for (k4 = k - 1; isspace(expression[k4]); k4--);
		if (!(isdigit(expression[k4]) || expression[k4] == ')' || expression[k4] == 'e' || expression[k4] == 'E' || ((k4 - 1 >= 0) && ((expression[k4] == 'i' || expression[k4] == 'I') && (expression[k4 - 1] == 'p' || expression[k4 - 1] == 'P')))))
		{
			delMem(err, Opers, Out);
			*err = ERR_UNNAMED;
			return 0;
		}
		while (Opers->c != '(')
		{
			outnode n;
			n.c = pop(&Opers);
			n.digit = 0;
			n.mode = 0;
			Out = outpush(err, Out, n);
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
			if (Opers == NULL)
			{
				delMem(err, Opers, Out);
				*err = ERR_UNNAMED;
				return 0;
			}

		}
		if (Opers->c == '(')
		{
			;
		}
		else
		{
			delMem(err, Opers, Out);
			*err = ERR_UNNAMED;
			return 0;
		}
		pop(&Opers);
		if (Opers != NULL)
		{
			outnode n;
			switch (Opers->c)
			{
			case 's':
			case 'c':
			case 'S':
			case 'C':
			case 't':
			case 'T':
			case 'l':
			case 'g':
			case 'f':
			case 'i':
			case 'q':
			case 'm':
				n.c = pop(&Opers);
				n.digit = 0;
				n.mode = 0;
				Out = outpush(err, Out, n);
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
				break;
			default:
				;
			}
		}
	}
	else if (expression[k] == '.' && (expression[k + 1] >= '0' && expression[k + 1] <= '9'))
	{
		char *ptr1 = (char *)(&expression[k]);
		char *ptr2 = NULL;
		outnode n;
		n.digit = strtod(ptr1, &ptr2);
		if (ptr1 == ptr2)
		{
			delMem(err, Opers, Out);
			return 0;
		}
		n.c = 0;
		n.mode = 1;
		while ((char *)&expression[k] != ptr2)
			k++;
		if (expression[k] == 'f' || expression[k] == 'F')
		{
			n.digit = (double)((float)(n.digit));
			k++;
		}
		if (expression[k] == 'l' || expression[k] == 'L')
			k++;
		Out = outpush(err, Out, n);
		if (*err != ERR_OK)
		{
			delMem(err, Opers, Out);
			return 0;
		}
		continue;
	}
	else if (expression[k] >= '0' && expression[k] <= '9')
	{
		char *ptr1 = (char *)(&expression[k]);
		char *ptr2 = NULL;
		outnode n;
		n.digit = strtod(ptr1, &ptr2);
		if (ptr1 == ptr2)
		{
			delMem(err, Opers, Out);
			return 0;
		}
		n.c = 0;
		n.mode = 1;
		while ((char *)&expression[k] != ptr2)
			k++;
		if (expression[k] == 'f' || expression[k] == 'F')
		{
			n.digit = (double)((float)(n.digit));
			k++;
		}
		if (expression[k] == 'l' || expression[k] == 'L')
			k++;
		Out = outpush(err, Out, n);
		if (*err != ERR_OK)
		{
			delMem(err, Opers, Out);
			return 0;
		}
		continue;
	}
	else if (expression[k] == '-')
	{
		if (k == 0)
		{
			Opers = push(err, Opers, 'm');
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
			k++;
			continue;
		}
		int k4;
		for (k4 = k - 1; isspace(expression[k4]); k4--);
		int k5;
		for (k5 = k + 1; isspace(expression[k5]); k5++);
		if ((expression[k4] == '+'
			|| expression[k4] == '/'
			|| expression[k4] == '-'
			|| expression[k4] == '*'
			|| expression[k4] == '('
			|| expression[k4] == '^') && ((expression[k5] >= '0' && expression[k5] <= '9')
				|| expression[k5] == '('
				|| expression[k5] == '-'
				|| expression[k5] == 'p'
				|| expression[k5] == 'P'
				|| expression[k5] == 'e'
				|| expression[k5] == 'E'
				|| expression[k5] == 's'
				|| expression[k5] == 'c'
				|| expression[k5] == 'l'
				|| expression[k5] == 't'
				|| expression[k5] == 'g'
				|| expression[k5] == 'S'
				|| expression[k5] == 'C'
				|| expression[k5] == 'T'
				|| expression[k5] == 'f'
				|| expression[k5] == 'a'
				))
		{
			if (Opers == NULL)
			{
				Opers = push(err, Opers, 'm');
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
			else if (Opers->c == 'm')
			{
				pop(&Opers);
				k++;
				continue;
			}
			else if (priority(Opers->c) <= priority('m'))
			{
				Opers = push(err, Opers, 'm');
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
			else
			{
				while (priority(Opers->c) > priority('m'))
				{
					outnode n;
					n.c = pop(&Opers);
					n.digit = 0;
					n.mode = 0;
					Out = outpush(err, Out, n);
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
					if (Opers == NULL)
						break;
				}
				Opers = push(err, Opers, 'm');
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
		}
		else
		{
			if (Opers == NULL)
			{
				Opers = push(err, Opers, expression[k]);
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
			else if (priority(Opers->c) < priority(expression[k]))
			{
				Opers = push(err, Opers, expression[k]);
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
			else
			{
				while (priority(Opers->c) >= priority(expression[k]))
				{
					outnode n;
					n.c = pop(&Opers);
					n.digit = 0;
					n.mode = 0;
					Out = outpush(err, Out, n);
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
					if (Opers == NULL)
						break;
				}
				Opers = push(err, Opers, '-');
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
		}
	}
	else if (expression[k] == '^')
	{
		Opers = push(err, Opers, expression[k]);
		if (*err != ERR_OK)
		{
			delMem(err, Opers, Out);
			return 0;
		}
	}
	else if (expression[k] == '+'
		|| expression[k] == '/'
		|| expression[k] == '*')
	{
		if (Opers == NULL)
		{
			Opers = push(err, Opers, expression[k]);
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
		}
		else if (priority(Opers->c) < priority(expression[k]))
		{
			Opers = push(err, Opers, expression[k]);
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
		}
		else
		{
			while (priority(Opers->c) >= priority(expression[k]))
			{
				outnode n;
				n.c = pop(&Opers);
				n.digit = 0;
				n.mode = 0;
				Out = outpush(err, Out, n);
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
				if (Opers == NULL)
					break;
			}
			Opers = push(err, Opers, expression[k]);
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
		}
	}
	else if (isspace(expression[k]))
	;
	else
	{
		delMem(err, Opers, Out);
		return 0;
	}
	k++;
	}
double am1_calculate(unsigned char const* expression, error_t * err)
{
	node * Opers = NULL;
	outnode * Out = NULL;
	outnode * O = NULL;
	int k;
	double result = 0;

	for (k = 0; expression[k] != 0;)
	{
		if (expression[k] == 'e' || expression[k] == 'E')
		{
			outnode n;
			n.digit = c_E;
			n.c = 0;
			n.mode = 1;
			Out = outpush(err,Out, n);
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
			k++;
			continue;
		}
		else if (expression[k] == 'p' || expression[k] == 'P')
			if (expression[k + 1] == 'i' || expression[k + 1] == 'I')
			{
				outnode n;
				n.digit = c_PI;
				n.c = 0;
				n.mode = 1;
				Out = outpush(err,Out, n);
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
				k += 2;
				continue;
			}
			else
			{
				delMem(err, Opers, Out);
				return 0;
			}
		else if (expression[k] == 'l')
			if (expression[k + 1] == 'n')
			{
				if (expression[k + 2] != '(')
				{
					delMem(err, Opers, Out);
					return 0;
				}
				Opers = push(err,Opers, 'l');
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
				k += 2;
				continue;
			}
			else
			{
				delMem(err, Opers, Out);
				return 0;
			}
		else if (expression[k] == 's')
		{
			if (expression[k + 1] == 'i')
				if (expression[k + 2] == 'n')
				{
					if (expression[k + 3] != '(')
					{
						delMem(err, Opers, Out);
						return 0;
					}
					Opers = push(err,Opers, 's');
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
					k += 3;
					continue;
				} 
				else
				{
					delMem(err, Opers, Out);
					return 0;
				}
			else if (expression[k + 1] == 'q')
				if (expression[k + 2] == 'r')
					if (expression[k + 3] == 't')
					{
						if (expression[k + 4] != '(')
						{
							delMem(err, Opers, Out);
							return 0;
						}
						Opers = push(err,Opers, 'q');
						if (*err != ERR_OK)
						{
							delMem(err, Opers, Out);
							return 0;
						}
						k += 4;
						continue;
					}
					else
					{
						delMem(err, Opers, Out);
						return 0;
					}
				else
				{
					delMem(err, Opers, Out);
					return 0;
				}
			else
			{
				delMem(err, Opers, Out);
				return 0;
			}
		}

		else if (expression[k] == 'f')
		{
			if (expression[k + 1] == 'l')
				if (expression[k + 2] == 'o')
					if (expression[k + 3] == 'o')
						if (expression[k + 4] == 'r')
						{
							if (expression[k + 5] != '(')
							{
								delMem(err, Opers, Out);
								return 0;
							}
							Opers = push(err,Opers, 'f');
							if (*err != ERR_OK)
							{
								delMem(err, Opers, Out);
								return 0;
							}
							k += 5;
							continue;
						}
						else
						{
							delMem(err, Opers, Out);
							return 0;
						}
					else
					{
						delMem(err, Opers, Out);
						return 0;
					}
				else
				{
					delMem(err, Opers, Out);
					return 0;
				}
			else
			{
				delMem(err, Opers, Out);
				return 0;
			}
		}
		else if (expression[k] == 't')
			if (expression[k + 1] == 'g')
			{
				if (expression[k + 2] != '(')
				{
					delMem(err, Opers, Out);
					return 0;
				}
				Opers = push(err,Opers, 't');
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
				k += 2;
				continue;
			}
			else
			{
				delMem(err, Opers, Out);
				return 0;
			}
		else if (expression[k] == 'c')
		{
			if (expression[k + 1] == 'e')
				if (expression[k + 2] == 'i')
					if (expression[k + 3] == 'l')
					{
						if (expression[k + 4] != '(')
						{
							delMem(err, Opers, Out);
							return 0;
						}
						Opers = push(err,Opers, 'i');
						if (*err != ERR_OK)
						{
							delMem(err, Opers, Out);
							return 0;
						}
						k += 4;
						continue;
					}
					else
					{
						delMem(err, Opers, Out);
						return 0;
					}
				else
				{
					delMem(err, Opers, Out);
					return 0;
				}
			else if (expression[k + 1] == 'o')
				if (expression[k + 2] == 's')
				{
					if (expression[k + 3] != '(')
					{
						delMem(err, Opers, Out);
						return 0;
					} 
					Opers = push(err,Opers, 'c');
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
					k += 3; 
					continue;
				}
				else
				{
					delMem(err, Opers, Out);
					return 0;
				}
			else if (expression[k + 1] == 't')
				if (expression[k + 2] == 'g')
				{
					if (expression[k + 3] != '(')
					{
						delMem(err, Opers, Out);
						return 0;
					}
					Opers = push(err,Opers, 'g');
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
					k += 3;
					continue;
				}
				else
				{
					delMem(err, Opers, Out);
					return 0;
				}
			else
			{
				delMem(err, Opers, Out);
				return 0;
			}
		} 
		
		else if (expression[k] == 'a')
		{
			if (expression[k + 1] != 'r')
			{
				delMem(err, Opers, Out);
				return 0;
			}
			if (expression[k + 2] != 'c')
			{
				delMem(err, Opers, Out);
				return 0;
			}
			k += 2;
			if (expression[k + 1] == 's')
			{
				if (expression[k + 2] == 'i')
					if (expression[k + 3] == 'n')
					{
						if (expression[k + 4] != '(')
						{
							delMem(err, Opers, Out);
							return 0;
						}
						Opers = push(err,Opers, 'S');
						if (*err != ERR_OK)
						{
							delMem(err, Opers, Out);
							return 0;
						}
						k += 4;
						continue;
					}
					else
					{
						delMem(err, Opers, Out);
						return 0;
					}
				else
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
			else if (expression[k+1] == 't')
				if (expression[k + 2] == 'g')
				{
					if (expression[k + 3] != '(')
					{
						delMem(err, Opers, Out);
						return 0;
					}
					Opers = push(err,Opers, 'T');
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
					k += 3;
					continue;
				}
				else
				{
					delMem(err, Opers, Out);
					return 0;
				}
			else if (expression[k+1] == 'c')
			{
				if (expression[k + 2] == 'o')
					if (expression[k + 3] == 's')
					{
						if (expression[k + 4] != '(')
						{
							delMem(err, Opers, Out);
							return 0;
						}
						Opers = push(err,Opers, 'C');
						if (*err != ERR_OK)
						{
							delMem(err, Opers, Out);
							return 0;
						}
						k += 4;
						continue;
					}
					else
					{
						delMem(err, Opers, Out);
						return 0;
					}
				else
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
			else
			{
				delMem(err, Opers, Out);
				return 0;
			}
		}

		///////////////////


		else if (expression[k] == '(')
		{
			int k5;
			for (k5 = k + 1; isspace(expression[k5]); k5++);
			if (!(isdigit(expression[k5]) || expression[k5] == '('

				|| expression[k5] == 'a'
				|| expression[k5] == 's'
				|| expression[k5] == 'c'
				|| expression[k5] == 'f'
				|| expression[k5] == 's'
				|| expression[k5] == 't'
				|| expression[k5] == 'l'
				|| expression[k5] == '-'
				|| expression[k5] == 'e' || expression[k5] == 'E' || ((expression[k5 + 1] == 'i' || expression[k5 + 1] == 'I') && (expression[k5] == 'p' || expression[k5] == 'P'))))
			{
				delMem(err, Opers, Out);
				*err = ERR_UNNAMED;
				return 0;
			}
			Opers = push(err,Opers, '(');
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
			k++;
			continue;
		}

		if (expression[k] == ')')
		{
			int k4;
			for (k4 = k - 1; isspace(expression[k4]); k4--);
			if (Opers == NULL || expression[k4] == '(')
			{
				delMem(err, Opers, Out);
				*err = ERR_UNNAMED;
				return 0;
			}
			for (k4 = k - 1; isspace(expression[k4]); k4--);
			if (!(isdigit(expression[k4]) || expression[k4] == ')' || expression[k4] == 'e' || expression[k4] == 'E' || ((k4 - 1 >= 0) && ((expression[k4] == 'i' || expression[k4] == 'I') && (expression[k4 - 1] == 'p' || expression[k4 - 1] == 'P')))))
			{
				delMem(err, Opers, Out);
				*err = ERR_UNNAMED;
				return 0;
			}
			while (Opers->c != '(') 
			{
				outnode n;
				n.c = pop(&Opers);
				n.digit = 0;
				n.mode = 0;
		 		Out = outpush(err,Out, n);
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
				if (Opers == NULL)
				{
					delMem(err, Opers, Out);
					*err = ERR_UNNAMED;
					return 0;
				}

			}
			if (Opers->c == '(')
			{
				;
			}
			else
			{
				delMem(err, Opers, Out);
				*err = ERR_UNNAMED;
				return 0;
			}
			pop(&Opers);
			if (Opers != NULL)
			{
				outnode n;
				switch (Opers->c)
				{
				case 's':
				case 'c':
				case 'S':
				case 'C':
				case 't':
				case 'T':
				case 'l':
				case 'g':
				case 'f':
				case 'i':
				case 'q':
				case 'm':
					n.c = pop(&Opers);
					n.digit = 0;
					n.mode = 0;
					Out = outpush(err,Out, n);
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
					break;
				default:
					;
				}
			}
		}
		else if (expression[k] == '.' && (expression[k+1] >= '0' && expression[k+1] <= '9'))
		{
			char *ptr1 = (char *)(&expression[k]);
			char *ptr2 = NULL;
			outnode n;
			n.digit = strtod(ptr1, &ptr2);
			if (ptr1 == ptr2)
			{
				delMem(err, Opers, Out);
				return 0;
			}
			n.c = 0;
			n.mode = 1;
			while ((char *)&expression[k] != ptr2)
				k++;
			if (expression[k] == 'f' || expression[k] == 'F')
			{
				n.digit = (double)((float)(n.digit));
				k++;
			}
			if (expression[k] == 'l' || expression[k] == 'L')
				k++;
			Out = outpush(err,Out, n);
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
			continue;
		}
		else if (expression[k] >= '0' && expression[k] <= '9')
		{
			char *ptr1 = (char *)(&expression[k]);
			char *ptr2 = NULL;
			outnode n;
			n.digit = strtod(ptr1, &ptr2);
			if (ptr1 == ptr2)
			{
				delMem(err, Opers, Out);
				return 0;
			}
			n.c = 0;
			n.mode = 1;
			while ((char *)&expression[k] != ptr2)
				k++;
			if (expression[k] == 'f' || expression[k] == 'F')
			{
				n.digit = (double)((float)(n.digit));
				k++;
			}
			if (expression[k] == 'l' || expression[k] == 'L')
				k++;
			Out = outpush(err,Out, n);
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
			continue;
		}
		else if (expression[k] == '-')
		{
			if (k == 0)
			{
				Opers = push(err,Opers, 'm');
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
				k++;
				continue;
			}
			int k4;
			for (k4 = k - 1; isspace(expression[k4]); k4--);
			int k5;
			for (k5 = k + 1; isspace(expression[k5]); k5++);
			if ((expression[k4] == '+'
				|| expression[k4] == '/'
				|| expression[k4] == '-'
				|| expression[k4] == '*'
				|| expression[k4] == '('
				|| expression[k4] == '^') && ((expression[k5] >= '0' && expression[k5] <= '9') 
					                          || expression[k5] == '(' 
					                          || expression[k5] == '-' 
					                          || expression[k5] == 'p' 
					                          || expression[k5] == 'P' 
					                          || expression[k5] == 'e' 
					                          || expression[k5] == 'E'
											  || expression[k5] == 's'
											  || expression[k5] == 'c'
					                          || expression[k5] == 'l'
											  || expression[k5] == 't'
											  || expression[k5] == 'g'
												|| expression[k5] == 'S'
												|| expression[k5] == 'C'
												|| expression[k5] == 'T'
												|| expression[k5] == 'f'
					                            || expression[k5] == 'a'
					))
			{ 
				if (Opers == NULL)
				{
					Opers = push(err, Opers, 'm');
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
				}
				else if (Opers->c == 'm')
				{
					pop(&Opers);
					k++;
					continue;
				}
				else if (priority(Opers->c) <= priority('m'))
				{
					Opers = push(err, Opers, 'm');
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
				}
				else
				{
					while (priority(Opers->c) > priority('m'))
					{
						outnode n;
						n.c = pop(&Opers);
						n.digit = 0;
						n.mode = 0;
						Out = outpush(err,Out, n);
						if (*err != ERR_OK)
						{
							delMem(err, Opers, Out);
							return 0;
						}
						if (Opers == NULL)
							break;
					}
					Opers = push(err,Opers, 'm');
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
				}
			}
			else
			{
				if (Opers == NULL)
				{
					Opers = push(err, Opers, expression[k]);
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
				}
				else if (priority(Opers->c) < priority(expression[k]))
				{
					Opers = push(err, Opers, expression[k]);
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
				}
				else
				{
					while (priority(Opers->c) >= priority(expression[k]))
					{
						outnode n;
						n.c = pop(&Opers);
						n.digit = 0;
						n.mode = 0;
						Out = outpush(err,Out, n);
						if (*err != ERR_OK)
						{
							delMem(err, Opers, Out);
							return 0;
						}
						if (Opers == NULL)
							break;
					}
					Opers = push(err,Opers, '-');
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
				}
			}
		}
		else if (expression[k] == '^')
		{
			Opers = push(err, Opers, expression[k]);
			if (*err != ERR_OK)
			{
				delMem(err, Opers, Out);
				return 0;
			}
		}
		else if (expression[k] == '+'
			|| expression[k] == '/' 
			|| expression[k] == '*')
		{
			if (Opers == NULL)
			{
				Opers = push(err, Opers, expression[k]);
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
			else if (priority(Opers->c) < priority(expression[k]))
			{
				Opers = push(err, Opers, expression[k]);
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
			else
			{
				while (priority(Opers->c) >= priority(expression[k]))
				{
					outnode n;
					n.c = pop(&Opers);
					n.digit = 0;
					n.mode = 0;
					Out = outpush(err,Out, n);
					if (*err != ERR_OK)
					{
						delMem(err, Opers, Out);
						return 0;
					}
					if (Opers == NULL)
						break;
				}
				Opers = push(err,Opers, expression[k]);
				if (*err != ERR_OK)
				{
					delMem(err, Opers, Out);
					return 0;
				}
			}
		}
		else if (isspace(expression[k]))
			;
		else
		{
			delMem(err, Opers, Out);
			return 0;
		}
		k++;
	}
	/////////////////////////////////////////////

	if (Out == NULL)
	{
		delMem(err, Opers, Out);
		*err = ERR_UNNAMED;
		return 0;
	}
	
	while(Opers != NULL)
	{
		outnode n;
		n.c = pop(&Opers);
		n.digit = 0;
		n.mode = 0;
		if (n.c == ')' || n.c == '(')
		{
			delMem(err, Opers, Out);
			*err = ERR_UNNAMED;
			return 0;
		}
		Out = outpush(err,Out, n);
		if (*err != ERR_OK)
		{
			delMem(err, Opers, Out);
			return 0;
		}
	} 

	O = NULL;
	outnode * Out1 = Out;
	while (Out->next != NULL)
	{
		Out->prev = O;
		O = Out;
		Out = Out->next;
	}
	Out->prev = O;

	if (Out->prev == NULL)
	{
		double tmp = Out->digit;
		if (Out->mode != 0)
		{
			if (Out != NULL)
			free(Out);
			return tmp;
		}
		else
		{
			if (Out != NULL)
			free(Out);
			*err = ERR_UNNAMED;
			return 0;
		}
	}
	O = Out;
	
	int fl = 0;
	for (;Out != NULL; )
	{
		if (Out->mode == 0)
		{
			int md = 0;
			double tmp = 0;
			fl = 1;
			switch (Out->c)
			{
			case 'm':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				if (Out->next->digit == 0)
					tmp = 0;
				else
					tmp = (-1.0) * Out->next->digit;
				break;
			case 's':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = sin(Out->next->digit);
				break;
			case 'q':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				if (Out->next->digit < 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = sqrt(Out->next->digit);
				break;
			case 'l':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				if (Out->next->digit < 0)
				{
					delMem(err, Opers, Out1);
					return 0;
				}
				if (Out->next->digit <= 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = log(Out->next->digit);
				break;

			case 'i':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = ceil(Out->next->digit);
				break;
			case 'f':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = floor(Out->next->digit);
				break;
			case 'c':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = cos(Out->next->digit);
				break;
			case 't':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				if (cos(Out->next->digit) == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = tan(Out->next->digit);
				break;
			case 'T':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = atan(Out->next->digit);
				break;

			case 'g':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				if (sin(Out->next->digit) == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = 1/tan(Out->next->digit);
				break;
			case 'C':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				if (Out->next->digit > 1  || Out->next->digit < -1)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = acos(Out->next->digit);
				break;
			case 'S':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				if (Out->next->digit > 1 || Out->next->digit < -1)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = asin(Out->next->digit);
				break;
			case '+':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0 || Out->next->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = Out->next->digit + Out->next->next->digit;
				md = 1;
				break;
			case '-':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0 || Out->next->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = Out->next->next->digit - Out->next->digit;
				md = 1;
				break;
			case '/':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0 || Out->next->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				if (Out->next->digit != 0)
					tmp = Out->next->next->digit / Out->next->digit;
				else
				{
					delMem(err, Opers, Out1);
					return 0;
				}
				md = 1;
				break;
			case '*':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0 || Out->next->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = Out->next->digit * Out->next->next->digit;
				md = 1;
				break;
			case '^':
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->next == NULL)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				else if (Out->next->mode == 0 || Out->next->next->mode == 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				if (Out->next->next->digit == 0 && Out->next->digit < 0)
				{
					delMem(err, Opers, Out1);
					*err = ERR_UNNAMED;
					return 0;
				}
				tmp = pow(Out->next->next->digit, Out->next->digit);
				md = 1;
				break;
			}
			
			if (Out->prev == NULL && md == 1)
			{
				if (Out->next->next->next != NULL)
				{
					delMem(err, Opers, Out1);
					return 0;
				}

				if (Out1 != NULL)
					if (Out1 != Out->next->next && Out1 != Out->next && Out1 != Out)
					{
						delMem(err, Opers, Out1);
						return 0;
					}
				if (Out->next->next != NULL)
				free(Out->next->next);
				if (Out->next != NULL)
				free(Out->next);
				if (Out != NULL)
				free(Out);
				return tmp;
			}
			else if (Out->prev == NULL && md == 0)
			{
				if (Out->next->next != NULL)
				{
					delMem(err, Opers, Out1);
					return 0;
				}
				if (Out1 != NULL)
					if (Out1 != Out->next && Out1 != Out)
					{
						delMem(err, Opers, Out1);
						return 0;
					}
				if (Out->next != NULL)
				free(Out->next);
				if (Out != NULL)
				free(Out);
				return tmp;
			}
			
			if (md == 1)
			{
				if (Out->next == NULL)
				{
					delMem(err, Opers, Out1);
					return 0;
				}
				if (Out->next->next == NULL)
				{
					delMem(err, Opers, Out1);
					return 0;
				}
				outnode *p1, *p2;
				p1 = Out->next;
				p2 = Out;
				Out->next->next->prev = Out->prev;
				Out->prev->next = Out->next->next;
				Out->next->next->mode = 1;
				Out->next->next->digit = tmp;
				Out = Out->prev;
				if (Out1 == p1 || Out1 == p2)
					Out1 = NULL;
				if (p1 != NULL)
				free(p1);
				if (p2 != NULL)
				free(p2);
			}
			else
			{

				if (Out->next->next == NULL)
				{
					outnode *p1;
					p1 = Out->next;
					Out->mode = 1;
					Out->digit = tmp;
					Out->next = NULL;
					Out = Out->prev;
					if (Out1 == p1)
						Out1 = NULL;
					if(p1!=NULL)
					free(p1);
				}
				else
				{
					outnode *p1;
					p1 = Out->next;
					Out->next = Out->next->next;
					Out->next->prev = Out;
					Out->mode = 1;
					Out->digit = tmp;
					Out = Out->prev;
					if (Out1 == p1)
						Out1 = NULL;
					if (p1 != NULL)
					free(p1);
				}
			}
			continue;
		}
		Out = Out->prev;
	}

	if (!fl)
	{
		delMem(err, Opers, Out1);
		*err = ERR_UNNAMED;
	}
	return result;
}


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define	ID	1
#define	CONSTANT	2
#define	INT	3
#define	IF	4
#define	ELSE	5
#define	WHILE	6
#define	CONTINUE	7
#define	BREAK	8
#define	RETURN	9
#define	ASSIGN	10
#define	PLUS	11
#define	MINUS	12
#define	MULT	13
#define	DIV	14
#define	L	15
#define	LE	16
#define	G	17
#define	GE	18
#define	E	19
#define	NE	20
#define	COMMA	21
#define	SEMICOLON	22
#define	LPAREN	23
#define	RPAREN	24
#define	LBRACE	25
#define	RBRACE	26


#define MAX_VAR_NUM 128
#define MAX_PRO_NUM 128
#define MAX_GEN_NUM 1024
#define VAR	0
#define PARAM	1

struct sym_t{
	char sname[17];
	int stype;
};

struct var_t{
	char vname[17];
	char vproc[17];
	int vkind;
	int vtype;
	int vadr;
};

struct pro_t{
	char pname[17];
	int ptype;
	int fadr;
	int ladr;
};

struct gen4{
	char op[17];
	int op1;
	int op2;
	int opr; 	
};

//syantax var
struct E
{
	int place;	//store addr of non-terminate E in the var_tab
};

FILE *fdyd, *fvar, *fqud, *ferr;
char err_msg[128];

int ip;		//current ip
int proc;	//current proc

struct sym_t sym;
int current_line;
struct var_t var_tab[MAX_VAR_NUM];
struct pro_t pro_tab[MAX_PRO_NUM];
struct gen4 gen_tab[MAX_GEN_NUM];
int var_count;
int pro_count;

void start();
void advance();
void error(char *);
void add_var(char *, int, int, char *);
struct var_t *find_var(char *, int, char *);
void add_pro(char *, int, int);
struct pro_t *find_pro(char *);
void fill_pro(char *pname, int fadr, int ladr);

void write_var();
void write_pro();


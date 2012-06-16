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
}gen4;

FILE *fdyd, *fvar, *fqud, *ferr;
char err_msg[128];

int ip;

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

void start(){
	current_line = 1;
	var_count = 0;
	pro_count = 0;	
	advance();
	prog();
	write_var();
	write_pro();
}

void advance(){
	char buf[128];
	fgets(buf, 127, fdyd);
	sscanf(buf, "%s %d\n", sym.sname, &sym.stype);
	if (sym.stype == EOLN){
		current_line++;
		advance();
	}

	if (sym.stype == MYEOF){
		error("unexpected eof");
		exit(1);
	}
}

void add_var(char * vname, int vkind, int vtype, char *func)
{
	if (find_var(vname, vkind, func) != NULL){
		error("varible reduntant definition");
		return;
	}
	strcpy(var_tab[var_count].vname, vname);
	var_tab[var_count].vname[strlen(vname)] = 0;
	strcpy(var_tab[var_count].vproc, func);
	var_tab[var_count].vproc[strlen(func)] = 0;
	var_tab[var_count].vkind = vkind;
	var_tab[var_count].vtype = vtype;
	var_tab[var_count].vadr = var_count;

	var_count++;
}

struct var_t *find_var(char *vname, int vkind, char *proc)
{
	int i;
	for (i = 0; i < var_count; ++i){
		if (!strcmp(var_tab[i].vname, vname) && !strcmp(var_tab[i].vproc, proc) && var_tab[i].vkind == vkind){
			return &var_tab[i];
		}
	}
	return NULL;
	
}

void add_pro(char *pname, int ptype)
{
	if (find_pro(pname) != NULL){
		error("varible reduntant definition");
		return;
	}
	strcpy(pro_tab[pro_count].pname, pname);
	pro_tab[pro_count].pname[strlen(pname)] = 0;
	pro_tab[pro_count].ptype = ptype;

	pro_count++;
}

void fill_pro(char *pname, int fadr, int ladr)
{
	find_pro(pname)->fadr = fadr;
	find_pro(pname)->ladr = ladr;
}

struct pro_t *find_pro(char *func)
{
	int i;
	for (i = 0; i < pro_count; ++i){
		if (!strcmp(pro_tab[i].pname, func)){
			return &pro_tab[i];
		}
	}
	return NULL;
}

void write_var()
{
	int i;
	for (i = 0; i < var_count; ++i){
		fprintf(fvar, "%s, %s, %d, %d, %d\n", var_tab[i].vname,var_tab[i].vproc,var_tab[i].vkind,var_tab[i].vtype,var_tab[i].vadr);
	}
}

void write_pro()
{
	int i;
	for (i = 0; i < pro_count; ++i){
		fprintf(fpro, "%s, %d, %d, %d\n", pro_tab[i].pname,pro_tab[i].ptype,pro_tab[i].fadr,pro_tab[i].ladr);
	}
}

void error(char *msg)
{
	fprintf(ferr, "Line %d: %s\n", current_line, msg);
}

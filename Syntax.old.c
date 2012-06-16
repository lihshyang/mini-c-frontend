#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BEGIN		1
#define END		2
#define INTEGER		3
#define IF		4
#define THEN		5
#define ELSE		6
#define FUNCTION	7
#define READ		8
#define WRITE		9
#define IDENTIFIER	10
#define CONSTANT	11
#define E		12
#define NE		13
#define LE		14
#define L		15
#define GE		16
#define G		17
#define SUB		18
#define MUL		19
#define ASSIGN		20
#define LPAR		21
#define RPAR		22
#define SEM		23
#define EOLN		24
#define MYEOF		25

#define MAX_VAR_NUM 128
#define MAX_PRO_NUM 128

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
	int vlev;
	int vadr;
};

struct pro_t{
	char pname[17];
	int ptype;
	int plev;
	int fadr;
	int ladr;
};

FILE *fdyd, *fvar, *fpro, *ferr;
char err_msg[128];

struct sym_t sym;
int current_line;
struct var_t var_tab[MAX_VAR_NUM];
struct pro_t pro_tab[MAX_PRO_NUM];
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

void prog();
void subprog();
void decltab(char *);
void decltab2(char *);
void decl(char *);
void decltype(char *);
void vardecl(char *);
void var();
void funcdecl(char *);
void arg();
void funcbody(char *);
void exectab(char *);
void exectab2(char *);
void exec(char *);
void _read(char *);
void _write(char *);
void assign(char *);
void expr(char *);
void expr2(char *);
void item(char *);
void item2(char *);
void factor(char *);
void funccall(char *);
void cond(char *);
void condexpr(char *);
void relatexpr();

void prog()
{
	subprog();
}

void subprog()
{
	add_pro("__main__", 3, 0);
	if (sym.stype == BEGIN){
		advance();
		decltab("__main__");
		exectab("__main__");
		if (sym.stype == END){
			;
		} else {
			error("expect \"end\"");
		}
	} else {
		error("expect \"begin\"");
	}
}

void decltab(char *func)
{
	decl(func);
	decltab2(func);
}

void decltab2(char *func)
{
	if (sym.stype == SEM){
		advance();
		decl(func);
		decltab2(func);
	}
}

void decl(char *func)
{
	if (sym.stype == INTEGER){
		advance();
		decltype(func);
	}
}

void decltype(char *func)
{
	if (sym.stype == IDENTIFIER){
		vardecl(func);
	} else if (sym.stype == FUNCTION){
		funcdecl(func);
	} else {
		error("expect identifier or \"function\"");
	}
}

void vardecl(char *func)
{
	add_var(sym.sname, VAR, INTEGER, func);
	advance();
}

void var()
{
	if (sym.stype != IDENTIFIER){
		error("expect identifier");
		advance();
		return;
	} else {
		advance();
	}
}

void funcdecl(char *func)
{
	int place;
	char subfunc[128];
	if (sym.stype == FUNCTION){
		advance();
		if (sym.stype == IDENTIFIER){
			add_pro(sym.sname, 3, find_pro(func)->plev + 1);
			strcpy(subfunc, sym.sname);
			subfunc[strlen(sym.sname)] = 0; 
			advance();
			if (sym.stype == LPAR){
				advance();
				if (sym.stype == IDENTIFIER){
					add_var(sym.sname, PARAM, INTEGER, subfunc);
					place = find_var(sym.sname, PARAM, subfunc)->vadr;
					fill_pro(subfunc, place, place);
					advance();
					if (sym.stype == RPAR){
						advance();
						if(sym.stype == SEM){
							advance();
							funcbody(subfunc);
						} else {
							error("expect \";\"");
						}
					} else {
						error("expect \")\"");
					}
				} else {
					error("expect identifier");
				}
			} else {
				error("expect \"(\"");
			}
		} else {
			error("expect identifier");
		}
	} else {
		error("expect \"function\"");
	}
}

void arg(){;}

void funcbody(char *func)
{
	if (sym.stype == BEGIN){
		advance();
		decltab(func);
		exectab(func);
		if (sym.stype == END){
			advance();
		} else {
			error("expect \"end\"");
		}
	} else {
		error("expect \"begin\"");
	}
}

void exectab(char *func)
{
	exec(func);
	exectab2(func);
}

void exectab2(char *func)
{
	if (sym.stype == SEM){
		advance();
		exec(func);
		exectab2(func);
	}
}

void exec(char *func)
{
	if (sym.stype == READ){
		_read(func);
	} else if (sym.stype == WRITE){
		_write(func);
	} else if (sym.stype == IDENTIFIER){
		assign(func);
	} else if (sym.stype == IF){
		cond(func);
	} else {
		error("not an execute sentence");
	}
}

void _read(char *func)
{
	if (sym.stype == READ){
		advance();
		if (sym.stype == LPAR){
			advance();
			if (sym.stype == IDENTIFIER){
				if (find_var(sym.sname, VAR, func) != NULL){
					advance();
					if (sym.stype == RPAR){
						advance();
					} else {
						error("expect \")\"");
					}
				} else {
					sprintf(err_msg, "symbol %s undefined", sym.sname);
					error(err_msg);
				}
			} else {
				error("expect identifier");
			}
		} else {
			error("expect \"(\"");
		}
	} else {
		error("expect \"read\"");
	}
}

void _write(char *func)
{
	if (sym.stype == WRITE){
		advance();
		if (sym.stype == LPAR){
			advance();
			if (sym.stype == IDENTIFIER){
				if (find_var(sym.sname, VAR, func) != NULL){
					advance();
					if (sym.stype == RPAR){
						advance();
					} else {
						error("expect \")\"");
					}
				} else {
					sprintf(err_msg, "symbol %s undefined", sym.sname);
					error(err_msg);
				}
			} else {
				error("expect identifier");
			}
		} else {
			error("expect \"(\"");
		}
	} else {
		error("expect \"write\"");
	}
}


void assign(char *func)
{
	if (sym.stype == IDENTIFIER){
		if (find_var(sym.sname, VAR, func) != NULL||find_pro(sym.sname) != NULL){
			advance();
			if (sym.stype == ASSIGN){
				advance();
				expr(func);
			} else {
				error("expect \":=\'");
			}
		} else {
			sprintf(err_msg, "symbol %s undefined", sym.sname);
			error(err_msg);
		}
	} else {
		error("expect identifier");
	}
}

void expr(char *func)
{
	item(func);
	expr2(func);
}

void expr2(char *func)
{
	if (sym.stype == SUB){
		advance();
		item(func);
		expr2(func);
	}
}

void item(char *func)
{
	factor(func);
	item2(func);
}

void item2(char *func)
{
	if (sym.stype == MUL){
		advance();
		factor(func);
		item2(func);
	}
}

void factor(char *func)
{
	if (sym.stype == CONSTANT){
		advance();
	} else if (sym.stype == IDENTIFIER){
		if (find_var(sym.sname, VAR, func) != NULL){
			advance();
		} else if (find_pro(sym.sname) != NULL){
			advance();
			funccall(func);
		} else {
			sprintf(err_msg, "symbol %s undefined", sym.sname);
			error(err_msg);
		}
	} else {
		error("expect a factor");
	}
	
}

void funccall(char *func)
{
	if (sym.stype == LPAR){
		advance();
		expr(func);
		if (sym.stype == RPAR){
			advance();
		} else {
			error("expect \")\"");
		}
	}
}

void cond(char *func)
{
	if (sym.stype == IF){
		advance();
		condexpr(func);
		if (sym.stype == THEN){
			advance();
			exec(func);
			if (sym.stype == ELSE){
				advance();
				exec(func);
			} else {
				error("expect \"else\"");
			}
		} else {
			error("expect \"then\"");
		}
	} else {
		error("expect \"if\"");
	}
}

void condexpr(char *func)
{
	expr(func);
	relatexpr();
	expr(func);
}

void relatexpr()
{
	if (sym.stype == E||sym.stype == NE||sym.stype == LE||sym.stype == L||
			sym.stype == GE||sym.stype == G){
		advance();
	} else {
		error("expect relational operator");
	}
}

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
	var_tab[var_count].vlev = find_pro(func)->plev;
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

void add_pro(char *pname, int ptype, int plev)
{
	if (find_pro(pname) != NULL){
		error("varible reduntant definition");
		return;
	}
	strcpy(pro_tab[pro_count].pname, pname);
	pro_tab[pro_count].pname[strlen(pname)] = 0;
	pro_tab[pro_count].ptype = ptype;
	pro_tab[pro_count].plev = plev;

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
		fprintf(fvar, "%s, %s, %d, %d, %d, %d\n", var_tab[i].vname,var_tab[i].vproc,
			var_tab[i].vkind,var_tab[i].vtype,var_tab[i].vlev,var_tab[i].vadr);
	}
}

void write_pro()
{
	int i;
	for (i = 0; i < pro_count; ++i){
		fprintf(fpro, "%s, %d, %d, %d, %d\n", pro_tab[i].pname,pro_tab[i].ptype,
			pro_tab[i].plev,pro_tab[i].fadr,pro_tab[i].ladr);
	}
}

void error(char *msg)
{
	fprintf(ferr, "Line %d: %s\n", current_line, msg);
}

FILE *Fopen(const char *fname, const char *mod)
{
	FILE *ret = fopen(fname, mod);
	if (ret == NULL){
		perror("fopen");
		exit(-1);
	}
	return ret;
}

int main(int argc, char *argv[])
{	
	char *ptr, fname[256];
	int namelen;
	
	if (argc != 2){
		printf("usage: %s <.dyd file>\n", argv[0]);
		exit(-1);
	}
	
	if ((ptr = strrchr(argv[1], '.')) == NULL){
		printf("not a .dyd file!\n");
		exit(-1);
	} else if (strcmp(ptr, ".dyd") != 0){
		printf("not a .dyd file!\n");
		exit(-1);
	}
	
	fdyd = Fopen(argv[1], "r");

	namelen = strlen(argv[1])-4;
	strncpy(fname, argv[1], namelen);

	strcpy(fname + namelen, ".err");
	fname[namelen+4] = '\0';

	ferr = Fopen(fname, "r+");

	if (fgetc(ferr) != EOF){
		exit(1);
	}

	strcpy(fname + namelen, ".var");
	fvar = Fopen(fname, "w+");
	
	strcpy(fname + namelen, ".pro");
	fpro = Fopen(fname, "w+");

	start();

	fclose(fdyd);
	fclose(fvar);
	fclose(fpro);
	fclose(ferr);

	return 0;
}

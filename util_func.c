#include"compiler.h"
//advance
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

//base function
void start(){
	current_line = 1;
	var_count = 0;
	pro_count = 0;	
	advance();
	prog();
	write_var();
	write_pro();
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

struct var_t *find_var(char *vname, char *proc)
{
	int i;
	for (i = 0; i < var_count; ++i){
		if (!strcmp(var_tab[i].vname, vname) && !strcmp(var_tab[i].vproc, proc)){
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


//error
void error(char* msg)
{
	printf("%s in line %d", msg, current_line);
}

//new temp
newtemp()
{
	//Q
}

//entry
struct var_t *entry(char* varname)
{
	return find_var(varname, proc);
}

//gen
void gen(int op, int )
{

}

//merge
void merge()
{

}

//backpatch
void backpatch()
{

}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"

extern FILE *fdyd, *fvar, *fqud, *ferr;

FILE *Fopen(const char *fname, const char *mod)
{
	FILE *ret = fopen(fname, mod);
	if (ret == NULL){
		perror("fopen");
		exit(-1);
	}
	return ret;
}

int main(int argc, char **argv)
{
	char *ptr, fname[256];
	int namelen;
	
	if (argc != 2){
		printf("usage: %s <.dyd file>\n", argv[0]);
		exit(1);
	}
	
	if ((ptr = strrchr(argv[1], '.')) == NULL){
		printf("not a .dyd file!\n");
		exit(1);
	} else if (strcmp(ptr, ".dyd") != 0){
		printf("not a .dyd file!\n");
		exit(1);
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
	
	strcpy(fname + namelen, ".qud");
	fqud = Fopen(fname, "w+");

	start();

	fclose(fdyd);
	fclose(fvar);
	fclose(fqud);
	fclose(ferr);

}

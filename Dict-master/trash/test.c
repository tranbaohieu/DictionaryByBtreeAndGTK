#include <stdio.h>
#include <string.h>
#include "btree.h"
#include <stdlib.h>
BTA *tree;
int main(){
	btinit();
  	tree= btcrt("test.dat",0,0);
  	int i;
  	if (btins(tree,"hien","1234",10)==0) printf("fail");
  	if (btins(tree,"hien1","12434",10)==0) printf ("fail");
  	if (btins(tree,"hien2","12345",10)==0) printf ("fail");
  	if (btins(tree,"hien3","12346",10)==0) printf ("fail");
  	char a[10];
  	char b[10];
  	strcpy(a,"hien1");
  	int rsize;
  	btsel(tree, a, b, 6500, &rsize);
  	printf("%s",b);
}
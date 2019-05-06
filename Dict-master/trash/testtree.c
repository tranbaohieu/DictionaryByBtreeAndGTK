#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/btree.h"

BTA *tree;

int main(){
    //init btree, mo file .dat
    char a[10];
    strcpy(a,"abb");
    btinit();
    tree = btopn("data/tudienanhviet.dat", 0, 0);
    int rsize;
    char defnTemp[6500];
    btsel(tree, a, defnTemp, 6500, &rsize);
    printf("%s-%s",a,defnTemp);
    btcls(tree);
}

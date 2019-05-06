// Creator: 
// Vu tuan Dat
// ICT 58
// Edit:
// Nguyen Quang Hien
// ICT 58


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "inc/btree.h"
typedef struct xau_s {
	char tu[100];
	char vitri[20];
	char dodai[20];
} XAU;

char *strrev(char *str){ 			/* reverse a string */    
    char *p1, *p2;

    if (! str || ! *str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2){
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

XAU xu_ly_xau(char *str) {			/* break the input into 3 parts */
	XAU xuly;
	int i=0, j=0;
	char str_temp[strlen(str)];
	str = strrev(str);
	strncpy(xuly.dodai, str, 20);
	strcpy(str_temp, str);

	for (i=0; i<20; i++){
		if (xuly.dodai[i] == ' ' || xuly.dodai[i] == '\t'){
			xuly.dodai[i] = '\0';
			j = i; break;
		}
	}
	strrev(xuly.dodai);

	i=0; while(str_temp[j] == ' ' || str_temp[j] == '\t') j++;

	while (str_temp[j] != '\0' && str_temp[j] != ' ' && str_temp[j] != '\t'){
		xuly.vitri[i] = str_temp[j];
		i++;j++;
	} 
	xuly.vitri[i] = '\0';
	strrev(xuly.vitri);

	i=0; while(str_temp[j] == ' ' || str_temp[j] == '\t') j++;

	while (str_temp[j] != '\0'){
		xuly.tu[i] = str_temp[j];
		i++;j++;	
	} 
	xuly.tu[i] = '\0';
	strrev(xuly.tu);

	return xuly;
}

// int main(){
// 	XAU xaubatky;
// 	char str[100] = "Vu Tuan Dat 		ICT58   	 sds";
// 	xaubatky = xu_ly_xau(str);
// 	printf("%s.\n%s.\n%s.\n", xaubatky.tu, xaubatky.vitri, xaubatky.dodai);
// 	return 0;
// }

int lay_chiso (char ch, char *str) {
	int i=0;
	for (i=0; i<strlen(str); i++){
		if (ch == str[i]) return i;
	}
}

long luy_thua (int chiso, int luythua){
	long ketqua=1;
	while (luythua != 0){
		ketqua*=64;
		luythua--;
	}
	return chiso*ketqua;
}

long change_base_10(char *str) {  /* chuyen he co so 64 -> 10 */
	char SAMPLE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	long ketqua=0;
	long sohang=0;
	int i, n=strlen(str);
	strrev(str);
	for (i=0; i<n; i++){
		sohang = luy_thua(lay_chiso(str[i], SAMPLE), i);
		ketqua += sohang;
	}
	strrev(str);
	return ketqua;
}

// int main(){
// 	char str[] = "kbpP";
// 	printf("%li\n", change_base_10(str));
// 	return 0;
// }

char *timkiem(char *str, FILE *fp){            /* tim kiem nghia cua tu trong file tu dien */
	XAU tach;
	long vitri, dodai;
	char *nghia;
	tach = xu_ly_xau(str);
	vitri = change_base_10(tach.vitri);
	dodai = change_base_10(tach.dodai);
	nghia = (char *)calloc(dodai+1, 1);
	fseek(fp, vitri, SEEK_SET);
	long n = ftell(fp);
	fread(nghia, 1, dodai, fp);
	nghia[dodai] = '\0';
	return nghia;
}

// void insert_btree(){                 /* doc ly lieu vao cay */
// 	FILE *fp1, *fp2;
// 	BTA *tree;
// 	char *str;
// 	XAU daxuly;
// 	fp1 = fopen("data/anhviet109K.index", "r");
// 	fp2 = fopen("data/anhviet109K.dict", "r");
// 	btinit();

// 	tree = btcrt("data/tudienanhviet.dat", 0, 0);
// 	btopn("data/tudienanhviet.dat", 0, 0);

// 	while (!feof(fp1)) {
// 		fgets(str, 81, fp1);
// 		daxuly = xu_ly_xau(str);
// 		btins(tree, daxuly.tu, timkiem(str, fp2), change_base_10(daxuly.dodai) + 1 );
// 		free(str);
// 	}
// 	btcls(tree);
// 	fclose(fp1);
// 	fclose(fp2);
// } 

// int main(){
// 	btinit();
// 	insert_btree();	
// }

void tachString(char *a){
	int i=0;
	while(a[i]!='\t')i++;
	a[i]='\0';
}

int main (){
	
	//char string[] = "enthusiasm	K4ej	BR";
	FILE *fp1, *fp2;
	btinit();
	BTA *tree;
	tree=btcrt("data/tudienanhviet.dat",0,0);
	btopn("data/tudienanhviet.dat",0,0);
	fp1 = fopen("data/anhviet109K.index", "r");
	fp2 = fopen("data/anhviet109K.dict", "r");
	//char *str = timkiem(string, fp2);
	char string[100];
	char temp[100];
	char *str;
	rewind(fp1);
	// while(fgets(string,100,fp1)){
		
	// }
	int value;
	// int i=0;
	// fgets(string,100,fp1);
	// string[strlen(string)-1]='\0';
	// tachString(string);
	// printf("%s",string);
	while(fgets(string,100,fp1)){
		
		// i++;
		// if(i==10) break;
		string[strlen(string)-1]='\0';
		strcpy(temp,string);
		tachString(string);
		str=timkiem(temp,fp2);
		
		if (bfndky(tree,string,&value)!=0)  //Neu s1 khong co
	     {
	     	// printf("\n.%s.-.%s.",string,str);
	       btins(tree,string,str,strlen(str)+1);   //insert vao btree
	     }

	}
	btcls(tree);
	fclose(fp1);
	fclose(fp2);
	return 0;
}








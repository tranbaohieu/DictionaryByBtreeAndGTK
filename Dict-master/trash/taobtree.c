#include <stdio.h>
#include <string.h>
#include "inc/btree.h"
#include <stdlib.h>

typedef struct xau2Rec
{
  char s1[30];
  char s2[10];
} xau2;

xau2 tachxau(char s[])
{
  xau2 a;
  int i,n,d=0,k=1,t=1;
  n=strlen(s);
  for (i=1;i<=n;i++)
    {
      if(s[i-1]=='\t') {d=1;i++;}
      if(d==0)  
  {
    a.s1[t-1]=s[i-1];
    t++;
  }
      if(d==1)  
  {
    a.s2[k-1]=s[i-1];
    k++;
  }
      a.s1[t-1]='\0';
      a.s2[k-1]='\0';
    }
  return a;
}


long chiso(char ch,char*s)
{
  long i,n=strlen(s);
  for(i=0;i<n;i++)
    if(ch==s[i]) 
      return i;
}
long luythua(long a,long n)
{
  long i,lt;
  lt=1;
  if(n==0)
    return 1;
  else
    {
      for(i=1;i<=n;i++)
  {
    lt=lt*a;
  }
      return lt;
    }
}

long coso10(char *s)
{
  long i,n=strlen(s);
  long vt,coso=0;
  char ma64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  for(i=0;i<n;i++)
    {
      vt=chiso(s[i],ma64);
      coso += (long)vt*luythua(64,n-1-i);
    }
  return coso;
}

// char*dichtiengviet(char t[])
// {
//   char*s;
//   long vitri,dodai;
//   xau2 a;
//   FILE*fin;
//   fin=fopen("data/anhviet109K.dict","r");
//   a=tachxau(t);
//   vitri=coso10(a.s1);
//   dodai=coso10(a.s2);
//   s=(char*)malloc(sizeof(char)*(dodai+1));
 
//   fseek(fin,vitri,SEEK_SET);
//   fread(s,sizeof(char),dodai,fin);
//   s[dodai]='\0';
//   fclose(fin);
//   return s;
// }
char*dichtiengviet(char t[],FILE *p)
{
  char*s;
  long vitri,dodai;
  xau2 a;
  
  a=tachxau(t);
  vitri=coso10(a.s1);
  dodai=coso10(a.s2);
  s=(char*)malloc(sizeof(char)*(dodai+1));
 
  fseek(p,vitri,SEEK_SET);
  fread(s,sizeof(char),dodai,p);
  s[dodai]='\0';
  return s;
}

BTA* nhapbtree(BTA*tree)
{
  FILE*p;
  p=fopen("data/anhviet109K.dict","r");
  int value;
  int n;
  FILE *fin;
  char s1[100],s2[40],s[100],*s3;
  tree=btcrt("data/tudienanhviet.dat",0,0);  //tao file btree: tudienanhviet.dat
  btopn("data/tudienanhviet.dat",0,0);
  fin=fopen("data/anhviet109K.index","r"); //doc file index
  // int i=0;
  while(!feof(fin))
    {
      // i++;
      // if(i==10) break;
      fscanf(fin,"%[^\t]\t%[^\n]\n",s1,s2);
      s3=dichtiengviet(s2,p);
      if (bfndky(tree,s1,&value)!=0)  //Neu s1 khong co
	     {
          // printf("\n.%s.-.%s.",s1,s3);
	       btins(tree,s1,s3,strlen(s3)+1);   //insert vao btree

	     }
      free(s3);
    }
  fclose(p);
  fclose(fin);
  return tree;
}

// BTA *taofiledat(BTA *tree){
//   FILE *p;
//   char s1[10000],s2[10000],s3[100];
//   while(fgets(s1,10000,p)!=0){

//   }

// }

BTA *tree;
int main(){
  btinit();
  tree= btopn("data/tudienanhviet.dat",0,0);
  if(tree==0)
    {
      tree=nhapbtree(tree);
      // tree=xoatu(tree,fdel);
      btcls(tree);
    }
}

// int main(){
//   FILE *fin;
//   char s1[100],s2[40],s[100],*s3;
//   btinit();
//   fin=fopen("data/anhviet109K.index","r"); 
//   int i;
//   for(i=1;i<100;i++)
//     {
//       fscanf(fin,"%[^\t]\t%[^\n]\n",s1,s2);
//       s3=dichtiengviet(s2);
//       printf("\n%s-%s",s1,s3);
//       free(s3);
//     }
//   fclose(fin);

// }
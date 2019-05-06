//gcc `pkg-config --cflags gtk+-3.0` -o main main.c `pkg-config --libs gtk+-3.0` -w libbt.a -rdynamic

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/btree.h"
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define NUMBERSUGGEST 30
#define MAXSUGGESTCHAR 100

typedef struct
{
    GtkWidget *searchentry;
    GtkWidget *textview;
    GtkListStore *foundlist;
    GtkWidget *suggests;
} Widgets;


BTA *tree,*sug;
GtkWidget  *window;
Widgets *widget;

GtkWidget  *restore, *about, *dev;
GtkWidget  *search;
GtkWidget  *add, *edit, *del , *undo;
GtkWidget  *treeview;
GtkWidget  *aboutdialog;
GtkWidget  *thongbao;

GtkTextBuffer *buff;

void closeAbout(GtkWidget *button, Widgets *app){
    gtk_widget_hide(aboutdialog);
}

char thongbao_text[100];
#define size 512
void RestoreDataFile()
{
    remove("data/tudienanhviet.dat");
    FILE* ori = fopen("data/tudienanhviet.old","rb");
    FILE* cur = fopen("data/tudienanhviet.dat", "wb");
    int n;
    char buffer[512];
    while (!feof(ori))
    {
        n=fread(buffer,1,size,ori);
        if (n==0) break;
        fwrite(buffer,1,n,cur);
    }
    fclose(ori);
    fclose(cur);   
}

void doRestore(GtkWidget *button, Widgets *app){
    RestoreDataFile();
    GtkTextBuffer *textbuffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->textview));
    gtk_entry_set_text(app->searchentry,"");
    gtk_text_buffer_set_text(textbuffer,"",-1);
    gtk_widget_hide(del);
    gtk_widget_hide(edit);
    gtk_widget_hide(add);
    strcpy(thongbao_text,"Restore done !");
    gtk_label_set_text(thongbao,thongbao_text);
}

void doEdit(GtkWidget *button, Widgets *app){
    tree = btopn("data/tudienanhviet.dat", 0, 0);
    GtkTextIter start, end;
    gchar *text,*btext;
    GtkTextBuffer *textbuffer;
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->textview));
    text = gtk_entry_get_text(GTK_ENTRY(app->searchentry));
    gtk_text_buffer_get_bounds (textbuffer, &start, &end);
    btext=gtk_text_buffer_get_text(textbuffer,&start,&end ,FALSE);
    long value;
    if (bfndky(tree,text,&value)==0)
    {
        btupd(tree,text,btext,strlen(btext)+1);   
        strcpy(thongbao_text,"Edited data !");
        gtk_label_set_text(thongbao,thongbao_text);
    }

    gtk_widget_hide(add);
    btcls(tree); 
}

void doAdd(GtkWidget *button, Widgets *app){
    tree = btopn("data/tudienanhviet.dat", 0, 0);
    GtkTextIter start, end;
    gchar *text,*btext;
    GtkTextBuffer *textbuffer;
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->textview));
    text = gtk_entry_get_text(GTK_ENTRY(app->searchentry));
    gtk_text_buffer_get_bounds (textbuffer, &start, &end);
    btext=gtk_text_buffer_get_text(textbuffer,&start,&end ,FALSE);
    long value;
    if (bfndky(tree,text,&value)!=0)  //Neu s1 khong co
    {
        btins(tree,text,btext,strlen(btext)+1);   //insert vao btree
       	strcpy(thongbao_text,"Added data !");
        gtk_label_set_text(thongbao,thongbao_text);
    }
    gtk_widget_hide(add);
    btcls(tree); 
}

void doRemove(GtkWidget *button, Widgets *app)
{
    tree = btopn("data/tudienanhviet.dat", 0, 0);
    gchar *text;
    GtkTextBuffer *textbuffer;
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->textview));
    text = gtk_entry_get_text(GTK_ENTRY(app->searchentry));
    char defnFind[6500];
    int rsize;
    long value;
    int exist = bfndky(tree,text,&value);
    if (exist == 0)
    {
        bdelky(tree, text);
        gtk_entry_set_text(app->searchentry,"");
	    gtk_text_buffer_set_text(textbuffer,"",-1);
	    
        strcpy(thongbao_text,"Deleted data !");
    	gtk_label_set_text(thongbao,thongbao_text);
    }  
 	gtk_widget_hide(del);
	gtk_widget_hide(edit);
	gtk_widget_hide(add);
    btcls(tree); 
}

void SearchSuggest(BTA*b, int n, char* wordFind, Widgets *app)
{
    GtkTreeIter  iter;
    int i, rsize;
    char tmp[85];
    strcpy(tmp, wordFind);
    char defnFind[6500];
    char pre[85], now[85];
    char suggests[100];
    strcpy(suggests,"");
    int check;
    for(i=0; i<n; i++)
    {
        check=btseln(b, wordFind, defnFind, 6500, &rsize);
        if(check!=0){   //neu ko tim dc tu nao thi ko suggest
            gtk_entry_set_text(app->suggests,""); 
            return;
        }

        strcpy(now, wordFind);
        if (strcmp(pre,now)==0) break;
        strncat(suggests,now,strlen(now)+1);
        strncat(suggests,"\t",2);
        strcpy(pre,now);
        if(strlen(suggests)>=MAXSUGGESTCHAR) break;
    }
    gtk_entry_set_text(app->suggests,suggests);
    strcpy(wordFind,tmp);
}

void xulyxau(char *input, char *output1, char *output2){
	int i=0, j=0;
	strcpy(output1, input);
	while (output1[i] != '\0') {
		if (output1[i] == '\t'){
			output1[i] = '\0';
			j=i; break;
		}
		i++;
	}
	while (input[j] == '\t') j++; 
	i=0;

	while (input[j] != '\0') {
		output2[i] = input[j];
		if (input[j] == '\t') {
			output2[i] = '\0';
			break;
		}
		i++; j++;
	}
}

void doSearch (GtkButton *button,Widgets *app){	//Khong su dung function nay nua. Su dung 2 function ben duoi de thay the
    char suggestword[85];
    gtk_widget_hide(del);
    gtk_widget_hide(edit);
    gtk_widget_hide(add);
    int check=1;

    tree = btopn("data/tudienanhviet.dat", 0, 0);
    char defnTemp[6500];
    int rsize;
    gchar *textget;
    GtkTextBuffer *textbuffer;
    textget = gtk_entry_get_text(GTK_ENTRY(app->searchentry));
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->textview));
    //printf("%s",textget);
    strcpy(suggestword, textget);
    GtkTreeIter  iter;
    if (strcmp(textget,"")==0){     //neu search entry == null
        gtk_text_buffer_set_text(textbuffer,"",-1); //xoa text view     
    }
    else{
        check=btsel(tree, textget, defnTemp, sizeof(defnTemp), &rsize);
        if (/**strcmp(defnTemp,"")==0*/check){  //neu khong tim duoc
            gtk_text_buffer_set_text(textbuffer,"Not found.\nInput to add",-1);
            gtk_widget_show(add);
            SearchSuggest(tree,NUMBERSUGGEST,suggestword,app);
        }
        else{   //neu tim duoc
            gtk_text_buffer_set_text(textbuffer,defnTemp,-1);
            gtk_list_store_append(app->foundlist, &iter);
            gtk_list_store_set (app->foundlist, &iter, 0, textget, -1);
            gtk_widget_show(del);
            gtk_widget_show(edit);
        }
    }  
    btcls(tree); 
}

int isFind;
	char input[200];
	char s1[100];
	char s2[100];
gboolean autoComplete(Widgets *wg,
               GdkEvent  *event,
               gpointer   user_data){
	
	gchar *sugget,*entryget;
	
	GtkTreeIter  iter;
	GdkEventKey key=event->key;
	
	if(key.keyval==GDK_KEY_Tab){
		isFind=0;
		sugget=gtk_entry_get_text(GTK_ENTRY(widget->suggests));
		entryget=gtk_entry_get_text(GTK_ENTRY(wg));
		strcpy(input,sugget);
		xulyxau(input,s1,s2);
		if(strcmp(entryget,s1)!=0) gtk_entry_set_text(wg,s1);
		else{
			 gtk_entry_set_text(wg,s2);
			 SearchSuggest(sug,NUMBERSUGGEST,s2,widget);
			 
		}
		return 1;
	}
	return 0;
}
void quickSuggest(GtkButton *button,Widgets *app){
    char suggestword[85];
    strcpy(thongbao_text,"");
    gtk_label_set_text(thongbao,thongbao_text);
    
    if(!isFind){  
        gtk_widget_hide(del);
        gtk_widget_hide(edit);
        gtk_widget_hide(add);
    }
    int check=1;

    char suggests[200];
    strcpy(suggests,"");
    char defnTemp[6500];
    int rsize;
    gchar *textget;
    GtkTextBuffer *textbuffer;
    textget = gtk_entry_get_text(GTK_ENTRY(app->searchentry));
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->textview));
    strcpy(suggestword, textget);
    GtkTreeIter  iter;
    if (strcmp(textget,"")==0){     //neu search entry == null
        gtk_text_buffer_set_text(textbuffer,"",-1); //xoa text view
        gtk_entry_set_text(app->suggests,"");
    }
    else{
        check=btsel(sug, textget, defnTemp, sizeof(defnTemp), &rsize);
        if (check){  //neu khong tim duoc
            SearchSuggest(sug,NUMBERSUGGEST,suggestword,app);
            isFind=0;
        }
        else{   //neu tim duoc
        
        }
    }
}

void key_enter(GtkButton *button,Widgets *app){		//bam phim enter
    tree = btopn("data/tudienanhviet.dat", 0, 0);
    int check=1;
    char defnTemp[6500];
    int rsize;
    gchar *textget;
    GtkTextBuffer *textbuffer;
    textget = gtk_entry_get_text(GTK_ENTRY(app->searchentry));
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->textview));
    GtkTreeIter  iter;
    if (strcmp(textget,"")==0){     //neu search entry == null
        gtk_text_buffer_set_text(textbuffer,"",-1); //xoa text view
    }
    else{
        check=btsel(tree, textget, defnTemp, sizeof(defnTemp), &rsize);
        if (check){  //neu khong tim duoc
             gtk_text_buffer_set_text(textbuffer,"Not found.\nInput to add",-1);
             gtk_widget_show(add);  
             strcpy(thongbao_text,"Do you wanna add a new word ?\nInput the meaning of word below here.");
    		 gtk_label_set_text(thongbao,thongbao_text);
        }
        else{   //neu tim duoc
        	if(!isFind){
        		gtk_list_store_append(app->foundlist, &iter);
            	gtk_list_store_set (app->foundlist, &iter, 0, textget, -1);
        	}
            gtk_text_buffer_set_text(textbuffer,defnTemp,-1);
            
            gtk_widget_show(del);
            gtk_widget_show(edit);
            isFind=1;
        }
    }
    btcls(tree); 
}

//Tra tu o cot translated . 
void selectRow(GtkTreeSelection *treeselection,
               gpointer          user_data){
	// char defnTemp[6500];
 //    int rsize;
 //    GtkTextBuffer *textbuffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget->textview));
	// gchar *getselect=gtk_tree_selection_get_user_data(treeselection);
 //    tree = btopn("data/tudienanhviet.dat", 0, 0);
 //    if(btsel(tree, getselect, defnTemp, sizeof(defnTemp), &rsize)==0) 
 //    	gtk_text_buffer_set_text(textbuffer,defnTemp,-1);
 //    btcls(tree); 
}

void showUndo(GtkButton *button,Widgets *app){	//an hien nut undo
    //if(isFind)gtk_widget_show(undo);
}
		
int doUndo(GtkButton *button, Widgets *app){	//thuc hien undo
    tree = btopn("data/tudienanhviet.dat", 0, 0);
    gchar *textget;
    GtkTextBuffer *textbuffer;
    textget = gtk_entry_get_text(GTK_ENTRY(app->searchentry));
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->textview));
    char defnTemp[6500];
    int rsize;
    if(btsel(tree, textget, defnTemp, sizeof(defnTemp), &rsize)==0){
        gtk_text_buffer_set_text(textbuffer,defnTemp,-1);
    }
    btcls(tree); 
}

main (int argc, char *argv[])
{
	printf("\nHelpful Dictionary Program\n");

	btinit();
	sug = btopn("data/tudienanhviet.old", 0, 0);	

    GtkBuilder      *builder; 
    
    GError     *error = NULL;

    widget = g_slice_new(Widgets);
    
    gtk_init (&argc, &argv);
    

    //init btree, mo file .dat
    
    //tree = btopn("data/tudienanhviet.dat", 0, 0);	//file nay ko can mo nua
    

    //khoi tao builder va cac widget
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "dict.glade", NULL);
    window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));

    aboutdialog = GTK_WIDGET (gtk_builder_get_object (builder, "aboutdialog"));

    restore = GTK_WIDGET (gtk_builder_get_object (builder, "restore"));
    about = GTK_WIDGET (gtk_builder_get_object (builder, "about"));
	dev = GTK_WIDGET (gtk_builder_get_object (builder, "dev"));

	search = GTK_WIDGET (gtk_builder_get_object (builder, "search"));

	add = GTK_WIDGET (gtk_builder_get_object (builder, "add"));
	edit = GTK_WIDGET (gtk_builder_get_object (builder, "edit"));
	del = GTK_WIDGET (gtk_builder_get_object (builder, "del"));
    undo = GTK_WIDGET (gtk_builder_get_object (builder, "undo"));

    treeview=GTK_WIDGET (gtk_builder_get_object (builder, "treeview"));

    thongbao=GTK_WIDGET (gtk_builder_get_object (builder, "thongbao"));
	

	widget->searchentry = GTK_WIDGET( gtk_builder_get_object( builder, "searchentry" ) );
    widget->textview = GTK_WIDGET( gtk_builder_get_object( builder, "textview" ) );
    widget->foundlist = GTK_LIST_STORE( gtk_builder_get_object( builder, "foundlist" ) );
    widget->suggests = GTK_WIDGET( gtk_builder_get_object( builder, "suggests" ) );

    // buff=gtk_text_view_get_buffer(widget->textview);
    // g_signal_connect(G_OBJECT(buff) , "changed", G_CALLBACK(showUndo), (gpointer) window);

    //tao ket noi singal 
    gtk_builder_connect_signals (builder, widget);

    g_object_unref (G_OBJECT (builder));
        
    gtk_widget_show (window);   

    gtk_main ();
   
    btcls(tree);   
    btcls(sug);

    return 0;
}


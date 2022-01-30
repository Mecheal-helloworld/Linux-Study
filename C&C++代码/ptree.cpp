#include<stdio.h>
#include<sys/types.h>
#include<string.h> 
#include<dirent.h>
#include<unistd.h>
using namespace std;
#define FILE 8
#define CATALOG 4
struct Node{
	dirent* file;
	Node* next;
	Node* child;
};
Node* creat_tree(char* pathname){
	DIR* head=opendir(pathname);
	Node* hnode;
	dirent* files;
	Node* before=new Node;
	hnode=before;
	while(files=readdir(head)){
		char* pathname1=pathname;
		Node* now=new Node;
		before->file=files;
		before->child=creat_tree(strcat(strcat(pathname1,"/"),files->d_name));
		before->next=now;
		before=now;
	}
	return hnode;
}
void print_tree(Node* node,int layer=0){
	if(node!=NULL){
		for(int i=0;i<layer;i++)printf("  ");
		if(node->file->d_type==FILE)printf("o--o %s %s\n",node->file->d_off,node->file->d_name);
		if(node->file->d_type==CATALOG)printf(">--> %s %s\n",node->file->d_off,node->file->d_name);
		print_tree(node->child,layer+1);
		print_tree(node->next,layer);
	}
	delete node;
	return;
}
int main(int argc, char* argv[]){
	char* pathname;
	DIR* now;
	Node* hnode;
	if(argc==1){
		pathname=getcwd(pathname,1024);
	}else if(argc==2){
		if(argv[1]=="-help"||argv[1]=="/?"){
			printf("Usage:\n");
			printf("  ptree [path]\n");
			printf("General Options:\n");
			printf("                        print now direction.\n");
			printf("/                       print root direction.\n");
			printf("./                      print now direction.\n");
			printf("../                     print parent direction.\n");
			return 0;
		}else if(argv[1]=="/"){
			strcpy(pathname,"/root");
		}else if(argv[1]=="./"){
			pathname=getcwd(pathname,1024);
		}else if(argv[1]=="../"){
			pathname=getcwd(pathname,1024);
			int i=0,end=1;
			while(pathname[i]){
				if(pathname[i]=='/'){
					end=i+1;
				}
			}
			pathname[i+1]='\0';
		}else{
			pathname=argv[1];
		}
	}else{
		printf("Usage:\n");
		printf("  ptree [path]\n");
		return 0;
	}
	if(now=opendir(pathname)){
		printf("ptree: cannot access %s: No such file or directory.",pathname);
		return 0;
	}
	closedir(now);
	hnode=creat_tree(pathname);
	print_tree(hnode);
	return 0;
}
 

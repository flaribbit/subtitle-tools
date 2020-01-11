#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 1024

int getAssInfo(char *path,double framerate,int *ResX,int *ResY);
void writeAVS(char *path,int w,int h,double framerate,int frames);

int main(int argc, char *argv[])
{
    if(argc==2){
        int w,h,frames;
        double framerate=60;
        char input[MAXLEN];
        printf("[ASS File] %s\n",argv[1]);
        printf("Please input framerate (default 60): ");
        gets(input);
        if(*input){
            framerate=atof(input);
        }
        frames=getAssInfo(argv[1],framerate,&w,&h);
        writeAVS(argv[1],w,h,framerate,frames);
    }else{
        puts("Tips: Drag and drop *.ass file on this exe.\n");
        getchar();
    }
    return 0;
}

void writeAVS(char *path,int w,int h,double framerate,int frames){
    FILE *fp;
    char buf[MAXLEN];
    int len=strlen(path)-3;
    strncpy(buf,path,len);
    strcpy(buf+len,"avs");
    fp=fopen(buf,"w");
    fprintf(fp,"LoadPlugin(\"VSFilter/VSFilterMod64.dll\")\n");
    fprintf(fp,"MaskSubMod(\"%s\",%d,%d,%f,%d)\n",path,w,h,framerate,frames);
    fprintf(fp,"FlipVertical()\n");
    fclose(fp);
}

int getAssInfo(char *path,double framerate,int *ResX,int *ResY){
    FILE *fp=fopen(path,"r");
    int h,m,s,p;
    while(!feof(fp)){
        char line[MAXLEN];
        fgets(line,MAXLEN,fp);
        if(!strncmp("PlayResX:",line,8)){
            *ResX=atoi(line+10);
        }else if(!strncmp("PlayResY:",line,8)){
            *ResY=atoi(line+10);
        }else if(!strncmp("Dialogue:",line,8)){
            h=atoi(line+23);
            m=atoi(line+25);
            s=atoi(line+28);
            p=atoi(line+31);
        }
    }
    return (int)((h*3600.+m*60.+s+p*.01)*framerate);
}

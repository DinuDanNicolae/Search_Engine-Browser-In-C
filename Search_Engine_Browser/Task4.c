#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct site
{
    char URL[51],titlu[51],nume[100];
    int viz,cs,sz;
    int fundal,text;
    char *cod,*continut;
    int cuv,key;
} site;

void dec_to_bin (int n, char *s) //transformam un numar din baza 10 in baza 2
{
    int i;
    for(i=0;n>0;i++)
    {
        s[7-i]=(char)(n%2+48); 
        n/=2;
    }
}

int bin_to_dec(char s[]) //transformam un numar din baza 2 in baza 10
{
    int b=1,n=0,r,i=7;
    while(i>=0)
    {
        r=(int)(s[i]-48);
        n+=r*b;
        b=b*2;
        i--;
    }
    return n;
} 

int rotr(char x, int k) //functia de rotire a bitilor spre dreapta
{
    char b[9],c;
    int i,nr=0;
    for(i=0;i<8;i++)
        b[i]='0'; 
    dec_to_bin((int)(x),b); //transformam numarul din baza 10 in baza 2
    while(k)
    {
        c=b[7]; //retinem ultimul caracter
        for(i=7;i>0;i--)
        {
            b[i]=b[i-1]; //rotim bitii cu o pozitie
        }
        k--; //scadem numarul de rotatii
        b[0]=c; //adaugam bitul retinut pe prima pozitie
    }
    b[8]='\0'; //adaugam terminatorul de sir
    return bin_to_dec(b); //returnam numarul in forma zecimala
}

int rotl(char x, int k)
{
    char b[9],c;
    int i,nr=0;
    for(i=0;i<8;i++)
        b[i]='0';
    dec_to_bin((int)(x),b); //transformam numarul din baza 10 in baza 2
    while(k)
    {
        c=b[0]; //retinem primul caracter
        for(i=0;i<7;i++)
        {
            b[i]=b[i+1]; //rotim bitii cu o pozitie
        }
        k--; //scadem numarul de rotatii
        b[7]=c; //adaugam caracterul retinut de ultima pozitie
    }
    b[8]='\0'; //adaugam terminatorul de sir
    return bin_to_dec(b); //returnam numarul in forma zecimala
}

void init(struct site *s,int lim)
{
    int i;
    for(i=0; i<lim; i++)
        s[i].key=0,s[i].cuv=0,s[i].cs=0,s[i].viz=0,s[i].fundal=0,s[i].text=1;
}

void init2(struct site *s,int lim,int ind)
{
    int i;
    for(i=ind;i<lim;i++)
        s[i].key=0,s[i].cuv=0,s[i].cs=0,s[i].viz=0,s[i].fundal=0,s[i].text=1;
}

char *cd(FILE *fp)
{
    char *continut=NULL,*temp=NULL;
    size_t size=0,i=0;
    int c=EOF;
    while(c)
    {
        c=getc(fp);
        if(c==EOF)
            c=0;
        if(i>=size)
        {
            size+=5;
            temp=realloc(continut,size);
            if(!temp)
            {
                free(continut);
                continut=NULL;
                break;
            }
            continut=temp;
        }
        continut[i++]=c;
    }
    return continut;
}

int culoare(char cul[][15],char *p)
{
    int i;
    for(i=0; i<6; i++)
        if(!strcmp(cul[i],p))
            return i;
    return -1;
}

int caut(char *sursa,char *p,int nr)
{
    int k=0,i=0;
    char *buffer=(char *)calloc(nr,sizeof(char));
    while(i<strlen(sursa))
    {
        if(sursa[i]==' ')
        {
            buffer[k]='\0';
            k=0;
            if(!strcmp(buffer,p))
            {
                free(buffer);
                return 1;
            }
            i++;
        }
        else buffer[k++]=sursa[i++];
    }
    buffer[k]='\0';
    k=0;
    if(!strcmp(buffer,p))
    {
        free(buffer);
        return 1;
    }
    free(buffer);
    return 0;
}

int cmp(struct site a, struct site b)
{
    int c=strcmp(a.continut,b.continut);
    if(c>0)
        return 1;
    else if(!c) return 0;
    else return -1;
}

void swp(struct site *a, struct site *b)
{
    site aux;
    aux=*a;
    *a=*b;
    *b=aux;
}

void sortare(int (*comp)(struct site, struct site), struct site *s,int nr)
{
    int i;
    char *temp;
    comp=cmp;
    int ok;
    do
    {
        ok=1;
        for(i=0; i<nr-1; i++)
            if(comp(s[i],s[i+1])==1)
                ok=0,swp(&s[i],&s[i+1]);
            else if(comp(s[i],s[i+1])==0 && s[i].viz<s[i+1].viz)
                ok=0,swp(&s[i],&s[i+1]);
    }
    while(!ok);
}

void cautpoz(struct site *s, char p[],int nr)
{
    int i;
    for(i=0;i<nr;i++)
        if(caut(s[i].continut,p,strlen(s[i].continut)))
            s[i].cuv++;
}

void cautneg(struct site *s, char p[],int nr)
{
    int i;
    for(i=0;i<nr;i++)
        if(caut(s[i].continut,p,strlen(s[i].continut)))
            s[i].cuv=0;
}

int main()
{
    char cul[6][15]= {"white","black","red","green","blue","yellow"};
    FILE *master=fopen("master.txt","r"),*st;
    int nr=0,lim=3,k;
    int ok=0;
    char *p;
    char v[51];
    struct site *s=(struct site*)calloc(lim,sizeof(struct site));
    init(s,lim);
    char *ptr;
    while(fscanf(master,"%s",s[nr].nume) != EOF)
    {
        ok=0;
        nr++;
        if(nr%3==0)
        {
            lim=nr + 3;
            s=realloc(s,lim * sizeof(struct site));
            init2(s,lim,nr);
        }
    }
    int i;
    int  cnr=nr;
    nr=0;
    for(i=0; i<cnr; i++)
    {

        st=fopen(s[i].nume,"r");

        fscanf(st,"%s",s[nr].URL);
        fscanf(st,"%d %d %d",&s[nr].sz,&s[nr].viz,&s[nr].cs);
        s[nr].cod=(char *)calloc((s[nr].sz+1),sizeof(char *));
        if(s[nr].cod==NULL)
        {
            printf("MEMORY NOT ALLOCATED cod site %d",nr);
            return 0;
        }
        s[nr].cod=cd(st);
        char *pi=strstr(s[nr].cod,"<title>");
        char *ps=strstr(s[nr].cod,"</title>");
        int inc=pi-s[nr].cod+7;
        int sf=ps-s[nr].cod;
        k=0;
        int j;
        for(j=inc; j<sf; j++)
            s[nr].titlu[k++]=s[nr].cod[j];
        s[nr].titlu[k]='\0';
        if(strstr(s[nr].cod,"<p>"))
        {
            pi=strstr(s[nr].cod,"<p>");
            ps=strstr(s[nr].cod,"</p>");
            inc=pi-s[nr].cod+3;
            sf=ps-s[nr].cod;
            k=0;
            s[nr].continut=(char *)calloc((sf-inc+1),sizeof(char));
            if(s[nr].continut==NULL)
            {
                printf("MEMORY NOT ALLOCATED for site %d",nr);
                return 0;
            }
            for(j=inc; j<sf; j++)
                s[nr].continut[k++]=s[nr].cod[j];
            s[nr].continut[k]='\0';
        }
        else
        {
            char *cultxt=malloc(15*sizeof(char));
            pi=strstr(s[nr].cod,"color:");
            ps=strstr(s[nr].cod,";");
            inc=pi-s[nr].cod+6;
            sf=ps-s[nr].cod;
            k=0;
            for(j=inc; j<sf; j++)
                cultxt[k++]=s[nr].cod[j];
            cultxt[k]='\0';
            char *culfnd=malloc(20*sizeof(char));
            pi=strstr(s[nr].cod,"background-color:");
            ps=strstr(s[nr].cod,";");
            inc=pi-s[nr].cod+17;
            sf=ps-s[nr].cod;
            k=0;
            for(j=inc; j<sf; j++)
                culfnd[k++]=s[nr].cod[j];
            culfnd[k]='\0';
            s[nr].fundal=culoare(cul,culfnd);
            s[nr].text=culoare(cul,cultxt);
            free(culfnd);
            free(cultxt);
            pi=strstr(s[nr].cod,";\">");
            ps=strstr(s[nr].cod,"</p>");
            inc=pi-s[nr].cod+3;
            sf=ps-s[nr].cod;
            s[nr].continut=(char *)calloc((sf-inc+1),sizeof(char));
            if(s[nr].continut==NULL)
            {
                printf("MEMORY NOT ALLOCATED for site %d",nr);
                return 0;
            }
            k=0;
            for(j=inc; j<sf; j++)
                s[nr].continut[k++]=s[nr].cod[j];
            s[nr].continut[k]='\0';
        }

        nr++;
        fclose(st);
    }

    fclose(master);
    char cautare[101],temp[101]="";
    int ind,l;
    while(fgets(cautare,100,stdin))
    {
        cautare[strcspn(cautare,"\n")]='\0'; 
        ok=0;
        for(i=0;i<nr && !ok;i++)
        {
            if(!strcmp(s[i].URL,cautare)) //verificam daca am gasit URL-ul cautat
            {
                ok=1,l=strlen(s[i].cod);
                ind=i;
                for(k=1;k<l;k++)
                    if((k-1)%2==0) //verificam paritatea caracterului la care am ajuns
                        s[i].key^=rotl(s[i].cod[k],(k-1)%8); //facem rotatia la stanga a caracterului respectiv
                    else s[i].key^=rotr(s[i].cod[k],(k-1)%8); //facem rotatia la stanga a caracterului respectiv
                    //calculam checksum-ul real folosind operatia XOR
            }
        }
        if(!ok)
            printf("Website not found!\n");
        else {
            if(s[ind].cs==s[ind].key) //intrebam daca cheia reala este egala cu cea data
                printf("Website safe!\n");
            else printf("Malicious website! Official key: %d. Found key: %d\n",s[ind].cs,s[ind].key);
        }
    }
    for(i=0; i<nr; i++)
    {
        free(s[i].continut);
        free(s[i].cod);
    }
    free(s);
    return 0;
}
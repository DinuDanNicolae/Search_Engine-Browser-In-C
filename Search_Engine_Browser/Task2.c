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
    int cuv;
} site;

void init(struct site *s,int lim)
{
    int i;
    for(i=0; i<lim; i++)
        s[i].cuv=0,s[i].cs=0,s[i].viz=0,s[i].fundal=0,s[i].text=1;
}

void init2(struct site *s,int lim,int ind)
{
    int i;
    for(i=ind;i<lim;i++)
        s[i].cuv=0,s[i].cs=0,s[i].viz=0,s[i].fundal=0,s[i].text=1;
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

int caut(char *sursa,char *p,int nr) //cautam intr-un sir de caractere sursa un sir de caractere p
{
    int k=0,i=0;
    char *buffer=(char *)calloc(nr,sizeof(char)); //alocam dinamic memoria pentru pointerul buffer
    while(i<strlen(sursa))
    {
        if(sursa[i] == ' ' || sursa[i] == '\n') //intrebam daca am intalnit un separator
        {
            buffer[k]='\0'; //daca am gasit un separator punem terminatorul de sir si ne va rezulta un cuvant
            k=0;
            if(!strcmp(buffer,p)) //comparam cuvantul pe care l-am retinut cu cuvantul cautat
            {
                free(buffer);
                return 1;
            }
            i++;
        }
        else buffer[k++]=sursa[i++]; //adugam in buffer caracter cu caracter din continut
    }
    //la iesirea din while mai verificam o data daca cuvantul de la ultimul separator pana la finalul sirului
    //este cuvantul cautat
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

int cmp(struct site a, struct site b) //functia comparata utilizata pentru sortarea ulterioara
{
    int c=strcmp(a.continut,b.continut);
    if(c>0)
        return 1;
    else if(!c) return 0;
    else return -1;
}

void swp(struct site *a, struct site *b) //functia in care interschimbam site-urile in sortare
{
    site aux;
    aux=*a;
    *a=*b;
    *b=aux;
}

void sortare(int (*comp)(struct site, struct site), struct site *s,int nr) //sortam site-urile in functie de numarul de vizualizari
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
    char cautare [101];
    scanf("%[^\n]",cautare); //citim in cautare pana la intalnirea caracterului newline
    p=strtok(cautare," "); //luam cuvant cu cuvant din cautare
    while(p)
    {
        for(i=0;i<nr;i++)
            if(caut(s[i].continut,p,strlen(s[i].continut))) //intrebam daca exista in continut
                s[i].cuv++;
        p=strtok(NULL," ");
    }
    int (*compar)(struct site,struct site); //cream un pointer catre functia de comparat
    sortare(compar,s,nr); //apelam functia de sortare
    for(i=0;i<nr;i++)
        if(s[i].cuv)
            printf("%s\n",s[i].URL);
    for(i=0; i<nr; i++)
    {
        free(s[i].continut);
        free(s[i].cod);
    }
    free(s);
    return 0;
}
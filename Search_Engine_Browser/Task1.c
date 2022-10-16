#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct site //definim tipul de date "site"
{
    char URL[51],titlu[51],nume[100]; 
    //retinem URL-ul, titlul si numele siturilor
    int viz,cs,sz;
    //retinem numarul de vizualizari, numarul de octeti si checksum-ul
    int fundal,text;
    //retinem codul asociat fundalului si textului
    char *cod,*continut;
    //retinem codul html si continutul parsat utilizant doi pointeri pentru a folosi alocarea dinamica
} site;

void init(struct site *s,int lim) //initializam campurile de tip int ale primelor 3 site-uri
{
    int i;
    for(i=0; i<lim; i++)
        s[i].cs=0,s[i].viz=0,s[i].fundal=0,s[i].text=1;
}

void init2(struct site *s,int lim,int ind) //initializam campurile de tip int ale site-urilor de la indicele int pana la final
{
    int i;
    for(i=ind;i<lim;i++)
        s[i].cs=0,s[i].viz=0,s[i].fundal=0,s[i].text=1;
}

char *cd(FILE *fp) //functia de citire a codului html
{
    char *continut=NULL,*temp=NULL; 
    size_t size=0,i=0;
    int c=EOF;
    while(c)
    {
        c=getc(fp); //citim un caracter din fisierul fp
        if(c==EOF) 
            c=0; //daca am ajuns la finalul fisierul punem terminatorul de sir
        if(i>=size) //daca marimea codului o depaseste pe cea actual alocata
        {
            size+=5; //incrementam marimea totala a continutului
            temp=realloc(continut,size); 
            if(!temp) //testam daca realocarea continutului e corecta
            {
                free(continut);
                continut=NULL;
                break;
            }
            continut=temp;
        }
        continut[i++]=c; //concatenam caracterul citit la restul sirului
    }
    return continut;
}

int culoare(char cul[][15],char *p) //convertim culoarea dintr-un sir de caractere intr-un indice retinut in campurile fundal si text
{
    int i;
    for(i=0; i<6; i++)
        if(!strcmp(cul[i],p))
            return i;
    return -1;
}

int main()
{
    char cul[6][15]= {"white","black","red","green","blue","yellow"};
    FILE *master=fopen("master.txt","r"),*st; 
    //deschidem master.txt si incepe sa citim din fisier, folosind un pointer st catre site-uri
    int nr=0,lim=3,k;
    int ok=0;
    char *p;
    char v[51];
    struct site *s=(struct site*)calloc(lim,sizeof(struct site)); 
    //alocam dinamic memoria pentru primele 3 site-uri
    init(s,lim);
    char *ptr;
    while(fscanf(master,"%s",s[nr].nume) != EOF) //citim site-urile pe rand si realocam memoria necesara 
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
    for(i=0; i<cnr; i++) //incepem citirea din fisiere
    {

        st=fopen(s[i].nume,"r");

        fscanf(st,"%s",s[nr].URL);
        fscanf(st,"%d %d %d",&s[nr].sz,&s[nr].viz,&s[nr].cs); 
        //citim numrul de octeti, numarul de vizualizari si checksum-ul
        s[nr].cod=(char *)calloc((s[nr].sz+1),sizeof(char *)); //alocam dinamic memoria pentru cod
        if(s[nr].cod==NULL)
        {
            printf("MEMORY NOT ALLOCATED cod site %d",nr);
            return 0;
        }
        s[nr].cod=cd(st);
        char *pi=strstr(s[nr].cod,"<title>"); //folosim 2 pointeri pentru a citi ce se afla intre tag-urile de title
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
            pi=strstr(s[nr].cod,"<p>"); //folosim 2 pointeri pentru a citi ce se afla intre tag-urile de p
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
            pi=strstr(s[nr].cod,"color:"); //folosim 2 pointeri pentru a citi ce se afla dupa "color;" si pana la ";"
            ps=strstr(s[nr].cod,";");
            inc=pi-s[nr].cod+6;
            sf=ps-s[nr].cod;
            k=0;
            for(j=inc; j<sf; j++)
                cultxt[k++]=s[nr].cod[j];
            cultxt[k]='\0';
            char *culfnd=malloc(20*sizeof(char));
            pi=strstr(s[nr].cod,"background-color:"); //folosim 2 pointeri pentru a citi ce se afla dupa "background-color:" si pana la ";"
            ps=strstr(s[nr].cod,";");
            inc=pi-s[nr].cod+17;
            sf=ps-s[nr].cod;
            k=0;
            for(j=inc; j<sf; j++) //parsam codul si retinem culoarea fundalului
                culfnd[k++]=s[nr].cod[j];
            culfnd[k]='\0';
            s[nr].fundal=culoare(cul,culfnd); //stabilim culoarea fundalului si a textului
            s[nr].text=culoare(cul,cultxt);
            free(culfnd);
            free(cultxt);
            pi=strstr(s[nr].cod,";\">"); ////folosim 2 pointeri pentru a citi ce se afla intre ";\" si "</p>"
            ps=strstr(s[nr].cod,"</p>");
            inc=pi-s[nr].cod+3;
            sf=ps-s[nr].cod;
            s[nr].continut=(char *)calloc((sf-inc+1),sizeof(char)); //alocam dinamic memoria pentru continut
            if(s[nr].continut==NULL)
            {
                printf("MEMORY NOT ALLOCATED for site %d",nr);
                return 0;
            }
            k=0;
            for(j=inc; j<sf; j++) //adugam in continut codul parsat
                s[nr].continut[k++]=s[nr].cod[j]; 
            s[nr].continut[k]='\0'; //adugam terminatorul de sir
        }

        nr++;
        fclose(st);
    }
    fclose(master);
    for(i=0; i<nr; i++)
        printf("%s %d %s\n",s[i].URL,s[i].viz,s[i].titlu); //afisam numarul de vizualizari, URL-ul si titlul
    for(i=0; i<nr; i++)
    {
        free(s[i].continut);
        free(s[i].cod);
    }
    free(s);
    return 0;
}
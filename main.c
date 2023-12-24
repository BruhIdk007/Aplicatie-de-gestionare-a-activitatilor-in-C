#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
struct Activitate{
    char *nume;
    char *descriere;
    char *data_limita;
    unsigned int *prioritate; // 0 - scazuta, 1 - medie, 2 - ridicata
    struct Activitate *urm;    // ne vom folosi de o lista simplu inlantuita de activitati
};
void elibereazaActivitate(struct Activitate *act)
{
    free(act->nume);
    free(act->descriere);
    free(act->data_limita);
    free(act->prioritate);
    free(act);
}
void adaugaActivitate(struct Activitate **prima)
{
    struct Activitate *noua = (struct Activitate *)malloc(sizeof(struct Activitate));
    if(noua==NULL)
    {
        printf("Nu s-a putut aloca memorie pentru un nod de tip activitate =(");
        return;
    }
    noua->nume=(char *)malloc(100*sizeof(char)); // ne asteptam la maxim 100 de caractere
    noua->descriere=(char *)malloc(100*sizeof(char));
    noua->data_limita=(char *)malloc(12*sizeof(char)); // o data in format DD.MM.YYYY ar necesita 11 caractere, cu caracterul \0, 12
    noua->prioritate=(unsigned int *)malloc(sizeof(unsigned int));
    if(noua->nume==NULL || noua->descriere==NULL || noua->data_limita==NULL || noua->prioritate==NULL)
    {
        printf("Nu am putut aloca memorie pentru cel putin un atribut al unei activitati");
        elibereazaActivitate(noua);
        return;
    }
    FILE *file=fopen("date.txt","a");
    if(file==NULL)
    {
        printf("Fisierul nu poate fi deschis");
        elibereazaActivitate(noua);
        return;

    }
        getchar();
        printf("Introdu pe rand, atributele activitatii tale de adaugat\n");
        printf("Prima data, doresc numele activitatii tale(maxim 100 de caractere)\n");
        fgets(noua->nume,100,stdin);
        noua->nume[strlen(noua->nume)-1]='\0';
        printf("Acum, descrierea ei(idem ca la atributul anterior)\n");
        fgets(noua->descriere,100,stdin);
        noua->descriere[strlen(noua->descriere)-1]='\0';
        printf("Acum, data limita(maxim 12 caractere)\n");
        fgets(noua->data_limita,12,stdin);
        noua->data_limita[strlen(noua->data_limita)-1]='\0';
        printf("In final, prioritatea activitatii:0-scazuta,1-medie,2-ridicata\n");
        scanf("%u",noua->prioritate);
        while(*noua->prioritate>2)
        {
            printf("Prioritatea introdusa nu este valida. Alege un numar valid pentru prioritate\n");
            scanf("%u",noua->prioritate);
        }
        getchar();
        noua->urm=NULL;
    if(*prima==NULL)
    {
        *prima=noua;
    }
    else
    {
        struct Activitate *temp= *prima;
        while(temp->urm!=NULL)
    {
        temp=temp->urm;
    }
    temp->urm=noua;
    }
    printf("Adaugare in lista cu succes\n");
    fprintf(file, "%s %s %s %u\n",noua->nume,noua->descriere,noua->data_limita,*noua->prioritate);
    printf("Adaugare in fisier cu succes\n");
    fclose(file);
}
void vizualizeazaActivitati(struct Activitate *prima)
{
    struct Activitate *temp=prima;
    while(temp!=NULL)
    {
        printf("%s %s %s %u\n",temp->nume,temp->descriere,temp->data_limita,*temp->prioritate);
        temp=temp->urm;
    }
    printf("NULL\n");
}
void stergeActivitate(struct Activitate **prima, char *nume)
{
    struct Activitate *temp=*prima;
    struct Activitate *ant=NULL;
    bool gasit=false;
    while(temp!=NULL && !gasit)
    {
        if(strcmp(temp->nume,nume)==0)
        {
            if(ant==NULL)
            {
                *prima=temp->urm;

            }
            else
            {
                ant->urm=temp->urm;
            }
            elibereazaActivitate(temp);
            gasit=true;
        }
        else
        {
            ant=temp;
            temp=temp->urm;
        }
    }
    if(!gasit)
    {
        printf("Activitatea cu numele dat nu exista\n");
        return;
    }
    struct Activitate *curent=*prima;
    FILE *file=fopen("date.txt","a");
     if(file==NULL)
    {
        printf("Fisierul nu poate fi deschis");
        elibereazaActivitate(curent);
        return;

    }
    fprintf(file,"Noua lista dupa stergerea activitatii:\n");
    while(curent!=NULL)
    {
        fprintf(file, "%s %s %s %u\n",curent->nume,curent->descriere,curent->data_limita,*curent->prioritate);
        curent=curent->urm;
    }
    fprintf(file,"NULL\n");
    printf("Stergerea s-a realizat cu succes, deschide fisierul date.txt pentru a te convinge ;)\n");
    fclose(file);
}
void meniuEditare()
{
            printf("Ce doresti sa modifici la activitatea ta?\n");
            printf("1.Numele\n");
            printf("2.Descrierea\n");
            printf("3.Data limita\n");
            printf("4.Prioritatea(cu numere de la 0 la 2)\n");
            printf("5.Salveaza si iesi\n");
}
 void editareActivitate(struct Activitate **prima, char *nume)
{
    unsigned int optiune;
    bool gasit=false;
    struct Activitate *temp=*prima;
    while(temp!=NULL && !gasit)
    {
        if(strcmp(temp->nume,nume)==0)
        {
            printf("Am gasit activitatea pe care vrei sa o editezi dupa nume\n");
            gasit=true;
            while(1)
            {
            meniuEditare();
            scanf("%u",&optiune);
            getchar();
            switch(optiune)
            {
            case 1:
            printf("Introdu un nou nume pentru activitatea ta:\n");
            char *numeNou;
            numeNou=(char *)malloc(100*sizeof(char));
            fgets(numeNou,100,stdin);
            numeNou[strlen(numeNou)-1]='\0';
            strcpy(temp->nume,numeNou);
            free(numeNou);
            break;
            case 2:
            printf("Introdu o noua descriere pentru activitatea ta:\n");
            char *descriereNoua;
            descriereNoua=(char *)malloc(100*sizeof(char));
            fgets(descriereNoua,100,stdin);
            descriereNoua[strlen(descriereNoua)-1]='\0';
            strcpy(temp->descriere,descriereNoua);
            free(descriereNoua);
            break;
            case 3:
            printf("Introdu o noua data limita pentru activitatea ta:\n");
            char *data_noua;
            data_noua=(char *)malloc(12*sizeof(char));
            fgets(data_noua,12,stdin);
            data_noua[strlen(data_noua)-1]='\0';
            strcpy(temp->data_limita,data_noua);
            free(data_noua);
            break;
            case 4:
            printf("Introdu un nou ordin de prioritate(0-2) pentru activitatea ta:\n");
            unsigned int *prioritateNoua;
            prioritateNoua=(unsigned int*)malloc(sizeof(unsigned int));
            scanf("%u",prioritateNoua);
            temp->prioritate=prioritateNoua;
            getchar();
            break;
            case 5:
            printf("Am salvat si urmeaza sa iesim\n");
            return;
            default:
            printf("Optiunea ta nu se regaseste in lista celor disponibile\n");
            break;
            }
            }
        }
        temp=temp->urm;
    }
    if(!gasit)
    {
        printf("Activitatea cu numele introdus nu exista\n");;
        return;
    }
    elibereazaActivitate(temp);
}
void elibereazaLista(struct Activitate **prima)
{
    struct Activitate *temp=*prima;
    struct Activitate *urmator;
    while(temp!=NULL)
    {
        urmator=temp->urm;
        elibereazaActivitate(temp);
        temp=urmator;
    }
    *prima=NULL;
}
void meniu()
{
        printf("Ai urmatoarele optiuni la dispozitie:\n");
        printf("1.Adauga o activitate\n");
        printf("2.Vizualizarea activitatilor(sarcinilor)\n");
        printf("3.Stergerea unei sarcini\n");
        printf("4.Editarea unei activitati\n");
        printf("5.Iesire din aplicatie\n");
        printf("Daca te plictisesti tare, poti incerca sa introduci si valori mai mari decat 5 ;)\n");
}
int main()
{
    unsigned int optiune;
    char *nume;
    struct Activitate *prima=NULL;
    printf("Written in C by RAT_M0nster on Pop_OS xD\n");
    printf("Bine ai venit in umila mea aplicatie de gestionare a activitatilor, scrisa in limbajul C\n");
    printf("Sper sa te distrezi testand functionalitatile ei ;) \n");
    meniu();
    while(1)
    {
         scanf("%u",&optiune);
         switch(optiune)
         {
            case 1:
            adaugaActivitate(&prima);
            break;
            case 2:
            vizualizeazaActivitati(prima);
            break;
            case 3:
            nume=(char *)malloc(100*sizeof(char));
            printf("Introdu numele activitatii pe care vrei sa o stergi:\n");
            getchar();
            fgets(nume,100,stdin);
            nume[strlen(nume)-1]='\0';
            stergeActivitate(&prima,nume);
            free(nume);
            break;
            case 4:
            nume=(char *)malloc(100*sizeof(char));
            printf("Introdu numele activitatii pe care vrei sa o editezi:\n");
            getchar();
            fgets(nume,100,stdin);
            nume[strlen(nume)-1]='\0';
            editareActivitate(&prima,nume);
            free(nume);
            break;
            case 5:
            printf("Asa repede ma parasesti?\nNe revedem pe Github(pe contul BruhIdk007), nu scapi de mine;)\n");
            elibereazaLista(&prima);
            return 0;
            default:
            printf("Hopa, ai introdus o valoare ce nu se gaseste in meniu, introdu una din valorile asteptate(1-5)\n");
            break;
         }
    }
}

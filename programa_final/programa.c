#include <string.h>
#include <stdlib.h>
#include <stdio.h>
struct Calendar
{
    char subject[300];
    int StartYear,EndYear;
    int StartMonth,EndMonth;
    int StartDay,EndDay;
    int StartHour,EndHour;
    int StartMin,EndMin;
    int StartSecond,EndSecond;
    int allDay;
    char location[300];
    char description[300];
    char uid[300];
};//Estrutura do ICalender(ICS)

struct Calendar calendarioICS[365*24]; //Calendario ICS
struct Calendar calendarCSV[365*24]; //Calendario CSV

int numeroDeEventosICS; //Numero de entradas do calendor ICS
int numeroDeEventosCSV; //Numero de entradas no calendar CSV
void menu();
void openICS();
void openCSV();
void changeEntry(int j);
void printCalendar(struct Calendar calendarioICS[],int numeroDeEventosICS);
void printCSV();
int parseICS(char linha[],struct Calendar calendarioICS[],int numeroEvento);
int parseCSV(char linha[],int numeroDeEventosCSV);
void exportCSV();
void searchEntryByName(int change);
void searchEntryByDate(int change);
int compareCalendar(int Sday,int Eday,int Smonth,int Emonth,int Syear,int Eyear,int Shour,int Ehour,int Smin,int Emin);

int main()
{
    menu();
    return 0;
}
void menu()
{
    int opcao;
    printf("1-Importar                  ICS\n");
    printf("2-Importar                  CSV\n");
    printf("3-Visualizar                ICS\n");
    printf("4-Pesquisar Entrada por     data\n");
    printf("5-Pesquisar Entrada por     evento\n");
    printf("6-Editar uma Entrada por    data\n");
    printf("7-Editar uma Entrada por    evento\n");
    printf("8-Exportar em               CSV\n");
    printf("9-Visualizar                CSV\n");
    printf("10-Sair\n");
    scanf("%d",&opcao);
    switch(opcao)
    {
    case(1):
        openICS();
        menu();
        break;
    case(2):
        openCSV();
        menu();
        break;
    case(3):
        printCalendar(calendarioICS,numeroDeEventosICS);
        menu();
        break;
    case(4):
        searchEntryByDate(0);
        menu();
        break;
    case(5):
        searchEntryByName(0);
        menu();
        break;
    case(6):
        searchEntryByDate(1);
        menu();
        break;
    case(7):
        searchEntryByName(1);
        menu();
        break;
    case(8):
        exportCSV();
        menu();
        break;
    case(9):
        printCSV();
        menu();
        break;
    case(10):
        return;
    default:
        menu();
    }
}

void openICS()
{
    char nomeFicheiro[300];
    printf("Qual é ficheiro ICS a importar?\n");
    scanf("%s",nomeFicheiro); //Tem de ter extensão, ou seja se o ficheiro se chamar bd, tem se colocar bd.ics na leitura
    FILE *fp;
    char linha[255];
    fp = fopen(nomeFicheiro, "r");
    numeroDeEventosICS=0;
    while(fgets(linha,1000, fp)!=NULL)
    {
        if(parseICS(linha,calendarioICS,numeroDeEventosICS)==1)
            numeroDeEventosICS=numeroDeEventosICS+1;
    }
}

void openCSV()
{

    char nomeFicheiro[300];
    printf("Qual é ficheiro ICS a importar?\n");
    scanf("%s",nomeFicheiro); //Tem de ter extensão, ou seja se o ficheiro se chamar bd, tem se colocar bd.ics na leitura
    FILE *fp;
    char linha[255];
    fp = fopen(nomeFicheiro, "r");
    int primeiraLinha=0;
    numeroDeEventosCSV=0;
    while(fgets(linha,3500, fp)!=NULL)
    {
        if(primeiraLinha==0)
        {
            primeiraLinha=1;
        }
        else
        {
            parseCSV(linha,numeroDeEventosCSV);
            numeroDeEventosCSV=numeroDeEventosCSV+1;
        }
    }
    int i;
    for(i=0;i<numeroDeEventosCSV;i++)
    {
        int result=compareCalendar(calendarCSV[i].StartDay,calendarCSV[i].EndDay,calendarCSV[i].StartMonth,calendarCSV[i].EndMonth,calendarCSV[i].StartYear,calendarCSV[i].EndYear,calendarCSV[i].StartHour,
                                   calendarCSV[i].EndHour,calendarCSV[i].StartMin,calendarCSV[i].EndMin);
        if(result==0)
        {
            strcpy(calendarioICS[numeroDeEventosICS].subject,calendarCSV[i].subject);
            strcpy(calendarioICS[numeroDeEventosICS].location,calendarCSV[i].location);
            strcpy(calendarioICS[numeroDeEventosICS].uid,calendarCSV[i].uid);
            strcpy(calendarioICS[numeroDeEventosICS].description,calendarCSV[i].description);
            calendarioICS[numeroDeEventosICS].EndDay=calendarCSV[i].EndDay;
            calendarioICS[numeroDeEventosICS].EndHour=calendarCSV[i].EndHour;
            calendarioICS[numeroDeEventosICS].EndMin=calendarCSV[i].EndMin;
            calendarioICS[numeroDeEventosICS].EndMonth=calendarCSV[i].EndMonth;
            calendarioICS[numeroDeEventosICS].EndSecond=0;
            calendarioICS[numeroDeEventosICS].EndYear=calendarCSV[i].EndYear;
            calendarioICS[numeroDeEventosICS].StartDay=calendarCSV[i].StartDay;
            calendarioICS[numeroDeEventosICS].StartHour=calendarCSV[i].StartHour;
            calendarioICS[numeroDeEventosICS].StartMin=calendarCSV[i].StartMin;
            calendarioICS[numeroDeEventosICS].StartSecond=0;
            calendarioICS[numeroDeEventosICS].StartYear=calendarCSV[i].StartYear;
            calendarioICS[numeroDeEventosICS].StartMonth=calendarCSV[i].StartMonth;
            numeroDeEventosICS=numeroDeEventosICS+1;
        }
    }
}
void changeEntry(int j)
{
    char subject[300];
    int StartYear,EndYear;
    int StartMonth,EndMonth;
    int StartDay,EndDay;
    int StartHour,EndHour;
    int StartMin,EndMin;
    int StartSecond,EndSecond;
    int allDay;
    char location[300];
    char description[300];
    char uid[300];

    printf("Insira um subject:\n");
    fgets( subject, 300, stdin );
    printf("Insira o mes de inicio:\n");
    scanf("%d",&StartMonth);
    printf("Insira o dia de inicio \n");
    scanf("%d",&StartDay);
    printf("Insira o ano de inicio:\n");
    scanf("%d",&StartYear);
    printf("Insira a hora de inicio:\n");
    scanf("%d",&StartHour);
    printf("Insira os minutos de inicio:\n");
    scanf("%d",&StartMin);
    printf("Insira o mes de final:\n");
    scanf("%d",&EndMonth);
    printf("Insira o dia de final \n");
    scanf("%d",&EndDay);
    printf("Insira o ano de final:\n");
    scanf("%d",&EndYear);
    printf("Insira a hora de final:\n");
    scanf("%d",&EndHour);
    printf("Insira os minutos de final:\n");
    scanf("%d",&EndMin);
    printf("Insira se é o dia todo(0/1):\n");
    scanf("%d",&allDay);
    printf("Insira a descrição:\n");
    fgets( description, 300, stdin );
    fgets( description, 300, stdin );
    printf("Insira uma localização:\n");
    fgets( location, 300, stdin );
    printf("Insira um UID:\n");
    fgets( uid, 300, stdin );
    if(compareCalendar(StartDay,EndDay,StartMonth,EndMonth,StartYear,EndYear,StartHour,EndHour,StartMin,EndMin)==0)
    {
            calendarioICS[j].subject[0]='\0';
            calendarioICS[j].location[0]='\0';
            calendarioICS[j].uid[0]='\0';
            calendarioICS[j].description[0]='\0';
            strcpy(calendarioICS[j].subject,subject);
            strcpy(calendarioICS[j].location,location);
            strcpy(calendarioICS[j].uid,uid);
            strcpy(calendarioICS[j].description,description);
            calendarioICS[j].EndDay=EndDay;
            calendarioICS[j].EndHour=EndHour;
            calendarioICS[j].EndMin=EndMin;
            calendarioICS[j].EndMonth=EndMonth;
            calendarioICS[j].EndSecond=0;
            calendarioICS[j].allDay=allDay;
            calendarioICS[j].EndYear=EndYear;
            calendarioICS[j].StartDay=StartDay;
            calendarioICS[j].StartHour=StartHour;
            calendarioICS[j].StartMin=StartMin;
            calendarioICS[j].StartSecond=0;
            calendarioICS[j].StartYear=StartYear;
            calendarioICS[j].StartMonth=StartMonth;
            printf("Indice actualizado!\n");
    }
    else
        printf("Essa data já contem um evento!\n");
}
void printCalendar(struct Calendar calendarioICS[],int numeroDeEventosICS)
{
    int j;
    for(j=0;j<numeroDeEventosICS;j++)
    {
        printf("Subject:%s\nStartYear=%d\nStartMonth=%d\nStartDay=%d\nStartHour=%d\nStartMin=%d\nStartSeconds=%d\nAllday=%d\nEndYear=%d\nEndMonth=%d\nEndDay=%d\nEndHour=%d\nEndMin=%d\nEndSeconds=%d\nLocation=%s\nDescription=%s\nUID=%s\n",
               calendarioICS[j].subject,calendarioICS[j].StartYear,calendarioICS[j].StartMonth,calendarioICS[j].StartDay,calendarioICS[j].StartHour,calendarioICS[j].StartMin,calendarioICS[j].StartSecond,calendarioICS[j].allDay,
               calendarioICS[j].EndYear,calendarioICS[j].EndMonth,calendarioICS[j].EndDay,calendarioICS[j].EndHour,calendarioICS[j].EndMin,calendarioICS[j].EndSecond,
               calendarioICS[j].location,calendarioICS[j].description,calendarioICS[j].uid);
    }
}
void printCSV()
{
    int j;
    for(j=0;j<numeroDeEventosCSV;j++)
    {
        printf("Subject:%s\nStartYear=%d\nStartMonth=%d\nStartDay=%d\nStartHour=%d\nStartMin=%d\nStartSeconds=%d\nAllday=%d\nEndYear=%d\nEndMonth=%d\nEndDay=%d\nEndHour=%d\nEndMin=%d\nEndSeconds=%d\nLocation=%s\nDescription=%s\nUID=%s\n\n",
               calendarCSV[j].subject,calendarCSV[j].StartYear,calendarCSV[j].StartMonth,calendarCSV[j].StartDay,calendarCSV[j].StartHour,calendarCSV[j].StartMin,calendarCSV[j].StartSecond,calendarCSV[j].allDay,
               calendarCSV[j].EndYear,calendarCSV[j].EndMonth,calendarCSV[j].EndDay,calendarCSV[j].EndHour,calendarCSV[j].EndMin,calendarCSV[j].EndSecond,
               calendarCSV[j].location,calendarCSV[j].description,calendarCSV[j].uid);
    }
}
int parseICS(char linha[],struct Calendar calendarioICS[],int numeroEvento)
{
    const char s[2] = ":";
    char *token;
    token = strtok(linha, s);

    if(strcmp(token,"SUMMARY")==0)
    {
        token = strtok(NULL, s);
        strcpy(calendarioICS[numeroEvento].subject,token);
    }
    else if(strcmp(token,"DESCRIPTION")==0)
    {
        token = strtok(NULL, s);
        strcpy(calendarioICS[numeroEvento].description,token);
    }
    else if(strcmp(token,"DTSTART")==0)
    {
            token = strtok(NULL, s);
            char *year=(char*)malloc(4);
            char *month=(char*)malloc(2);
            char *day=(char*)malloc(2);
            char *hour=(char*)malloc(2);
            char *min=(char*)malloc(2);
            char *sec=(char*)malloc(2);


            //strncpy(Variavel destino,Variavel origem,Caracteres a copiar)
            strncpy(year, token, 4);//Da posição 0 ate a posicao 4
            strncpy(month, token+4, 2);//Da posição 4 do token até á posição 6
            strncpy(day, token+6, 2);//Da posicao 6 ate a 8
            strncpy(hour, token+9, 2);//Da 9 ate a 11
            strncpy(min, token+11, 2);//Da 11 a 13
            strncpy(sec, token+13, 2);//13 a 15

            calendarioICS[numeroEvento].StartYear=atoi(year);
            calendarioICS[numeroEvento].StartMonth=atoi(month);
            calendarioICS[numeroEvento].StartDay=atoi(day);
            calendarioICS[numeroEvento].StartHour=atoi(hour);
            calendarioICS[numeroEvento].StartMin=atoi(min);
            calendarioICS[numeroEvento].StartSecond=atoi(sec);
            calendarioICS[numeroEvento].allDay=0;
    }
    else if(strcmp(token,"DTSTART;VALUE=DATE")==0)
    {
            token = strtok(NULL, s);
            char *year=(char*)malloc(4);
            char *month=(char*)malloc(2);
            char *day=(char*)malloc(2);

            strncpy(year, token, 4);
            strncpy(month, token+4, 2);
            strncpy(day, token+6, 2);

            calendarioICS[numeroEvento].StartYear=atoi(year);
            calendarioICS[numeroEvento].StartMonth=atoi(month);
            calendarioICS[numeroEvento].StartDay=atoi(day);

            calendarioICS[numeroEvento].StartHour=0;
            calendarioICS[numeroEvento].StartMin=0;
            calendarioICS[numeroEvento].StartSecond=0;

            calendarioICS[numeroEvento].allDay=1;
    }
    else if(strcmp(token,"DTEND;VALUE=DATE")==0)
    {
            token = strtok(NULL, s);
            char Eyear[5];
            char Emonth[3];
            char Eday[3];
            strncpy(Eyear, token, 4);
            strncpy(Emonth, token+4, 2);
            strncpy(Eday, token+6, 2);
            calendarioICS[numeroEvento].EndYear=atoi(Eyear);
            calendarioICS[numeroEvento].EndMonth=atoi(Emonth);
            calendarioICS[numeroEvento].EndDay=atoi(Eday);
            calendarioICS[numeroEvento].EndHour=24;
            calendarioICS[numeroEvento].EndMin=0;
            calendarioICS[numeroEvento].EndSecond=0;
    }
    else if(strcmp(token,"DTEND")==0)
    {
            token = strtok(NULL, s);
            char *year=(char*)malloc(4);
            char *month=(char*)malloc(2);
            char *day=(char*)malloc(2);
            char *hour=(char*)malloc(2);
            char *min=(char*)malloc(2);
            char *sec=(char*)malloc(2);
            strncpy(year, token, 4);
            strncpy(month, token+4, 2);
            strncpy(day, token+6, 2);
            strncpy(hour, token+9, 2);
            strncpy(min, token+11, 2);
            strncpy(sec, token+13, 2);
            calendarioICS[numeroEvento].EndYear=atoi(year);
            calendarioICS[numeroEvento].EndMonth=atoi(month);
            calendarioICS[numeroEvento].EndDay=atoi(day);
            calendarioICS[numeroEvento].EndHour=atoi(hour);
            calendarioICS[numeroEvento].EndMin=atoi(min);
            calendarioICS[numeroEvento].EndSecond=atoi(sec);
    }
    else if(strcmp(token,"LOCATION")==0)
    {
        token = strtok(NULL, s);
        strcpy(calendarioICS[numeroEvento].location,token);
    }
    else if(strcmp(token,"UID")==0)
    {
        token = strtok(NULL, s);
        strcpy(calendarioICS[numeroEvento].uid,token);
    }
    else if(strcmp(token,"END")==0)
    {
        token = strtok(NULL, s);
        if(strcmp(token,"VEVENT\n")==0)
            return 1;
    }
    return 0;
}
int parseCSV(char linha[],int numeroDeEventosCSV)
{
    const char s[2] = ",";
    char *token;
    char StartMonth[3];
    char StartDay[3];
    char StartYear[5];

    char StartHour[3];
    char StartMin[3];
    char type[3];

    char min[2];
    char tipo[2];

    char EndMonth[3];
    char EndDay[3];
    char EndYear[5];
    char EndHour[3];
    char EndMin[3];

    token = strtok(linha, s);

    if(token==NULL)
    {
        printf("ERROR READING THE FILE!\n");
        return;
    }
    printf("TOKEN 1 %s\n",token);

    strcpy(calendarCSV[numeroDeEventosCSV].subject,token);

    token= strtok(NULL,s);
    strncpy(StartMonth,token,2);
    strncpy(StartDay,token+3,2);
    strncpy(StartYear,token+6,4);

    calendarCSV[numeroDeEventosCSV].StartMonth=atoi(StartMonth);
    calendarCSV[numeroDeEventosCSV].StartDay=atoi(StartDay);
    calendarCSV[numeroDeEventosCSV].StartYear=atoi(StartYear);

    printf("Day %d Month %d Year %d\n",calendarCSV[numeroDeEventosCSV].StartDay,calendarCSV[numeroDeEventosCSV].StartMonth,calendarCSV[numeroDeEventosCSV].StartYear);
    token= strtok(NULL,s);

    //Strstr,verifica se uma string está contida noutra, senão null
    if (strstr(token, "/") == NULL) //SENAO TIVER / NA HORA
    {
        strncpy(StartHour,token,2);
        strncpy(StartMin,token+2,2);
        strncpy(type,token+4,2);
        if(strstr(token,"pm")!=NULL)
        {
            calendarCSV[numeroDeEventosCSV].StartHour=atoi(StartHour)+12;
            calendarCSV[numeroDeEventosCSV].StartMin=atoi(StartMin);
        }
        else
        {
            calendarCSV[numeroDeEventosCSV].StartHour=atoi(StartHour);
            calendarCSV[numeroDeEventosCSV].StartMin=atoi(StartMin);
        }
        token=strtok(NULL,s);
        strncpy(EndMonth,token,2);
        strncpy(EndDay,token+3,2);
        strncpy(EndYear,token+6,4);

        calendarCSV[numeroDeEventosCSV].EndMonth=atoi(EndMonth);
        calendarCSV[numeroDeEventosCSV].EndDay=atoi(EndDay);
        calendarCSV[numeroDeEventosCSV].EndYear=atoi(EndYear);

        token=strtok(NULL,s);
        printf("TOKEN %s\n",token);
        strncpy(EndHour,token,2);
        strncpy(EndMin,token+2,2);

        if(strstr(token,"pm")!=NULL)
        {
            printf("OLA");
            calendarCSV[numeroDeEventosCSV].EndHour=atoi(EndHour)+12;
            calendarCSV[numeroDeEventosCSV].EndMin=atoi(EndMin);
        }
        else
        {
            calendarCSV[numeroDeEventosCSV].EndHour=atoi(EndHour);
            calendarCSV[numeroDeEventosCSV].EndMin=atoi(EndMin);
        }
        printf("End Hour %d End Min %d",calendarCSV[numeroDeEventosCSV].EndHour,calendarCSV[numeroDeEventosCSV].EndMin);
    }
    else //Se não existir horas!
    {
        strncpy(EndMonth,token,2);
        strncpy(EndDay,token+3,2);
        strncpy(EndYear,token+6,4);
        calendarCSV[numeroDeEventosCSV].EndMonth=atoi(EndMonth);
        calendarCSV[numeroDeEventosCSV].EndDay=atoi(EndDay);
        calendarCSV[numeroDeEventosCSV].EndYear=atoi(EndYear);
    }

    token=strtok(NULL,s);
    if(token==NULL)
    {
        printf("ERROR READING THE FILE!\n");
        return;
    }
    printf("TOKEN 6 %s\n",token);

    if(strcmp(token,"false")==0)
        calendarCSV[numeroDeEventosCSV].allDay=0;
    else
    {
       calendarCSV[numeroDeEventosCSV].allDay=1;
       calendarCSV[numeroDeEventosCSV].EndHour=24;
    }
    token=strtok(NULL,s);
    if(token==NULL)
    {
        printf("ERROR READING THE FILE!\n");
        return;
    }
    printf("TOKEN 7 %s\n",token);
    strcpy(calendarCSV[numeroDeEventosCSV].description,token);

    token=strtok(NULL,s);

    if(token==NULL)
    {
        printf("ERROR READING THE FILE!\n");
        return;
    }
    printf("TOKEN 8 %s\n",token);
    strcpy(calendarCSV[numeroDeEventosCSV].location,token);

    token=strtok(NULL,s);
    strcpy(calendarCSV[numeroDeEventosCSV].uid,token);
}
void exportCSV()
{ //Exportar CSV
    printf("Qual é o nome do ficheiro a exportar?\n");
    char nome[300];
    scanf("%s",nome);
    FILE *f = fopen(nome, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    int i;
    char s[1000]="Subject,Start Date,Start Time,End Date,End Time,All Day,Description,Location,UID\n";
    fputs(s,f);
    for(i=0;i<numeroDeEventosICS;i++)
    {
        char str[1000];
        char auxiliar[100];
        int hora;
        int tipo=0;
        str[0]='\0';
        auxiliar[0]='\0';

        if(calendarioICS[i].subject[strlen(calendarioICS[i].subject)-1]=='\n')
            calendarioICS[i].subject[strlen(calendarioICS[i].subject)-1]='\0';
        if(calendarioICS[i].location[strlen(calendarioICS[i].location)-1]=='\n')
            calendarioICS[i].location[strlen(calendarioICS[i].location)-1]='\0';
        if(calendarioICS[i].description[strlen(calendarioICS[i].description)-1]=='\n')
            calendarioICS[i].description[strlen(calendarioICS[i].description)-1]='\0';
        if(calendarioICS[i].uid[strlen(calendarioICS[i].uid)-1]=='\n')
            calendarioICS[i].uid[strlen(calendarioICS[i].uid)-1]='\0';

        strcat(str,calendarioICS[i].subject);
        strcat(str,",");

        sprintf(auxiliar, "%d", calendarioICS[i].StartMonth);
        if(strlen(auxiliar)==1)
        {
            auxiliar[1]=auxiliar[0];
            auxiliar[0]='0';
            auxiliar[2]='\0';
        }
        strcat(str,auxiliar);
        strcat(str,"/");
        sprintf(auxiliar, "%d", calendarioICS[i].StartDay);
        if(strlen(auxiliar)==1)
        {
            auxiliar[1]=auxiliar[0];
            auxiliar[0]='0';
            auxiliar[2]='\0';
        }
        strcat(str,auxiliar);
        strcat(str,"/");
        sprintf(auxiliar, "%d", calendarioICS[i].StartYear);
        strcat(str,auxiliar);
        strcat(str,",");

        hora=calendarioICS[i].StartHour;
        if(hora>12)
        {
           hora=hora-12;
           tipo=1;
        }
        sprintf(auxiliar, "%d", hora);
        strcat(str,auxiliar);
        strcat(str,":");

        sprintf(auxiliar, "%d", calendarioICS[i].StartMin);
        if(strlen(auxiliar)==1)
            strcat(auxiliar,"0");
        strcat(str,auxiliar);
        strcat(str,"");
        if(tipo==0)
            strcat(str,"am,");
        else
            strcat(str,"pm,");

        sprintf(auxiliar, "%d", calendarioICS[i].EndMonth);
        if(strlen(auxiliar)==1)
        {
            auxiliar[1]=auxiliar[0];
            auxiliar[0]='0';
            auxiliar[2]='\0';
        }
        strcat(str,auxiliar);
        strcat(str,"/");
        sprintf(auxiliar, "%d", calendarioICS[i].EndDay);
        if(strlen(auxiliar)==1)
        {
            auxiliar[1]=auxiliar[0];
            auxiliar[0]='0';
            auxiliar[2]='\0';
        }
        strcat(str,auxiliar);
        strcat(str,"/");
        sprintf(auxiliar, "%d", calendarioICS[i].EndYear);
        strcat(str,auxiliar);
        strcat(str,",");

        hora=calendarioICS[i].EndHour;
        if(hora>12)
        {
           hora=hora-12;
           tipo=1;
        }
        sprintf(auxiliar, "%d", hora);
        strcat(str,auxiliar);
        strcat(str,":");

        sprintf(auxiliar, "%d", calendarioICS[i].EndMin);
        if(strlen(auxiliar)==1)
            strcat(auxiliar,"0");
        strcat(str,auxiliar);
        strcat(str,"");
        if(tipo==0)
            strcat(str,"am,");
        else
            strcat(str,"pm,");
        int allday=calendarioICS[i].allDay;
        if(allday==0)
            strcat(str,"false,");
        else
            strcat(str,"true,");

        strcat(str,calendarioICS[i].description);
        strcat(str,",");
        strcat(str,calendarioICS[i].location);
        strcat(str,",");
        strcat(str,calendarioICS[i].uid);
        strcat(str,"\n");
        fputs(str,f);
    }
}
void searchEntryByName(int change)
{
    char pesquisa[300];
    printf("Qual é o nome que quer pesquisar?\n");
    fgets( pesquisa, 300, stdin );
    fgets( pesquisa, 300, stdin );
    pesquisa[strlen(pesquisa)-1]='\0';
    int j;
    for(j=0;j<numeroDeEventosICS;j++)
    {
        if(strcmp(calendarioICS[j].subject,pesquisa)==0)
        {
             printf("Subject:%s\nStartYear=%d\nStartMonth=%d\nStartDay=%d\nStartHour=%d\nStartMin=%d\nStartSeconds=%d\nAllday=%d\nEndYear=%d\nEndMonth=%d\nEndDay=%d\nEndHour=%d\nEndMin=%d\nEndSeconds=%d\nLocation=%s\nDescription=%s\nUID=%s\n",
               calendarioICS[j].subject,calendarioICS[j].StartYear,calendarioICS[j].StartMonth,calendarioICS[j].StartDay,calendarioICS[j].StartHour,calendarioICS[j].StartMin,calendarioICS[j].StartSecond,calendarioICS[j].allDay,
               calendarioICS[j].EndYear,calendarioICS[j].EndMonth,calendarioICS[j].EndDay,calendarioICS[j].EndHour,calendarioICS[j].EndMin,calendarioICS[j].EndSecond,
               calendarioICS[j].location,calendarioICS[j].description,calendarioICS[j].uid);
               break;
        }
    }
    if(change==1)
        changeEntry(j);
}
void searchEntryByDate(int change)
{
    char data[300];
    printf("Qual é a data que quer pesquisar?(dia/mes/ano)\n");
    scanf("%s",data);
    int j;
    int ano,mes,dia;
    char year[5],month[3],day[3];
    strncpy(year, data+6, 4);
    strncpy(month, data+3, 2);
    strncpy(day, data, 2);

    ano=atoi(year);
    mes=atoi(month);
    dia=atoi(day);

    char opcao[2];

    for(j=0;j<numeroDeEventosICS;j++)
    {
        if((calendarioICS[j].StartDay==dia && calendarioICS[j].StartMonth==mes && calendarioICS[j].StartYear==ano)||(calendarioICS[j].EndDay==dia && calendarioICS[j].EndMonth==mes && calendarioICS[j].EndYear==ano))
        {
                printf("Subject:%s\nStartYear=%d\nStartMonth=%d\nStartDay=%d\nStartHour=%d\nStartMin=%d\nStartSeconds=%d\nAllday=%d\nEndYear=%d\nEndMonth=%d\nEndDay=%d\nEndHour=%d\nEndMin=%d\nEndSeconds=%d\nLocation=%s\nDescription=%s\nUID=%s\n",
               calendarioICS[j].subject,calendarioICS[j].StartYear,calendarioICS[j].StartMonth,calendarioICS[j].StartDay,calendarioICS[j].StartHour,calendarioICS[j].StartMin,calendarioICS[j].StartSecond,calendarioICS[j].allDay,
               calendarioICS[j].EndYear,calendarioICS[j].EndMonth,calendarioICS[j].EndDay,calendarioICS[j].EndHour,calendarioICS[j].EndMin,calendarioICS[j].EndSecond,
               calendarioICS[j].location,calendarioICS[j].description,calendarioICS[j].uid);
               if(change==1)
               {
                    printf("Quer mudar este evento?(S/N)\n");
                    scanf("%c",opcao);
                    if(strcmp(opcao,"S")==0)
                    {
                        changeEntry(j);
                        break;
                    }
               }
        }
    }

}
int compareCalendar(int Sday,int Eday,int Smonth,int Emonth,int Syear,int Eyear,int Shour,int Ehour,int Smin,int Emin)
{
    int i;
    int flag;
    flag=0;
    for(i=0;i<numeroDeEventosICS;i++)
    {
            //Se o ano dos eventos é o mesmo!
            if(calendarioICS[i].StartYear==Syear && calendarioICS[i].EndYear==Syear && calendarioICS[i].StartYear==Eyear && calendarioICS[i].EndYear==Eyear)
            {
                //Se o mes inicial é o mesmo!
                if(calendarioICS[i].StartMonth==Smonth && calendarioICS[i].EndMonth==Emonth)
                {
                    //Se for tudo no mesmo dia!
                    if(calendarioICS[i].StartDay==Sday && calendarioICS[i].EndDay==Eday)
                    {
                        //Se for o dia todo!
                        if(calendarioICS[i].allDay==1)
                        {
                            flag=1;
                            break;
                        }
                        //Evento calendarioICS 10.30-10.45
                        //Evento calendarioCSV 10.00-10.15
                        //É sempre possivel
                        if(Shour==calendarioICS[i].StartHour && Ehour==calendarioICS[i].StartHour)
                        {
                            if(Smin>=calendarioICS[i].StartMin)
                            {
                                printf("FLAG 2\n");
                                flag=1;
                                break;
                            }
                            else if(Emin>=calendarioICS[i].StartMin)
                            {
                                flag=1;
                                break;
                            }
                        }
                        //Evento calendarioICS 14-18.15
                        //Evento calendarioCSV 18.00-18.45
                        //Possivel
                        if(Shour==calendarioICS[i].EndHour)
                        {
                            if(calendarioICS[i].EndMin<Smin)
                            {
                                flag=1;
                                break;
                            }
                            if(calendarioICS[i].EndHour == Ehour && calendarioICS[i].EndMin<Emin)
                            {
                                flag=1;
                                break;
                            }
                        }
                        //Evento calendarioICS 12-18
                        //Evento calendarioCSV 10-11
                        //Possivel
                        if(Shour<calendarioICS[i].StartHour && Ehour>calendarioICS[i].StartHour)
                        {
                            flag=1;
                            break;
                        }
                        if(Shour>=calendarioICS[i].StartHour && Shour<calendarioICS[i].EndHour)
                        {
                            flag=1;
                            break;
                        }
                    }
                    //Dia inicial e final diferente!!
                    //Se Evento 1 :dia 2 as 14 até 3 as 18
                    //Evento 2: dia 2 as 10 ate as 18
                    else if(calendarioICS[i].StartDay==Sday && calendarioICS[i].StartDay==Eday)
                    {
                        if(Shour>calendarioICS[i].StartHour || Ehour>calendarioICS[i].StartHour)
                        {
                            flag=1;
                            break;
                        }
                    }
                    //Dia inicial e final diferente!!
                    //Se Evento 1 :dia 2 as 14 até 3 as 18
                    //Evento 2: dia 2 as 10 ate dia 3
                    else if(calendarioICS[i].StartDay==Sday && calendarioICS[i].StartDay!=Eday)
                    {
                        flag=1;
                        break;
                    }
                    //Dia inicial e final diferente!!
                    //Se Evento 1 :dia 2 as 14 até 3 as 18
                    //Evento 2: dia 1 as 10 ate dia 2 as 11
                    else if(calendarioICS[i].StartDay==Eday)
                    {
                        if(calendarioICS[i].StartHour<Ehour)
                        {
                            flag=1;
                            break;
                        }
                    }
                    //Dia inicial e final diferente
                    //Dia 30 ate dia 31
                    //Dia 29 ate 30
                    else if(Sday<calendarioICS[i].StartDay && calendarioICS[i].StartDay<Eday)
                    {
                        flag=1;
                        break;
                    }
                }
                //Evento 1: dia 29 mes 1 ate dia 1 mes 2
                //Evento 2: dia 30 mes 1 ate dia 31 mes 1
                else if(calendarioICS[i].StartMonth==Smonth && calendarioICS[i].StartMonth==Emonth)
                {
                    if(calendarioICS[i].StartDay==Eday)
                    {
                        //Evento 1: 14.45 - 2
                        //Evento 2: 14.00-15
                        if(Ehour > calendarioICS[i].StartHour)
                        {
                            flag=1;
                            break;
                        }
                        else if(Ehour==calendarioICS[i].StartHour)
                        {
                            if(Emin >= calendarioICS[i].StartMin)
                            {
                                flag=1;
                                break;
                            }
                        }
                    }
                    else if(calendarioICS[i].StartDay<Eday)
                    {
                        flag=1;
                        break;
                    }
                }
                //Evento 1: dia 29 mes 2 até dia 1 mes 2
                //Evento 2: dia 28 mes 1 até dia 1 mes 2
                else if(Emonth>calendarioICS[i].StartMonth && Smonth<calendarioICS[i].StartMonth)
                {
                    flag=1;
                    break;
                }
                //Evento 1: dia 29 mes 2 até dia 1 mes 3
                //Evento 2: dia 29 mes 1 até dia 29 mes 2
                else if(Emonth==calendarioICS[i].StartMonth)
                {
                    if(Eday>calendarioICS[i].StartDay)
                    {
                        flag=1;
                        break;
                    }
                    else if(Eday==calendarioICS[i].StartDay)
                    {
                        if(Ehour>calendarioICS[i].StartHour)
                        {
                            flag=1;
                            break;
                        }
                        else if(Ehour==calendarioICS[i].StartHour && Emin >= calendarioICS[i].EndMin)
                        {
                            flag=1;
                            break;
                        }
                    }
                }
            }
        }
        return flag;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct //Armazena informacoes relevantes de cada linha do arquivo de transferencias
{
    char uf[3];
    double valor;
}InfoTransf;

typedef struct //Armazena as informacoes de cada estado com as informacoes agrupadas
{
    InfoTransf *dadosUf;
    char uf[3];
    double totalTransf;
    int nInformacoes;
}Estado;

typedef struct//Armazena informacoes relevantes de cada linha do arquivo de diarias
{
    char org_sup[50];
    double valor;
}InfoDiaria;

typedef struct //Armazena as informacoes de cada orgao superior com as informacoes agrupadas
{
    InfoDiaria *dadosDiaria;
    char org_sup[50];
    double totalDiarias;
    int nInformacoes;
}OrgaoSuperior;

void Quicksort1(InfoTransf all[], int esq, int dir); //Ordena os dados de transferencia
void Quicksort2(Estado all[], int esq, int dir); //Ordena os dados agrupados por estado
void Quicksort3(InfoDiaria all[], int esq, int dir); //Ordena os dados de diarias
void Quicksort4(OrgaoSuperior all[], int esq, int dir);  //Ordena os dados agrupados por orgao superior
Estado *AgruparEstados(InfoTransf *dados, int nInfo, int *nEstados); //Agrupa as transferencias por estado
OrgaoSuperior *AgruparOrgaos(InfoDiaria *dados, int nInfo, int *nOrgaos); //Agrupa as diarias pagas por orgao superior
InfoTransf *ColetarDadosTransf(FILE *f, int *nInfo); //Coleta os dados do arquivo de transferencias
InfoDiaria *ColetarDadosDiarias(FILE *f, int *nInfo); //Coleta os dados do arquivo de diarias

int main(int argc, char* argv[])
{
    int i, j;
    char *filename[6]; //Nomes dos arquivos

    for (i = 0; i < 6; i++)
        filename[i] = argv[i+1];

    int nInfoTransf, nEstados;
    for (i = 1; i <= 3; i++) //3 primeiros arquivos = transferencia
    {
        FILE *f = fopen(argv[i], "r");
        printf("\n::.. %s ..::\n", argv[i]);
        printf("Coletando dados...\n");
        InfoTransf *dadosTransf = ColetarDadosTransf(f, &nInfoTransf); //Coleta os dados do arquivo de transferencias e armazena em structs InfoTransf
        fclose(f);
        printf("Quicksort...\n");
        Quicksort1(dadosTransf, 0, nInfoTransf-1); //Ordena as informacoes de transferencias por estado
        Estado *estadosSep = AgruparEstados(dadosTransf, nInfoTransf, &nEstados); //Agrupa as informacoes ordenadas por estado em structs Estado
        Quicksort2(estadosSep, 0, nEstados-1); //Ordena as structs Estado pelo valor total das transferencias relacionadas aquele estado

        char *temp = filename[i-1]; //Substitui csv por txt no filename para salvar as informacoes manipuladas num arquivo com mesmo nome e extensao diferente
        for(j = 0; j < strlen(temp); j++)
        {
            if (temp[j] == 'c' && temp[j+1] == 's' && temp[j+2] == 'v')
            {
                temp[j] = 't'; temp[j+1] = 'x'; temp[j+2] = 't';
            }
        }
        filename[i-1] = temp;

        FILE *novoTransf = fopen(filename[i-1], "w"); //Salva as informacoes manipuladas no novo arquivo txt
        for (j = 0; j < nEstados; j++)
            fprintf(novoTransf, "%s %.2f\n", estadosSep[j].uf, estadosSep[j].totalTransf);
        fclose(novoTransf);
    }

    int nInfoDiaria, nOrgaos;
    for (i = 4; i <= 6; i++) //3 ultimos arquivs = diarias
    {
        FILE *f = fopen(argv[i], "r");
        printf("\n::.. %s ..::\n", argv[i]);
        printf("Coletando dados...\n");
        InfoDiaria *dadosDiaria = ColetarDadosDiarias(f, &nInfoDiaria);//Coleta os dados do arquivo de diarias e armazena em structs InfoDiarias
        fclose(f);
        printf("Quicksort...\n");
        Quicksort3(dadosDiaria, 0, nInfoDiaria-1); //Ordena as informacoes de transferencias por orgao superior
        OrgaoSuperior *orgaosSep = AgruparOrgaos(dadosDiaria, nInfoDiaria, &nOrgaos); //Agrupa as informacoes ordenadas por orgao superior em structs OrgaoSuperior
        Quicksort4(orgaosSep, 0, nOrgaos-1); //Ordena as structs OrgaoSuperior pelo valor total das diarias relacionadas aquele ministerio

        char *temp = filename[i-1]; //Substitui csv por txt no filename para salvar as informacoes manipuladas num arquivo com mesmo nome e extensao diferente
        for(j = 0; j < strlen(temp); j++)
        {
            if (temp[j] == 'c' && temp[j+1] == 's' && temp[j+2] == 'v')
            {
                temp[j] = 't'; temp[j+1] = 'x'; temp[j+2] = 't';
            }
        }
        filename[i-1] = temp;

        FILE *novoDiarias = fopen(filename[i-1], "w"); //Salva as informacoes manipuladas no novo arquivo txt
        for (j = 0; j < nOrgaos; j++)
            fprintf(novoDiarias, "%s %.2f\n", orgaosSep[j].org_sup, orgaosSep[j].totalDiarias);
        fclose(novoDiarias);
    }

    return 0;
}

void Quicksort1(InfoTransf *all, int esq, int dir)
{
    int i = esq, j = dir;
    int pivot = (i + j)/2;
    char item[3];
    strcpy(item, all[pivot].uf);

    do
    {
        while (strcmp(item, all[i].uf) > 0) i++;
        while (strcmp(item, all[j].uf) < 0) j--;

        if (i <= j)
        {
            InfoTransf aux = all[i];
            all[i] = all[j];
            all[j] = aux;
            i++;
            j--;
        }
    }while (i <= j);

    if (esq < j) Quicksort1(all, esq, j);
    if (i < dir) Quicksort1(all, i, dir);
}

void Quicksort2(Estado *all, int esq, int dir)
{
    int i = esq, j = dir;
    int pivot = (i + j)/2;
    double item;
    item = all[pivot].totalTransf;

    do
    {
        while (item > all[i].totalTransf) i++;
        while (item < all[j].totalTransf) j--;

        if (i <= j)
        {
            Estado aux = all[i];
            all[i] = all[j];
            all[j] = aux;
            i++;
            j--;
        }
    }while (i <= j);

    if (esq < j) Quicksort2(all, esq, j);
    if (i < dir) Quicksort2(all, i, dir);
}

void Quicksort3(InfoDiaria *all, int esq, int dir)
{
    int i = esq, j = dir;
    int pivot = (i + j)/2;
    char item[50];
    strcpy(item, all[pivot].org_sup);

    do
    {
        while (strcmp(item, all[i].org_sup) > 0) i++;
        while (strcmp(item, all[j].org_sup) < 0) j--;

        if (i <= j)
        {
            InfoDiaria aux = all[i];
            all[i] = all[j];
            all[j] = aux;
            i++;
            j--;
        }
    }while (i <= j);

    if (esq < j) Quicksort3(all, esq, j);
    if (i < dir) Quicksort3(all, i, dir);
}

void Quicksort4(OrgaoSuperior *all, int esq, int dir)
{
    int i = esq, j = dir;
    int pivot = (i + j)/2;
    double item;
    item = all[pivot].totalDiarias;

    do
    {
        while (item > all[i].totalDiarias) i++;
        while (item < all[j].totalDiarias) j--;

        if (i <= j)
        {
            OrgaoSuperior aux = all[i];
            all[i] = all[j];
            all[j] = aux;
            i++;
            j--;
        }
    }while (i <= j);

    if (esq < j) Quicksort4(all, esq, j);
    if (i < dir) Quicksort4(all, i, dir);
}

Estado *AgruparEstados(InfoTransf *dados, int nInfo, int *nEstados)
{
    int i = 0;

    Estado *estados = (Estado*) malloc (sizeof(Estado)); //Armazena os estados
    *nEstados = 0;

    char uf[3];
    while (i < nInfo)
    {
        InfoTransf *estado = (InfoTransf*) malloc (sizeof(InfoTransf)); //Armazena todas as informacoes relacionadas ao estado
        int totalEst = 0; //Numero de informacoes relacionadas ao estado

        double valorTot = 0; //Valor total de transferencias relacionadas ao estado
        strcpy(uf, dados[i].uf);
        while (strcmp(dados[i].uf, uf) == 0 && i < nInfo) //Agrupa todas as informacoes ate que o estado seja trocado
        {
            estado[totalEst] = dados[i];
            totalEst++;
            estado = (InfoTransf*) realloc (estado, (totalEst+1) * sizeof(InfoTransf));

            valorTot += dados[i].valor;

            i++;
        }

        estados[*nEstados].dadosUf = estado;
        estados[*nEstados].nInformacoes = totalEst;
        strcpy(estados[*nEstados].uf, uf);
        estados[*nEstados].totalTransf = valorTot;

        *nEstados = *nEstados + 1;
        estados = (Estado*) realloc (estados, (*nEstados+1) * sizeof(Estado));
    }

    return estados;
}

OrgaoSuperior *AgruparOrgaos(InfoDiaria *dados, int nInfo, int *nOrgaos)
{
    int i = 0;

    OrgaoSuperior *orgaos = (OrgaoSuperior*) malloc (sizeof(OrgaoSuperior)); //Armazena os ministerios
    *nOrgaos = 0;

    char org_sup[50];
    while (i < nInfo)
    {
        InfoDiaria *orgao = (InfoDiaria*) malloc (sizeof(InfoDiaria)); //Armazena todas as informacoes relacionadas ao ministerio
        int totalOrg = 0; //Numero de informacoes relacionadas ao ministerio

        double valorTot = 0;
        strcpy(org_sup, dados[i].org_sup);
        while (strcmp(dados[i].org_sup, org_sup) == 0 && i < nInfo) //Agrupa todas as informacoes ate que o ministerio seja trocado
        {
            orgao[totalOrg] = dados[i];
            totalOrg++;
            orgao = (InfoDiaria*) realloc (orgao, (totalOrg+1) * sizeof(InfoDiaria));

            valorTot += dados[i].valor;

            i++;
        }

        orgaos[*nOrgaos].dadosDiaria = orgao;
        orgaos[*nOrgaos].nInformacoes = totalOrg;
        strcpy(orgaos[*nOrgaos].org_sup, org_sup);
        orgaos[*nOrgaos].totalDiarias = valorTot;

        *nOrgaos = *nOrgaos + 1;
        orgaos = (OrgaoSuperior*) realloc (orgaos, (*nOrgaos+1) * sizeof(OrgaoSuperior));
    }

    return orgaos;
}

InfoTransf *ColetarDadosTransf(FILE *f, int *nInfo)
{
    int i, j;
    char c;
    char uf_temp[2];
    char nome_temp[50];
    char valor_temp[25];

    InfoTransf *informacoes = (InfoTransf*) malloc(sizeof(InfoTransf));
    *nInfo = 0;

    //Ignorar primeira linha
    c = 0;
    do
    {
        fscanf(f, "%c", &c);
    }
    while (c != '\n');

    while (fscanf(f, "%c", &c) != EOF)
    {
        for (i = 0; i < 18; i++) //Numero de colunas do arquivo
        {
            switch (i) //Colunas de interesse
            {
                case 0: //Sigla do estado
                {
                    j = 0;
                    while (c != 9 && c != '\n' && c != EOF) //Le ate encontrar um separador, quebra de linha ou fim do arquivo (9 = TAB(separador))
                    {
                        uf_temp[j] = c;
                        j++;
                        fscanf(f, "%c", &c);
                    }
                    uf_temp[j] = '\0';
                    strcpy(informacoes[*nInfo].uf, uf_temp);
                }break;

                case 17: //Valor
                {
                    c = 0;
                    j = 0;
                    while (fscanf(f, "%c", &c) != EOF && c != '\n' && c != 9) //Le ate encontrar um separador, quebra de linha ou fim do arquivo (9 = TAB(separador))
                    {
                        if (c == ',') c = '.';
                        valor_temp[j] = c;
                        j++;
                    }
                    valor_temp[j] = '\0';
                    sscanf(valor_temp, "%lf", &informacoes[*nInfo].valor);
                }break;

                default: //Ignora colunas que nao interessam
                {
                    c = 0;
                    while (c != 9)
                        fscanf(f, "%c", &c);
                }break;
            }
        }
        *nInfo = *nInfo + 1;
        informacoes = (InfoTransf*) realloc(informacoes, (*nInfo+1) * sizeof(InfoTransf));
    }

    return informacoes;
}

InfoDiaria *ColetarDadosDiarias(FILE *f, int *nInfo)
{
    int i, j;
    char c;
    char id[10];
    char org_sup[50];
    char valor_temp[25];

    InfoDiaria *informacoes = (InfoDiaria*) malloc(sizeof(InfoDiaria));
    *nInfo = 0;

    //Ignorar primeira linha
    c = 0;
    do
    {
        fscanf(f, "%c", &c);
    }
    while (c != '\n');

    while (fscanf(f, "%c", &c) != EOF)
    {
        for (i = 0; i < 21; i++) //Numero de colunas do arquivo
        {
            switch (i) //Colunas de interesse
            {
                case 1: //Nome do ministerio
                {
                    c = 0;
                    j = 0;
                    while (fscanf(f, "%c", &c) != EOF && c != '\n' && c != 9)//Le ate encontrar um separador, quebra de linha ou fim do arquivo (9 = TAB(separador))
                    {
                        org_sup[j] = c;
                        j++;
                    }
                    org_sup[j] = '\0';
                    strcpy(informacoes[*nInfo].org_sup, org_sup);
                }break;

                case 20: //Valor
                {
                    c = 0;
                    j = 0;
                    while (fscanf(f, "%c", &c) != EOF && c != '\n' && c != 9)//Le ate encontrar um separador, quebra de linha ou fim do arquivo (9 = TAB(separador))
                    {
                        if (c == ',') c = '.';
                        valor_temp[j] = c;
                        j++;
                    }
                    valor_temp[j] = '\0';
                    sscanf(valor_temp, "%lf", &informacoes[*nInfo].valor);
                }break;

                default: //Ignora colunas que nao interessam
                {
                    c = 0;
                    while (c != 9)
                        fscanf(f, "%c", &c);
                }break;
            }
        }
        *nInfo = *nInfo + 1;
        informacoes = (InfoDiaria*) realloc(informacoes, (*nInfo+1) * sizeof(InfoDiaria));
    }

    *nInfo = *nInfo - 1; //Ultima linha do arquivo invalida
    return informacoes;
}

// torre.c
// Sistema de priorização e montagem da torre de resgate
// Implementa: bubble sort por nome, insertion sort por tipo, selection sort por prioridade,
// busca binária por nome (apenas após ordenar por nome), contagem de comparações e tempo.
// Compilar: gcc -o torre torre.c -std=c11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define MAX_NOME 30
#define MAX_TIPO 20

// ------------------------------------------------------------
// STRUCT DO COMPONENTE
// ------------------------------------------------------------
typedef struct
{
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade;
} Componente;

// ------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES (conforme solicitado)
// ------------------------------------------------------------
void bubbleSortNome(Componente arr[], int n, long *comparacoes);
void insertionSortTipo(Componente arr[], int n, long *comparacoes);
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes);
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes);
void mostrarComponentes(Componente arr[], int n);
double medirTempo(void (*algoritmo)(Componente[], int, long *), Componente arr[], int n, long *comparacoes);

// ------------------------------------------------------------
// FUNÇÕES AUXILIARES
// ------------------------------------------------------------

// Remove newline no final de uma string obtida por fgets
void trimNewline(char *s)
{
    size_t len = strlen(s);
    if (len == 0)
        return;
    if (s[len - 1] == '\n')
        s[len - 1] = '\0';
}

// Copia vetor de componentes (útil para medir tempos em cópias)
void copiarComponentes(Componente dest[], Componente src[], int n)
{
    for (int i = 0; i < n; ++i)
        dest[i] = src[i];
}

// Mostra um componente formatado
void mostrarCabecalho()
{
    printf("\n%-3s | %-30s | %-20s | %-9s\n", "Idx", "Nome", "Tipo", "Prioridade");
    printf("----+--------------------------------+----------------------+-----------\n");
}

// ------------------------------------------------------------
// IMPLEMENTAÇÕES DOS ALGORITMOS
// ------------------------------------------------------------

// 1) Bubble sort por nome (ordenacao lexicográfica ascendente)
// Conta comparações (cada strcmp é considerada uma comparação)
void bubbleSortNome(Componente arr[], int n, long *comparacoes)
{
    *comparacoes = 0;
    int trocou;
    for (int i = 0; i < n - 1; ++i)
    {
        trocou = 0;
        for (int j = 0; j < n - 1 - i; ++j)
        {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j + 1].nome) > 0)
            {
                Componente tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou)
            break;
    }
}

// 2) Insertion sort por tipo (string)
// Ordena por tipo; se tipos iguais, mantém ordem relativa (estável)
void insertionSortTipo(Componente arr[], int n, long *comparacoes)
{
    *comparacoes = 0;
    for (int i = 1; i < n; ++i)
    {
        Componente chave = arr[i];
        int j = i - 1;
        // Comparar chave.tipo com arr[j].tipo
        while (j >= 0)
        {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0)
            {
                arr[j + 1] = arr[j];
                j--;
            }
            else
            {
                break;
            }
        }
        arr[j + 1] = chave;
    }
}

// 3) Selection sort por prioridade (int) - menor prioridade primeiro (ascendente)
// Conta comparações (cada comparação de prioridade é contada)
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes)
{
    *comparacoes = 0;
    for (int i = 0; i < n - 1; ++i)
    {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j)
        {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[minIdx].prioridade)
            {
                minIdx = j;
            }
        }
        if (minIdx != i)
        {
            Componente tmp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = tmp;
        }
    }
}

// Busca binária por nome (aplicável somente depois de ordenar por nome).
// Retorna índice ou -1 se não encontrado. Conta comparações (cada strcmp com meio é 1).
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes)
{
    int low = 0, high = n - 1;
    *comparacoes = 0;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        (*comparacoes)++;
        int cmp = strcmp(arr[mid].nome, chave);
        if (cmp == 0)
            return mid;
        else if (cmp < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// Exibe todos os componentes formatados
void mostrarComponentes(Componente arr[], int n)
{
    if (n == 0)
    {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    mostrarCabecalho();
    for (int i = 0; i < n; ++i)
    {
        printf("%-3d | %-30s | %-20s | %-9d\n", i, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
}

// Mede o tempo de execução de um algoritmo de ordenação que segue
// a assinatura: void algoritmo(Componente[], int, long*)
double medirTempo(void (*algoritmo)(Componente[], int, long *), Componente arr[], int n, long *comparacoes)
{
    // Faz cópia do vetor para não alterar o original quando só queremos medir
    Componente copia[MAX_COMPONENTES];
    copiarComponentes(copia, arr, n);

    clock_t inicio = clock();
    algoritmo(copia, n, comparacoes);
    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Caso o usuário queira ver os resultados da ordenação feita na cópia, poderia retornar ou imprimir aqui.
    return tempo;
}

// ------------------------------------------------------------
// PROGRAMA PRINCIPAL E MENU
// ------------------------------------------------------------
int main()
{
    Componente componentes[MAX_COMPONENTES];
    int total = 0;
    int ultimoOrdenadoPorNome = 0; // flag: 1 se o vetor atual estiver ordenado por nome
    char buffer[128];

    printf("=== SISTEMA DE MONTAGEM: TORRE DE RESGATE (NIVEL FINAL) ===\n");

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Cadastrar componente (max %d)\n", MAX_COMPONENTES);
        printf("2. Listar componentes\n");
        printf("3. Ordenar por NOME (Bubble Sort) e mostrar desempenho\n");
        printf("4. Ordenar por TIPO (Insertion Sort) e mostrar desempenho\n");
        printf("5. Ordenar por PRIORIDADE (Selection Sort) e mostrar desempenho\n");
        printf("6. Buscar componente-chave por NOME (Busca Binária) [requer ordenacao por NOME]\n");
        printf("7. Montagem final (exibir componentes e confirmar componente-chave)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        if (!fgets(buffer, sizeof(buffer), stdin))
            break;
        int opc = atoi(buffer);

        if (opc == 0)
        {
            printf("Encerrando...\n");
            break;
        }

        if (opc == 1)
        {
            if (total >= MAX_COMPONENTES)
            {
                printf("Limite de componentes atingido (%d).\n", MAX_COMPONENTES);
                continue;
            }
            Componente c;
            printf("Nome do componente (ex: chip central): ");
            if (!fgets(c.nome, sizeof(c.nome), stdin))
            {
                printf("Erro de leitura.\n");
                continue;
            }
            trimNewline(c.nome);

            printf("Tipo do componente (ex: controle, suporte, propulsao): ");
            if (!fgets(c.tipo, sizeof(c.tipo), stdin))
            {
                printf("Erro de leitura.\n");
                continue;
            }
            trimNewline(c.tipo);

            printf("Prioridade (1 a 10): ");
            if (!fgets(buffer, sizeof(buffer), stdin))
            {
                printf("Erro de leitura.\n");
                continue;
            }
            c.prioridade = atoi(buffer);
            if (c.prioridade < 1)
                c.prioridade = 1;
            if (c.prioridade > 10)
                c.prioridade = 10;

            componentes[total++] = c;
            ultimoOrdenadoPorNome = 0;
            printf("Componente cadastrado com sucesso.\n");
        }
        else if (opc == 2)
        {
            mostrarComponentes(componentes, total);
        }
        else if (opc == 3)
        {
            if (total == 0)
            {
                printf("Nenhum componente para ordenar.\n");
                continue;
            }
            long comparacoes = 0;
            // Medir diretamente na cópia usando medirTempo e depois aplicar ordenacao real ao array atual
            double tempo = medirTempo(bubbleSortNome, componentes, total, &comparacoes);
            // Agora aplicar de fato ao vetor principal (para permitir busca binária posteriormente)
            bubbleSortNome(componentes, total, &comparacoes);
            printf("\n--- Resultado: Bubble Sort por NOME ---\n");
            printf("Comparacoes: %ld\n", comparacoes);
            printf("Tempo de execucao (segundos): %.6f\n", tempo);
            mostrarComponentes(componentes, total);
            ultimoOrdenadoPorNome = 1;
        }
        else if (opc == 4)
        {
            if (total == 0)
            {
                printf("Nenhum componente para ordenar.\n");
                continue;
            }
            long comparacoes = 0;
            double tempo = medirTempo(insertionSortTipo, componentes, total, &comparacoes);
            // aplicar de fato
            insertionSortTipo(componentes, total, &comparacoes);
            printf("\n--- Resultado: Insertion Sort por TIPO ---\n");
            printf("Comparacoes: %ld\n", comparacoes);
            printf("Tempo de execucao (segundos): %.6f\n", tempo);
            mostrarComponentes(componentes, total);
            ultimoOrdenadoPorNome = 0;
        }
        else if (opc == 5)
        {
            if (total == 0)
            {
                printf("Nenhum componente para ordenar.\n");
                continue;
            }
            long comparacoes = 0;
            double tempo = medirTempo(selectionSortPrioridade, componentes, total, &comparacoes);
            // aplicar de fato
            selectionSortPrioridade(componentes, total, &comparacoes);
            printf("\n--- Resultado: Selection Sort por PRIORIDADE ---\n");
            printf("Comparacoes: %ld\n", comparacoes);
            printf("Tempo de execucao (segundos): %.6f\n", tempo);
            mostrarComponentes(componentes, total);
            ultimoOrdenadoPorNome = 0;
        }
        else if (opc == 6)
        {
            if (total == 0)
            {
                printf("Nenhum componente cadastrado.\n");
                continue;
            }
            if (!ultimoOrdenadoPorNome)
            {
                printf("A busca binaria por NOME exige que os componentes estejam ordenados por NOME.\n");
                printf("Execute a opcao 3 (Ordenar por NOME) primeiro.\n");
                continue;
            }
            char chave[MAX_NOME];
            printf("Digite o NOME exato do componente-chave para buscar: ");
            if (!fgets(chave, sizeof(chave), stdin))
            {
                printf("Erro de leitura.\n");
                continue;
            }
            trimNewline(chave);
            long comparacoes = 0;
            clock_t inicio = clock();
            int idx = buscaBinariaPorNome(componentes, total, chave, &comparacoes);
            clock_t fim = clock();
            double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            printf("\n--- Resultado: Busca Binaria por NOME ---\n");
            printf("Comparacoes realizadas: %ld\n", comparacoes);
            printf("Tempo de execucao (segundos): %.6f\n", tempo);
            if (idx >= 0)
            {
                printf("Componente encontrado no indice %d:\n", idx);
                mostrarCabecalho();
                printf("%-3d | %-30s | %-20s | %-9d\n", idx, componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
            }
            else
            {
                printf("Componente '%s' nao encontrado.\n", chave);
            }
        }
        else if (opc == 7)
        {
            // Montagem final: exibe componentes (na ordenacao atual) e pede nome do componente-chave para confirmar presença
            printf("\n--- MONTAGEM FINAL ---\n");
            mostrarComponentes(componentes, total);
            if (total == 0)
                continue;
            char chave[MAX_NOME];
            printf("\nDigite o NOME exato do componente-chave (para confirmar presenca): ");
            if (!fgets(chave, sizeof(chave), stdin))
            {
                printf("Erro de leitura.\n");
                continue;
            }
            trimNewline(chave);
            int encontrado = 0;
            for (int i = 0; i < total; ++i)
            {
                if (strcmp(componentes[i].nome, chave) == 0)
                {
                    encontrado = 1;
                    printf("\nComponente-chave encontrado! (indice %d)\n", i);
                    printf("Preparando ativacao da torre...\n");
                    break;
                }
            }
            if (!encontrado)
            {
                printf("\nComponente-chave '%s' nao encontrado. Verifique cadastro/ordenacao.\n", chave);
            }
        }
        else
        {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}
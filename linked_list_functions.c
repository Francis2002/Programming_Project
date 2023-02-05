/******************************************************************************
 * Last modified: 27-03-2022
 *
 * NAME
 *   linked_list_functions.c
 *
 * DESCRIPTION
 *   Program with functions that deal with the linked list
 *
 * COMMENTS
 *   Todo: 
 ******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"linked_list_functions.h"

/*linked_list* allocate_node(linked_list* new_node, init_info_struct* info)
{
    int max_size;
    if (info->ncolunas > info->nlinhas)
    {
        max_size = info->ncolunas;
    }
    else
    {
        max_size = info->nlinhas;
    }
    new_node = (linked_list*)malloc(sizeof(linked_list));
    new_node->next = NULL;

    return new_node;
}*/

/******************************************************************************
 *  NAME
 *      insert_move: funcao que insere um node ja allocado na lista passada como parametro
 *                         
 *  Arguments
 *      listhead: ponteiro para o inicio da lista
 *      new_node: node ja allocado a inserir
 * 
 *  Return 
 *      listhead: ponteiro para o inicio da lista
 *      
 ******************************************************************************/
linked_list* insert_move(linked_list* listhead, linked_list* new_node)
{
    if (already_in_list(listhead, new_node))    //se ja esta na lista nao colocamos para nao haver jogadas repetidas
    {
        free(new_node->word);
        free(new_node);
        return listhead;
    }
    if(listhead == NULL) //se nao houver lista novo node e a cabeca da lista
    {
        listhead = new_node;
    }
    else
    {
        new_node->next = listhead;
        listhead = new_node;
    }
    return listhead;
}


int already_in_list(linked_list* listhead, linked_list* new_node)
{
    linked_list* aux;
    for (aux = listhead; aux != NULL; aux = aux->next)
    {
        if ((strcmp(aux->word, new_node->word) == 0) && (aux->linha == new_node->linha) && (aux->coluna == new_node->coluna) && (aux->direction == new_node->direction))
        {
            return 1;
        }
    }
    return 0;
}

/******************************************************************************
 *  NAME
 *      print_list: funcao que escreve a lista para o ficheiro passado com o parametro
 *                         
 *  Arguments
 *      listhead: ponteiro para o inicio da lista
 *      fp: ficheiro onde escrever
 *      move_number: usado para imprimr a jogada em que estamos
 * 
 *      
 ******************************************************************************/
void print_list(linked_list* listhead, FILE* fp, int move_number)
{
    linked_list* aux;

    fprintf(fp, "Jogada %d\n", move_number);

    for (aux = listhead; aux != NULL; aux = aux->next)
    {
        fprintf(fp,"%c%d%c %s %d pontos\n", aux->coluna + 'A', aux->linha + 1, aux->direction, aux->word, aux->points);
    }
}

/******************************************************************************
 *  NAME
 *      print_matrix_stdout: funcao que escreve o tabuleiro para o stdout
 * 
 *  Arguments
 *      matrix: tabuleiro a imprimir
 *      linhas, colunas: tamanho do tabuleiro a imprimir
 *      before: letras antes da jogada
 *      after: letras depois da jogada
 *      
 ******************************************************************************/
void print_matrix_stdout(char** matrix, int linhas, int colunas, char* before, char* after, linked_list* node)
{
    char numero = '1';
    char letra = 'A';

    for (int i = 0; i < linhas+1; ++i)
    {
        if (i!=linhas)
        {
            if (i < 9)  //primeiras 9 linhas so tem 1 digito
            {
                printf(" ");
                printf("%c ", numero);
            }
            else
            {
                printf("1"); //linhas seguintes comecam por 1
                printf("%c ", numero-10);
            }
            numero++;
            for (int j = 0; j < colunas; ++j)
            {
                if (j == colunas-1)
                {
                    printf("%c",matrix[i][j]);
                }
                else
                {
                    printf("%c ",matrix[i][j]);
                }
            }
            printf("\n");
        }
        else            //////////colocar letras por baixo
        {
            printf("   ");
            for (int j = 0; j < colunas; ++j)
            {
                printf("%c ",letra);
                letra++;
            }
        }
    }
    printf("\n");
    if (before != NULL)
    {
        printf("Letras na mão: %s\n", before);
    }
    printf("%c%d%c %s %d pontos\n", node->coluna + 'A', node->linha + 1, node->direction, node->word, node->points);
    if (after != NULL)
    {
        printf("Letras na mão: %s\n", after);
    }
}


/******************************************************************************
 *  NAME
 *      sort_linked_list: funcao que ordena a linked list segundo os criterios no enunciado
 *                         
 *  Arguments
 *      listhead: ponteiro para o inicio da lista
 * 
 *  Return 
 *      listhead: ponteiro para o inicio da lista ja ordenada
 *      
 ******************************************************************************/
linked_list* sort_linked_list(linked_list* listhead)
{
    int trocou = 1;
    linked_list* left = NULL;
    linked_list* right = NULL;
    linked_list* head = NULL;
    linked_list dummy;

    head = &dummy;
    head->next = listhead;

    if ((listhead != NULL) && ((listhead->next != NULL)))
    {
        while(trocou)
        {
            trocou = 0;
            left = head;
            right = head->next;
            while(right->next != NULL)
            {
                if (right->points < right->next->points)
                {
                    left->next = troca(right, right->next);
                    trocou = 1;
                }
                else if (right->points == right->next->points) //em caso de igualdade passa se para o criterio seguinte
                {
                    if (right->linha > right->next->linha)
                    {
                        left->next = troca(right, right->next);
                        trocou = 1;
                    }
                    else if (right->linha == right->next->linha) //em caso de igualdade passa se para o criterio seguinte
                    {
                        if (right->coluna > right->next->coluna)
                        {
                            left->next = troca(right, right->next);
                            trocou = 1;
                        }
                        else if (right->coluna == right->next->coluna) //em caso de igualdade passa se para o criterio seguinte
                        {
                            if (right->direction < right->next->direction)
                            {
                                left->next = troca(right, right->next);
                                trocou = 1;
                            }
                            else if (right->direction == right->next->direction) //em caso de igualdade passa se para o criterio seguinte
                            {
                                if (alfa(right->word, right->next->word) < 0)
                                {
                                    left->next = troca(right, right->next);
                                    trocou = 1;
                                }
                            }
                        }
                    }
                }
                left = right;
                if (right->next != NULL)
                {
                    right = right->next;
                }
            }
        }   
    }
    right = head->next;
    return right;
}

linked_list* troca(linked_list* left, linked_list* right)
{
    left->next = right->next;
    right->next = left;
    return right;
}

/******************************************************************************
 *  NAME
 *      alfa: funcao que recebe duas strings e retorna uma valor com informacao sobre a ordem alfabetica das strings
 *                         
 *  Arguments
 *      str1
 *      str2
 * 
 *  Return 
 *      return > 0 significa str1 vem primeiro
        return < 0 significa str2 vem primeiro
 *      
 ******************************************************************************/
int alfa(char* str1, char* str2)
{
    int max = strlen(str1);

    if (strlen(str1) > strlen(str2))
    {
        max = strlen(str2);
    }
    for (int i = 0; i < max; ++i)
    {
        if (str1[i] != str2[i])
        {
            return (str2[i] - str1[i]);     // return > 0 significa str1 vem primeiro
                                            // return < 0 significa str2 vem primeiro
        }
    }
    if (max == strlen(str1))    //todas letras iguais implica que menor palavra vem primeiro alfabeticamente
    {
        return 1;   // return > 0 significa str1 vem primeiro
    }
    else
    {
        return -1;  // return < 0 significa str2 vem primeiro
    }
}

void free_list(linked_list* listhead)
{
    linked_list* temp;

    temp = listhead;
    while(listhead != NULL)
    {
        temp = listhead;    
        listhead = listhead->next;
        free(temp->word);
        free(temp);
        temp = NULL;
    }
}


/******************************************************************************
 *  NAME
 *      remove_bad_moves: funcao que remove as jogadas com menos de metade dos pontos da jogada com mais pontos
 *                         
 *  Arguments
 *      listhead
 * 
 *  Return 
 *      listhead: ponteiro para lista ja encurtada
 *      
 ******************************************************************************/
linked_list* remove_bad_moves(linked_list* listhead)
{

    linked_list* aux;

    int half_points = (listhead->points)/2;
    if (((listhead->points)%2 == 1))
    {
        half_points++;
    }

    for (aux = listhead; aux != NULL; aux = aux->next)
    {
        if(aux->next != NULL)
        {
            if ((aux->next->points) < half_points) //procurar primeiro node com menos de metade dos pontos
            {
                free_list(aux->next); //como a lista ja esta ordenada queremos dar free a todos os nodes a partir dai
                aux->next = NULL;
                break;
            }
        }
    }
    return listhead;
}


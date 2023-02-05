/******************************************************************************
 * Last modified: 27-03-2022
 *
 * NAME
 *   linked_list_functions.h
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

#include"basic_functions.h"

typedef struct linked_list_
{
    char* word;
    char direction;
    char coluna;
    int linha;
    int points;
    struct linked_list_* next;
}linked_list;

linked_list* allocate_node(linked_list* new_node, init_info_struct* info);
linked_list* insert_move(linked_list* listhead, linked_list* new_node);
linked_list* sort_linked_list(linked_list* listhead);
int alfa(char* str1, char* str2);
linked_list* troca(linked_list* left, linked_list* right);
linked_list* remove_bad_moves(linked_list* listhead);
int already_in_list(linked_list* listhead, linked_list* new_node);
void print_list(linked_list* listhead, FILE* fp, int move_number);
void free_list(linked_list* listhead);
void print_matrix_stdout(char** matrix, int linhas, int colunas, char* before, char* after, linked_list* node);


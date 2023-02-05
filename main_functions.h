/******************************************************************************
 * Last modified: 27-03-2022
 *
 * NAME
 *   main_functions.h
 *
 * DESCRIPTION
 *   lista de funcoes principais para a implementacao do jogo
 *
 * COMMENTS
 *
 ******************************************************************************/

#ifndef __main_functions__
#define __main_functions__
 
#include "linked_list_functions.h"

char** faz_tabuleiro(init_info_struct* info, char** tabuleiro);
char** allocate_matrix(int nlinhas,int ncolunas, char** tabuleiro);
init_info_struct* process_init_input(init_info_struct* info, int argc, char *argv[]);
/*char** game_mode_1(char** tabuleiro, int nlinhas, int ncolunas);*/
char** game_mode_2(char** tabuleiro,init_info_struct* info);
char** game_mode_3(char** tabuleiro,init_info_struct* info);
char** game_mode_4(char** tabuleiro,init_info_struct* info);
void free_info(init_info_struct* info);

#endif    /* __main_functions__ */

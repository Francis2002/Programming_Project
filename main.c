#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "main_functions.h"

int main(int argc, char *argv[])
{
    char** tabuleiro = NULL;
    init_info_struct* info = (init_info_struct*)malloc(sizeof(init_info_struct));
    info->dict_filename = NULL;
    info->letras_filename = NULL;
    info->init_board_filename = NULL;
    info->out_board_filename = NULL;
    info->register_filename = NULL;

    FILE* fp_out_tab = NULL;

    info = process_init_input(info, argc, argv);

    tabuleiro = allocate_matrix(info->nlinhas, info->ncolunas, tabuleiro);

    tabuleiro = faz_tabuleiro(info, tabuleiro);

    switch(info->game_mode)
    {
        case 1:
            //tabuleiro = game_mode_1(tabuleiro, info->nlinhas, info->ncolunas);
            break;
        case 2:
            tabuleiro = game_mode_2(tabuleiro, info);
            break;
        case 3:
            tabuleiro = game_mode_3(tabuleiro, info);
            break;
        case 4:
            tabuleiro = game_mode_4(tabuleiro, info);
            break;
        default:
            exit_("modo de jogo nao valido");
            break;
    }
    if (info->out_board_filename == NULL)
    {
        fp_out_tab = stdout;
    }
    else if((fp_out_tab = fopen(info->out_board_filename, "w")) == NULL)
    {
        fp_out_tab = stdout;
    }
    print_matrix(tabuleiro, info->nlinhas, info->ncolunas, fp_out_tab);
    if (fp_out_tab != NULL)
    {
        fclose(fp_out_tab);
    }
    free_matrix(tabuleiro, info->nlinhas);
    free_info(info);
    return 0;
}


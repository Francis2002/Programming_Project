/******************************************************************************
 * Last modified: 27-03-2022
 *
 * NAME
 *   main_functions.c
 *
 * DESCRIPTION
 *   Programa com o codigo das funcoes principais para a implementacao do jogo
 *
 * COMMENTS
 *   Todo: 
 * \vertical segfault
 * por alteracoes do modo 3 para o modo 2
 ******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "main_functions.h"

#define MAX_SIZE 100

/******************************************************************************
 *  NAME
 *      process_init_input: funcao que recebe os argumentos da linha de comando
 * 
 *  Arguments
 *      info: ponteiro para struct onde a informacao vai ser guardada
 *      argc
 *      argv
 * 
 *  Return
 *      info: ponteiro para struct ja com informacao
 *         
 ******************************************************************************/
init_info_struct* process_init_input(init_info_struct* info, int argc, char *argv[])
{
    char* tab_size = NULL;
    char* dict_filename = NULL;
    char* letras_filename = NULL;
    char* max_letters = NULL;
    char* max_plays = NULL;
    char* init_board_filename = NULL;
    char* game_mode = NULL;
    char* out_board_filename = NULL;
    char* register_filename = NULL;
    
    //  percorrer argv ate encontrar uma flag
    //  quando encontra verificar se existe argv seguinte para prevenir seg fault
    //  guarda informacao do argv para variaveis temporarias cuja validade e testada a seguir

    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-t") == 0)
        {
            prevent_seg_fault(argc, i+1);
            tab_size = argv[i+1];
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            prevent_seg_fault(argc, i+1);
            dict_filename = argv[i+1];
        }
        else if (strcmp(argv[i], "-l") == 0)
        {
            prevent_seg_fault(argc, i+1);
            letras_filename = argv[i+1];
        }
        else if (strcmp(argv[i], "-m") == 0)
        {
            prevent_seg_fault(argc, i+1);
            max_letters = argv[i+1];
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            prevent_seg_fault(argc, i+1);
            max_plays = argv[i+1];
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            prevent_seg_fault(argc, i+1);
            init_board_filename = argv[i+1];
        }
        else if (strcmp(argv[i], "-j") == 0)
        {
            prevent_seg_fault(argc, i+1);
            game_mode = argv[i+1];
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            prevent_seg_fault(argc, i+1);
            out_board_filename = argv[i+1];
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            prevent_seg_fault(argc, i+1);
            register_filename = argv[i+1];
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            //help_();
        }
    }

    //chama funcao que valida argumentos lidos e os insere na struct info
    info = check_correct_input(tab_size, 
        dict_filename, 
        letras_filename, 
        max_letters, 
        max_plays, 
        init_board_filename, 
        game_mode, 
        out_board_filename, 
        register_filename, 
        info);

    return info;

}
/******************************************************************************
 *  NAME
 *      allocate_matrix: funcao que aloca o double pointer tabuleiro
 * 
 *  Arguments
 *      nlinhas: numero de linhas da matriz a alocar
 *      ncolunas: numero de colunas da matriz a alocar
 *      tabuleiro: double pointer para a matriz
 * 
 *  Return
 *      tabuleiro: matriz alocada
 *         
 ******************************************************************************/
char** allocate_matrix(int nlinhas,int ncolunas, char** tabuleiro)
{
    tabuleiro = (char**)malloc((nlinhas)*sizeof(char*));
    for (int i = 0; i < ncolunas; ++i)
    {
        tabuleiro[i] = (char*)malloc((ncolunas)*sizeof(char));
    }
    return tabuleiro;
}

/******************************************************************************
 *  NAME
 *      faz_tabuleiro: funcao que inicializa a matriz
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao
 *      tab: double pointer para a matriz ja alocada
 * 
 *  Return
 *      tab: tabuleiro inicializado
 *         
 ******************************************************************************/
char** faz_tabuleiro(init_info_struct* info, char** tab)
{
    if (info->init_board_filename != NULL)
    {
        return process_input_tab(info, tab);    //tabuleiro inicial dado como ficheiro
    }

    int colunas = info->ncolunas;
    int linhas = info->nlinhas;
    int aux_col = (colunas+1)/2;
    int aux_lin = (linhas+1)/2;
    
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            tab[i][j]='.'; /* Inicializar todas as casas com . */


            /* Colocar chars de pontos bonus */
            if ((i==j) || ((i == (colunas -1 - j))))    //'2' nas diagonais principais
            {
              tab[i][j]='2'; 
            }
            if ((i==0 || i==(linhas-1)) && (j==0 || j==(colunas-1)))    //'$' nos 4 cantos
            {
              tab[i][j]='$';
            }
            if (((j==aux_col-1) && (i==0 || i== (linhas-1))) || ((i==aux_lin-1) && (j==0 || j==(colunas-1))))
            {
              tab[i][j]='3';    //'3' nos pontos medios das "paredes"
            }
            if (((j==(aux_col) || (j==aux_col-2)) && (i==1 || i==(linhas-2))) || (((i==aux_lin) || i==(aux_lin-2)) && (j==1 || j==colunas-2)))
            {
              tab[i][j]='#'; // nas casas diagonalmente aos '3'
            }          
        }
    }

    return tab;
}

/******************************************************************************
 *  NAME
 *      game_mode_2: funcao que implementa o modo de jogo 2
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao
 *      tabuleiro: double pointer para a matriz ja alocada
 * 
 *  Return
 *      tabuleiro: tabuleiro preenchido com palavras
 * 
 *  Variaveis declaradas
 *      word: double pointer para lista das palavras no dicionario
 *      move_counter: contador da jogada corrente; usado para motivos de print do registo
 *      starting_row: linha inicial da palavra que esta a tentar ser posta; usado para motivos do deslocamento das palvras no algoritmo do computador
 *      starting_col: coluna inicial da palavra que esta a tentar ser posta; usado para motivos do deslocamento das palvras no algoritmo do computador
 *      dict_size: numero de palavras passadas do ficheiro do dicionario para a lista
 *      
 ******************************************************************************/
char** game_mode_2(char** tabuleiro, init_info_struct* info)
{
    char** word;
    int move_counter = 0;
    int starting_row;
    int starting_col;
    int dict_size = 0;
    int total_points = 0;
    int highest_points = 0;
    linked_list* listhead = NULL;
    FILE* fp_reg = NULL;
    linked_list* new_node;

    linked_list* highest_node = NULL;

    ////////////    inicializacao do dicionario
    FILE* fp_dict = read_dict(info->dict_filename);

    if (strcmp(info->dict_filename, "pt_PT.dic") == 0)
    {
        word = sort_pt_dict(fp_dict, &dict_size, info); //o dicionario em portugues tem uma estrutura especifica
    }
    else
    {
        word = sort_english_dict(fp_dict, &dict_size, info);
    }
    ////////////////

    if (info->register_filename != NULL)
    {
        if ((fp_reg = fopen(info->register_filename,"w")) == NULL)
        {
            exit_("erro ao abrir ficheiro de registo");
        }
    }
    char* current_word = NULL;



    while(move_counter < info->max_plays)
    {
        ///se for a primeira jogada mas o tabuleiro nao estiver vazio nao queremos fazer a primeira jogada no meio
        if((move_counter == 0) && (is_letter(tabuleiro,info->nlinhas/2,info->ncolunas/2,'\0') == 1))
        {
            for (int dict_counter = 0; dict_counter < dict_size; ++dict_counter)
            {
                current_word = word[dict_counter];  ///escolher palavra da lista de palavras
                starting_row = info->nlinhas/2;   ///primeira jogada so se pode por na linha do meio

                for (int i = 0; i <= (info->ncolunas/2 + 1); ++i)
                {
                    starting_col = i;
                    if (starting_col + strlen(current_word) >= info->ncolunas/2 + 1) //palavra tem de conter casa do meio
                    {
                        if(try_word(tabuleiro, current_word, starting_row, starting_col, 'H', info, word, dict_size, NULL) != 0) 
                        {
                            
                            /////////// alloca e inicializa os valores do node
                            new_node = (linked_list*)malloc(sizeof(linked_list));
                            new_node->next = NULL;
            
                            new_node->word = (char*)malloc(sizeof(char)*(info->nlinhas)+1);
                            strcpy(new_node->word, current_word);
            
                            new_node->linha = starting_row;
                            new_node->coluna = starting_col;
                            new_node->direction = 'H';
                            new_node->points = calculate_points(tabuleiro, current_word, info->nlinhas/2, starting_col, 'H');
                            //////////////////////////////////////////


                            if (new_node->points > highest_points)
                            {
                                highest_node = new_node;
                                highest_points = new_node->points;
                            }

                            listhead = insert_move(listhead, new_node);
                        }
                    }
                }                 
            }
        }
        else    ///entra aqui se nao for a primeira jogada
        {
            for (int i = 0; i < info->nlinhas; ++i) ///procurar letras para por palavras na VERTICAL
            {
                for (int j = 0; j < info->ncolunas; ++j)
                {
                    if(is_letter(tabuleiro, i, j, '\0') == 0)   //verifica se a posição a testar tem uma letra
                    {
                        for (int dict_counter = 0; dict_counter < dict_size; ++dict_counter)
                        {
                            current_word = word[dict_counter]; //ir buscar palavra a lista de palavras
                            starting_col = j; //na vertical a coluna e constante

                            for (int k = 0; k <= i; ++k) //iteracoes do deslocamento da palavra escolhida
                            {
                                starting_row = k;

                                if (at_least_one_letter(tabuleiro, current_word, starting_row, starting_col, 'V'))
                                {
                                    if(try_word(tabuleiro, current_word, starting_row, starting_col, 'V', info, word, dict_size, NULL) != 0) 
                                    {
                                        ///se entrou aqui entao e possivel por a palavra escolhida na posicao escolhida na vertical

                                        /////////// alloca e inicializa os valores do node
                                        new_node = (linked_list*)malloc(sizeof(linked_list));
                                        new_node->next = NULL;

                                        new_node->word = (char*)malloc(sizeof(char)*(info->nlinhas)+1);
                                        strcpy(new_node->word, current_word);

                                        new_node->linha = starting_row;
                                        new_node->coluna = starting_col;
                                        new_node->direction = 'V';
                                        new_node->points = calculate_points(tabuleiro, current_word, starting_row, starting_col, 'V');
                                        /////////////////////////////////


                                        if (new_node->points > highest_points)
                                        {
                                            highest_node = new_node;
                                            highest_points = new_node->points;
                                        }

                                        listhead = insert_move(listhead, new_node);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < info->nlinhas; ++i) ///procurar letras para por palavras na HORIZONTAL
            {
                for (int j = 0; j < info->ncolunas; ++j)
                {
                    if(is_letter(tabuleiro, i, j, '\0') == 0)   //verifica se a posição a testar tem uma letra
                    {
                        for (int dict_counter = 0; dict_counter < dict_size; ++dict_counter)
                        {
                            current_word = word[dict_counter];                       
                            starting_row = i; ///na horizontal linha e constante
                            for (int k = 0; k <= j; ++k) //iteracoes do deslocamento da palavra escolhida
                            {
                                starting_col = k;
                                if (at_least_one_letter(tabuleiro, current_word, starting_row, starting_col, 'H'))
                                {
                                    if(try_word(tabuleiro, current_word, starting_row, starting_col, 'H', info, word, dict_size, NULL) != 0) 
                                    {
                                        ///se entrou aqui entao e possivel por a palavra escolhida na posicao escolhida na horizontal                                    

                                        /////////// alloca e inicializa os valores do node
                                        new_node = (linked_list*)malloc(sizeof(linked_list));
                                        new_node->next = NULL;

                                        new_node->word = (char*)malloc(sizeof(char)*(info->nlinhas)+1);
                                        strcpy(new_node->word, current_word);

                                        new_node->linha = starting_row;
                                        new_node->coluna = starting_col;
                                        new_node->direction = 'H';
                                        new_node->points = calculate_points(tabuleiro, current_word, starting_row, starting_col, 'H');
                                        ///////////////////////////////////////////


                                        if (new_node->points > highest_points)
                                        {
                                            highest_node = new_node;
                                            highest_points = new_node->points;
                                        }

                                        listhead = insert_move(listhead, new_node);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /// se nao houver mais jogadas possiveis antes de chegar ao max_plays saimos do loop
        if (listhead == NULL)
        {
            break;
        }
        move_counter++;

        listhead = sort_linked_list(listhead);

        if(highest_node->direction == 'V')
        {
            tabuleiro = play_vertical(tabuleiro, highest_node->word, highest_node->linha, highest_node->coluna);
        }
        if(highest_node->direction == 'H')
        {
            tabuleiro = play_horizontal(tabuleiro, highest_node->word, highest_node->linha, highest_node->coluna);
        }

        print_matrix_stdout(tabuleiro, info->nlinhas, info->ncolunas, NULL, NULL, highest_node);

        listhead = remove_bad_moves(listhead);

        total_points += highest_node->points;

        highest_points = 0;


        if(listhead != NULL)
        {
            if (info->register_filename != NULL)
            {
                print_list(listhead, fp_reg, move_counter);
            }
        
            free_list(listhead);
            listhead = NULL;
        }
        //print_matrix(tabuleiro, info->nlinhas, info->ncolunas,  );
    }
    printf("Fim do jogo. Total de pontos obtidos: %d\n", total_points);


    for (int i = 0; i < dict_size; ++i)
    {
        free(word[i]);
    }
    free(word);

    if (fp_reg != NULL)
    {
        fclose(fp_reg);
    }
    fclose(fp_dict);
    return tabuleiro;
}

/******************************************************************************
 *  NAME
 *      game_mode_4: funcao que implementa o modo de jogo 3
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao
 *      tabuleiro: double pointer para a matriz ja alocada
 * 
 *  Return
 *      tabuleiro: tabuleiro preenchido com palavras
 * 
 *  Variaveis declaradas
 *      word: double pointer para lista das palavras no dicionario
 *      move_counter: contador da jogada corrente; usado para motivos de print do registo
 *      starting_row: linha inicial da palavra que esta a tentar ser posta; usado para motivos do deslocamento das palvras no algoritmo do computador
 *      starting_col: coluna inicial da palavra que esta a tentar ser posta; usado para motivos do deslocamento das palvras no algoritmo do computador
 *      dict_size: numero de palavras passadas do ficheiro do dicionario para a lista
 *      index: numero de letras ja utilizadas; usado para saber se ainda ha letras
 *      no_more_letters: flag que indica se ainda ha letras no saco para biscar
 *      
 ******************************************************************************/
char** game_mode_4(char** tabuleiro, init_info_struct* info)
{
    char** word;
    int move_counter = 0;
    int starting_row;
    int starting_col;
    int total_points = 0;
    int dict_size = 0;
    int index = 0;
    int highest_points = 0;
    int no_more_letters = 0;

    linked_list* highest_node = NULL;

    linked_list* listhead = NULL;
    FILE* fp_reg = NULL;
    linked_list* new_node;

    int tem_letra_ao_lado_flag = 0;

    adj_info_struct* perpendicular_struct = NULL;

    if (info->register_filename != NULL)
    {
        if ((fp_reg = fopen(info->register_filename,"w")) == NULL)
        {
            exit_("erro ao abrir ficheiro de registo");
        }
    }

    //////////////inicializar mao e lista de letras
    char aux_char = ' ';

    FILE* fp_letters = read_letters(info->letras_filename);

    fseek(fp_letters, 0, SEEK_END);
    long f_size = ftell(fp_letters);    ///descobrir numero de letras no ficheiro
    fseek(fp_letters, 0, SEEK_SET);

    char* all_letters = (char*)malloc(sizeof(char)*f_size+1); //allocar espaco para o numero de letras calculado
    size_t letter_count = 0;


    while((aux_char = fgetc(fp_letters)) != EOF)
    {
        if ((aux_char >= 'a') && (aux_char <= 'z')) ///so ir buscar chars validos
        {
            all_letters[letter_count++] = aux_char;
        }
    }
    all_letters[letter_count] = '\0';   ///terminador de string

    if (info->max_letters > strlen(all_letters))
    {
        info->max_letters = strlen(all_letters);  ///so allocar na mao o numero de letras necessario
    }
    char* letters_in_hand = (char*)calloc(info->max_letters + 1,sizeof(char));
    char* letters_in_hand_before_update = (char*)calloc(info->max_letters + 1,sizeof(char));

    for (int i = 0; i < info->max_letters; ++i)
    {
        letters_in_hand[i] = ' ';   ///inicializar mao
    }

    for (int i = 0; i < strlen(letters_in_hand); ++i)
    {
        if(letters_in_hand[i] == ' ')
        {
            if (index < strlen(all_letters))
            {
                letters_in_hand[i] = all_letters[index];
                index++;
            }
            else
            {
                no_more_letters = 1;
                break;
                /*if (fp_reg != NULL)
                {
                    fclose(fp_reg);
                }
                if (fp_letters != NULL)
                {
                    fclose(fp_letters);
                }
                free(all_letters);
                free(letters_in_hand);
                return tabuleiro;*/
            }
        }
    }
    //////////////////////////////////////


    ////////////    inicializacao do dicionario
    FILE* fp_dict = read_dict(info->dict_filename);

    if (strcmp(info->dict_filename, "pt_PT.dic") == 0)
    {
        word = sort_pt_dict(fp_dict, &dict_size, info); //o dicionario em portugues tem uma estrutura especifica
    }
    else
    {
        word = sort_english_dict(fp_dict, &dict_size, info);
    }
    ////////////////

    char* current_word = NULL;


    while(move_counter < info->max_plays)
    {
        ///se for a primeira jogada mas o tabuleiro nao estiver vazio nao queremos fazer a primeira jogada no meio
        if((move_counter == 0) && (is_letter(tabuleiro,info->nlinhas/2,info->ncolunas/2,'\0') == 1))
        {
            for (int dict_counter = 0; dict_counter < dict_size; ++dict_counter)
            {
                current_word = word[dict_counter];  ///escolher palavra da lista de palavras
                starting_row = info->nlinhas/2;   ///primeira jogada so se pode por na linha do meio

                for (int i = 0; i <= (info->ncolunas/2 + 1); ++i)
                {
                    starting_col = i;
                    if (starting_col + strlen(current_word) >= info->ncolunas/2 + 1) //palavra tem de conter casa do meio
                    {
                        if(try_word(tabuleiro, current_word, starting_row, starting_col, 'H', info, word, dict_size, letters_in_hand) != 0) 
                        {
                            
                            /////////// alloca e inicializa os valores do node
                            new_node = (linked_list*)malloc(sizeof(linked_list));
                            new_node->next = NULL;
            
                            new_node->word = (char*)malloc(sizeof(char)*(info->nlinhas)+1);
                            strcpy(new_node->word, current_word);
            
                            new_node->linha = starting_row;
                            new_node->coluna = starting_col;
                            new_node->direction = 'H';
                            new_node->points = calculate_points(tabuleiro, current_word, info->nlinhas/2, starting_col, 'H');
                            //////////////////////////////////////////

                            if (new_node->points > highest_points)
                            {
                                highest_node = new_node;
                                highest_points = new_node->points;
                            }

                            listhead = insert_move(listhead, new_node);
                        }
                    }
                }                 
            }
        }
        else    ///entra aqui se nao for a primeira jogada
        {
            for (int i = 0; i < info->nlinhas; ++i) ///procurar letras para por palavras na VERTICAL
            {
                for (int j = 0; j < info->ncolunas; ++j)
                {
                    tem_letra_ao_lado_flag = 0;
                    if(check_letra_ao_lado(tabuleiro, i, j, 'V', info) == 1)
                    {
                        tem_letra_ao_lado_flag = 1;
                    }
                    if((is_letter(tabuleiro, i, j, '\0') == 0) || tem_letra_ao_lado_flag)   //verifica se a posição a testar tem uma letra
                    {
                        for (int dict_counter = 0; dict_counter < dict_size; ++dict_counter)
                        {
                            current_word = word[dict_counter]; //ir buscar palavra a lista de palavras
                            starting_col = j; //na vertical a coluna e constante
                            for (int k = 0; k <= i; ++k) //iteracoes do deslocamento da palavra escolhida
                            {
                                starting_row = k;

                                if (check_word_includes_coordinates(current_word, starting_row, starting_col, 'V', i, j))
                                {
                                    if(try_word(tabuleiro, current_word, starting_row, starting_col, 'V', info, word, dict_size, letters_in_hand) != 0) 
                                    {
                                        if ((perpendicular_struct = check_adjacente(tabuleiro, current_word, 'V', starting_row, starting_col, info, word, dict_size, perpendicular_struct)) != NULL)
                                        {
                                            ///se entrou aqui entao e possivel por a palavra escolhida na posicao escolhida na vertical

                                            /////////// alloca e inicializa os valores do node
                                            new_node = (linked_list*)malloc(sizeof(linked_list));
                                            new_node->next = NULL;

                                            new_node->word = (char*)malloc(sizeof(char)*(info->nlinhas)+1);
                                            strcpy(new_node->word, current_word);

                                            new_node->linha = starting_row;
                                            new_node->coluna = starting_col;
                                            new_node->direction = 'V';
                                            new_node->points = calculate_points(tabuleiro, current_word, starting_row, starting_col, 'V');
                                            free_return_struct(perpendicular_struct, strlen(current_word));
                                            /////////////////////////////////

                                            if (new_node->points > highest_points)
                                            {
                                                highest_node = new_node;
                                                highest_points = new_node->points;
                                            }

                                            listhead = insert_move(listhead, new_node);

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < info->nlinhas; ++i) ///procurar letras para por palavras na HORIZONTAL
            {
                for (int j = 0; j < info->ncolunas; ++j)
                {
                    tem_letra_ao_lado_flag = 0;
                    if(check_letra_ao_lado(tabuleiro, i, j, 'H', info) == 1)
                    {
                        tem_letra_ao_lado_flag = 1;
                    }
                    if((is_letter(tabuleiro, i, j, '\0')) == 0 || tem_letra_ao_lado_flag)   //verifica se a posição a testar tem uma letra
                    {
                        for (int dict_counter = 0; dict_counter < dict_size; ++dict_counter)
                        {
                            current_word = word[dict_counter];                       
                            starting_row = i; ///na horizontal linha e constante    
                            for (int k = 0; k <= j; ++k) //iteracoes do deslocamento da palavra escolhida
                            {
                                starting_col = k;
                                if (check_word_includes_coordinates(current_word, starting_row, starting_col, 'H', i, j))
                                {
                                    if(try_word(tabuleiro, current_word, starting_row, starting_col, 'H', info, word, dict_size, letters_in_hand) != 0) 
                                    {
                                        if ((perpendicular_struct = check_adjacente(tabuleiro, current_word, 'H', starting_row, starting_col, info, word, dict_size, perpendicular_struct)) != NULL)
                                        {
                                            ///se entrou aqui entao e possivel por a palavra escolhida na posicao escolhida na vertical

                                            /////////// alloca e inicializa os valores do node
                                            new_node = (linked_list*)malloc(sizeof(linked_list));
                                            new_node->next = NULL;

                                            new_node->word = (char*)malloc(sizeof(char)*(info->nlinhas)+1);
                                            strcpy(new_node->word, current_word);

                                            new_node->linha = starting_row;
                                            new_node->coluna = starting_col;
                                            new_node->direction = 'H';
                                            
                                            new_node->points = calculate_points(tabuleiro, current_word, starting_row, starting_col, 'H');
                                            free_return_struct(perpendicular_struct, strlen(current_word));
                                            /////////////////////////////////

                                            if (new_node->points > highest_points)
                                            {
                                                highest_node = new_node;
                                                highest_points = new_node->points;
                                            }

                                            listhead = insert_move(listhead, new_node);

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /// se nao houver mais jogadas possiveis antes de chegar ao max_plays saimos do loop
        if (listhead == NULL)
        {
            break;
        }
        move_counter++;

        listhead = sort_linked_list(listhead);

        //highest_node = listhead;


        strcpy(letters_in_hand_before_update, letters_in_hand);

        letters_in_hand = remove_letters_from_hand(tabuleiro, highest_node->word, highest_node->direction, highest_node->linha, highest_node->coluna, letters_in_hand);

        if(highest_node->direction == 'V')
        {
            tabuleiro = play_vertical(tabuleiro, highest_node->word, highest_node->linha, highest_node->coluna);
        }
        if(highest_node->direction == 'H')
        {
            tabuleiro = play_horizontal(tabuleiro, highest_node->word, highest_node->linha, highest_node->coluna);
        }

        print_matrix_stdout(tabuleiro, info->nlinhas, info->ncolunas, letters_in_hand_before_update, letters_in_hand, highest_node);

        letters_in_hand = update_hand(letters_in_hand,all_letters, &index, &no_more_letters);

        listhead = remove_bad_moves(listhead);

        total_points += highest_node->points;

        highest_points = 0;

        if(listhead != NULL)
        {
            if (info->register_filename != NULL)
            {
                print_list(listhead, fp_reg, move_counter);
            }
        
            free_list(listhead);
            listhead = NULL;
        }
        //print_matrix(tabuleiro, info->nlinhas, info->ncolunas,  );
    }

    printf("Fim do jogo. Total de pontos obtidos: %d\n", total_points);

    free(letters_in_hand_before_update);
    free(letters_in_hand);
    free(all_letters);
    fclose(fp_letters);
    for (int i = 0; i < dict_size; ++i)
    {
        free(word[i]);
    }
    free(word);

    if (fp_reg != NULL)
    {
        fclose(fp_reg);
    }
    fclose(fp_dict);
    return tabuleiro;
}

/******************************************************************************
 *  NAME
 *      game_mode_3: funcao que implementa o modo de jogo 3
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao
 *      tabuleiro: double pointer para a matriz ja alocada
 * 
 *  Return
 *      tabuleiro: tabuleiro preenchido com palavras
 * 
 *  Variaveis declaradas
 *      word: double pointer para lista das palavras no dicionario
 *      move_counter: contador da jogada corrente; usado para motivos de print do registo
 *      starting_row: linha inicial da palavra que esta a tentar ser posta; usado para motivos do deslocamento das palvras no algoritmo do computador
 *      starting_col: coluna inicial da palavra que esta a tentar ser posta; usado para motivos do deslocamento das palvras no algoritmo do computador
 *      dict_size: numero de palavras passadas do ficheiro do dicionario para a lista
 *      index: numero de letras ja utilizadas; usado para saber se ainda ha letras
 *      no_more_letters: flag que indica se ainda ha letras no saco para biscar
 *      
 ******************************************************************************/
char** game_mode_3(char** tabuleiro, init_info_struct* info)
{
    char** word;
    int move_counter = 0;
    int starting_row;
    int starting_col;
    int total_points = 0;
    int dict_size = 0;
    int index = 0;
    int highest_points = 0;
    int no_more_letters = 0;

    linked_list* highest_node = NULL;

    linked_list* listhead = NULL;
    FILE* fp_reg = NULL;
    linked_list* new_node;


    if (info->register_filename != NULL)
    {
        if ((fp_reg = fopen(info->register_filename,"w")) == NULL)
        {
            exit_("erro ao abrir ficheiro de registo");
        }
    }

    //////////////inicializar mao e lista de letras
    char aux_char = ' ';

    FILE* fp_letters = read_letters(info->letras_filename);

    fseek(fp_letters, 0, SEEK_END);
    long f_size = ftell(fp_letters);    ///descobrir numero de letras no ficheiro
    fseek(fp_letters, 0, SEEK_SET);

    char* all_letters = (char*)malloc(sizeof(char)*f_size+1); //allocar espaco para o numero de letras calculado
    size_t letter_count = 0;


    while((aux_char = fgetc(fp_letters)) != EOF)
    {
        if ((aux_char >= 'a') && (aux_char <= 'z')) ///so ir buscar chars validos
        {
            all_letters[letter_count++] = aux_char;
        }
    }
    all_letters[letter_count] = '\0';   ///terminador de string

    if (info->max_letters > strlen(all_letters))
    {
        info->max_letters = strlen(all_letters);  ///so allocar na mao o numero de letras necessario
    }
    char* letters_in_hand = (char*)calloc(info->max_letters + 1,sizeof(char));
    char* letters_in_hand_before_update = (char*)calloc(info->max_letters + 1,sizeof(char));

    for (int i = 0; i < info->max_letters; ++i)
    {
        letters_in_hand[i] = ' ';   ///inicializar mao
    }

    for (int i = 0; i < strlen(letters_in_hand); ++i)
    {
        if(letters_in_hand[i] == ' ')
        {
            if (index < strlen(all_letters))
            {
                letters_in_hand[i] = all_letters[index];
                index++;
            }
            else
            {
                no_more_letters = 1;
                break;
                /*if (fp_reg != NULL)
                {
                    fclose(fp_reg);
                }
                if (fp_letters != NULL)
                {
                    fclose(fp_letters);
                }
                free(all_letters);
                free(letters_in_hand);
                return tabuleiro;*/
            }
        }
    }
    //////////////////////////////////////


    ////////////    inicializacao do dicionario
    FILE* fp_dict = read_dict(info->dict_filename);

    if (strcmp(info->dict_filename, "pt_PT.dic") == 0)
    {
        word = sort_pt_dict(fp_dict, &dict_size, info); //o dicionario em portugues tem uma estrutura especifica
    }
    else
    {
        word = sort_english_dict(fp_dict, &dict_size, info);
    }
    ////////////////

    char* current_word = NULL;


    while(move_counter < info->max_plays)
    {
        ///se for a primeira jogada mas o tabuleiro nao estiver vazio nao queremos fazer a primeira jogada no meio
        if((move_counter == 0) && (is_letter(tabuleiro,info->nlinhas/2,info->ncolunas/2,'\0') == 1))
        {
            for (int dict_counter = 0; dict_counter < dict_size; ++dict_counter)
            {
                current_word = word[dict_counter];  ///escolher palavra da lista de palavras
                starting_row = info->nlinhas/2;   ///primeira jogada so se pode por na linha do meio

                for (int i = 0; i <= (info->ncolunas/2 + 1); ++i)
                {
                    starting_col = i;
                    if (starting_col + strlen(current_word) >= info->ncolunas/2 + 1) //palavra tem de conter casa do meio
                    {
                        if(try_word(tabuleiro, current_word, starting_row, starting_col, 'H', info, word, dict_size, letters_in_hand) != 0) 
                        {
                            
                            /////////// alloca e inicializa os valores do node
                            new_node = (linked_list*)malloc(sizeof(linked_list));
                            new_node->next = NULL;
            
                            new_node->word = (char*)malloc(sizeof(char)*(info->nlinhas)+1);
                            strcpy(new_node->word, current_word);
            
                            new_node->linha = starting_row;
                            new_node->coluna = starting_col;
                            new_node->direction = 'H';
                            new_node->points = calculate_points(tabuleiro, current_word, info->nlinhas/2, starting_col, 'H');
                            //////////////////////////////////////////

                            listhead = insert_move(listhead, new_node);

                            if (new_node->points > highest_points)
                            {
                                highest_node = new_node;
                                highest_points = new_node->points;
                            }
                        }
                    }
                }                 
            }
        }
        else    ///entra aqui se nao for a primeira jogada
        {
            for (int i = 0; i < info->nlinhas; ++i) ///procurar letras para por palavras na VERTICAL
            {
                for (int j = 0; j < info->ncolunas; ++j)
                {
                    if(is_letter(tabuleiro, i, j, '\0') == 0)   //verifica se a posição a testar tem uma letra
                    {
                        for (int dict_counter = 0; dict_counter < dict_size; ++dict_counter)
                        {
                            current_word = word[dict_counter]; //ir buscar palavra a lista de palavras
                            starting_col = j; //na vertical a coluna e constante
                            for (int k = 0; k <= i; ++k) //iteracoes do deslocamento da palavra escolhida
                            {
                                starting_row = k;

                                if (at_least_one_letter(tabuleiro, current_word, starting_row, starting_col, 'V'))
                                {
                                    if(try_word(tabuleiro, current_word, starting_row, starting_col, 'V', info, word, dict_size, letters_in_hand) != 0) 
                                    {
                                        ///se entrou aqui entao e possivel por a palavra escolhida na posicao escolhida na vertical

                                        /////////// alloca e inicializa os valores do node
                                        new_node = (linked_list*)malloc(sizeof(linked_list));
                                        new_node->next = NULL;

                                        new_node->word = (char*)malloc(sizeof(char)*(info->nlinhas)+1);
                                        strcpy(new_node->word, current_word);

                                        new_node->linha = starting_row;
                                        new_node->coluna = starting_col;
                                        new_node->direction = 'V';
                                        new_node->points = calculate_points(tabuleiro, current_word, starting_row, starting_col, 'V');
                                        /////////////////////////////////

                                        if (new_node->points > highest_points)
                                        {
                                            highest_node = new_node;
                                            highest_points = new_node->points;
                                        }

                                        listhead = insert_move(listhead, new_node);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < info->nlinhas; ++i) ///procurar letras para por palavras na HORIZONTAL
            {
                for (int j = 0; j < info->ncolunas; ++j)
                {
                    if(is_letter(tabuleiro, i, j, '\0') == 0)   //verifica se a posição a testar tem uma letra
                    {
                        for (int dict_counter = 0; dict_counter < dict_size; ++dict_counter)
                        {
                            current_word = word[dict_counter];                       
                            starting_row = i; ///na horizontal linha e constante    
                            for (int k = 0; k <= j; ++k) //iteracoes do deslocamento da palavra escolhida
                            {
                                starting_col = k;

                                if (at_least_one_letter(tabuleiro, current_word, starting_row, starting_col, 'H'))
                                {
                                    if(try_word(tabuleiro, current_word, starting_row, starting_col, 'H', info, word, dict_size, letters_in_hand) != 0) 
                                    {
                                        ///se entrou aqui entao e possivel por a palavra escolhida na posicao escolhida na horizontal                                    

                                        /////////// alloca e inicializa os valores do node
                                        new_node = (linked_list*)malloc(sizeof(linked_list));
                                        new_node->next = NULL;

                                        new_node->word = (char*)malloc(sizeof(char)*(info->nlinhas)+1);
                                        strcpy(new_node->word, current_word);

                                        new_node->linha = starting_row;
                                        new_node->coluna = starting_col;
                                        new_node->direction = 'H';
                                        new_node->points = calculate_points(tabuleiro, current_word, starting_row, starting_col, 'H');
                                        ///////////////////////////////////////////


                                        if (new_node->points > highest_points)
                                        {
                                            highest_node = new_node;
                                            highest_points = new_node->points;
                                        }

                                        listhead = insert_move(listhead, new_node);

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /// se nao houver mais jogadas possiveis antes de chegar ao max_plays saimos do loop
        if (listhead == NULL)
        {
            break;
        }
        move_counter++;

        listhead = sort_linked_list(listhead);

        //highest_node = listhead;


        strcpy(letters_in_hand_before_update, letters_in_hand);

        letters_in_hand = remove_letters_from_hand(tabuleiro, highest_node->word, highest_node->direction, highest_node->linha, highest_node->coluna, letters_in_hand);

        if(highest_node->direction == 'V')
        {
            tabuleiro = play_vertical(tabuleiro, highest_node->word, highest_node->linha, highest_node->coluna);
        }
        if(highest_node->direction == 'H')
        {
            tabuleiro = play_horizontal(tabuleiro, highest_node->word, highest_node->linha, highest_node->coluna);
        }

        print_matrix_stdout(tabuleiro, info->nlinhas, info->ncolunas, letters_in_hand_before_update, letters_in_hand, highest_node);

        letters_in_hand = update_hand(letters_in_hand,all_letters, &index, &no_more_letters);

        listhead = remove_bad_moves(listhead);

        total_points += highest_node->points;

        highest_points = 0;

        if(listhead != NULL)
        {
            if (info->register_filename != NULL)
            {
                print_list(listhead, fp_reg, move_counter);
            }
        
            free_list(listhead);
            listhead = NULL;
        }
        //print_matrix(tabuleiro, info->nlinhas, info->ncolunas,  );
    }

    printf("Fim do jogo. Total de pontos obtidos: %d\n", total_points);

    free(letters_in_hand_before_update);
    free(letters_in_hand);
    free(all_letters);
    fclose(fp_letters);
    for (int i = 0; i < dict_size; ++i)
    {
        free(word[i]);
    }
    free(word);

    if (fp_reg != NULL)
    {
        fclose(fp_reg);
    }
    fclose(fp_dict);
    return tabuleiro;
}

/******************************************************************************
 *  NAME
 *      free_info: funcao que faz free da struct com informacao da linha de comando
 * 
 *  Arguments
 *      info: ponteiro para struct com informacao
 *      
 ******************************************************************************/
void free_info(init_info_struct* info)
{
    if (info->dict_filename != NULL)
    {
        free(info->dict_filename);
    }
    if (info->register_filename != NULL)
    {
        free(info->register_filename);
    }
    if (info->init_board_filename != NULL)
    {
        free(info->init_board_filename);
    }
    if (info->out_board_filename != NULL)
    {
        free(info->out_board_filename);
    }
    if (info->letras_filename != NULL)
    {
        free(info->letras_filename);
    }
    free(info);
}

    
    
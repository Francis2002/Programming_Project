/******************************************************************************
 * Last modified: 27-03-2022
 *
 * NAME
 *   functions.c
 *
 * DESCRIPTION
 *   Program with the basic functions called by main_functions.c for palavras
 *
 * COMMENTS
 *   Todo: atencao que mudei o default do dict para words em vez da bodega toda
 ******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "basic_functions.h"

#define INFINITY 2147483640

/******************************************************************************
 *  NAME
 *      check_correct_input: funcao que verifica a validade dos argumentos recebidos da linha de comando
 * 
 *  Arguments
 *      variaveis temporarias com informacao a ser validada
 * 
 *  Return
 *      info: ponteiro para struct com informacao validada
 *      
 ******************************************************************************/
init_info_struct* check_correct_input(char* tab_size, 
   char* dict_filename, 
   char* letras_filename, 
   char* max_letters, 
   char* max_plays, 
   char* init_board_filename,
   char* game_mode, 
   char* out_board_filename, 
   char* register_filename, 
   init_info_struct* info)
{
    char* aux_str;

    ///////////processar input sobre o modo de jogo
    info->game_mode = atoi(game_mode);
    if ((info->game_mode < 1) || (info->game_mode > 4))
    {
        exit_("Modo de jogo não válido");
    }
    //////////////////////////

    ///////////processar input sobre o tabuleiro
    if(tab_size != NULL)
    {
        info->ncolunas = atoi(aux_str = split_string(tab_size, 'x'));
        info->nlinhas = atoi(tab_size);
        free(aux_str);
        if ((info->nlinhas > 15) || (info->nlinhas < 7) || (info->ncolunas > 15) || (info->ncolunas < 7))
        {
            exit_("Dimensões de tabuleiro inválidas");
        }
        if ((info->nlinhas%2)==0 || (info->ncolunas%2)==0) /* Ver se numero de linhas e colunas sao impares*/
        {
            exit_("Dimensoes nao impares");
        }
    }
    else
    {
        info->nlinhas = 9;
        info->ncolunas = 9;
    }
    if((init_board_filename != NULL))
    {
        info->init_board_filename = (char*)malloc(100*sizeof(char));
        strcpy(info->init_board_filename, init_board_filename);
    }
    //////////////////////////

    ///////////processar input sobre as letras e jogadas
    if ((letras_filename == NULL))
    {
        info->letras_filename = (char*)malloc(100*sizeof(char));
        strcpy(info->letras_filename, "letras.txt");
    }
    else
    {
        info->letras_filename = (char*)malloc(100*sizeof(char));
        strcpy(info->letras_filename, letras_filename);
    }
    if (max_letters == NULL)
    {
        info->max_letters = INFINITY;
    }
    else
    {
        info->max_letters = atoi(max_letters);
    }
    if (max_plays == NULL)
    {
        info->max_plays = INFINITY;
    }
    else
    {
        info->max_plays = atoi(max_plays);
    }
    //////////////////////////

    ///////////processar input sobre o dicionario
    if ((dict_filename == NULL))
    {
        info->dict_filename = (char*)malloc(100*sizeof(char));
        //strcpy(info->dict_filename, "/usr/share/dict/words");
        strcpy(info->dict_filename, "words");
    }
    else
    {
        info->dict_filename = (char*)malloc(100*sizeof(char));
        strcpy(info->dict_filename, dict_filename);
    }
    //////////////////////////

    ///////////processar input sobre os ficheiros de output
    if(out_board_filename != NULL)
    {
        info->out_board_filename = (char*)malloc(100*sizeof(char));
        strcpy(info->out_board_filename, out_board_filename);
    }
    else
    {
        info->out_board_filename = NULL;
    }
    if(register_filename != NULL)
    {
        info->register_filename = (char*)malloc(100*sizeof(char));
        strcpy(info->register_filename, register_filename);
    }
    else
    {
        info->register_filename = NULL;   
    }
    //////////////////////////

    return info;
}

/******************************************************************************
 *  NAME
 *      process_input_tab: funcao que processa o tabuleiro inicial de um ficheiro
 * 
 *  Arguments
 *      tabuleiro: matriz alocada por inicializar
 *      info: ponteiro para struct com informacao da linha de comando
 * 
 *  Return
 *      tabuleiro: tabuleiro inicializado
 *      
 ******************************************************************************/
char** process_input_tab(init_info_struct* info,char** tabuleiro)
{
    FILE *file;
    if((file=fopen(info->init_board_filename, "r")) == NULL)
    {
        exit_("erro a abrir ficheiro de jogo");
    }
    char* aux_str = (char*)malloc(50*sizeof(char));
    int counter = 0;
    int i = 0;
    while((fscanf(file, "%[^\n] ", aux_str) != EOF) && i != info->nlinhas)
    {

        counter = 0;
        for (int j = 2; j < strlen(aux_str); ++j) //ignorar os primeiros 3 chars de cada linha
        {
            if ((aux_str[j] != ' ') && (aux_str[j] != '\0')) //guardar apenas os chars que nos interessam
            {
                tabuleiro[i][counter] = aux_str[j];
                counter++;
            }
        }
        i++;
    }
    /*for(int i = 0; i < info->nlinhas; i++)
    {
        counter = 0;
        if (fscanf(file, "%[^\n]", aux_str) == EOF) //ler o ficheiro linha a linha
        {
            break;
        }
        else
        {
            aux_str[strlen(aux_str)-1]= '\0';
        }
        for (int j = 3; j < strlen(aux_str); ++j) //ignorar os primeiros 3 chars de cada linha
        {
            if ((aux_str[j] != ' ') && (aux_str[j] != '\0'))//guardar apenas os chars que nos interessam
            {
                printf("word:%s\n", aux_str);
                printf("char:%c || j: %d || i: %d || counter: %d\n", aux_str[j], j, i, counter);
                tabuleiro[i][counter] = aux_str[j];
                counter++;
            }
        }

    }*/
    free(aux_str);
    fclose(file);
    return tabuleiro;
}

/******************************************************************************
 *  NAME
 *      prevent_seg_fault: funcao que previne seg fault
 * 
 *  Arguments
 *      length: tamanho do array
 *      index: index a ver se esta dentro do array
 *      
 ******************************************************************************/
void prevent_seg_fault(int length, int index)
{
   if(index > length)
   {
      exit_("Comando invalido");
   }
}

/******************************************************************************
 *  NAME
 *      split_string: funcao que separa uma string em 2
 * 
 *  Arguments
 *      source: string a separar
 *      token: char onde separar a string
 * 
 *  Return
 *      str2: segunda metade da string
 * 
 *  Comments
 *      A primeira metade da string fica na source
 *      
 ******************************************************************************/
char* split_string(char* source, char token)
{
    char* str2 = (char*)malloc((strlen(source)+1)*sizeof(char));
    strcpy(str2, source);
    int start_split = 0;
    int token_pos = 0;
    for (int i = 1; i < strlen(source)+1; ++i)
    {
        if (source[i-1] == token)
        {
            start_split = 1;
            token_pos = i-1;
        }
        if (start_split)
        {
            str2[i - token_pos-1] = source[i];
        }
    }
    source[token_pos] = '\0';
    return str2;
}


void exit_(char* message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

/******************************************************************************
 *  NAME
 *      calculate_points: funcao que calcula pontos de uma palavra
 * 
 *  Arguments
 *      tabuleiro: tabuleiro preenchido
 *      word: palavra cujos pontos se pretende calcular
 *      linha: linha da primeira letra da palavra
 *      coluna: coluna da primeira letra da palavra
 *      direction: direcao da palavra
 * 
 *  Return
 *      points: pontos calculados
 *      
 ******************************************************************************/
int calculate_points(char** tabuleiro, char* word, int linha, int coluna, char direction)
{
    //////// valores das letras
    int values[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1 ,1 ,3 ,10 ,1 ,1 ,1 ,1 ,4 ,4 ,8 ,4 ,10};
    ///////

    int bonus_letter=1, bonus_word=1; //inicializados a 1 pq vao ser usados numa multiplicacao
    int points = 0;
    for (int i = 0; i < strlen(word); ++i)
    {
        bonus_letter=1;
        //if (i != do_not_count_index)
        //{
            if (direction=='V')
            {
                if (tabuleiro[linha+i][coluna]=='2')
                {
                    bonus_letter=2;
                }
                if (tabuleiro[linha+i][coluna]=='3')
                {
                    bonus_letter=3;
                }
                if (tabuleiro[linha+i][coluna]=='$')
                {
                    bonus_word= bonus_word*2; //se houver mais que um '$' os bonus acumulam
                }
                //printf("vertical: %c\n", tabuleiro[linha+i][coluna]);

            }
             if (direction=='H')
            {
                if (tabuleiro[linha][coluna+i]=='2')
                {
                    bonus_letter=2;
                }
                if (tabuleiro[linha][coluna+i]=='3')
                {
                    bonus_letter=3;
                }
                if (tabuleiro[linha][coluna+i]=='$')
                {
                    bonus_word=2;
                }
                //printf("horizontal: %c\n", tabuleiro[linha][coluna+i]);
                
            }
            points += values[word[i]-97]*bonus_letter; //word[i]-97 faz o alfabeto corresponder as letras pela mesma ordem
        //}
    }
    points*=bonus_word;
    return points;
}

/******************************************************************************
 *  NAME
 *      print_matrix: funcao que escreve o tabuleiro final para o ficheiro
 * 
 *  Arguments
 *      matrix: tabuleiro a imprimir
 *      linhas, colunas: tamanho do tabuleiro a imprimir
 *      
 ******************************************************************************/
void print_matrix(char** matrix, int linhas, int colunas, FILE* fp)
{
    char numero = '1';
    char letra = 'A';

    for (int i = 0; i < linhas+1; ++i)
    {
        if (i!=linhas)
        {
            if (i < 9)  //primeiras 9 linhas so tem 1 digito
            {
                fprintf(fp, " ");
                fprintf(fp, "%c ", numero);
            }
            else
            {
                fprintf(fp, "1"); //linhas seguintes comecam por 1
                fprintf(fp, "%c ", numero-10);
            }
            numero++;
            for (int j = 0; j < colunas; ++j)
            {
                if (j == colunas-1)
                {
                    fprintf(fp, "%c",matrix[i][j]);
                }
                else
                {
                    fprintf(fp, "%c ",matrix[i][j]);
                }
            }
            fprintf(fp, "\n");
        }
        else            //////////colocar letras por baixo
        {
            fprintf(fp, "   ");
            for (int j = 0; j < colunas; ++j)
            {
                fprintf(fp, "%c ",letra);
                letra++;
            }
        }
    }
    fprintf(fp, "\n");
}

/******************************************************************************
 *  NAME
 *      free_matrix: funcao que faz free do tabuleiro
 * 
 *  Arguments
 *      matrix: tabuleiro a imprimir
 *      linhas, colunas: tamanho do tabuleiro a imprimir
 *      
 ******************************************************************************/
void free_matrix(char** matrix, int linhas)
{
    for (int i = 0; i < linhas ; ++i)
    {       
        free(matrix[i]);
    }
    free(matrix);
}

/******************************************************************************
 *  NAME
 *      is_letter: funcao que indica se uma dada casa esta livre, tem uma letra, tem uma letra especifica ou tem um '#'
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      letter: (opcional) se quisermos ver se uma dada casa contem esta letra especifica
 * 
 *  Return
 *      0->ocupada por uma letra
 *      1->casa livre
 *      2->letra especifica
 *      3->ocupada por um '#'
 *      
 ******************************************************************************/
int is_letter(char**tabuleiro, int linha, int coluna, char letter)
{
    if((tabuleiro[linha][coluna] == '.') || (tabuleiro[linha][coluna] == '$') || (tabuleiro[linha][coluna] == '2') || (tabuleiro[linha][coluna] == '3'))
    {
        return 1;
    }
    if(tabuleiro[linha][coluna] == letter)
    {
        return 2;
    }
    if(tabuleiro[linha][coluna] == '#')
    {
        return 3;
    }
   return 0;
}

char** insert_word(char** tabuleiro, int nlinhas, int ncolunas, char letra, int linha, char* word, char direction, int length, int coluna)
{
   if (direction == 'H')
   {
      for (int i = 0; i < length; i++)
      {
        tabuleiro[linha-1][coluna]=word[i];
        coluna++;  
      } 
   }
    
   if (direction == 'V')
   {
      for (int i = 0; i < length; i++)
      {
        tabuleiro[linha-1][coluna] = word[i];
        linha++; 
      }
   }
   return tabuleiro;
}

/******************************************************************************
 *  NAME
 *      play_vertical: funcao que escreve uma palavra vertical no tabuleiro
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      word: palavra a colocar
 * 
 *  Return 
 *      tabuleiro: tabuleiro com a palavra colocada
 *      
 ******************************************************************************/
char** play_vertical(char** tabuleiro, char* word, int linha, int coluna)
{
    int length = strlen(word);
    for (int i = 0; i < length; i++)
    {
        tabuleiro[linha][coluna] = word[i];
        linha++; //coluna contante, linha incrementada
    }
    return tabuleiro;
}

/******************************************************************************
 *  NAME
 *      play_horizontal: funcao que escreve uma palavra horizontal no tabuleiro
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      word: palavra a colocar
 * 
 *  Return 
 *      tabuleiro: tabuleiro com a palavra colocada
 *      
 ******************************************************************************/
char** play_horizontal(char** tabuleiro, char* word, int linha, int coluna)
{
    int length = strlen(word);
    for (int i = 0; i < length; i++)
    {
        tabuleiro[linha][coluna] = word[i];
        coluna++; //linha contante, coluna incrementada
    }
    return tabuleiro;
}

/******************************************************************************
 *  NAME
 *      remove_letters_from_hand: funcao que remove da mão as letras usadas pela palavra
 * 
 *  Arguments
 *      tabuleiro
 *      linha, coluna, direction
 *      letters_in_hand: mao antes de remover
 *      current_word: palavra a colocar
 * 
 *  Return 
 *      letters_in_hand: mao sem letras usadas
 *      
 *  Comments
 *      sabemos que as letras da palavra estao na mao porque isso ja foi testado antes
 ******************************************************************************/
char* remove_letters_from_hand(char **tabuleiro, char* current_word, char direction, int linha, int coluna, char* letters_in_hand)
{ 
    for (int i = 0; i < strlen(current_word); ++i) //percorrer a palavra
    {
        if (direction == 'V')
        {
            if (is_letter(tabuleiro,linha+i,coluna,'\0') == 1) //so retiramos letra da mao se ainda nao houver uma letra nessa casa
            {
                for (int j = 0; j < strlen(letters_in_hand); ++j) //procurar a letra necessaria na mao
                {
                    if(current_word[i] == letters_in_hand[j])
                    {
                        letters_in_hand[j] = ' ';  //se encontarmos removemos da mao
                        break;
                    }
                }
            }
        }
        if (direction == 'H')
        {
            if (is_letter(tabuleiro,linha,coluna+i,'\0') == 1) //so usamos letra da mao se ainda nao houver uma letra nessa casa
            {
                for (int j = 0; j < strlen(letters_in_hand); ++j)//ver se temos a letra necessaria na mao
                {
                    if(current_word[i] == letters_in_hand[j])
                    {
                        letters_in_hand[j] = ' ';  //se encontarmos removemos da copia da mao
                        break;
                    }
                }
            }
        }
    }
    return letters_in_hand;
}

/******************************************************************************
 *  NAME
 *      update_hand: funcao que atualiza as letras disponiveis na mao
 * 
 *  Arguments
 *      all_letters: letras no saco
 *      letters_in_hand: mao antes de atualizar
 *      current_word: palavra a colocar
 * 
 *  Return 
 *      letters_in_hand: mao atualizada
 *      
 *  Comments
 *      primeiro percorremos as letras na mao e chegamos as letras para a esquerda
 *      depois repomos a mao com letras do saco
 * 
 *      tambem sabemos que as letras da palavra estao na mao porque isso ja foi testado antes
 ******************************************************************************/
char* update_hand(char* letters_in_hand, char* all_letters, int* index, int* no_more_letters)
{
    char aux_char;
    for (int i = 0; i < strlen(letters_in_hand); ++i)
    {
        if (letters_in_hand[i] == ' ') ///procurar espacos vazios
        {
            for (int j = i; j < strlen(letters_in_hand); ++j)
            {
                if (letters_in_hand[j] != ' ') ///procurar primeira letra depois do espaco vazio e trocar
                {
                    aux_char = letters_in_hand[i];
                    letters_in_hand[i] = letters_in_hand[j];
                    letters_in_hand[j] = aux_char;
                    break;
                }
            }
        }
    }

    if (!(*no_more_letters))
    {
        for (int i = 0; i < strlen(letters_in_hand); ++i)
        {
            if (letters_in_hand[i] == ' ') //procurar espacos em branco na mao
            {
                if (*index < strlen(all_letters)) //verificar que ainda ha letras no saco
                {
                    letters_in_hand[i] = all_letters[*index]; //repor mao
                    *index = *index + 1;
                }
                else
                {
                    *no_more_letters = 1;
                    return letters_in_hand;
                }
            }
        }
    }
    return letters_in_hand;
}

/******************************************************************************
 *  NAME
 *      try_word: funcao que verifica se uma palavra pode ser posta no tabuleiro
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      current_word: palavra a colocar
 *      direction
 *      info
 *      dict: lista de palavras validas
 *      dict_size: numero de palavras validas
 * 
 *  Return 
 *      1 -> palavra pode ser colocada
 *      0 -> palavra nao pode ser colocada
 * 
 *  Comments
 *      nomes das funcoes chamadas explicam se a si proprios
 *      
 ******************************************************************************/
int try_word(char** tabuleiro, char* current_word, int linha, int coluna, char direction, init_info_struct* info, char** dict, int dict_size, char* letters_in_hand)
{
    if ((linha > info->nlinhas) || (coluna > info->ncolunas))
    {
        return 0;
    }
    if (excede_tabuleiro(tabuleiro, current_word, linha, coluna, direction, info))
    {
        return 0;
    }
    if (hashtag(tabuleiro, direction, linha, coluna, strlen(current_word)))
    {
        return 0;
    }
    if (!at_least_one_blank(tabuleiro, current_word, linha, coluna, direction))
    {
        return 0;
    }
    if (!all_letters_match(tabuleiro, current_word, linha, coluna, direction))
    {
        return 0;
    }
    if(!blank_before_after(tabuleiro, current_word, linha, coluna, direction, info)) //usado para impedir palavras adjacentes nos extremos
    {
        return 0;
    }
    if(((info->game_mode == 3) || (info->game_mode == 4)))
    {
        if ((!letters_available(tabuleiro, current_word, direction, linha, coluna, letters_in_hand)))
        {
            return 0;
        }
    }
    return 1;
}

//// ------------------------------funcoes chamadas pelo try_word-----------------------------------------------------

/******************************************************************************
 *  NAME
 *      letters_available: funcao que verifica se a palavra pode ser formada pelas letras na mao
 * 
 *  Arguments
 *      tabuleiro
 *      current_word: palavra a colocar
 *      letters_in_hand
 *      direction, linha, coluna: informação sobre a palavra
 * 
 *  Return 
 *      1 -> palavra pode ser colocada
 *      0 -> palavra nao pode ser colocada
 *      
 ******************************************************************************/
int letters_available(char **tabuleiro, char* current_word, char direction, int linha, int coluna, char* letters_in_hand)
{
    char* aux_hand = (char*)malloc(sizeof(char)*(strlen(letters_in_hand))+1);

    int letter_found = 0;

    strcpy(aux_hand, letters_in_hand);  //criar cópia da mão

    for (int i = 0; i < strlen(current_word); ++i) //percorrer a palavra
    {
        if (direction == 'V')
        {
            if (is_letter(tabuleiro,linha+i,coluna,'\0') == 1) //so usamos letra da mao se ainda nao houver uma letra nessa casa
            {
                letter_found = 0;
                for (int j = 0; j < strlen(aux_hand); ++j) //ver se temos a letra necessaria na mao
                {
                    if(current_word[i] == aux_hand[j])
                    {
                        letter_found = 1;
                        aux_hand[j] = ' ';  //se encontarmos removemos da copia da mao
                        break;
                    }
                }
                if(!letter_found)   /// se a letra nao estiver na mao, a palavra nao é válida
                {
                    free(aux_hand);
                    return 0;
                }
            }
        }
        if (direction == 'H')
        {
            if (is_letter(tabuleiro,linha,coluna+i,'\0') == 1) //so usamos letra da mao se ainda nao houver uma letra nessa casa
            {
                letter_found = 0;
                for (int j = 0; j < strlen(aux_hand); ++j)//ver se temos a letra necessaria na mao
                {
                    if(current_word[i] == aux_hand[j])
                    {
                        letter_found = 1;
                        aux_hand[j] = ' ';  //se encontarmos removemos da copia da mao
                        break;
                    }
                }
                if(!letter_found) /// se a letra nao estiver na mao, a palavra nao é válida
                {
                    free(aux_hand);
                    return 0;
                }
            }
        }
    }
    free(aux_hand);
    return 1;
}

/******************************************************************************
 *  NAME
 *      check_adjacente: funcao que verifica se as palavras formadas na perpendicular sao validas
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      current_word: palavra a colocar
 *      direction
 *      info
 *      dict: lista de palavras validas
 *      dict_size: numero de palavras validas
 * 
 *  Return 
 *      NULL -> palavra nao pode ser colocada
 *      return_struct: 
 *          valid_words -> lista de palavras validas na perpendicular
 *          valid_words_rows, valid_words_cols -> listas com as coordenadas das palavras validas na pependicular
 *          valid_words_direction -> lista com a direção das palavras validas na perpendicular
 * 
 *  Comments
 *      é preciso guardar as coordenadas e direção das palavras na perpendicular para ser possivel contar os seus pontos na função calculate points
 *      
 ******************************************************************************/
adj_info_struct* check_adjacente(char **tabuleiro, char* current_word, char direction, int linha, int coluna, init_info_struct* info, char** dict, int dict_size, adj_info_struct* return_struct)
{
    /////////////inicializar struct
    return_struct = (adj_info_struct*)malloc(sizeof(adj_info_struct));
    return_struct->adjacent_words = (char**)malloc(strlen(current_word)*sizeof(char*));
    return_struct->starting_rows = (int*)malloc(strlen(current_word)*sizeof(int));
    return_struct->starting_cols = (int*)malloc(strlen(current_word)*sizeof(int));
    return_struct->directions = (char*)malloc(strlen(current_word)*sizeof(char));
    for (int i = 0; i < strlen(current_word); i++)
    {
        return_struct->adjacent_words[i]=(char*)malloc(info->nlinhas+1);
        strcpy(return_struct->adjacent_words[i]," ");
    }
    //////////////////////

    int number_adj_words = 0;
    int second_space_flag = 1;
    char *aux_str = (char*)malloc(sizeof(char)*info->nlinhas);  /// variavel que guarda possiveis palavras formadas 
    if (direction == 'H')
    {
        int j=0;
        int k=0;
        for (int i = 0; i < strlen(current_word); i++) ///percorrer a palavra dada
        {
            if (is_letter(tabuleiro, linha, coluna+i, '\0') == 1)
            {
                j = 0;
                k = 0;
                strcpy(aux_str, " ");
                //printf("%c\n", tabuleiro[linha-j][coluna+i]);
                while (is_letter(tabuleiro,linha-j,coluna+i,'\0')==0)
                {
                    j++; /// subir ate encontrar um espaco vazio ou bater na "parede"
                    if (linha-j < 0)
                    {
                        j--;
                        break;
                    }
                }
                while ((is_letter(tabuleiro,linha-j+k,coluna+i,'\0')==0) || (second_space_flag == 1))
                {
                    aux_str[k]=tabuleiro[linha-j+k][coluna+i];  /// descer ate encontrar um espaco vazio ou bater na "parede"                                       
                                                                /// guardando as letras de modo a formar uma possivel palavra
                    
                    if (j == k)
                    {
                        second_space_flag = 0;
                        aux_str[k] = current_word[i];
                    }

                    k++;  
                    if ((linha-j+k) > info->nlinhas - 1)
                    {
                        break;
                    }              
                } 
                aux_str[k] = '\0';
                second_space_flag = 1;
                if (strlen(aux_str) > 1)
                {
                    printf("original :%s palavra :%s %d %d %c j:%d i:%d k:%d\n", current_word,aux_str, linha -j + 1, coluna +i, 'V', j,i,k);
                    if (word_in_dict(dict,aux_str,dict_size))   ///ver se a palavra formada esta na lista de palavras validas
                    {
                        strcpy(return_struct->adjacent_words[i],aux_str);
                        return_struct->starting_rows[i] = linha-j + 1;
                        return_struct->starting_cols[i] = coluna + i;
                        return_struct->directions[i] = 'V';
                        number_adj_words++;  
                    }
                    else
                    {
                        free_return_struct(return_struct, strlen(current_word));
                        free(aux_str);
                        return NULL;    ///se nao houver uma palavra valida a palavra dada nao pode ser colocada
                    }
                }
            }           
        }
    }
    if (direction == 'V')   ///igual as palavras na horizontal so que rodando o tabuleiro
    {
        int j=0;
        int k=0;
        for (int i = 0; i < strlen(current_word); i++)
        {
            if (is_letter(tabuleiro, linha+i, coluna, '\0') == 1)
            {
                j = 0;
                k = 0;
                strcpy(aux_str, " ");
                //printf("%c\n", tabuleiro[linha+i][coluna-j]);
                while (is_letter(tabuleiro,linha+i,coluna-j,'\0')==0)
                {
                    j++; /// andar para a esquerda ate encontrar um espaco vazio ou bater na "parede"
                    if (coluna-j < 0)
                    {
                        j--;
                        break;
                    }                    
                }
                while ((is_letter(tabuleiro,linha+i,coluna-j+k,'\0')==0) || (second_space_flag == 1))
                {
                    aux_str[k]=tabuleiro[linha+i][coluna-j+k];  /// andar para a direita ate encontrar um espaco vazio ou bater na "parede"                                       
                                                                /// guardando as letras de modo a formar uma possivel palavra
                    if (j == k)
                    {
                        second_space_flag = 0;
                        aux_str[k] = current_word[i];
                    }
                    k++;
                    if ((coluna-j+k) > info->ncolunas - 1)
                    {
                        break;
                    }                                                                
                } 
                aux_str[k] = '\0';
                second_space_flag = 1;
                if (strlen(aux_str) > 1)
                {
                    printf("original :%s palavra :%s %d %d %c j:%d i:%d k:%d\n",current_word, aux_str, linha + i, coluna -j + 1, 'H',j,i,k);
                    if (word_in_dict(dict,aux_str,dict_size))   ///ver se a palavra formada esta na lista de palavras validas
                    {
                        strcpy(return_struct->adjacent_words[i],aux_str);
                        return_struct->starting_rows[i] = linha + i;
                        return_struct->starting_cols[i] = coluna -j + 1;
                        return_struct->directions[i] = 'H';
                        number_adj_words++;  
                    }
                    else
                    {
                        free_return_struct(return_struct, strlen(current_word));
                        free(aux_str);
                        return NULL;    ///se nao houver uma palavra valida a palavra dada nao pode ser colocada
                    }
                }
            }   
        }  
    }
    return_struct->number_adj_words = number_adj_words;
    free(aux_str);
    return return_struct;
}

void free_return_struct(adj_info_struct* return_struct, size_t lentgh)
{
    free(return_struct->starting_rows);
    free(return_struct->starting_cols);
    free(return_struct->directions);
    for (int i = 0; i < lentgh; i++)
    {
        free(return_struct->adjacent_words[i]);
    }
    free(return_struct->adjacent_words);
    free(return_struct);
}
/******************************************************************************
 *  NAME
 *      word_in_dict: funcao que verifica se uma palavra esta na lista de palavras validas
 * 
 *  Arguments
 *      dict: lista de palavras validas 
 *      word: palavra a testar
 *      dict_size: numero de palavras na lista dict
 * 
 *  Return 
 *      1 -> palavra valida
 *      0 -> palavra nao valida
 *      
 ******************************************************************************/
int word_in_dict(char **dict, char* word, int dict_size)
{
    for (int i = 0; i < dict_size; i++)
    {
        if (strcmp(word,dict[i])==0)
        {
            return 1;
        }
    }
    return 0;
}

/******************************************************************************
 *  NAME
 *      blank_before_after: funcao que verifica se uma palavra nao tem letras nos seus extremos
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      current_word: palavra a colocar
 *      direction
 *      info
 * 
 *  Return 
 *      1 -> palavra pode ser colocada
 *      0 -> palavra nao pode ser colocada
 * 
 *  Comments
 *      palavra encostada a uma "parede" nao precisa de ser testada nesse extremo
 *      palavra encostada as duas paredes nao precisa de ser testada
 *      
 ******************************************************************************/
int blank_before_after(char** tabuleiro, char* current_word, int linha, int coluna, char direction, init_info_struct* info)
{
    if (direction == 'V')
    {
        if (linha > 0)
        {
            if (is_letter(tabuleiro, linha - 1, coluna, '\0') == 0)
            {
                return 0;
            }
        }
        if (linha + strlen(current_word) <  info->nlinhas)
        {
            if (is_letter(tabuleiro, linha + strlen(current_word), coluna, '\0') == 0)
            {
                return 0;
            }
        }
    }
    if (direction == 'H')
    {
        if (coluna > 0)
        {
            if (is_letter(tabuleiro, linha, coluna - 1, '\0') == 0)
            {
                return 0;
            }
        }
        if (coluna + strlen(current_word) <  info->ncolunas)
        {
            if (is_letter(tabuleiro, linha, coluna + strlen(current_word), '\0') == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

/******************************************************************************
 *  NAME
 *      at_least_one_letter: funcao que verifica se pelo menos uma letra que ja esta no tabuleiro e usada
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      current_word: palavra a colocar
 *      direction
 * 
 *  Return 
 *      1 -> palavra pode ser colocada
 *      0 -> palavra nao pode ser colocada
 *      
 ******************************************************************************/
int at_least_one_letter(char** tabuleiro, char* current_word, int linha, int coluna, char direction)
{
    for (int i = 0; i < strlen(current_word); ++i)
    {
        if (direction == 'V')
        {
            if (is_letter(tabuleiro, linha + i, coluna, '\0') == 0)
            {
                return 1;
            }
        }
        if (direction == 'H')
        {
            if (is_letter(tabuleiro, linha, coluna + i, '\0') == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

/******************************************************************************
 *  NAME
 *      check_word_includes_coordinates: funcao que verifica se uma palavra a colocar ocupará uma certa casa
 * 
 *  Arguments

 *      linhas, colunas: tamanho do tabuleiro
 *      current_word: palavra a verificar
 *      direction
 *      co_linha, co_coluna: coordenadas da casa a verificar
 * 
 *  Return 
 *      1 -> palavra inclui casa
 *      0 -> palavra nao inclui casa
 *      
 ******************************************************************************/
int check_word_includes_coordinates(char* current_word, int linha, int coluna, char direction, int co_linha, int co_coluna)
{
    for (int i = 0; i < strlen(current_word); ++i)
    {
        if (direction == 'V')
        {
            if (linha + i == co_coluna)
            {
                return 1;
            }
        }
        if (direction == 'H')
        {
            if (coluna + i == co_coluna)
            {
                return 1;
            }
        }
    }
    return 0;
}

/******************************************************************************
 *  NAME
 *      at_least_one_blank: funcao que verifica se pelo menos uma letra nova vai ser posta
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      current_word: palavra a colocar
 *      direction
 * 
 *  Return 
 *      1 -> palavra pode ser colocada
 *      0 -> palavra nao pode ser colocadaS
 *      
 ******************************************************************************/
int at_least_one_blank(char** tabuleiro, char* current_word, int linha, int coluna, char direction)
{
    for (int i = 0; i < strlen(current_word); ++i)
    {
        if (direction == 'V')
        {
            if (is_letter(tabuleiro, linha + i, coluna, '\0') == 1)
            {
                return 1;
            }
        }
        if (direction == 'H')
        {
            if (is_letter(tabuleiro, linha, coluna + i, '\0') == 1)
            {
                return 1;
            }
        }
    }
    return 0;
}

/******************************************************************************
 *  NAME
 *      all_letters_match: funcao que verifica se as letras da palavra nao vao ser escritas por cima de letras diferentes
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      current_word: palavra a colocar
 *      direction
 * 
 *  Return 
 *      1 -> palavra pode ser colocada
 *      0 -> palavra nao pode ser colocada
 * 
 *  Comments
 *      depois de verificar que ha uma letra numa dada posicao, verifica se a letra bate certo
 *      
 ******************************************************************************/

int all_letters_match(char** tabuleiro, char* current_word, int linha, int coluna, char direction)
{
    for (int i = 0; i < strlen(current_word); ++i)
    {
        if (direction == 'V')
        {
            if (is_letter(tabuleiro, linha + i, coluna, '\0') == 0)
            {
                if (is_letter(tabuleiro, linha + i, coluna, current_word[i]) != 2)
                {
                    return 0;
                }
            }
        }
        if (direction == 'H')
        {
            if (is_letter(tabuleiro, linha, coluna + i, '\0') == 0)
            {
                if (is_letter(tabuleiro, linha, coluna + i, current_word[i]) != 2)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}


int excede_tabuleiro(char** tabuleiro, char* current_word, int linha, int coluna, char direction, init_info_struct* info)
{
    int length = strlen(current_word);
    if (direction == 'V')
    {
        if (linha + length > info->nlinhas)
        {
            return 1;
        }
    }
    if (direction == 'H')
    {
        if (coluna + length > info->ncolunas)
        {
            return 1;
        }
    }
    return 0;
}

/******************************************************************************
 *  NAME
 *      hashtag: funcao que verifica se uma palavra vai ser escrita sobre uma casa com '#'
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      current_word: palavra a colocar
 *      direction
 *      length: tamanho da palavra a colocar
 * 
 *  Return 
 *      0 -> palavra pode ser colocada
 *      1 -> palavra nao pode ser colocada
 * 
 *      
 ******************************************************************************/
int hashtag(char**tabuleiro, char direction, int linha, int coluna, int length)
{
    if (direction == 'H')
    {
        for (int i = 0; i < length; i++)
        {
            if (tabuleiro[linha][coluna] == '#')
            {
                return 1;
            }
            coluna++;
        }
    }
    if (direction == 'V')
    {
        for (int i = 0; i < length; i++)
        {
            if (tabuleiro[linha][coluna]=='#')
            {
                return 1;
            }
            linha++;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------------------------------------

/******************************************************************************
 *  NAME
 *      read_dict: funcao que abre e verifica a abretura do ficheiro do diconario
 * 
 *  Arguments
 *      dict_filename: nome do ficheiro a abrir
 * 
 *  Return 
 *      ptr_dict: ponteiro para o ficheiro aberto
 *      
 ******************************************************************************/
FILE *read_dict(char* dict_filename)
{
    FILE *ptrDict;
    if ((ptrDict=fopen(dict_filename,"r"))==NULL)
    {
        exit_("Imposasível abrir o dicionário");
    }
    return ptrDict;  
}

/******************************************************************************
 *  NAME
 *      read_letters: funcao que abre e verifica a abretura do ficheiro das letras
 * 
 *  Arguments
 *      dict_filename: nome do ficheiro a abrir
 * 
 *  Return 
 *      ptr_dict: ponteiro para o ficheiro aberto
 *      
 ******************************************************************************/
FILE *read_letters(char* letters_filename)
{
    FILE *ptrLetters;
    if ((ptrLetters=fopen(letters_filename,"r"))==NULL)
    {
        exit_("Impossível abrir o dicionário");
    }
    return ptrLetters;  
}

/******************************************************************************
 *  NAME
 *      sort_english_dict: funcao que passa as palavras de um dicionario que tem uma palavra por linha para uma lista
 *                         e ordena por tamanho
 *  Arguments
 *      ptrDict_en: ponteiro para o dicionario
 *      size: variavel usada como segundo return; representa numero de palavras passadas para a lista
 *      info
 * 
 *  Return 
 *      word: lista das palvras validas
 * 
 *  Variaveis declaradas
 *      aux_word: string usada para receber linhas do ficheiro
 *      verify: flag que indica a validade de cada palavra
 *      count: numero de palavra postas na lista (retornado atraves do parametro size)
 * 
 *  Comments
 *      apenas sao postas na lista palavras que cabem no tabuleiro
 *      
 ******************************************************************************/
char** sort_english_dict(FILE *ptrDict_en, int* size, init_info_struct* info)
{
    char **word;
    char *aux_word = (char*)malloc(42*sizeof(char));
    int verify = 0, count=0;
  
    word=(char**)malloc(sizeof(char*));
    word[0] = NULL;
    while (fscanf(ptrDict_en, "%[^\n] ", aux_word) != EOF) 
    {
        //aux_word[strlen(aux_word)-1] = '\0';    //  remover último char que esta a mais

        for (int j = 0; j < strlen(aux_word); j++)//exclusão de palavras que não têm apenas minúsculas
        {
            if (aux_word[j]>='a' && aux_word[j]<='z' && (strlen(aux_word)>=2)  && (strlen(aux_word)<=info->nlinhas))
            { 
                verify = 1;
            }
            else
            {
                verify = 0;
                break;
            }
            
        }
        if (verify==1)
        {
            word = (char**)realloc(word, (count+2)*sizeof(char*)); //palavra valida implica allocar mais espaco e alocar a nova palavra
            word[count]=(char*)malloc(sizeof(char)*42);
            strcpy(word[count], aux_word);
            count++;
        }
    }   

    char *aux_sort;
    for (int j = 0; j < count-1; j++) //sort da lista de palavras permitidas
    {
        for (int i = 0; i < count-j-1; i++)
        {
            if (strlen(word[i])<strlen(word[i+1]))
            {
                aux_sort = word[i];
                word[i]=word[i+1];
                word[i+1]=aux_sort;
            }
            
        }
    }
    
    /*for (int i = 0; i < count; i++)
    {
        printf("%s\n",word[i]);
    }*/
    free(aux_word);
    *size = count;
    return word;
}

/*char** sort_english_dict(FILE *ptrDict_en, int* size, init_info_struct* info)
{
    char **word;
    char *aux_word = (char*)malloc(42*sizeof(char));
    int verify = 0, count=0;
  
    word=(char**)malloc(sizeof(char*)*10000);//espaço alocado??
    for(int k = 0; k< 10000; ++k)
    {
        word[k] = NULL;
    }
    while (fscanf(ptrDict_en, "%[^\n] ", aux_word) != EOF) 
    {
        aux_word[strlen(aux_word)-1] = '\0';
        if (word[count] == NULL)
        {
            word[count]=(char*)malloc(sizeof(char)*42);
        }
        for (int j = 0; j < strlen(aux_word); j++)//exclusão de palavras que não têm apenas minúsculas
        {
            if (aux_word[j]>='a' && aux_word[j]<='z' && (strlen(aux_word)>=2)  && (strlen(aux_word)<=info->nlinhas))
            { 
                verify = 1;
            }
            else
            {
                verify = 0;
                break;
            }
            
        }
        if (verify==1)
        {
            strcpy(word[count], aux_word);
            count++;
        }
    }   

    char *aux_sort;
    for (int j = 0; j < count-1; j++) //sort da lista de palavras permitidas
    {
        for (int i = 0; i < count-j-1; i++)
        {
            if (strlen(word[i])<strlen(word[i+1]))
            {
                aux_sort = word[i];
                word[i]=word[i+1];
                word[i+1]=aux_sort;
            }
            
        }
    }
    
    for (int i = 0; i < count; i++)
    {
        printf("%s\n",word[i]);
    }
    free(aux_word);
    *size = count;
    return word;
}*/


/******************************************************************************
 *  NAME
 *      read_dict: funcao que abre e verifica a abretura do ficheiro do diconario
 * 
 *  Arguments
 *      dict_filename: nome do ficheiro a abrir
 * 
 *  Return 
 *      ptr_dict: ponteiro para o ficheiro aberto
 *      
 ******************************************************************************/
char** sort_pt_dict(FILE *ptrDict_pt, int* size, init_info_struct* info)
{
    char **word;
    char *aux_word = (char*)malloc(1000*sizeof(char));
    int verify = 0, count=0;
    char s[1] = "[";
    char s_1[1] = "/";
    char s_2[1] = "\t";
  
    word=(char**)malloc(sizeof(char*));
    word[0] = NULL;
    while (fscanf(ptrDict_pt, "%[^\n] ", aux_word) != EOF) 
    {
        //aux_word[strlen(aux_word)-1] = '\0';    //  remover último char que esta a mais


        strtok(aux_word, s);
        strtok(aux_word, s_1);
        strtok(aux_word, s_2);
        for (int j = 0; j < strlen(aux_word); j++)//exclusão de palavras que não têm apenas minúsculas
        {
            if (aux_word[j]>='a' && aux_word[j]<='z' && (strlen(aux_word)>=2)  && (strlen(aux_word)<=info->nlinhas))
            { 
                verify = 1;
            }
            else
            {
                verify = 0;
                break;
            }
            
        }
        if (verify==1)
        {
            word = (char**)realloc(word, (count+2)*sizeof(char*)); //palavra valida implica allocar mais espaco e alocar a nova palavra
            word[count]=(char*)malloc(sizeof(char)*42);
            strcpy(word[count], aux_word);
            count++;
        }
    }   

    char *aux_sort;
    for (int j = 0; j < count-1; j++) //sort da lista de palavras permitidas
    {
        for (int i = 0; i < count-j-1; i++)
        {
            if (strlen(word[i])<strlen(word[i+1]))
            {
                aux_sort = word[i];
                word[i]=word[i+1];
                word[i+1]=aux_sort;
            }
            
        }
    }
    
    for (int i = 0; i < count; i++)
    {
        printf("%s\n",word[i]);
    }
    free(aux_word);
    *size = count;
    return word;
}

/******************************************************************************
 *  NAME
 *      check_letra_ao_lado: funcao que verifica se uma casa tem letras ao lado
 * 
 *  Arguments
 *      tabuleiro: tabuleiro 
 *      linhas, colunas: tamanho do tabuleiro
 *      direction
 *      info
 * 
 *  Return 
 *      1 -> palavra pode ser colocada
 *      0 -> palavra nao pode ser colocadaS
 *      
 ******************************************************************************/
int check_letra_ao_lado(char** tabuleiro, int linha, int coluna, char direction, init_info_struct* info)
{
    if (direction == 'V')
    {
        if (coluna == 0)
        {
            if (is_letter(tabuleiro, linha, coluna+1, '\0') == 0)
            {
                return 1;
            }
        }
        else if (coluna == info->ncolunas-1)
        {
            if (is_letter(tabuleiro, linha, coluna-1, '\0') == 0)
            {
                return 1;
            }
        }
        else
        {
            if (is_letter(tabuleiro, linha, coluna-1, '\0') == 0)
            {
                return 1;
            }
            if (is_letter(tabuleiro, linha, coluna+1, '\0') == 0)
            {
                return 1;
            }
        }
    }
    if (direction == 'H')
    {
        if (linha == 0)
        {
            if (is_letter(tabuleiro, linha+1, coluna, '\0') == 0)
            {
                return 1;
            }
        }
        else if (linha == info->nlinhas-1)
        {
            if (is_letter(tabuleiro, linha-1, coluna, '\0') == 0)
            {
                return 1;
            }
        }
        else
        {
            if (is_letter(tabuleiro, linha-1, coluna, '\0') == 0)
            {
                return 1;
            }
            if (is_letter(tabuleiro, linha+1, coluna, '\0') == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

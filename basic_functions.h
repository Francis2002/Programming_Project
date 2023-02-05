/******************************************************************************
 * Last modified: 27-03-2022
 *
 * NAME
 *   basic_functions.h
 *
 * DESCRIPTION
 *   lista de funcoes auxiliares as funcoes principais
 *
 * COMMENTS
 *
 ******************************************************************************/

#ifndef __basic_functions__
#define __basic_functions__

typedef struct init_info
{
    int nlinhas;
    int ncolunas;
    char* dict_filename;
    char* letras_filename;
    int max_letters;
    int max_plays;
    char* init_board_filename;
    int game_mode;
    char* out_board_filename; 
    char* register_filename;

}init_info_struct;

typedef struct adj_info
{
    int *starting_rows;
    int *starting_cols;
    int number_adj_words;
    char *directions;
    char** adjacent_words;
}adj_info_struct;


void print_matrix(char** matrix, int linhas, int colunas, FILE* fp);
void exit_(char* message);
int calculate_points(char** tabuleiro, char* word, int linha, int coluna, char direction);
void free_matrix(char** matrix, int linhas);

char* split_string(char* source, char token);
char** insert_word(char** tabuleiro, int nlinhas, int ncolunas, char letra, int linha, char* word, char direction, int length, int coluna);
int is_letter(char**tabuleiro, int linha, int coluna, char letter);
int hashtag(char**tabuleiro, char direction, int linha, int coluna, int length);
char** play_horizontal(char** tabuleiro, char* word, int linha, int coluna);
char** play_vertical(char** tabuleiro, char* word, int linha, int coluna);
int try_word(char** tabuleiro, char* current_word, int linha, int coluna, char direction, init_info_struct* info, char** dict, int dict_size, char* letters_in_hand);
int excede_tabuleiro(char** tabuleiro,char* current_word,int linha, int coluna, char direction, init_info_struct* info);
int all_letters_match(char** tabuleiro, char* current_word, int linha, int coluna, char direction);
int at_least_one_letter(char** tabuleiro, char* current_word, int linha, int coluna, char direction);
int at_least_one_blank(char** tabuleiro, char* current_word, int linha, int coluna, char direction);
int blank_before_after(char** tabuleiro, char* current_word, int linha, int coluna, char direction, init_info_struct* info);
int letters_available(char **tabuleiro, char* current_word, char direction, int linha, int coluna, char* letters_in_hand);
adj_info_struct* check_adjacente(char **tabuleiro, char* current_word, char direction, int linha, int coluna, init_info_struct* info, char** dict, int dict_size, adj_info_struct* return_struct);
int word_in_dict(char **dict, char* word, int dict_size);

void prevent_seg_fault(int length, int index);
init_info_struct* check_correct_input(char* tab_size, 
   char* dict_filename, 
   char* letras_filename, 
   char* max_letters, 
   char* max_plays, 
   char* init_board_filename,
   char* game_mode, 
   char* out_board_filename, 
   char* register_filename, 
   init_info_struct* info);

char** process_input_tab(init_info_struct* info,char** tabuleiro);

FILE *read_letters(char* letters_filename);
char* remove_letters_from_hand(char **tabuleiro, char* current_word, char direction, int linha, int coluna, char* letters_in_hand);
char* update_hand(char* letters_in_hand, char* all_letters, int* index, int* no_more_letters);

FILE *read_dict(char* dict_filename);
char** sort_english_dict(FILE *ptrDict_en, int* size, init_info_struct* info);
char** sort_pt_dict(FILE *ptrDict_pt, int* size, init_info_struct* info);

void free_return_struct(adj_info_struct* return_struct, size_t lentgh);

int check_letra_ao_lado(char** tabuleiro, int linha, int coluna, char direction, init_info_struct* info);
int check_word_includes_coordinates(char* current_word, int linha, int coluna, char direction, int co_linha, int co_coluna);




#endif    /* __basic_functions__ */

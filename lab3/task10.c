#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum enum_errors
{
    WRONG_QUANTITY_OF_ARGUMENTS = 1,
    FILE_OPEN_ERROR,
    MEMORY_ALLOCATION_ERROR,
    FINE
}status_code;

status_code read_strings_from_file(FILE* input, char*** strings, int* quantity);
void do_work(char** strings,int quantity, FILE* output);
void print_node(FILE* output, char c, int depth);

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("WRONG QUANTITY OF ARGUMENTS\n");
        return WRONG_QUANTITY_OF_ARGUMENTS;
    }

    FILE* input = NULL;
    input = fopen(argv[1], "r");
    if(input == NULL)
    {
        printf("%s - NO SUCH FILE DIRECTORY\n", argv[1]);
        return FILE_OPEN_ERROR;
    }

    char** strings = NULL;
    int quantity = 0;
    status_code check = read_strings_from_file(input, &strings,  &quantity);
    fclose(input);

    if(check != FINE)
    {
        printf("MEMORY ALLOCATION ERROR\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    FILE* output = NULL;
    output = fopen(argv[2], "w");
    if(output == NULL)
    {
        printf("%s - NO SUCH FILE DIRECTORY\n", argv[2]);
        fclose(input);
        return FILE_OPEN_ERROR;
    }

    do_work(strings,quantity, output);
    fclose(output);
    return 0;
}

status_code read_strings_from_file(FILE* input, char*** strings, int* quantity)
{
    *strings = (char**)malloc(sizeof (char*));
    if(*strings == NULL) return MEMORY_ALLOCATION_ERROR;
    int strings_cap = 1;
    while(!feof(input))
    {
        char* str = (char*)malloc(sizeof(char));
        if(str == NULL)
            return MEMORY_ALLOCATION_ERROR;
        int len = 0;
        int capacity = 1;
        char c = 1;
        while(c != '\n' && c != EOF)
        {
            c = getc(input);
            if(c == EOF || c == '\n')
            {
                if(strings_cap == (*quantity))
                {
                    char** for_realloc = (char**)realloc((*strings), sizeof(char *) * 2 * strings_cap);
                    if(for_realloc == NULL)
                    {
                        for(int i = 0; i < *quantity; ++i) free((*strings)[i]);
                        free(*strings);
                        return MEMORY_ALLOCATION_ERROR;
                    }
                    strings_cap<<=1;
                    *strings = for_realloc;
                }
                (*strings)[(*quantity)] = (char*)malloc(sizeof(char) * len);
                if((*strings)[(*quantity)] == NULL) return MEMORY_ALLOCATION_ERROR;
                strcpy((*strings)[(*quantity)++], str);
                free(str);
            }
            if(len == capacity)
            {
                char* for_realloc = realloc(str, sizeof(char) * 2 * capacity);
                if(for_realloc == NULL)
                {
                    free(str);
                    return MEMORY_ALLOCATION_ERROR;
                }
                str = for_realloc;
                capacity<<=1;
            }
            str[len++] = c;
        }
    }
    return FINE;
}

void print_node(FILE* output, char c, int depth)
{
    for(int i = 0; i < depth; ++i) fprintf(output, "\t");
    fprintf(output, "%c\n", c);
}

void do_work(char** strings, int quantity, FILE* output)
{
    for(int i = 0; i < quantity; ++i)
    {
        fprintf(output, "tree number: %d\n", i + 1);
        int depth = 0;
        int length = strlen(strings[i]);
        for(int j = 0; j < length; ++j)
        {
            if(strings[i][j] == '(') depth++;
            else if(strings[i][j] == ')') depth--;
            else if(isalpha(strings[i][j])) print_node(output, strings[i][j], depth);
        }
    }
}

//A (L, M) B (S (O, P) J) C , D , E , F , G
//A,B,I
//A (B (E (G,T,R (W,Z)),F (L,M)),C)
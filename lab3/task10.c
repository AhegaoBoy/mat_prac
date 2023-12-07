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

status_code read_string_from_file(FILE* input, char** string);
status_code do_work(FILE* input, FILE* output);
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

    FILE* output = NULL;
    output = fopen(argv[2], "w");
    if(output == NULL)
    {
        printf("%s - NO SUCH FILE DIRECTORY\n", argv[2]);
        fclose(input);
        return FILE_OPEN_ERROR;
    }

    status_code check = do_work(input, output);
    if(check != FINE)
    {
        printf("MEMORY ALLOCATION ERROR\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    fclose(input);
    fclose(output);
    return 0;
}

status_code read_string_from_file(FILE* input, char** string)
{
    char* str = (char*)malloc(sizeof(char));
    if(str == NULL)
        return MEMORY_ALLOCATION_ERROR;
    int len = 0;
    int capacity = 1;
    char c = 1;
    while(!feof(input))
    {
        c = getc(input);
        if(c == EOF)
        {
            (*string) = (char*)malloc(sizeof(char) * len);
            strcpy(*string, str);
            free(str);
            return FINE;
        }
        if(c == '\n')
        {
            (*string) = (char*)malloc(sizeof(char) * len);
            if(*string == NULL) return MEMORY_ALLOCATION_ERROR;
            strcpy(*string, str);
            free(str);
            return FINE;
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
    return FINE;
}

void print_node(FILE* output, char c, int depth)
{
    for(int i = 0; i < depth; ++i) fprintf(output, "\t");
    fprintf(output, "%c\n", c);
}

status_code do_work(FILE* input, FILE* output)
{
    status_code check;
    char* string;
    int number_of_seq = 1;
    while(!feof(input))
    {
        fprintf(output, "tree number: %d\n", number_of_seq);
        check = read_string_from_file(input, &string);
        int depth = 0;
        if(check != FINE) return check;
        int length = strlen(string);
        for(int i = 0; i < length; ++i)
        {
            if(string[i] == '(') depth++;
            else if(string[i] == ')') depth--;
            else if(isalpha(string[i])) print_node(output, string[i], depth);
        }
        number_of_seq++;
        free(string);
    }
    return FINE;
}

//A (L, M) B (S (O, P) J) C , D , E , F , G
//A,B,I
//A (B (E (G,T,R (W,Z)),F (L,M)),C)

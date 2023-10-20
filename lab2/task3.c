#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>

typedef enum errors
{
    WONG_QUANTITY_OF_ARGUMENTS = 1,
    NO_FILES_INPUT,
    OPEN_FILE_ERROR,
    MEMORY_ALLOCATION_ERROR
}ERRORS;
int quantity_of_str_in_files(int** arr_of_quantity, char* str, int n, ...);

int main(int argc, char* argv[])
{
//    if(argc < 2)
//    {
//        printf("INPUT SUBSTRING AND AT LEAST ONE FILE\n");
//        return WONG_QUANTITY_OF_ARGUMENTS;
//    }
//    if(argc < 3)
//    {
//        printf("INPUT AT LEAST ONE FILE\n");
//        return NO_FILES_INPUT;
//    }
    int files = 2;
    int* arr_of_quantity = (int*)calloc(sizeof(int), files);

    if(arr_of_quantity == NULL)
    {
        free(arr_of_quantity);
        printf("MEMORY ALLOCATION ERROR\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    int check = quantity_of_str_in_files(&arr_of_quantity, "12\n", files, "/home/serzkik/mat_prac/file1.txt", "/home/serzkik/mat_prac/file2.txt");

    if(check == OPEN_FILE_ERROR)
    {
        printf("SOME FILES'VE BEEN INPUT INCORRECTLY\n");
        free(arr_of_quantity);
        return OPEN_FILE_ERROR;
    }
    for(int i = 0; i < files; ++i)
        printf("AMOUNT OF SUBSTRING %s IN FILE %d\t%d\n", argv[1], i+1, arr_of_quantity[i]);
    free(arr_of_quantity);
    return 0;
}

int quantity_of_str_in_files(int** arr_of_quantity, char* str, int n,...)
{
    int line = 1, column = 1, for_pos = n;
    int len_of_str = strlen(str);
    va_list args;
    va_start(args, n);
    while(n > 0)
    {
        FILE* input = NULL;
        char* directory = va_arg(args, char*);
        input = fopen(directory, "r");
        if(input == NULL)
        {
            va_end(args);
            return OPEN_FILE_ERROR;
        }
        printf("FOR FILE WITH DIRECTORY: %s\n", directory);
        while(!feof(input))
        {
            char c = getc(input);//fread(&c, sizeof(char), 1, input);
            if(c == str[0])
            {
                printf("Curren line: %d\tCurrent column: %d\tCommon symbol: %c\n", line, column, c);
                int i;
                for (i = 1; i < len_of_str; ++i)
                {
                    c = getc(input);//fread(&c, sizeof(char), 1, input);
                    if (c != str[i] || feof(input)) break;
                    else
                    {
                        column++;
                        if(c == '\n')
                        {
                            line++;
                            column = 1;
                        }
                    }
                }
                if(i == len_of_str)
                {
                    (*arr_of_quantity)[for_pos - n] ++ ;
                    column = column - len_of_str + 3;


                fseek(input, -len_of_str + 2, SEEK_CUR);
                }
                else fseek(input, -i+1, SEEK_CUR);
            }
            else if(c == '\n')
            {
                line++;
                column = 1;
            }
            else column++;
        }
        line = 1;
        column = 1;
        n--;
    }
    va_end(args);
    return 0;
}
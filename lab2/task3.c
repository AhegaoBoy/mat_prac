#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>

typedef enum errors
{
    OPEN_FILE_ERROR = 1,
    MEMORY_ALLOCATION_ERROR,
    FINE
}ERRORS;
int** quantity_of_str_in_files(int** arr_of_quantity, int* valid, int** arr_for_files, char* str, int n, ...);

int main()
{
    int files = 2;
    int* arr_of_quantity = (int*)calloc(sizeof(int), files);
    int flag = FINE;
    int pos = 0;
    int pos_next;
    if(arr_of_quantity == NULL)
    {
        free(arr_of_quantity);
        printf("MEMORY ALLOCATION ERROR\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    int* arr_for_files = (int*)malloc(sizeof(int) * files);
    if(arr_for_files == NULL)
    {
        free(arr_for_files);
        printf("MEMORY ALLOCATION ERROR\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    int** check = quantity_of_str_in_files(&arr_of_quantity, &flag,&arr_for_files,"2\n", files, "/home/serzkik/mat_prac/file1.txt", "/home/serzkik/mat_prac/file2.txt");

    switch (flag) {
        case OPEN_FILE_ERROR:
            printf("Some files input wrong\n");
            free(arr_of_quantity);
            for(int i = 0; i < sizeof(check)/sizeof(check[0]); ++i)
                free(check[i]);
            free(check);
            return OPEN_FILE_ERROR;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation error\n");
            free(arr_of_quantity);
            for(int i = 0; i < sizeof (check) / sizeof(check[0]); ++i)
                free(check[i]);
            free(check);
            return MEMORY_ALLOCATION_ERROR;
    }
    for(int i = 0; i < files; ++i)
    {
        printf("AMOUNT OF SUBSTRING IN FILE %d\t%d\n", i+1, arr_of_quantity[i]);
        pos_next = arr_for_files[i];
        for(int j = pos; j<pos + pos_next; ++j)
        {
            printf("LINE NUMBER: %d, COLUMN NUMBER: %d\n", check[j][0],check[j][1]);
        }
        pos = pos_next;
    }
    free(arr_of_quantity);
    return 0;
}

int** quantity_of_str_in_files(int** arr_of_quantity, int* valid,int** arr_for_files, char* str, int n,...)
{

    int** array_of_lines_and_columns = (int**)malloc(sizeof(int*));
    if(array_of_lines_and_columns == NULL)
    {
        *valid = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }
    array_of_lines_and_columns[0] = (int*)malloc(sizeof(int) * 2);
    if(array_of_lines_and_columns[0] == NULL)
    {
        *valid = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }
    int capacity = 1;
    int len_of_pairs = 0;
    int line = 1, column = 1, for_pos = n;
    int number_of_file = 0;
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
            *valid = OPEN_FILE_ERROR;
        }
        while(!feof(input))
        {
            char c = getc(input);
            if(c == str[0])
            {
                int current_line = line;
                int current_column = column;
                int i;
                for (i = 1; i < len_of_str; ++i)
                {
                    c = getc(input);
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
                    if(len_of_pairs == capacity)
                    {
                        int** for_realloc = (int**)realloc(array_of_lines_and_columns, 2*capacity * sizeof (int*));
                        capacity <<= 1;
                        if(for_realloc == NULL) {
                            *valid = MEMORY_ALLOCATION_ERROR;
                            return NULL;
                        }
                        array_of_lines_and_columns = for_realloc;
                        for(int j = capacity - len_of_pairs; j < capacity; ++j)
                        {
                            array_of_lines_and_columns[j] = (int *) malloc(sizeof(int) * 2);
                            if (array_of_lines_and_columns[j] == NULL) {
                                *valid = MEMORY_ALLOCATION_ERROR;
                                return NULL;
                            }
                        }
                    }
                    array_of_lines_and_columns[len_of_pairs][0] = current_line;
                    array_of_lines_and_columns[len_of_pairs][1] = current_column;
                    len_of_pairs ++;
                    (*arr_for_files)[number_of_file]++;
                    column = column - len_of_str + (len_of_str + 1);
                    fseek(input, -len_of_str + 1, SEEK_CUR);
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
        number_of_file++;
        line = 1;
        column = 1;
        n--;
    }
    va_end(args);
    return array_of_lines_and_columns;
}
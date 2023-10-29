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
void clear(void* to_clear, ...)
{
    va_list args;
    va_start(args,to_clear);
    void* tmp = va_arg(args, void*);
    while(tmp!= NULL)
    {
        free(tmp);
        tmp = va_arg(args, void*);
    }
    va_end(args);
}
int** quantity_of_str_in_files(int** arr_of_quantity, int* valid, int** arr_for_files, char* str, int n, ...);

int main()
{
    int files = 1;
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
        free(arr_of_quantity);
        printf("MEMORY ALLOCATION ERROR\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    int** check = quantity_of_str_in_files(&arr_of_quantity, &flag,&arr_for_files,"", files,"/home/serzkik/mat_prac/file2.txt", "/home/serzkik/mat_prac/file1.txt");

    switch (flag) {
        case OPEN_FILE_ERROR:
            printf("Some files input wrong\n");
            free(arr_of_quantity);
            for(int i = 0; i < sizeof(check)/sizeof(check[0]); ++i)
                free(check[i]);
            free(check);
            free(arr_for_files);
            free(arr_of_quantity);
            return OPEN_FILE_ERROR;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation error\n");
            free(arr_of_quantity);
            for(int i = 0; i < sizeof (check) / sizeof(check[0]); ++i)
                free(check[i]);
            free(check);
            free(arr_for_files);
            free(arr_of_quantity);
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
    free(arr_for_files);
    //ну тут еще почистить элементы check
    free(check);
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
        free(array_of_lines_and_columns);
        *valid = MEMORY_ALLOCATION_ERROR;
        return NULL;
    }
    int capacity = 1;
    int len_of_pairs = 0;
    int line = 1, column = 1, for_pos = n, prev_line, prev_column;
    int number_of_file = 0;
    int len_of_str = strlen(str);
    int current_line, current_column;
    va_list args;
    va_start(args, n);
    if(str[0] == '\0')
    {
        while(n > 0)
        {
            FILE* input = NULL;
            char* directory = va_arg(args, char*);
            input = fopen(directory, "r");
            if(input == NULL)
                {
                    va_end(args);
                    *valid = OPEN_FILE_ERROR;
                    free(array_of_lines_and_columns);
                    fclose(input);
                    return NULL;
                }
            current_column = 1;
            current_line = 1;
            while(!feof(input))
            {
                int c = fgetc(input);
                if(c == EOF) break;
                if(len_of_pairs == capacity)
                {
                    int** for_realloc = (int**)realloc(array_of_lines_and_columns, 2*capacity * sizeof (int*));
                    capacity <<= 1;
                    if(for_realloc == NULL) {
                        *valid = MEMORY_ALLOCATION_ERROR;
                        free(array_of_lines_and_columns);
                        fclose(input);
                        return NULL;
                    }
                    array_of_lines_and_columns = for_realloc;
                    for(int j = capacity - len_of_pairs; j < capacity; ++j)
                    {
                        array_of_lines_and_columns[j] = (int *) malloc(sizeof(int) * 2);
                        if (array_of_lines_and_columns[j] == NULL) {
                            *valid = MEMORY_ALLOCATION_ERROR;
                            free(array_of_lines_and_columns);
                            fclose(input);
                            return NULL;
                        }
                    }
                }
                if(c =='\n') {
                    array_of_lines_and_columns[len_of_pairs][0] = ++current_line;
                    current_column = 1;
                    array_of_lines_and_columns[len_of_pairs][1] = current_column;
                }
                else
                {
                    array_of_lines_and_columns[len_of_pairs][0] = current_line;
                    array_of_lines_and_columns[len_of_pairs][1] = current_column++;
                }
                len_of_pairs ++;
                (*arr_for_files)[for_pos - n]++;
                (*arr_of_quantity)[for_pos - n]++;
            }
            n--;
        }

    }

    while(n > 0)
    {
        FILE* input = NULL;
        char* directory = va_arg(args, char*);
        input = fopen(directory, "r");
        if(input == NULL)
        {
            va_end(args);
            *valid = OPEN_FILE_ERROR;
            free(array_of_lines_and_columns);
            fclose(input);
            return NULL;
        }
        int flag_for_line = 0;
        while(!feof(input))
        {
            char c = getc(input);
            if(c == str[0])
            {
                current_line = line;
                current_column = column;
                prev_column = current_column;
                prev_line = current_line;
                int i;
                for (i = 1; i < len_of_str; ++i)
                {
                    c = getc(input);
                    if(feof(input)) break;
                    if (c != str[i])
                    {
                        if(c == '\n')
                        {

                            line++;
                            column = 1;
                        }
                        else column++;
                        break;
                    }
                    else
                    {

                        if(c == '\n')
                        {
                            flag_for_line = 1;
                            column = 1;
                            line++;
                        }
                        else column++;
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
                            free(array_of_lines_and_columns);
                            fclose(input);
                            return NULL;
                        }
                        array_of_lines_and_columns = for_realloc;
                        for(int j = capacity - len_of_pairs; j < capacity; ++j)
                        {
                            array_of_lines_and_columns[j] = (int *) malloc(sizeof(int) * 2);
                            if (array_of_lines_and_columns[j] == NULL) {
                                *valid = MEMORY_ALLOCATION_ERROR;
                                free(array_of_lines_and_columns);
                                fclose(input);
                                return NULL;
                            }
                        }
                        line = prev_line;
                        column = prev_column++;
                    }
                    array_of_lines_and_columns[len_of_pairs][0] = current_line;
                    array_of_lines_and_columns[len_of_pairs][1] = current_column;
                    len_of_pairs ++;
                    (*arr_for_files)[number_of_file]++;
                    column = column - len_of_str + (len_of_str + 1);
                    fseek(input, -len_of_str + 1, SEEK_CUR);
                    if(flag_for_line == 1)
                    {
                        line = prev_line;
                        column = prev_column;
                        flag_for_line = 0;
                    }
                }
                else {
                    fseek(input, -i + 1, SEEK_CUR);
                    if(flag_for_line == 1)
                    {
                        line = prev_line;
                        column = ++prev_column;
                        flag_for_line = 0;
                    }
                }
            }
            else if(c == '\n')
            {
                prev_line = line++;
                prev_column = column;
                flag_for_line = 1;
                column = 1;
            }
            else column++;
        }
        fclose(input);
        number_of_file++;
        line = 1;
        column = 1;
        n--;
    }
    va_end(args);
    return array_of_lines_and_columns;
}
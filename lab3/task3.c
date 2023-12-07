#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

typedef enum enum_errors
{
    WRONG_QUANTITY_OF_ARGUMENTS = 1,
    FILE_OPEN_ERROR,
    MEMORY_ALLOCATION_ERROR,
    WRONG_FIELD_OF_STRUCTURE,
    WRONG_LENGTH,
    WRONG_START,
    WRONG_FLAG,
    FINE

}status_code;
struct Employee
{
    unsigned int id;
    char* name;
    char* surname;
    double salary;
};

status_code read_from_file(FILE* input, struct Employee** array_of_employees, int* quantity);
status_code sort_and_write_in_file(FILE* output, struct Employee* array, int* quantity_of_employees, char key);
int compare_a (const void* e1, const void* e2);
int compare_d (const void* e1, const void* e2);
status_code read_field_fom_file(struct Employee*, FILE*);
status_code validation(char*);
status_code check_id(char*);
status_code check_name_surname(char*);
status_code check_salary(char*);
void free_array_of_struct(struct Employee**, int);
void free_fields_of_struct(const void*, ...);
void skip_dividers(int* c, FILE* f);
int main(int argc, char* argv[])

{
    if(argc != 4)
    {
        printf("WRONG AMOUNT OF ARGUMENTS, PLEASE, INPUT <INPUT FILE DIRECTORY> <KEY> <OUTPUT FILE DIRECTORY>\n");
        return WRONG_QUANTITY_OF_ARGUMENTS;
    }

    FILE* input = NULL;
    input = fopen(argv[1], "r");
    if(input == NULL)
    {
        perror("FILE HASN'T BEEN OPENED");
        return FILE_OPEN_ERROR;
    }

    status_code statusCode;
    int quantity_of_employees = 0;
    struct Employee* array;
    statusCode = read_from_file(input, &array, &quantity_of_employees);

    if(statusCode != FINE)
    {
        printf("SOME PROBLEMS, STATUS CODE: %d", statusCode);
        return statusCode;
    }
    fclose(input);

    statusCode = validation(argv[2]);
    if(statusCode != FINE)
    {
        printf("INVALID KEY\n");
        return statusCode;
    }

    FILE* output = NULL;
    output = fopen(argv[3], "w");
    if(output == NULL)
    {
        perror("FILE HASN'T BEEN OPENED");
        return FILE_OPEN_ERROR;
    }
    statusCode = sort_and_write_in_file(output, array, &quantity_of_employees, argv[2][1]);
    fclose(output);
    free_array_of_struct(&array, quantity_of_employees);
    return 0;
}

void skip_dividers(int* c, FILE* f)
{
    do
    {
        if(*c == EOF || isalnum(*c)) break;
        *c = fgetc(f);
    }while(*c == ' ' || *c== '\t' || *c=='\n');
    //fseek(f, -1, SEEK_CUR);
}

void free_fields_of_struct(const void* to_clear, ...)
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
void free_array_of_struct(struct Employee** array, int quantity)
{
    for(int i = 0; i <= quantity; i++)
    {
        free((*array)[i].surname);
        free((*array)[i].name);
    }
    free(*array);
}
status_code validation(char* str)
{
    if(strlen(str) != 2) return WRONG_LENGTH;
    if(str[0] !='-' && str[0] != '/') return WRONG_START;
    if(str[1] !='d' && str[1] != 'a') return WRONG_FLAG;
    return FINE;
}

status_code read_from_file(FILE* input, struct Employee** array_of_employees, int* quantity)
{
    (*array_of_employees) = (struct Employee*)malloc(sizeof(struct Employee));
    int capacity = 1;
    status_code statusCode;
    while(!feof(input))
    {
        if((*quantity) == capacity)
        {
            struct Employee* for_realloc = (struct Employee*)realloc((*array_of_employees) , sizeof(struct Employee) * capacity * 2);
            if(for_realloc == NULL)
            {
                free_array_of_struct(array_of_employees, (*quantity) - 1);
                array_of_employees = NULL;
                return MEMORY_ALLOCATION_ERROR;
            }
            capacity<<=1;
            (*array_of_employees) = for_realloc;
        }
        statusCode = read_field_fom_file( &(*array_of_employees)[*quantity], input);
        if(statusCode != FINE)
        {
            free_fields_of_struct((*array_of_employees)[*quantity].name, (*array_of_employees)[*quantity].surname, NULL);
            free_array_of_struct(array_of_employees, (*quantity) - 1);
            return statusCode;
        }
        (*quantity)++;
    }
    return FINE;
}

status_code sort_and_write_in_file(FILE* output, struct Employee* array, int* quantity_of_employees, char c)
{
    switch(c)
    {
        case 'a':
            qsort(array, *quantity_of_employees, sizeof(struct Employee), compare_a);

            for(int i = 0; i < *quantity_of_employees; ++i)
            {
                fprintf(output, "%u %s %s %lf\n", array[i].id, array[i].name, array[i].surname, array[i].salary);
            }
            return FINE;
        case 'd':
            qsort(array, *quantity_of_employees, sizeof(struct Employee), compare_d);

            for(int i = 0; i < *quantity_of_employees; ++i)
            {
                fprintf(output, "%u %s %s %lf\n", array[i].id, array[i].name, array[i].surname, array[i].salary);
            }
            return FINE;
    }
}

int compare_a(const void* employee1, const void* employee2)
{
    const struct Employee* e1 = (struct Employee*) employee1;
    const struct Employee* e2 = (struct Employee*) employee2;

    if (e1->salary > e2->salary) return 1;
    else if (e1->salary < e2->salary) return -1;
    else
    {
        int cmp_surnames = strcmp(e1->surname, e2->surname);
        if (cmp_surnames > 0) return 1;
        else if (cmp_surnames < 0) return -1;
        else
        {
            int cmp_names = strcmp(e1->name, e2->name);
            if (cmp_names > 0) return 1;
            else if (cmp_names < 0) return -1;
            else
            {
                if (e1->id > e2->id) return 1;
                else if (e1->id < e2->id) return -1;
                else return 0;
            }
        }
    }
}
int compare_d(const void* employee1, const void* employee2)
{
    const struct Employee* e1 = (struct Employee*) employee1;
    const struct Employee* e2 = (struct Employee*) employee2;

    if (e1->salary > e2->salary) return -1;
    else if (e1->salary < e2->salary) return 1;
    else
    {
        int cmp_surnames = strcmp(e1->surname, e2->surname);
        if (cmp_surnames > 0) return -1;
        else if (cmp_surnames < 0) return 1;
        else
        {
            int cmp_names = strcmp(e1->name, e2->name);
            if (cmp_names > 0) return -1;
            else if (cmp_names < 0) return 1;
            else
            {
                if (e1->id > e2->id) return -1;
                else if (e1->id < e2->id) return 1;
                else return 0;
            }
        }
    }
}
status_code check_id(char* str)
{
    int len = strlen(str);
    for(int i = 0; i < len; ++i)
    {
        if(!isdigit(str[i])) return WRONG_FIELD_OF_STRUCTURE;
    }
    return FINE;
}
status_code check_name_surname(char* str)
{
    if(str[0] == '\0') return WRONG_FIELD_OF_STRUCTURE;
    int len = strlen(str);
    for(int i = 0; i < len; ++i)
    {
        if(!isalpha(str[i])) return WRONG_FIELD_OF_STRUCTURE;
    }
    return FINE;
}
status_code check_salary(char* str)
{
    int len = strlen(str);
    int count_dot = 0;
    for (int i = 0; i < len; ++i)
    {
        if(!isdigit(str[i]))
        {
            if(str[i] == '.' && count_dot <= 1) count_dot++;
            else return WRONG_FIELD_OF_STRUCTURE;
        }
    }
    return FINE;
}

status_code read_field_fom_file(struct Employee* employee, FILE* input)
{
    int number_of_field = 1;
    int c = 0;
    while(c != '\n' && c != EOF)
    {
        skip_dividers(&c, input);
        char* str = (char*)malloc(sizeof(char));
        int len = 0, capacity = 1;

        while(c != ' ' && c!= '\t' && c != '\n' && c != EOF)
        {
            if(len == capacity)
            {
                char* for_realloc = realloc(str, 2*capacity);
                if(for_realloc == NULL)
                {
                    free(str);
                    return MEMORY_ALLOCATION_ERROR;
                }
                capacity<<=1;
            }
            str[len++] = c;
            c = fgetc(input);
        }
        str[len] = '\0';
        char* for_strtod;

        int check;
        switch (number_of_field)
        {
            case 1:
                check = check_id(str);
                if(check != FINE)
                {
                    free(str);
                    return check;
                }
                employee->id =  atoi(str);
                break;
            case 2:
                check = check_name_surname(str);
                if(check != FINE)
                {
                    free(str);
                    return MEMORY_ALLOCATION_ERROR;
                }
                employee->name =  (char*)malloc(sizeof(char) * len);
                if(employee->name == NULL)
                {
                    free(str);
                    return MEMORY_ALLOCATION_ERROR;
                }
                strcpy(employee->name, str);
                break;
            case 3:
                check = check_name_surname(str);
                if(check != FINE)
                {
                    free(str);
                    free(employee->name);
                    return MEMORY_ALLOCATION_ERROR;
                }
                employee->surname =  (char*)malloc(sizeof(char) * len);
                if(employee->surname == NULL)
                {
                    free(str);
                    free(employee->name);
                    return MEMORY_ALLOCATION_ERROR;
                }

                strcpy(employee->surname, str);
                break;
            case 4:
                check = check_salary(str);
                if(check != FINE)
                {
                    free(str);
                    free_fields_of_struct(employee->name, employee->surname, NULL);
                    return MEMORY_ALLOCATION_ERROR;
                }
                employee->salary = strtod(str, &for_strtod);
                break;
        }
        number_of_field++;
    }
    if(number_of_field!=5)
        return WRONG_FIELD_OF_STRUCTURE;
    return FINE;
}
//2 Sanyon Pozdniy 1001010101.000000
//1 Serzhanyan Popov 1001010101.000000
//3 Aboba Bebrou 10101.000000
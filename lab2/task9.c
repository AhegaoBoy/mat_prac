#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
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

char** convert_to_base(int base, int n, ...)
{
    double epsilon = 1e-3;
    char const* str_no_conv = "no finit convertion";
    char** result = (char**)malloc(sizeof(char*) * n);
    if(result == NULL) return NULL;
    va_list args;
    va_start(args, n);
    int pos_of_number = 0;
    while(n > 0)
    {
        double number = va_arg(args, double);
        int iterations = 0;
        char* converted = (char*)malloc(sizeof(char)*2);
        if(converted == NULL)
        {
            free(result);
            return NULL;
        }
        int capacity = 2;
        int len_of_number = 2;
        double fraction = number;
        converted[0] = '0';
        converted[1] = ',';
        do
        {
            if(iterations == 50)
                break;
            fraction *= base;
            if(capacity == len_of_number)
            {
                char* for_realloc = (char*) realloc(converted, capacity*2);
                capacity<<=1;
                if(for_realloc == NULL)
                {
                    clear(result, converted, NULL);
                    return NULL;
                }
                converted = for_realloc;
            }
            int digit = (int)fraction;
            if(digit < 10) converted[len_of_number++] = (char)(digit + '0');
            else converted[len_of_number++] = (digit - 10) + 'a';
            fraction = fraction - (int)fraction;
            iterations ++;
        }while(fraction >= epsilon);
        n--;
        converted[len_of_number + 1] = '\0';
        if(iterations != 50) {
            result[pos_of_number++] = converted;
        }
        else {
            result[pos_of_number] = (char*)malloc(sizeof(char) * (strlen(str_no_conv) + 1));
            if(result[pos_of_number] == NULL) {
                for(int i = 0; i<=pos_of_number; ++i)
                {
                    free(result[i]);
                }
                clear(result, converted, NULL);
                return NULL;
            }
            strcpy(result[pos_of_number],str_no_conv );
        }

    }
    return result;
}

int main()
{
    int base = 2;
    int quantity = 2;
    char** result = convert_to_base(base, quantity, 0.5, 0.3);
    if(result == NULL)
    {
        printf("MEMORY ALLOCATON ERROR\n");
        return -1;
    }
    for(int i = 0; i<quantity; ++i)
    {
        printf("NUMBER %d: %s\n",i+1, result[i]);
        free(result[i]);
    }
    free(result);
    return 0;
}

#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<ctype.h>
#define max(a, b) a > b ? a:b
typedef enum enum_errors
{
    ALLOCATION_ERROR = 1,
    NOT_IN_BASE,
    INCORRECT_SYMBOL,

}ERRORS;


void reverse(char* str, char** result);
char* sum_of_2_numbers(int base, char* str, char* str2);
int sum_of_all_numbers(char** result, int base, int n, ...);
int validations_of_symbol(char* str, int base);
int get_number(char c);


int main()
{
    int base = 36;
    char* result;
    int check = sum_of_all_numbers(&result, base,4,"-1", "z", "z","z");
    if(check != 0)
    {
        printf("SOME INCORRECT NUMBERS\n");
        return check;
    }
    printf("SUMM OF INPUT NUMBERS IN BASE %d: %s\n", base, result);
    free(result);
}

void reverse(char* string, char** result)
{
    int len = strlen(string);
    *result = (char*)malloc(sizeof(char)*(len+1));
    if(*result == NULL) return ;
    for(int i = 0; i < len; i++)
    {
        (*result)[i] = string[len-i-1];
    }
    (*result)[len] = '\0';
}

int get_number(char c)
{
    if(isdigit(c)) return (int)c - '0';
    else return (int)c - 'a' + 10;
}

char* sum_of_2_numbers(int base, char* str1, char* str2)
{
    unsigned int len1 = strlen(str1);
    unsigned int len2 = strlen(str2);
    int max_len = max(len1, len2);
    char* str1_reversed;
    reverse(str1, &str1_reversed);
    if(str1_reversed == NULL) return NULL;
    char* str2_reversed;
    reverse(str2, &str2_reversed);
    if(str2_reversed == NULL) return NULL;
    if(len1 > len2)
    {
        char* for_realloc = (char*)realloc(str2_reversed, len1);
        if(for_realloc == NULL) return NULL;
        str2_reversed = for_realloc;
        for(int i = len2; i < len1; ++i) str2_reversed[i] = '0';
    }
    else if(len2 > len1)
    {
        char* for_realloc = (char*)realloc(str1_reversed, len2);
        if(for_realloc == NULL) return NULL;
        str1_reversed = for_realloc;
        for(int i = len1; i < len2; ++i) str1_reversed[i] = '0';
    }
    int len_of_result_str = max_len + 2;
    char* result = (char*)malloc(sizeof(char) * len_of_result_str);
    if(str1_reversed == NULL || str2_reversed == NULL || result == NULL) return NULL;
    int next_place = 0;
    for(int i = 0; i < max_len; ++i)
    {
        int c;
        c = get_number(str1_reversed[i]) + get_number(str2_reversed[i]) + next_place;
        if(c >= base){
            next_place = c / base;
            if(c % base >= 10) result[i] = c % base - 10 + 'a';
            else result[i] = c % base + '0';
        }
        else{
            next_place = 0;
            if(c % base >= 10) result[i] = c % base - 10 + 'a';
            else result[i] = c % base + '0';
        }

    }
    if(next_place == 0) result[len_of_result_str - 1] = '\0';
    else
    {
        if(next_place >= 10) result[len_of_result_str - 2] = next_place - 10 + 'a';
        else result[len_of_result_str - 2] = next_place + '0';
        result[len_of_result_str - 1] = '\0';
    }
    reverse(result, &result);
    free(str1_reversed);
    free(str2_reversed);
    return result;
}

int sum_of_all_numbers(char** result, int base, int n,...)
{

    va_list args;
    *result = (char*)malloc(sizeof(char)*2);
    if((*result) == NULL) return ALLOCATION_ERROR;
    (*result)[0] = '0';
    (*result)[1] = '\0';
    va_start(args, n);
    while(n > 0)
    {
        char* current_number = va_arg(args, char*);
        int check = validations_of_symbol(current_number, base);
        if(check != 0) return check;
        *result = sum_of_2_numbers(base, *result, current_number);
        if(*result == NULL) return ALLOCATION_ERROR;
        n--;
    }
    va_end(args);
    return 0;
}

int validations_of_symbol(char* str, int base)
{
    int i = 0;
    for(i; i<strlen(str); ++i)
    {
        if(isdigit(str[i]))
        {
            if(str[i] - '0' >= base) return NOT_IN_BASE;
        }
        else
        {
            if(str[i] >= 'a' && str[i] <= 'z')
            {
                if(str[i] - 'a' + 10 >= base) return NOT_IN_BASE;
            }
            else
            {
                return INCORRECT_SYMBOL;
            }
        }
    }
    return 0;
}
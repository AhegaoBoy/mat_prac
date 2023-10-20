#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<ctype.h>
#define max(a, b) a > b ? a:b
typedef enum errors
{
    WRONG_BASE,
    ALLOCATION_ERROR
}ERRORS;

void reverse(char* str, char** result);
char* sum_of_2_numbers(int base, char* str, char* str2);
char* sum_of_all_numbers(int base, int n, ...);

int main()
{
    int base = 16;
    if(base < 2 || base > 36)
    {
        printf("INCORRECT BASE\n");
        return WRONG_BASE;
    }
    char* result = sum_of_all_numbers(base, 4,"a", "a", "a","a");
    if(result == NULL)
    {
        printf("MEMORY ALLOCATION ERROR\n");
        free(result);
        return ALLOCATION_ERROR;
    }
    printf("SUMM OF INPUT NUMBERS IN BASE %d: %s\n", base, result);
    free(result);
}

void reverse(char* string, char** result)
{
    int len = strlen(string);
    *result = (char*)malloc(sizeof(char)*(len+1));
    for(int i = 0; i < len; i++)
    {
        (*result)[i] = string[len-i-1];
    }
    (*result)[len] = '\0';
}


char* sum_of_2_numbers(int base, char* str1, char* str2)
{
    unsigned int len1 = strlen(str1);
    unsigned int len2 = strlen(str2);
    int max_len = max(len1, len2);
    char* str1_reversed;
    reverse(str1, &str1_reversed);
    char* str2_reversed;
    reverse(str2, &str2_reversed);
    if(len1 > len2)
    {
        str2_reversed = (char*)realloc(str2_reversed, len1);
        for(int i = len2; i < len1; ++i) str2_reversed[i] = '0';
    }
    else if(len2 > len1)
    {
        str1_reversed = (char*)realloc(str1_reversed, len2);
        for(int i = len1; i < len2; ++i) str1_reversed[i] = '0';
    }
    int len_of_result_str = max_len + 2;
    char* result = (char*)malloc(sizeof(char) * len_of_result_str);
    if(str1_reversed == NULL || str2_reversed == NULL || result == NULL) return NULL;
    int next_place = 0;
    for(int i = 0; i < max_len; ++i)
    {
        int c;
        if(isalpha(str1_reversed[i]) && isalpha(str2_reversed[i]))
        {
            c = (str1_reversed[i] - 'a' + 10) + (str2_reversed[i] - 'a' + 10) + next_place;
            if(c >= base)
            {
                next_place = c / base;
                if(c % base >= 10) result[i] = c % base + 'a';
                else result[i] = c % base + '0';
            }
            else
            {
                next_place = 0;
                if(c % base >= 10) result[i] = c % base + 'a';
                else result[i] = c % base + '0';
            }
        }
        else if(isalpha(str1_reversed[i]) && isdigit(str2_reversed[i]))
        {
            c = (str1_reversed[i] - 'a' + 10) + (str2_reversed[i] - '0') + next_place;
            if(c >= base)
            {
                next_place = c / base;
                if(c % base >= 10) result[i] = c % base + 'a';
                else result[i] = c % base + '0';
            }
            else
            {
                next_place = 0;
                if(c % base >= 10) result[i] = c % base + 'a';
                else result[i] = c % base + '0';
            }
        }
        else if(isdigit(str1_reversed[i]) && isalpha(str2_reversed[i]))
        {
            c = (str1_reversed[i] - '0') + (str2_reversed[i] - 'a' + 10) + next_place;
            if(c >= base)
            {
                next_place = c / base;
                if(c % base >= 10) result[i] = c % base + 'a';
                else result[i] = c % base + '0';
            }
            else
            {
                next_place = 0;
                if(c % base >= 10) result[i] = (c % base - 10) + 'a';
                else result[i] = c % base + '0';
            }
        }
        else if(isdigit(str1_reversed[i]) && isdigit(str2_reversed[i]))
        {
            c = (str1_reversed[i] - '0') + (str2_reversed[i] - '0') + next_place;
            if(c >= base)
            {
                next_place = c / base;
                if(c % base >= 10) result[i] = c % base + 'a';
                else result[i] = c % base + '0';
            }
            else
            {
                next_place = 0;
                if(c % base >= 10) result[i] = c % base + 'a';
                else result[i] = c % base + '0';
            }
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
    return result;
}

char* sum_of_all_numbers(int base, int n,...)
{
    va_list args;
    char* result = (char*)malloc(sizeof(char)*2);
    if(result == NULL) return NULL;
    result[0] = '0';
    result[1] = '\0';
    va_start(args, n);
    while(n > 0)
    {
        char* current_number = va_arg(args, char*);
        result = sum_of_2_numbers(base, result, current_number);
        n--;
    }
    va_end(args);
    return result;
}
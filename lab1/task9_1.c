#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum  enum_number
{
    OVERFLOW,
    NOT_A_NUMBER, 
    OK
}Number_Errors;

void removeLeadingZeros(char* str[]) {
    int i,j;
    if(str[0][0] == '-')
    {
        i = 1; 
        j = 1;
    }
    else 
    {
        i = 0;
        j = 0;
    }
    while (str[0][i] == '0') {
        i++;
    }
    while (str[0][i] != '\0') {
        str[0][j++] = str[0][i++];
    }
    str[0][j] = '\0';
}

int string_compare(char* str1, char* str2)
{
    int n1 = strlen(str1);
    int n2 = strlen(str2);
    if(n1 < n2) return 1;
    if(n1 > n2) return -1;
    else
    {
        for(int i = 0; i<n1; ++i)
        {
            if(str1[i] > str2[i]) return -1;
            else if(str1[i] < str2[i]) return 1;
        }
    return 0;
    }
}

int number_check(char* str[], int n)
{
    char max_value[] = "2147483647";
    if(str[n][0]>='0' && str[n][0] <='9' || str[n][0] == '-')
    {
            for(int i = 1; i<strlen(str[n]);++i)
            {
            if(str[n][i] >= '0' && str[n][i] <='9') continue;
            else return NOT_A_NUMBER;
            }
        
        if(str[n][0] == '-')
        {
            removeLeadingZeros(&str[n]);
            int compare = string_compare(*str, max_value);
            switch(compare)
            {
                case 1:
                    return OK;
                    break;
                default:
                    return OVERFLOW;
                    break;
            }
        }
        else
        {
            removeLeadingZeros(&str[n]);
            int compare = string_compare(*(str + n), max_value);
            switch(compare)
            {
                case 1: 
                    return OK;
                    break;
                case 0:
                    return OK;
                    break;
                default:
                    return OVERFLOW;
                    break;
            }
        }
    }
    else return NOT_A_NUMBER;
}
 
int main (int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Введите диапазон для генерации случайных чисел\n");
        return 0;
    }
    if(number_check(argv, 1) !=2 || number_check(&argv[0], 2) != 2)
    {
        if(number_check(&argv[0], 1) == 0 || number_check(&argv[0], 2) ==0)
        {
            printf("Переполнение числа целого типа\n");
            return 0;
        }
        else if(number_check(&argv[0], 1) == 1 || number_check(&argv[0], 2) ==1)
        {
            printf("Числа не должны содержать других символов, кроме -, для определения знака\n");
            return 0;
        }
    }
    srand(time(NULL));
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    if(b < a)
    {
        printf("Второе число должно быть больше первого\n");
        return 0;
    }
    const int n = 250;
    int array[n];
    for(int i = 0; i<n; ++i) 
    {
        array[i] = a + rand()%(b - a + 1);
        printf("%d ", array[i]);
    }
    printf("\n");
    int max_pos = 0;
    int min_pos = 0;
    for(int i = 1; i<n; ++i)
    {
        if(array[i] < array[min_pos]) min_pos = i;
        else if(array[i] > array[max_pos]) max_pos = i;
    }
    array[max_pos] = array[max_pos] ^ array[min_pos];
    array[min_pos] = array[min_pos] ^ array[max_pos];
    array[max_pos] = array[max_pos] ^ array[min_pos];
    for(int i = 0; i<n; ++i) printf("%d ", array[i]);
    printf("\n");    
}
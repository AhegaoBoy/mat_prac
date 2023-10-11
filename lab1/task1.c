#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef enum enum_errors
{
    ARGUMENTS_QUANTITY,
    WRONG_ORDER,
    NUMBER_OVERFLOW,
    NUMBER_IS_A_STRING,
    KEY_START,
    KEY_SYMBOL,
    ACCEPTABLE
}Errors;

typedef enum enum_simple
{
    NIETHER_NOR,
    TRUE,
    FALSE
}Simple_check;

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

int number_check(char* str[])
{

    char max_value[] = "2147483647";
    if(str[0][0]>='0' && str[0][0] <='9' || str[0][0] == '-')
    {
            for(int i = 1; i<strlen(str[0]);++i)
            {
            if(str[0][i] >= '0' && str[0][i] <='9') continue;
            else return NOT_A_NUMBER;
            }
        
        if(str[0][0] == '-')
        {
            removeLeadingZeros(str);
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
            removeLeadingZeros(str);
            int compare = string_compare(*str, max_value);
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

int key(int argc, char* argv[]) {
    if (argc < 3) return ARGUMENTS_QUANTITY;
    int check = number_check(&argv[2]);
    if(strlen(argv[1]) == 2 && ((argv[1][0] == '-' || argv[1][0]) && (argv[1][1] < '0' || argv[1][1] > '9')) && check == 2) return WRONG_ORDER;
    else 
    {
        int checker = number_check(&argv[1]);
        switch(checker)
        {
            case 0:
                return OVERFLOW;
                break;
            case 1:
                return NUMBER_IS_A_STRING;
            case 2:
                if (argv[2][0] == '-' || argv[2][0] == '/')
                {
                if (argv[2][1] != 'h' && argv[2][1] != 'p' && argv[2][1] != 's' && argv[2][1] != 'e' && argv[2][1] != 'a' && argv[2][1] != 'f') return KEY_SYMBOL;
                else return ACCEPTABLE;
                }
                else return KEY_START;
                break;
        }
    }
}

unsigned long long factorial(int n) {
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

int* multiple(int n, int** numbers) {
        int tmp = n;
        *numbers= (int*)malloc(sizeof(int) * (100 / n ));
        if(*numbers == NULL) return NULL;
        else{
        for (int i = 0; i < 100 / n; ++i) {
            int* p = (*numbers) + i;
            *p = tmp;
            tmp += n;
        }
        }
        return *numbers;
}

int simple(int n)
{
    if(n == 1) return NIETHER_NOR;
    else
    {
        if(n%2 == 0) return FALSE;
        else
        {
        for(int del = 3; del*del<=n; del+= 2)
            if(n%del == 0)
                return FALSE;
        }
    }
    return TRUE;
}

long long summ(int n)
{
    return ((long long)n*n+n)/2;
}

void print_spaces(int n)
{
    for(int i = 0; i<n; ++i) printf(" ");
}

void print_tabl(int exponent)
{
    for(int i = 1; i<11; ++i)
    {
        long long res = 1;
        for(int j = 1; j<=exponent; ++j)
        {
            res *= i;
            printf("%lli ", res);
        }
        printf("\n");
    }
}

void separate(char* str[])
{
    for(int i = 0; i<strlen(*str); ++i)  printf("%c ", str[0][i]);
    printf("\n");
}
int main(int argc, char* argv[]) 
{
    if(argc < 2)
    {
        printf("Введите ключ и число\n");
        return 0;
    }
    int simple_check;
    int n = atoi(argv[1]);
    unsigned long long ans;
    int* res;
    int reversed;
    int digit;
    long long summa;
    char* separated;
    long long int** tabl;

    switch (key(argc, argv)) {
        case 0:
            printf("Слишком мало аргументов\n");
            return 0;
            break;
        case 1:
            printf("Последовательность аргументов: число ключ\n");
            return 0;
            break;
        case 2:
            printf("Переполнение числа целого типа\n");
            return 0;
        case 3:
            printf("Введенное число не должно содержать символов, кроме '-' для определения знака\n");
            return 0;
        case 4:
            printf("Ключ может начинаться с символов - или /\n");
            return 0;
        case 5:
            printf("Возможные ключи для ввода:\n -h - вывод в консоль кратных натуральных чисел до 100\n -p - определение на простое/составное число\n -s - сплит введенного числа\n -e - таблица степеней по основаниям 1-10 от 1 до введенного\n -a - сумма натуральных чисел от 1 до введенного\n -f - факториал введенного числа\n");
            return 0;
        case 6:
            if (argv[2][1] == 'f') {
                if(n < 0) {printf("Введите неотрицательное число\n"); return 0;}
                if(n > 12) {printf("Введите число, небольше 12, иначе будет переполнение целого типа\n"); return 0 ;}
                ans = factorial(n);
                printf("Факториал числа %d равен: %llu\n",n, ans);
            }
            else if(argv[2][1] == 'h')
            {
                if(n <= 0) {printf("Число должно быть натуральным\n"); return 0;}
                res = multiple(n, &res);
                if(res == NULL)
                {
                    printf("Введенное число не должно превышать 100\n");
                    return 0;
                }
                else for(int i = 0; i<100/n; ++i) printf("%d ", *(res + i));
                free(res);               
            }
            else if(argv[2][1] == 'p')
            {
                simple_check = simple(n);
                if(n < 1)
                {
                    printf("Введите натуральное число\n");
                    return 0;
                }
                switch(simple_check)
                {
                    case 0:
                        printf("Вы ввели единицу\n");
                        break;
                    case 1:
                        printf("Введенное число является простым\n");
                        break;
                    default:
                        printf("Введенное число является составным\n");
                        break;
                }
            }
            else if(argv[2][1] == 's')
            {
                if(n >= 0)
                {
                    printf("Разделенное число:\n");
                    separate(&argv[1]);
                }
                else
                {
                    printf("Разделенное число:\n");
                    separate(&argv[1]);
                }
            }
            else if(argv[2][1] == 'a')
            {
                if(n > 65535) printf("Введите число, меньшее 65535, иначе при вычислении суммы будет переполнение\n");
                else{
                summa = summ(n);
                printf("Сумма чисел от 1 до введенного равна: %lld\n", summa);}
                }
            else if(argv[2][1] == 'e')
            {
                if(n < 1 || n > 10){
                printf("введите число от 1 до 10\n");
                return 0;
                }
                print_tabl(n);
            }
    }
return 0;
}
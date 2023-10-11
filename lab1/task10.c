#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

typedef enum enum_errors
{
    MALLOC_ERROR = 1,
    REALLOC_ERROR,
    NOT_IN_BASE,
    INCORRECT_SYMBOL,
    WRONG_BASE,
    FINE
}ERRORS;

int convert_to_10(char* str, int base);
int get_length_of_number_in_base(int n, int base);
char* convert_to_base(int n, int base);
void removeLeadingZeros(char* str);
int get_pow(int n, int pow);
int validations_of_symbol(char* c, int base);
char* get_string(int *len);
int input_wrong_number(char*** list, int quantity, char* str);
void Remove_dividers(char* str);

int main()
{

    int max = 0;
    int converted_to_10;
    char* numbers;
    char* result;
    int base;


    scanf("%d", &base);
    if(base < 2 || base > 36)
    {
        printf("WRONG BASE\n");
        return WRONG_BASE;
    }

    int len;
    numbers = get_string(&len);
    Remove_dividers(numbers);
    if(strcmp(numbers, "Stop") == 0) return 1;

    char** not_in_base_numbers;
    not_in_base_numbers = (char*)malloc(sizeof(char*));
    if(not_in_base_numbers == NULL) return MALLOC_ERROR;
    int size = 1;
    while(strcmp(numbers, "Stop") != 0)
    {
        removeLeadingZeros(numbers);
        if(validations_of_symbol(numbers, base) != 0)
        {
            int check = input_wrong_number(&not_in_base_numbers, size, numbers);
            if(check == REALLOC_ERROR) return REALLOC_ERROR;
            size++;
        }
        else {
            converted_to_10 = convert_to_10(numbers, 10);
            if (converted_to_10 > max) {
                max = converted_to_10;
                result = numbers;
            }
        }
        numbers = get_string(&len);
        Remove_dividers(numbers);
    }
    if(size == 1) printf("ALL NUMBERS WERE INPUTED CORRECTLY\n");
    else
    {
        printf("THERE'RE SOME BADLY INPUTED NUMBERS YOU SHOULD SEE: ");
        for (int i = 0; i < size-1; ++i) {
            printf("%s ",not_in_base_numbers[i]);
        }
    }

    printf("\nЧисло в десятичной сс: %d\n",max);
    printf("Найденное максимальное число: %s\n", result);
    printf("Строковое представление с основанием сс 9: %s\n", convert_to_base(max, 9));
    printf("Строковое представление с основанием сс 18: %s\n", convert_to_base(max, 18));
    printf("Строковое представление с основанием сс 27: %s\n", convert_to_base(max, 27));
    printf("Строковое представление с основанием сс 36: %s\n", convert_to_base(max, 36));
    return 0;

}

void Remove_dividers(char* str)
{
    int i = 0,j = 0;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;
    }
    while (str[i] != '\0') {
        str[j++] = str[i++];
    }
    str[j] = '\0';
}

char *get_string(int *len)
{
    *len = 0;
    int capacity = 1;
    char *s = (char*) malloc(sizeof(char));

    char c = getchar();

    do{
        s[(*len)++] = c;

        if (*len >= capacity) {
            capacity *= 2;
            s = (char*) realloc(s, capacity * sizeof(char));
        }

        c = getchar();
    }while (c != '\n' && c!='\0' && c!=' ' && c!='\t');

    s[*len] = '\0';

    return s;
}

int input_wrong_number(char*** list, int quantity, char* str)
{
    quantity += 1;
    char** for_realloc= (char**)realloc(*list,quantity*sizeof(char*));
    if(for_realloc == NULL) return REALLOC_ERROR;
    *list = for_realloc;
    (*list)[quantity - 2] = str;
    (*list)[quantity - 2] = str;
    return FINE;
}

int get_length_of_number_in_base(int n, int base)
{
    return (int)(log(n)/log(base)) + 1;
}

void removeLeadingZeros(char* str) {
    int i,j;
    if(str[0] == '-')
    {
        i = 1; 
        j = 1;
    }
    else 
    {
        i = 0;
        j = 0;
    }
    while (str[i] == '0') {
        i++;
    }
    while (str[i] != '\0') {
        str[j++] = str[i++];
    }
    str[j] = '\0';
}


char* convert_to_base(int n, int base)
{
    int length = get_length_of_number_in_base(n, base);
    char* result = (char*)malloc(sizeof(char) * (length + 1));
    if(result == NULL) return NULL;
    result[length] = '\0';
    if(n < base)
    {
       if(n >= 10)   result[0] = 'a' + (n - 10);
       else result[0] = '0' + n;
    }
    while(n >= base)
    {
        if(n % base < 10) 
        {
            result[--length] = '0' + n % base;
        }
        else 
        {
            result[--length] = 'a' + (n%base - 10);
        }
        n /= base;
        if(n < base)
        {
            if(n >= 10)   result[0] = 'a' + (n - 10);
            else result[0] = '0' + n;
        }
    }
    return result;
}

int get_pow(int n, int pow)
{
    if(pow == 0) return 1;
    int tmp = 1;
    for(int i = 1; i<=pow; ++i) tmp *= n;
    return tmp;
}

int convert_to_10(char* str, int base)
{
    int length = strlen(str);
    int pow = -1;
    int result = 87;
    for(int i = length; i>=0; i--)
    {
        if(isdigit(str[i]))
        {
            int tmp = (int)(str[i] - '0');
            result += tmp * (get_pow(base, pow++));
        }
        else
        {
            result += ((int)(str[i] - 'a') + 10)*get_pow(base, pow++);
        }
    }
    return result;
}

int validations_of_symbol(char* str, int base)
{
    for(int i = 0; i<strlen(str); ++i)
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
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>
#include<math.h>
typedef enum enum_errors
{
    WRONG_BASE = 1,
    MEMORY_ALLOCATION_ERROR,
    FINE
}status_code;

status_code convert_to_base(char** str, unsigned int n, int pow);
void removeLeadingZeros(char** str);

int main()
{
    char str1[10] = "qwerty\0";
    char str2[10] = "qwerta\0";
    printf("%d\n", strcmp(str1, str2));
    unsigned int number;
    double epsilon = 1e-2;
    printf("INSERT NUMBER :");
    scanf("%ud", &number);

    int base;
    printf("INSERT THE BASE YOU WANT TO CONVERT\n");
    scanf("%d", &base);
    if(((log2(base) - (int)(log2(base)) > epsilon)))
    {
        printf("YOU CAN INPUT ONLY POWER OF 2 BASE IN RANGE [2 ; 32]");
        return WRONG_BASE;
    }
    int power = (int)(log2(base));
    char* result;
    status_code check = convert_to_base(&result,number, power);
    if(check != FINE)
    {
        printf("MEMORY ALLOCATION ERROR\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    removeLeadingZeros(&result);
    printf("RESULT: %s\n", result);
    free(result);
    return 0;
}

status_code convert_to_base(char** result, unsigned int n, int pow)
{
    int length_of_result = sizeof(unsigned int) * 8 / pow + 2;
    (*result) = (char*)malloc(sizeof(char) * length_of_result);
    if(result == NULL) return MEMORY_ALLOCATION_ERROR;
    (*result)[length_of_result] = '\0';
    int rang_of_group = 1<<pow;
    for(int i = length_of_result - 1; i >=0; i--)
    {
        int group = n & 1;
        for(int j = 2; j < rang_of_group; j<<=1)
            group |= n & j;
        if(group < 10) (*result)[i] = group + '0';
        else (*result)[i] = group - 10 + 'a';
        n >>= pow;
    }
    return FINE;
}
void removeLeadingZeros(char** str)
{
    int i = 0,j = 0;
    while ((*str)[i] == '0') {
        i++;
    }
    while ((*str)[i] != '\0') {
        (*str)[j++] = (*str)[i++];
    }
    (*str)[j] = '\0';
}
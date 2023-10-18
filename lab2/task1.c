#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdarg.h>
#include<ctype.h>

typedef enum key_errors
{
    ARGUMENTS_QUANTITY = 1,
    WRONG_LENGTH,
    WRONG_START,
    WRONG_FLAG,
    FINE,
    MEMORY_ALLOCATION_ERROR
}ERRORS;

typedef enum  enum_number
{
    OVERFLOW,
    NOT_A_NUMBER,
    NEGATIVE_DIGIT,
    OK
}Number_Errors;

int get_len(char* str);
char* reverse(char* str);
char* up_honest(char* str);
char* sort(char* str);
char* concatenate(int n_seed, int n, char** str);
void get_concatenated(char** str1, char* str2, int len_str1, int* pos);
int validation_of_key(char* str);
int compare(void const* elem1, void const* elem2);
void removeLeadingZeros(char* str);
int string_compare(char* str1, char* str2);
int number_check(char* str);

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("INVALID QUANTITY OF ARGUMENTS\n");
        return ARGUMENTS_QUANTITY;
    }
    int key = validation_of_key(argv[1]);
    switch (key)
    {
        case WRONG_FLAG:
            printf("YOU MAY INPUT ONLY 'l' 'r' 'u' 'n' 'c'\n");
            return WRONG_FLAG;
        case WRONG_START:
            printf("SYMBOLS OF START SYMBOLS ARE '/' '-'\n");
            return WRONG_START;
        case WRONG_LENGTH:
            printf("INVALID KEY\n");
            return WRONG_LENGTH;
    }

    srand(time(NULL));
    int length;
    char* reversed;
    char* upped_honest;
    char* sorted;
    char* concatenate_strings;
    char* for_swap;
    int number_for_seed;
    int check_numbers;
    switch (argv[1][1])
    {
        case 'l':
            length = get_len(argv[2]);
            printf("LENGTH OF STRING %s  - %d\n", argv[2], length);
            return 0;
        case 'r':
            reversed = reverse(argv[2]);
            if(reversed == NULL)
            {
                free(reversed);
                printf("MEMORY ALLOCATION ERROR\n");
                return MEMORY_ALLOCATION_ERROR;
            }
            printf("REVERSED STRING\n%s", reversed);
            free(reversed);
            return 0;
        case 'u':
            upped_honest = up_honest(argv[2]);
            if(upped_honest == NULL)
            {
                free(upped_honest);
                printf("MEMORY ALLOCATION ERROR\n");
                return MEMORY_ALLOCATION_ERROR;
            }
            printf("NEW STRING WITH UPPED LETTERS ON HONEST POSITIONS\n%s", upped_honest);
            free(upped_honest);
            return 0;
        case 'n':
            sorted = sort(argv[2]);
            if(sorted == NULL)
            {
                free(sorted);
                printf("MEMORY ALLOCATION ERROR\n");
                return MEMORY_ALLOCATION_ERROR;
            }
            printf("NEW SORTED STRING\n%s", sorted);
            free(sorted);
            return 0;
        case 'c':
            check_numbers = number_check(argv[3]);
            if(check_numbers != OK)
            {
                printf("INVALID INPUT OF SEED\n");
                return check_numbers;
            }
            number_for_seed = atoi(argv[3]);
            for_swap = argv[3];
            argv[3] = argv[2];
            argv[2] = argv[3];

            concatenate_strings = concatenate(number_for_seed, argc - 3,argv + 3);
            if(concatenate_strings == NULL)
            {
                printf("MEMORY ALLOCATION ERROR\n");
                free(concatenate_strings);
                return MEMORY_ALLOCATION_ERROR;
            }
            printf("CONCATENATED STRINGS FROM COMMAND LINE IN PSEUDO - RANDOM SEQUENCE:\n%s\n", concatenate_strings);
            free(concatenate_strings);
            return 0;
    }
}
int validation_of_key(char* str)
{
    if(get_len(str) != 2) return WRONG_LENGTH;
    if(str[0] !='-' && str[0] != '/') return WRONG_START;
    if(str[1] !='r' && str[1] != 'l' && str[1] != 'u' && str[1] != 'n' && str[1] != 'c') return WRONG_FLAG;
    return FINE;
}

int number_check(char* str)
{
    int len_str = get_len(str);
    char max_value[] = "42949667294";
    if(str[0] == '-') return NEGATIVE_DIGIT;
    if(str[0]>='0' && str[0] <='9')
    {
        for(int i = 1; i<len_str;++i)
        {
            if(str[i] >= '0' && str[i] <='9') continue;
            else return NOT_A_NUMBER;
        }

        if(str[0] == '-')
        {
            removeLeadingZeros(str);
            int compare = string_compare(str, max_value);
            switch(compare)
            {
                case 1:
                    return OK;
                default:
                    return OVERFLOW;
            }
        }
        else
        {
            removeLeadingZeros(str);
            int compare = string_compare(str, max_value);
            switch(compare)
            {
                case 1:
                    return OK;
                case 0:
                    return OK;
                default:
                    return OVERFLOW;
            }
        }
    }
    else return NOT_A_NUMBER;
}

int string_compare(char* str1, char* str2)
{
    int n1 = get_len(str1);
    int n2 = get_len(str2);
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

void removeLeadingZeros(char* str)
{
    int i = 0,j = 0;
    while (str[i] == '0') {
        i++;
    }
    while (str[i] != '\0') {
        str[j++] = str[i++];
    }
    str[j] = '\0';
}

int get_len(char* str)
{
    int len = 0;
    while(str[len] != '\0') len++;
    return len;
}
char* reverse(char* str)
{
    int len = get_len(str);
    char* new = (char*)malloc(sizeof(char) * len);
    if(new == NULL) return NULL;
    for(int i = 0; i <= len/2; ++i)
        new[i] = str[len - i];
    return new;
}
char* up_honest(char* str)
{
    int len = get_len(str);
    char* new = (char*)malloc(sizeof(char) * len);
    if(new == NULL) return NULL;
    for(int i = 0; i<len; ++i)
    {
        if(i%2 == 0) new[i] = toupper(str[i]);
        else new[i] = str[i];
    }
    return new;
}

int compare(void const* elem1, void const* elem2)
{
    char const c1 = *( (char const*) elem1);
    char const c2 = *( (char const*) elem2);
    if(isalnum(c1) && isalnum(c2))
    {
        if(isdigit(c1) && isalpha(c2)) return 1;
        if(isdigit(c2) && isalpha(c1)) return -1;
        if(isdigit(c2) && isdigit(c1) || isalpha(c2) && isalpha(c1)) return 0;
    }
    else
    {
        return 0;
    }
}

char* sort (char* str)
{
    int len = get_len(str);
    char* new = (char*)malloc(sizeof(char) * len);
    if(new == NULL) NULL;
    for(int i = 0; i<len; ++i) new[i] = str[i];
    qsort(new, sizeof(char), len, compare);
    return new;
}

void get_concatenated(char** str1, char* str2, int len1, int* pos)
{
    int len2 = get_len(str2);
    for(int i = 0; i < len2; ++i)
    {
        (*str1)[i + *pos] = str2[i];
    }
    *pos += len2;
}

char* concatenate(int n_seed, int n, char** str)
{
    srand(n_seed);
    int len_of_all_str = 0;
    for(int i = 0; i < n; ++i)
        len_of_all_str += get_len(str[i]);
    char* concatenated = (char*)malloc(sizeof(char) * len_of_all_str);
    if(concatenated == NULL) return NULL;
    int pos = 0;
    concatenated[len_of_all_str] = '\0';
    while(n > 0)
    {
        int index = rand() % n;
        get_concatenated(&concatenated, str[index], len_of_all_str, &pos);
        str[index] = str[n-1];
        n--;
    }
    return concatenated;
}
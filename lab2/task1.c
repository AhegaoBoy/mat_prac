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
int get_len(char* str);
char* reverse(char* str);
char* up_honest(char* str);
char* sort(char* str);
char* concatenate(char* str[], int pos,...);
char* get_concatenated(char* str1, char* str2);
int validation_of_key(char* str);
int compare(void const* elem1, void const* elem2);

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
    }
}
int validation_of_key(char* str)
{
    if(get_len(str) != 2) return WRONG_LENGTH;
    if(str[0] !='-' && str[0] != '/') return WRONG_START;
    if(str[1] !='r' && str[1] != 'l' && str[1] != 'u' && str[1] != 'n' && str[1] != 'c') return WRONG_FLAG;
    return FINE;
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

char* get_concatenated(char* str1, char* str2)
{
    int len1 = get_len(str1);
    int len2 = get_len(str2);
    char* concatenated = (char*)malloc(sizeof(char) * (len1 + len2));
    if(concatenated == NULL) return NULL;
    for(int i = 0; i < len1 + len2; ++i)
    {
        if(i<len1) concatenated[i] = str1[i];
        else concatenated[i] = str2[i - len1];
    }
    return concatenated;
}

char* concatenate(char* str[], int pos,...)
{
    va_list strings;
    char* con = (char*)malloc(sizeof(char));
    if(con == NULL) return NULL;
    va_arg(strings, &(str[pos]));
    while(strings != NULL)
    {

    }
}
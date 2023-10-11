#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
typedef enum enum_error
{
    MALLOC_ERROR,
    REALLOC_ERROR,
    WRONG_BASE,
    DONE
}ERRORS;

void go_to_divider(int* c,FILE* f);
int convert_to_10(char* str, int base);
int get_pow(int n, int pow);
int task9(FILE* input, FILE* output);
int get_base(char** str);
int validation_of_symbol(char* str);

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Введите корректное колчисество аргументов\n");
        return 0;
    }
    char* input_dir = argv[1];
    char* output_dir = argv[2];
    FILE* input;
    FILE* output;

    input = fopen(input_dir, "r");
    output = fopen(output_dir, "w");

    if(input == NULL || output == NULL)
    {
        perror("Ошибка в открытии файла");
        return 0;
    }
     if(!task9(input, output))
     {
         printf("Ошибка при считывании или записи данных\n");
         return 0;
     }
    printf("Содержимое файла %s\nБыло переписано по условию задания в файл, с директорией %s\n", input_dir, output_dir);
    fclose(input);
    fclose(output);
    return 0;

}

void go_to_divider(int* c, FILE* f)
{
    do
    {   
        if(*c == EOF || isalnum((*c))) break;
        *c =fgetc(f);
    } while (*c!=' ' && *c!= '\t' && *c!='\t');
    fseek(f, -1, SEEK_CUR);
}

int task9(FILE* input, FILE* output)
{
    int c;
    do
    {
        c = fgetc(input);
        if(isalnum(c))
        {
            char* number = (char*)malloc(sizeof(char) * 1);
            if(number == NULL) return MALLOC_ERROR;
            int len = 1;
            number[0] = c;
            do
            {
                c = fgetc(input);
                if(c == EOF) break;
                number = (char*)realloc(number, ++len);
                if(number == NULL) return REALLOC_ERROR;
                number[len - 1] = c;

            } while (c != ' ' && c != '\t' && c != '\n' && c!=EOF);

            number[strlen(number)] = '\0';
            if(validation_of_symbol(number) != 0)
            {
                fprintf(output,"То, что вы ввели, не является числом\n");
                continue;
            }
            int base = get_base(&number);
            int number_in_10 = convert_to_10(number, base);

            if(fprintf(output, "Входное число без ведущих нулей: %s\t Минимальное основание СС для этого числа: %d\t Представление этого числа в 10тичной СС: %d\n", number,base, number_in_10) == EOF) return 0;
        }
        else
        {
            go_to_divider(&c, input);
        }
    } while (c != EOF);
    return DONE;
}

int get_base(char** str)
{
    char max_char = (*str)[0];
    for(int i = 0; i < strlen(*str); ++i)
    {
        if((*str)[i] > max_char) max_char = (*str)[i];
    }
    if(isdigit(max_char))
    {
        int tmp = max_char;
        return tmp - '0' + 1;
    }
    else if(isalpha(max_char))
    {
        return (int)max_char - 'a' + 11;
    }
    else return WRONG_BASE;
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

int validation_of_symbol(char* str)
{
    for(int i = 0; i<strlen(str); ++i)
    {
        if(!isalnum(str[i])) return 1;
    }
    return 0;
}
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>

typedef enum enum_errors
{
    WRONG_LENGTH=1,
    WRONG_START,
    WRONG_FLAG,
    FINE,
    MALLOC_ERROR,
    UNDONE,
    ACCESS_FILE_ERROR,
    WRONG_QUANTITY_OF_ARGUMENTS
}ERRORS;
int validation(char** str);
int write_from_2_files(FILE* f1, FILE* f2, FILE* output);
void skip_dividers(int* c, FILE* f);
int key_a(FILE* input, FILE* output);
void go_to_divider(int* c,FILE* f);


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Input the key\n");
        return WRONG_QUANTITY_OF_ARGUMENTS;
    }
    char* key = argv[1];
    int check_key = validation(&key);
    switch(check_key)
    {
        case 1:
            printf("Введите ключи '-a' '/a' '-r' '/r'\n");
            return WRONG_LENGTH;
        case 2:
            printf("Символы начала ключа: - и /\n");
            return WRONG_START;
        case 3:
            printf("Доступны только a and r\n");
            return WRONG_FLAG;
    }
    char* dir1;
    char* dir2;
    char* output;
    FILE* file1;
    FILE* file2;
    FILE* out_file;

    switch(key[1])
    {
        case 'r':
            if(argc != 5)
            {
                printf("Wrong quantity of arguments, input <FILE1> <FILE2> <OUT_FILE>\n");
                return WRONG_QUANTITY_OF_ARGUMENTS;
            }
            dir1 = argv[2];
            dir2 = argv[3];
            output = argv[4];
            file1 = fopen(dir1, "r");
            file2 = fopen(dir2, "r");
            out_file = fopen(output, "w");
            if(file1 == NULL)
            {
                perror("File 1 access failed");
                return ACCESS_FILE_ERROR;
            }

            if(file2 == NULL)
            {
                perror("File 2 access failed");
                return ACCESS_FILE_ERROR;
            }

            if(out_file == NULL)
            {
                perror("Output file access failed");
                return ACCESS_FILE_ERROR;
            }
            if(!write_from_2_files(file1, file2, out_file))
            {
                printf("Problems in/outstream file\n");
                fclose(file1);
                fclose(file2);
                fclose(out_file);
                return UNDONE;
            }
            printf("File1: %s\nFile2 : %s\nWERE CONCATENATED INTO FILE : %s\n", dir1, dir2, output);
            fclose(file1);
            fclose(file2);
            fclose(out_file);
            return 0;

        case 'a':
            if(argc != 4)
            {
                printf("Wrong quantity of arguments, input <FILE1> <OUT_FILE>\n");
                return WRONG_QUANTITY_OF_ARGUMENTS;
            }
            dir1 = argv[2];
            output = argv[3];
            file1 = fopen(dir1, "r");
            out_file = fopen(output, "w");
            if(file1 == NULL)
            {
                perror("File 1 access failed");
                return ACCESS_FILE_ERROR;
            }

            if(file2 == NULL)
            {
                perror("File 2 access failed");
                return ACCESS_FILE_ERROR;
            }

            if(out_file == NULL)
            {
                perror("Outfile access failed");
                return ACCESS_FILE_ERROR;
            }
            if(!key_a(file1, out_file))
            {
                printf("Problems in/outstream file\n");
                fclose(file1);
                fclose(file2);
                fclose(out_file);
                return UNDONE;
            }

            printf("File with directory: %s\nHas been rewritten according to key 'а' in file with directory: %s\n", dir1, output);
            fclose(file1);
            fclose(out_file);
            return 0;
    }
}


// реализация описанных процедур, используемых в основной программе


int validation(char** str)
{
    if(strlen(*str) != 2) return WRONG_LENGTH;
    if((*str)[0] !='-' && (*str)[0] != '/') return WRONG_START;
    if((*str)[1] !='r' && (*str)[1] != 'a') return WRONG_FLAG;
    return FINE;
}

void skip_dividers(int* c, FILE* f)
{
    do
    {
        if(*c == EOF || isalnum(*c)) break;
        *c = fgetc(f);
    }while(*c == ' ' || *c== '\t' || *c=='\n');
    fseek(f, -1, SEEK_CUR);
}
int write_from_2_files(FILE* f1, FILE* f2, FILE* f3)
{
    int number_of_liter = 1;
    int a = '1', b = '1';
    do
    {
        if(number_of_liter % 2 == 1)
        {
            do
            {
                a = fgetc(f1);
                if(a == EOF) break;
                fputc(a, f3);
            } while(a != ' ' && a != '\n' && a != '\t');

            skip_dividers(&a, f1);
            number_of_liter++;
        }
        else
        {
            do
            {
                b = fgetc(f2);
                if(b == EOF) break;
                fputc(b, f3);
            } while(b != ' ' && b != '\n' && b != '\t');

            skip_dividers(&b, f2);
            number_of_liter++;
        }

    } while((a != EOF) && (b != EOF));

    fputc(' ', f3);

    if(b != EOF)
    {
        while((b = fgetc(f2)) != EOF)
        {
            fputc(b, f3);
            if(b == ' ' && b == '\t' && b == '\n')  skip_dividers(&b, f2);
        }
    }
    else if(a != EOF)
    {
        while((a = fgetc(f1)) != EOF)
        {
            fputc(a, f3);
            if(a == ' ' && a == '\t' && a == '\n')  skip_dividers(&a, f1);
        }
    }
    return 1;
}

int get_length_of_number_in_base(int n, int base)
{
    return (int)(log(n)/log(base)) + 1;
}

char* gorner(int n, int base)
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
void go_to_divider(int* c, FILE* f)
{
    do
    {
        if(*c == EOF) break;
        *c =fgetc(f);
    } while (isalnum((*c)));
    fseek(f, -1, SEEK_CUR);
}

int key_a(FILE* f1, FILE* output)
{
    int c;
    int number_of_lexeme = 1;
    do
    {
        c= fgetc(f1);
        if(c == EOF) break;
        if(number_of_lexeme % 10 == 0)
        {
            do
            {
                c = fgetc(f1);
                if(c==EOF) break;
                char* converted = gorner(c, 4);
                if(converted == NULL) return MALLOC_ERROR;
                fprintf(output, "%s", converted);
            } while(c != ' ' && c != '\n' && c != '\t');
            skip_dividers(&c, f1);
            number_of_lexeme++;  
        }
        else if(number_of_lexeme % 2 ==0 && number_of_lexeme % 10 != 0)
        {
            do
            {
                if(c==EOF) break;
                char tmp = tolower(c);
                fputc(tmp, output);
                c = fgetc(f1);
            } while(c != ' ' && c != '\n' && c != '\t');
            skip_dividers(&c, f1);
            number_of_lexeme++;  
        }
        else if(number_of_lexeme % 5 == 0 && number_of_lexeme % 10 != 0)
        {
            do
            {
                if(c==EOF) break;
                char* converted = gorner(c, 8);
                if(converted == NULL) return MALLOC_ERROR;
                fprintf(output, "%s", converted);
                c = fgetc(f1);
            } while(c != ' ' && c != '\n' && c != '\t');
            skip_dividers(&c, f1);
            number_of_lexeme++;  
        }
        else
        {
            if(c == ' ' || c=='\t' || c=='\n')  skip_dividers(&c, f1);
            else
            {
                go_to_divider(&c, f1);
                skip_dividers(&c, f1);
            }
            number_of_lexeme++;
        }
        fputc(' ', output);
    }while(c != EOF);
    return 1;
}
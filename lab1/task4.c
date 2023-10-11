#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<ctype.h>

typedef enum enum_errors
{
    WRONG_LENGTH,
    WRONG_START,
    WRONG_FLAG,
    WRONG_POSITION,
    WRONG_OPTIONAL,
    FINE
}validation_of_key;

int validation(char* str[])
{
    int length = strlen(str[1]);
    if(length < 2 || length > 3) return WRONG_LENGTH;
    if(length == 2)
    {
        if(str[1][0] != '-' && str[1][0] != '/') return WRONG_START;
        if(str[1][1] != 'd' && str[1][1] != 'i' && str[1][1] != 'a' && str[1][1] != 's') return WRONG_FLAG;
    }
    else
    {
        if(str[1][0] != '-' && str[1][0] != '/') return WRONG_START;
        if(str[1][0] == 'n' && str[1][1] == 'n' && str[1][2] == 'n') return WRONG_POSITION;
        if(str[1][1] != 'n') return WRONG_OPTIONAL;
        if(str[1][1] != 'd' && str[1][1] != 'i' && str[1][1] != 'a' && str[1][1] != 's') return WRONG_FLAG;
    }
    return FINE;
}
// макросы для проверки окружения
char* out_file(char* input_dir)
{
    int length = strlen(input_dir);
    char* res = (char*)malloc(sizeof(char)*(length + 5));
    if(res == NULL) return NULL;
    int ch = '/';//проверка на NULL
    char* dash;
    dash = strrchr(input_dir, ch);
    if(dash == NULL)
    {
        return NULL;
    }

    int dash_pos = dash - input_dir;
    int tmp = dash_pos+1;
    for(int i = 0; i<=dash_pos; ++i) res[i] = (input_dir)[i];
    res[tmp++] = '_';
    res[tmp++] = 'o';
    res[tmp++] = 'u';
    res[tmp++] = 't';
    for(int i = dash_pos+1; i<length; ++i) res[i+4] = (input_dir)[i];
    return res;
}

int delete_arabic_numbers(FILE* input, FILE* output)
{
    int c = 0;
    while((c = fgetc(input)) != EOF)
    {
        if(!(c >= '0' && c<='9'))
        {
            if(fputc(c, output) == EOF) return 0;
        }
    }
    return 1;
}

int count_latin_letters(FILE* input, FILE* output)
{
    int count = 0;
    int c = 0;
    while((c = fgetc(input)))
    {
        if((c >= 'a' && c <= 'z') || (c >='A' && c <= 'Z')) count ++;
        else if(c == '\n' || c==EOF)
        {
            if(fprintf(output, "%d\n", count) == EOF) return 0;
            count = 0;
        }
        if(c == EOF) break;
    }
    return 1;
}

int count_extra_symbols(FILE* input, FILE* output)
{
    int count = 0;
    int c;
    do
    {
        c = fgetc(input);
        if(!isalnum(c))
        {        
        count++;
        if(c == '\n' || c==EOF)
        {
            fprintf(output, "%d\n", count - 1);
            count = 0;
        }
        }
    }while(c!=EOF);
    return 1;
} 

int convert_into_ascii(FILE* input, FILE* output)
{
    int c;
    while((c = fgetc(input)) != EOF)
    {
        if(!(c >= '0' && c <= '9'))
        {
            if(c == '\n' || c == EOF)
            {
                if(fprintf(output, "\n") == EOF) return 0;
            }
            else
            {
                if(fprintf(output, "%X", c) == EOF) return 0;
            }
        }
        else
        {
            if(fprintf(output, "%c", c) == EOF) return 0;
        }
    }
    return 1;
}


int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Введите ключ для действия с файлом, расположение исходного файла и, при необходимости, путь к результирующему файлу\n");
        return 0;
    }
    int check_key = validation(argv);
    switch (check_key)
    {
    case 0:
        printf("Вы можете ввести следующие ключи:\nОбчные ключи: “-d”, “/d”, “-i”, “/i”, “-s”, “/s”, “-a”, “/a”\nС опциональным символом 'n': “-nd”, “/nd”, “-ni”, “/ni”, “-ns”, “/ns”, “-na”, “/na”\n");
        return 0;
    case 1:
        printf("Ключ может начинаться с символов '-' или '/'\n");
        return 0;
    case 2:
        printf("Нет такого ключа %s, вы можете ввести только “-d”, “/d”, “-i”, “/i”, “-s”, “/s”, “-a”, “/a”\nИли то же самое, но еще и с опциональным символом\n", argv[1]);
        return 0;
    case 3:
        printf("Опциональный символ должен быть после '-' или '/'\n");
        return 0;
    case 4:
        printf("Опциональный символ - это n, а не какая-то хуйня\n");
        return 0;
    default:
        break;
    }
    char* key = argv[1];
    int length = strlen(key) - 1;
    char* input_dir = argv[2];
    char* output_dir;



    if(strlen(key) == 2)
    {
        if(argc!=3)
        {
        printf("Введено неверное количесвто аргументов\n");
        return 0;
        }
    output_dir = out_file(input_dir);
    }

    else if(strlen(key) == 3)
    {
        if(argc!=4)
        {
        printf("Введено неверное количесвто аргументов\n");
        return 0;
        }
    output_dir = argv[3];
    }

    FILE* input = fopen(input_dir, "r");
    FILE* output = fopen(output_dir, "w");
    if(input == NULL) perror("Невозможно открыть файл на чтение");
    if(output == NULL) perror("Выйди и зайди нормально");
    switch(key[length])
    {
        case 'd':
            if(delete_arabic_numbers(input, output)) printf("Файл по директории %s\nБыл успешно перезаписан без арабских цифр в файл с директорией %s\n", input_dir, output_dir);
            else printf("Ошибка при записи данных\n");
            fclose(input);
            fclose(output);
            return 0;
        case 'i':
            if(count_latin_letters(input, output)) printf("Количество латинских букв в каждой строке из файла с директорией %s\n Было успешно записано в файл с директорией %s\n", input_dir, output_dir);
            else printf("Ошибка при записи данных\n");
            fclose(input);
            fclose(output);
            return 0;
        case 's':
            if(count_extra_symbols(input, output)) printf("Количество символов, отличных от латинских и цифр в каждой строке из файла с директорией %s\n Было успешно записано в файл с директорией %s\n", input_dir, output_dir);
            else printf("Ошибка при записи данных\n");
            fclose(input);
            fclose(output);
            return 0;
        case 'a':
            if(convert_into_ascii(input, output)) printf("Файл по директории %s\n Успешно переписан в виде ASCII кода в файл по директории:\n%s\n", input_dir, output_dir);
            else printf("Чето пошло не так\n");
            fclose(input);
            fclose(output);
            return 0;
            
    }
    return 0;
}
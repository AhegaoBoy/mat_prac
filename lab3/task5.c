#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum exit_codes
{
    lesser = -1,
    equal = 0,
    bigger = 1,
    memory_error,
    success,
    invalid_file_content,
    unexpected,
    eof_reached,
    endl_reached,
    newline_reached,
    space_reached,
    input_invalid,
};

struct String
{
    char* string;
    int length;
};

struct String* create_String(const char* string);
int clear_string(struct String* s);
int compare_strings(struct String* s1, struct String* s2);
int are_equal(struct String* s1, struct String* s2);
int copy(struct String* from, struct String** to);
struct String* copy_to_new(struct String* string);
int concat_strings(struct String* to, struct String* from);
void delete_string(struct String* string);


struct Student
{
    unsigned int id;
    struct String first_name;
    struct String last_name;
    struct String group;
    unsigned char* marks;
};

enum command_codes
{
    print,
    trace,
    sort,
    find,
    higraders,
    quit,
    unknown
};

int is_correct_word(char* string);
int is_number(const char* string);
int get_string(FILE* stream, char** string, int* length);

int write_to_file(const struct Student* student, FILE* dest);
int read_from_file(FILE* source, struct Student** students, size_t* size);

int sort_students(struct Student** students, size_t amount, const char* field);
int compare_id(const void* number1, const void* number2);
int compare_group(const void* str1, const void* str2);
int compare_firstname(const void* str1, const void* str2);
int compare_lastname(const void* str1, const void* str2);

int search_for_student(struct Student** students, int students_amount, const char* field, const char* tofind, int* amount, struct Student** results);
int find_id(struct Student** students, int students_amount, const char* tofind, struct Student** results, int* amount);
int find_lastname(struct Student** students, int students_amount, const char* tofind, struct Student** results, int* amount);
int find_firstname(struct Student** students, int students_amount, const char* tofind, struct Student** results, int* amount);
int find_group(struct Student** students, int students_amount, const char* tofind, struct Student** results, int* amount);

double get_avg_all(struct Student** students, int amount);
double get_avg_one(struct Student*);

int dialogue(const char* input_path, int argc, const char** argv);


int main(int argc, char** argv) {
    if(argc < 2 || argc > 3)
    {
        printf("wrong amount of arguments given.\n");
        return 0;
    }
    dialogue(argv[1], argc, argv);
    return 0;
}

struct String* create_String(const char* string)
{
    char* copy = string;
    int length = 0;
    for(; *copy != '\0'; ++length && (*copy++));
    struct String* s = (struct String*)malloc(sizeof(struct String));
    if(s == NULL) return NULL;
    s->length = length;
    s->string = (char*)malloc(sizeof(char)*(length+1));
    if(s->string == NULL)
    {
        free(s);
        return NULL;
    }
    for(int i = 0; i < length; i++)
    {
        s->string[i] = string[i];
    }
    s->string[length] = '\0';
    return s;
}

int clear_string(struct String* s)
{
    if(s->string != NULL) free((void*)s->string);
    s->string = (char*)malloc(sizeof(char)*1);
    if(s->string == NULL) return memory_error;
    s->string[0] = '\0';
    s->length = 0;
    return success;
}

int compare_strings(struct String* s1, struct String* s2)
{//returns positive if s1 > s2
    if(s1->length > s2->length) return bigger;
    if(s1->length < s2->length) return lesser;
    for(int i = 0; i < s1->length; i++)
    {
        if(s1->string[i] - s2->string[i] > 0) return bigger;
        if(s1->string[i] - s2->string[i] < 0) return lesser;
    }
    return equal;
}

int are_equal(struct String* s1, struct String* s2)
{
    if(s1->length != s2->length) return 0;
    for(int i = 0; i < s1->length; i++)
    {
        if(s1->string[i] != s2->string[i]) return 0;
    }
    return 1;
}

int copy(struct String* from, struct String** to)
{
    clear_string(*to);
    *to = create_String(from->string);
    if (*to == NULL) return memory_error;
    return success;
}

struct String* copy_to_new(struct String* string)
{
    struct String* new = create_String(string->string);
    return new;
}

int concat_strings(struct String* to, struct String* from)
{
    if(from->length == 0) return success;
    if(to->length == 0)
    {
        to = from;
        return success;
    }
    char* for_realloc = (char*)realloc(to->string, to->length + from->length);
    if(for_realloc == NULL)
    {
        clear_string(to);
        return memory_error;
    }
    to->string = for_realloc;
    for(int i = 0; i < from->length + 1; i++)
    {
        to->string[i+to->length] = from->string[i];
    }
    to->length += from->length;
    return success;
}

void delete_string(struct String* string)
{
    if(string->string != NULL) free(string->string);
    string->length = 0;
}

int get_string(FILE* stream, char** string, int* length)
{
    *length = 0;
    int capacity = 1;
    char* for_realloc;
    int c = fgetc(stream);
    *string = (char*)malloc(sizeof(char)*1);
    if(*string == NULL) return memory_error;
    while(c != '\n' && c != '\0' && c != '\t' && c != ' ' && c != EOF)
    {
        (*string)[(*length)++] = c;
        if(*length >= capacity)
        {
            capacity *= 2;
            for_realloc = realloc(*string, capacity * sizeof(char));
            if(for_realloc == NULL)
            {
                free(*string);
                return memory_error;
            }
            *string = for_realloc;
        }
        c = fgetc(stream);
    }
    (*string)[*length] = '\0';
    if(c == ' ') return space_reached;
    if (c == EOF) return eof_reached;
    if (c == '\n') return newline_reached;
    if (c == '\0') return endl_reached;
    return success;
}

int is_number(const char* string)
{
    size_t string_length = strlen(string);
    for(int i = 0; i < string_length; i++){
        if(!isdigit(string[i])) return 0;
    }
    return 1;
}

int is_correct_word(char* string)
{
    size_t string_length = strlen(string);
    for(int i = 0; i < string_length; i++)
    {
        if(!isalpha(string[i])) return 0;
    }
    return 1;
}


int read_from_file(FILE* source, struct Student** students, size_t* size)
{
    struct Student read;
    unsigned char* line[5];
    int length;

    *students = (struct Student*)malloc(sizeof(struct Student)*1);
    int capacity = 1;
    *size = 0;

    while(!feof(source))
    {
        for(int i = 0; i < 5; i++)
        {
            int flag = get_string(source, &line[i], &length);
            switch(flag)
            {
                case eof_reached:
                case newline_reached:
                {
                    if(i != 4)
                    {
                        for(int j = 0; j < i; j++) free(line[j]);
                        return invalid_file_content;
                    }
                    break;
                }
                case space_reached:
                {
                    if(i == 4)
                    {
                        for(int j = 0; j < i; j++) free(line[j]);
                        return invalid_file_content;
                    }
                    break;
                }
                case memory_error:
                {
                    for(int j = 0; j < i; j++) free(line[j]);
                    return memory_error;
                }
            }
        }
        if(!is_number(line[0]) || !is_correct_word(line[1]) || !is_correct_word(line[2]) || strlen(line[4]) != 5 || !is_number(line[4]))
        {
            for(int i = 0; i < 5; i++) free(line[i]);
            return invalid_file_content;
        }
        (*size)++;
        if(*size == capacity)
        {
            capacity *= 2;
            struct Student* for_realloc = (struct Student*)realloc(*students, sizeof(struct Student)*capacity);
            if(for_realloc == NULL)
            {
                free(*students);
                return memory_error;
            }
            *students = for_realloc;
        }
        read.id = atoi(line[0]);
        read.first_name = *create_String(line[1]);
        read.last_name = *create_String(line[2]);
        read.group = *create_String(line[3]);
        read.marks = line[4];
        (*students)[(*size)-1] = read;
    }
    free(line[0]);
    free(line[1]);
    free(line[2]);
    free(line[3]);

    return success;
}

int compare_id(const void* number1, const void* number2)
{
    if((*(struct Student*)number1).id > (*(struct Student*)number2).id) return 1;
    if((*(struct Student*)number1).id < (*(struct Student*)number2).id) return -1;
    return 0;
}

int compare_group(const void* str1, const void* str2)
{
    char* string1 = (*(struct Student*)str1).group.string;
    char* string2 = (*(struct Student*)str2).group.string;
    size_t min = ((*(struct Student*)str1).group.length < (*(struct Student*)str2).group.length) ? (*(struct Student*)str1).group.length : (*(struct Student*)str2).group.length;
    for(int i = 0; i < min; i++)
    {
        if(string1[i] > string2[i]) return 1;
        if(string1[i] < string2[i]) return -1;
    }
    if((*(struct Student*)str1).group.length == (*(struct Student*)str2).group.length) return 0;
    return ((*(struct Student*)str1).group.length > (*(struct Student*)str2).group.length) ? 1 : -1;
}

int compare_firstname(const void* str1, const void* str2)
{
    char* string1 = (*(struct Student*)str1).first_name.string;
    char* string2 = (*(struct Student*)str2).first_name.string;
    size_t min = ((*(struct Student*)str1).first_name.length < (*(struct Student*)str2).first_name.length) ? (*(struct Student*)str1).first_name.length : (*(struct Student*)str2).first_name.length;
    for(int i = 0; i < min; i++)
    {
        if(string1[i] > string2[i]) return 1;
        if(string1[i] < string2[i]) return -1;
    }
    if((*(struct Student*)str1).first_name.length == (*(struct Student*)str2).first_name.length) return 0;
    return ((*(struct Student*)str1).first_name.length > (*(struct Student*)str2).first_name.length) ? 1 : -1;
}


int compare_lastname(const void* str1, const void* str2)
{
    char* string1 = (*(struct Student*)str1).last_name.string;
    char* string2 = (*(struct Student*)str2).last_name.string;
    size_t min = ((*(struct Student*)str1).last_name.length < (*(struct Student*)str2).last_name.length) ? (*(struct Student*)str1).last_name.length : (*(struct Student*)str2).last_name.length;
    for(int i = 0; i < min; i++)
    {
        if(string1[i] > string2[i]) return 1;
        if(string1[i] < string2[i]) return -1;
    }
    if((*(struct Student*)str1).last_name.length == (*(struct Student*)str2).last_name.length) return 0;
    return ((*(struct Student*)str1).last_name.length > (*(struct Student*)str2).last_name.length) ? 1 : -1;
}

int write_to_file(const struct Student* student, FILE* dest)
{
    return fprintf(dest, "%d %s %s %s %s\n", student->id, student->first_name.string, student->last_name.string, student->group.string, student->marks);
}

int sort_students(struct Student** students, size_t amount, const char* field)
{
    if(!strcmp(field, "id"))
    {
        qsort(*students, amount, sizeof(struct Student), compare_id);
        return success;
    }
    if(!strcmp(field, "lastname"))
    {
        qsort(*students, amount, sizeof(struct Student), compare_lastname);
        return success;
    }
    if(!strcmp(field, "firstname"))
    {
        qsort(*students, amount, sizeof(struct Student), compare_firstname);
        return success;
    }
    if(!strcmp(field, "group"))
    {
        qsort(*students, amount, sizeof(struct Student), compare_group);
        return success;
    }
    return input_invalid;
}

int search_for_student(struct Student** students, int students_amount, const char* field, const char* tofind, int* amount, struct Student** results)
{
    if(!strcmp(field, "id"))
    {
        return find_id(students, students_amount, tofind, results, amount);
    }
    if(!strcmp(field, "lastname"))
    {
        return find_lastname(students, students_amount, tofind, results, amount);
    }
    if(!strcmp(field, "firstname"))
    {
        return find_firstname(students, students_amount, tofind, results, amount);
    }
    if(!strcmp(field, "group"))
    {
        return find_group(students, students_amount, tofind, results, amount);
    }
    return input_invalid;
}

int find_id(struct Student** students, int students_amount, const char* tofind, struct Student** results, int* amount)
{
    *results = (struct Student*)malloc(sizeof(struct Student) * 1);
    if(*results == NULL) return memory_error;
    *amount = 0;
    int capacity = 1;
    int id = atoi(tofind);
    for(int i = 0; i < students_amount; i++)
    {
        if((*students)[i].id == id)
        {
            (*amount)++;
            if(*amount == capacity)
            {
                capacity <<= 1;
                struct Student* for_realloc = (struct Student*)realloc(*results, sizeof(struct Student) * capacity);
                if (for_realloc == NULL)
                {
                    free(*students);
                    return memory_error;
                }
                *results = for_realloc;
            }
            (*results)[*amount - 1] = (*students)[i];
        }
    }
    return success;
}

int find_lastname(struct Student** students, int students_amount, const char* tofind, struct Student** results, int* amount)
{
    *results = (struct Student*)malloc(sizeof(struct Student) * 1);
    if(*results == NULL) return memory_error;
    *amount = 0;
    int capacity = 1;
    for(int i = 0; i < students_amount; i++)
    {
        if(!strcmp((*students)[i].last_name.string, tofind))
        {
            (*amount)++;
            if(*amount == capacity)
            {
                capacity <<= 1;
                struct Student* for_realloc = (struct Student*)realloc(*results, sizeof(struct Student) * capacity);
                if (for_realloc == NULL)
                {
                    free(*students);
                    return memory_error;
                }
                *results = for_realloc;
            }
            (*results)[*amount - 1] = (*students)[i];
        }
    }
    return success;
}

int find_firstname(struct Student** students, int students_amount, const char* tofind, struct Student** results, int* amount)
{
    *results = (struct Student*)malloc(sizeof(struct Student) * 1);
    if(*results == NULL) return memory_error;
    *amount = 0;
    int capacity = 1;
    for(int i = 0; i < students_amount; i++)
    {
        if(!strcmp((*students)[i].first_name.string, tofind))
        {
            (*amount)++;
            if(*amount == capacity)
            {
                capacity <<= 1;
                struct Student* for_realloc = (struct Student*)realloc(*results, sizeof(struct Student) * capacity);
                if (for_realloc == NULL)
                {
                    free(*students);
                    return memory_error;
                }
                *results = for_realloc;
            }
            (*results)[*amount - 1] = (*students)[i];
        }
    }
    return success;
}

int find_group(struct Student** students, int students_amount, const char* tofind, struct Student** results, int* amount)
{
    *results = (struct Student*)malloc(sizeof(struct Student) * 1);
    if(*results == NULL) return memory_error;
    *amount = 0;
    int capacity = 1;
    for(int i = 0; i < students_amount; i++)
    {
        if(!strcmp((*students)[i].group.string, tofind))
        {
            (*amount)++;
            if(*amount == capacity)
            {
                capacity <<= 1;
                struct Student* for_realloc = (struct Student*)realloc(*results, sizeof(struct Student) * capacity);
                if (for_realloc == NULL)
                {
                    free(*students);
                    return memory_error;
                }
                *results = for_realloc;
            }
            (*results)[*amount - 1] = (*students)[i];
        }
    }
    return success;
}

double get_avg_one(struct Student* student)
{
    return (double)(student->marks[0] - '0' + student->marks[1] - '0' + student->marks[2] - '0' + student->marks[3] - '0' + student->marks[4] - '0') / 5;
}

double get_avg_all(struct Student** students, int amount)
{
    double avg = 0;
    for(int i = 0; i < amount; i++)
    {
        avg += (*students)[i].marks[0] - '0' + (*students)[i].marks[1] - '0' + (*students)[i].marks[2] - '0' + (*students)[i].marks[3] - '0' + (*students)[i].marks[4] - '0';
    }
    avg = avg/(amount * 5);
    return avg;
}

int get_cmd_code(const char* command)
{
    if(!strcmp(command, "print")) return print;
    if(!strcmp(command, "trace")) return trace;
    if(!strcmp(command, "sort")) return sort;
    if(!strcmp(command, "find")) return find;
    if(!strcmp(command, "higraders")) return higraders;
    if(!strcmp(command, "quit")) return quit;
    return unknown;
}

int dialogue(const char* input_path, int argc, const char** argv)
{
    printf("Primitive dialogue interface v0.0\n");
    FILE* in = fopen(input_path, "r");
    if(in == NULL) { printf("no\n"); return success; }
    printf("loaded file: %s\n", input_path);

    struct Student* students;
    size_t size;
    int exit_code = read_from_file(in, &students, &size);
    fclose(in);
    switch(exit_code)
    {
        case memory_error:
        {
            printf("Memory error occurred while reading from file. Exiting.\n");
            return memory_error;
        }
        case invalid_file_content:
        {
            printf("invalid file content. Exiting.\n");
            return invalid_file_content;
        }
        case success:
        {
            printf("File contents loaded into memory.\n");
            break;
        }
        default:
        {
            printf("Unexpected error. Exiting.\n");
            return unexpected;
        }
    }
    FILE* out = stdout;
    FILE* file = out;
    for(int i = 0; i < size; i++) write_to_file(&students[i], out);

    printf("choose action:\n"
           "sort id/firstname/lastname/group - sort by following field [and trace to file]\n"
           "[trace] find id/firstname/lastname/group <value> - search among students for given field values\n"
           "[trace] higraders - find students with average grade higher than average for whole file\n"
           "quit - self-explanatory, isn't it?\n");

    fflush(stdout);

    char* line[4];
    int len;
    int exit;
    int i = 0;
    int cmd_code;
    line[0] = strdup("");
    while(strcmp(line[0], "quit"))
    {
        if(cmd_code == trace) cmd_code = get_cmd_code(line[1]);
        else
        {
            i = 0;
            fflush(stdout);
            for(; i < 4; i++)
            {
                exit = get_string(stdin, &line[i], &len);
                if(exit == newline_reached) { i++; break; }
            }
            i--;
            cmd_code = get_cmd_code(line[0]);
            if(strcmp(line[0], "trace")) out = stdout;
        }
        if(exit != newline_reached)
        {
            printf("too many arguments\n");
            fflush(stdout);
        }
        else
        {
            fflush(stdout);
            switch(cmd_code)
            {
                case trace:
                {
                    if (strcmp(line[1], "higraders") && strcmp(line[1], "find"))
                    {
                        printf("Invalid arguments with using 'trace'.\n");
                        continue;
                    }
                    if(argc < 3)
                    {
                        printf("Trace file path was not given.\n");
                        continue;
                    }
                    if(file == stdout || file == NULL)
                    {
                        out = fopen(argv[2], "w");
                        file = out;
                        if(out == NULL)
                        {
                            printf("Couldn't create trace file at %s", argv[2]);
                            out = stdout;
                            continue;
                        }
                    }
                    if(file != out) out = file;
                    continue;
                }
                case print:
                {
                    if(i != 0) printf("Invalid input.\n");
                    else for(int s = 0; s < size; s++) write_to_file(&students[s], out);
                    break;
                }
                case sort:
                {
                    if(i < 1)
                    {
                        printf("No sorting field was given.\n");
                        continue;
                    }
                    sort_students(&students, size, line[i]);
                    printf("Students sorted by %s\n", line[i]);
                    for(int j = 0; j < size; j++) write_to_file(&students[j], out);
                    continue;
                }
                case find:
                {
                    int amount;
                    struct Student* found;
                    int flag = search_for_student(&students, size, line[i-1], line[i], &amount, &found);
                    if(flag == memory_error)
                    {
                        printf("Memory error while searching for students.\n");
                        return memory_error;
                    }
                    if(flag == input_invalid)
                    {
                        printf("Invalid input\n");
                        continue;
                    }
                    for(int s = 0; s < amount; s++)
                    {
                        write_to_file(&found[s], out);
                    }
                    printf("found %d students corresponding the query '%s' = '%s'\n", amount, line[i-1], line[i]);
                    free(found);
                    continue;
                }
                case higraders:
                {
                    double avg_avg = get_avg_all(&students, size);
                    for(int s = 0; s < size; s++)
                    {
                        double x =get_avg_one(&students[s]);
                        if(x > avg_avg)
                        {
                            write_to_file(&students[s], out);
                        }
                    }
                    printf("searching done.\n");
                    continue;
                }
                case quit:
                {
                    printf("\nGoodbye!\n");
                    break;
                }
                case unknown:
                {
                    printf("Dude wtf\n");
                    break;
                }
            }
        }
    }
    for(int j = 0; j <= i; j++) free(line[j]);
    for(i = 0; i < size; i++)
    {
        free(students[i].marks);
        delete_string(&students[i].first_name);
        delete_string(&students[i].last_name);
        delete_string(&students[i].group);
    }
    free(students);
    if(out != stdout && out != NULL) fclose(out);
    fclose(in);
    return success;
}
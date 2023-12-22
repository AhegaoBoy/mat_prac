#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name;
    int *data;
    size_t size;
    size_t capacity;
} Array;

typedef enum {
    OK,
    NO_MEMORY,
    NO_FILE,
    OUT_OF_BOUNDS,
    INVALID_INPUT,
    UNINITIALIZED_VALUE
} status_codes;

typedef struct Node {
    Array *data;
    struct Node *left;
    struct Node *right;
} Node;

status_codes addNode(Node **ptr, Array *arr);

void findNode(Node *ptr, Array **result, char name);
void freeNode(Node *ptr);
int int_validation(const char * number);
int comp_increase(int *first, int *second);
int comp_decrease(int *first, int *second);
int comp_random(int *first, int *second);
status_codes initArray(Array **arr, char _name);
status_codes push_backArray(Array *arr, int number);
status_codes loadArray(Array *arr, FILE *stream);
void saveArray(Array *arr, FILE *stream);
status_codes randArray(Array *arr, int count, int left_bound, int right_bound);
status_codes concatArray(Array *dest, Array *src);
status_codes freeArray(Array *arr);
void removeArray(Array *arr, int start, int count);
status_codes copyArray(Array *src, int start, int end, Array *dest);
void sort_increaseArray(Array *arr);
void sort_decreaseArray(Array *arr);
void shuffleArray(Array *arr);
void statsArray(Array *arr);
void printArray(Array *arr, int start, int end);

int int_validation(const char * number);
int comp_increase(int *first, int *second);
int comp_decrease(int *first, int *second);
int comp_random(int *first, int *second);

void errorProcessing(const status_codes error, const int line);
void to_lower(char *string);
status_codes makeCommand(char *command, Node **root);
status_codes getCommand(FILE *programm);


status_codes loadProcessing(Node **root, const char *filename, const char array_name);
status_codes saveProcessing(Node *root, const char *filename, const char array_name);
status_codes randProcessing(Node **root, const char array_name, int count, int left_bound, int right_bound);
status_codes concatProcessing(Node **root, const char dest_name, const char src_name);
status_codes freeProcessing(Node *root, const char array_name);
status_codes removeProcessing(Node *root, const char array_name, const int start, const int count);
status_codes copyProcessing(Node **root, const char src_name, const char dest_name, const int start, const int end);
status_codes sortProcessing(Node *root, const char array_name, int sort_flag);
status_codes statsProcessing(Node *root, const char array_name);
status_codes printProcessing(Node *root, const char array_name, const int start, const int end);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong flag\n");
        return -1;
    }
    FILE *programm = fopen(argv[1], "r");
    if (!programm) {
        printf("No such file\n");
        return -1;
    }
    status_codes status = getCommand(programm);
    if (status == NO_MEMORY) {
        printf("No memory\n");
        return -1;
    }
    else if (status != OK) {
        return -1;
    }
    fclose(programm);
    return 0;
}

void errorProcessing(const status_codes error, const int line)
{
    switch (error) {
        case NO_MEMORY:
            printf("No memory after %d line\n", line);
            return;
        case INVALID_INPUT:
            printf("Invalid input in %d line\n", line);
            return;
        case NO_FILE:
            printf("No such file (%d line)\n", line);
            return;
        case OUT_OF_BOUNDS:
            printf("Out of bounds of array in %d line\n", line);
            return;
        case UNINITIALIZED_VALUE:
            printf("Uninitialized value used in %d line\n", line);
            return;
    }
}

void to_lower(char *string)
{
    size_t string_size = strlen(string);
    for (int i = 0; i < string_size; i++) {
        string[i] = tolower(string[i]);
    }
}

status_codes makeCommand(char *command, Node **root)
{
    const char *separators = " ,()\n\t;";
    char *token = strtok(command, separators);
    to_lower(token);
    if (strcmp(token, "load") == 0 || strcmp(token, "save") == 0) {
        int command_type = 1;
        if (strcmp(token, "load") == 0) {
            command_type = 0;
        }
        token = strtok(NULL, separators);
        if (!token || strlen(token) != 1 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (!token) {
            return INVALID_INPUT;
        }
        char *filename = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        if (!filename) {
            return NO_MEMORY;
        }
        strcpy(filename, token);
        status_codes status;
        if (!command_type) {
            status = loadProcessing(root, filename, array_name);
        }
        else {
            status = saveProcessing((*root), filename, array_name);
        }
        free(filename);
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return status;
    }
    else if (strcmp(token, "rand") == 0) {
        char array_name;
        int count = 0, left_bound = 0, right_bound = 0;
        for (int i = 0; i < 4; i++) {
            token = strtok(NULL, separators);
            if (!token) {
                return INVALID_INPUT;
            }
            switch (i) {
                case 0:
                    if (strlen(token) != 1 || !isalpha(token[0])) {
                        return INVALID_INPUT;
                    }
                    array_name = toupper(token[0]);
                    break;
                case 1:
                    count = atoi(token);
                    if (!int_validation(token) || count < 0) {
                        return INVALID_INPUT;
                    }
                    break;
                case 2:
                    left_bound = atoi(token);
                    if (!int_validation(token)) {
                        return INVALID_INPUT;
                    }
                    break;
                case 3:
                    right_bound = atoi(token);
                    if (!int_validation(token)) {
                        return INVALID_INPUT;
                    }
                    break;
            }
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return randProcessing(root, array_name, count, left_bound, right_bound);
    }
    else if (strcmp(token, "concat") == 0) {
        char src_name, dest_name;
        for (int i = 0; i < 2; i++) {
            token = strtok(NULL, separators);
            if (!token || strlen(token) != 1 || !isalpha(token[0])) {
                return INVALID_INPUT;
            }
            if (i == 0) {
                dest_name = toupper(token[0]);
            }
            else {
                src_name = toupper(token[0]);
            }

        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return concatProcessing(root, dest_name, src_name);
    }
    else if (strcmp(token, "free") == 0) {
        token = strtok(NULL, separators);
        if (!token || strlen(token) != 1 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return freeProcessing((*root), array_name);
    }
    else if (strcmp(token, "remove") == 0) {
        char array_name;
        int start = 0, count = 0;
        for (int i = 0; i < 3; i++) {
            token = strtok(NULL, separators);
            if (!token) {
                return INVALID_INPUT;
            }
            if (i == 0) {
                if (strlen(token) != 1 || !isalpha(token[0])) {
                    return INVALID_INPUT;
                }
                array_name = toupper(token[0]);
            }
            else if (i == 1) {
                start = atoi(token);
                if (!int_validation(token) || start < 0) {
                    return INVALID_INPUT;
                }
            }
            else {
                count = atoi(token);
                if (!int_validation(token) || count < 0) {
                    return INVALID_INPUT;
                }
            }
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        removeProcessing((*root), array_name, start, count);
    }
    else if (strcmp(token, "copy") == 0) {
        int start = 0, end = 0;
        char src_name, dest_name;
        for (int i = 0; i < 4; i++) {
            token = strtok(NULL, separators);
            if (!token) {
                return INVALID_INPUT;
            }
            if (i == 0) {
                if (strlen(token) > 1) {
                    return INVALID_INPUT;
                }
                src_name = toupper(token[0]);
            }
            else if (i == 1) {
                start = atoi(token);
                if (!int_validation(token) || start < 0) {
                    return INVALID_INPUT;
                }
            }
            else if (i == 2) {
                end = atoi(token);
                if (!int_validation(token) || end < 0) {
                    return INVALID_INPUT;
                }
            }
            else {
                if (strlen(token) > 1) {
                    return INVALID_INPUT;
                }
                dest_name = toupper(token[0]);
            }
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return copyProcessing(root, src_name, dest_name, start, end);
    }
    else if (strcmp(token, "sort") == 0) {
        token = strtok(NULL, separators);
        if (!token) {
            return INVALID_INPUT;
        }
        if (strlen(token) != 2 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        int sort_flag = 0;
        if (token[1] == '+') {
            sort_flag = 0;
        }
        else if (token[1] == '-'){
            sort_flag = 1;
        }
        else {
            return INVALID_INPUT;
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return sortProcessing((*root), array_name, sort_flag);
    }
    else if (strcmp(token, "shuffle") == 0) {
        token = strtok(NULL, separators);
        if (!token || strlen(token) != 1 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return sortProcessing((*root), array_name, 2);
    }
    else if (strcmp(token, "stats") == 0) {
        token = strtok(NULL, separators);
        if (!token || strlen(token) != 1 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return statsProcessing((*root), array_name);
    }
    else if (strcmp(token, "print") == 0) {
        token = strtok(NULL, separators);
        if (!token || strlen(token) != 1 || !isalpha(token[0])) {
            return INVALID_INPUT;
        }
        char array_name = toupper(token[0]);
        token = strtok(NULL, separators);
        if (strcmp(token, "all") == 0) {
            token = strtok(NULL, separators);
            if (token) {
                return INVALID_INPUT;
            }
            return printProcessing((*root), array_name, -1, -1);
        }
        int left_bound = atoi(token);
        if (!int_validation(token) && left_bound < 0) {
            return INVALID_INPUT;
        }
        token = strtok(NULL, separators);
        if (!token) {
            return printProcessing((*root), array_name, left_bound, left_bound);
        }
        int right_bound = atoi(token);
        if (!int_validation(token) && right_bound < 0) {
            return INVALID_INPUT;
        }
        token = strtok(NULL, separators);
        if (token) {
            return INVALID_INPUT;
        }
        return printProcessing((*root), array_name, left_bound, right_bound);
    }
    else {
        return INVALID_INPUT;
    }
}


status_codes getCommand(FILE *programm)
{
    Node *root = NULL;
    char symbol = fgetc(programm);
    while (symbol == ' ' || symbol == '\t' || symbol == '\n') {
        symbol = fgetc(programm);
    }
    size_t command_size = 0;
    size_t command_capacity = 2;
    char *command = (char *)malloc(sizeof(char) * command_capacity);
    if (!command) {
        return NO_MEMORY;
    }
    int command_flag = 0;
    int line = 1;
    while (symbol != EOF) {
        if (symbol == '\n') {
            line++;
        }
        command_flag = 0;
        command[command_size] = symbol;
        command_size++;
        if (command_size >= command_capacity) {
            command_capacity *= 2;
            char *tmp = NULL;
            if (!(tmp = (char *)realloc(command, (sizeof(char) * command_capacity)))) {
                free(command);
                freeNode(root);
                return NO_MEMORY;
            }
            command = tmp;
        }
        if (symbol == ';') {
            command[command_size] = '\0';
            status_codes status = makeCommand(command, &root);
            if (status != OK) {
                errorProcessing(status, line);
                freeNode(root);
                free(command);
                return status;
            }
            char *tmp = NULL;
            command_size = 0;
            command_capacity = 2;
            if (!(tmp = (char *)realloc(command, sizeof(char) * command_capacity))) {
                free(command);
                return NO_MEMORY;
            }
            command = tmp;
            symbol = fgetc(programm);
            while (symbol == ' ' || symbol == '\n' || symbol == '\t') {
                symbol = fgetc(programm);
            }
            command_flag = 1;
        }
        if (!command_flag) {
            symbol = fgetc(programm);
        }
    }
    free(command);
    freeNode(root);
    return OK;
}

status_codes initArray(Array **arr, const char _name) {
    (*arr) = (Array *)malloc(sizeof(Array));
    if (!(*arr)) {
        return NO_MEMORY;
    }
    (*arr)->name = _name;
    (*arr)->size = 0;
    (*arr)->capacity = 2;
    (*arr)->data = (int *)malloc(sizeof(int) * (*arr)->capacity);
    if (!(*arr)->data) {
        free(*arr);
        return NO_MEMORY;
    }
    return OK;
}

status_codes push_backArray(Array *arr, const int number) {
    arr->data[arr->size] = number;
    arr->size++;
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        int *tmp = NULL;
        if (!(tmp = (int *)realloc(arr->data, sizeof(int) * arr->capacity))) {
            return NO_MEMORY;
        }
        arr->data = tmp;
    }
    return OK;
}

status_codes loadArray(Array *arr, FILE *stream) {
    char symbol = fgetc(stream);
    size_t size = 0;
    size_t capacity = 2;
    char *string = (char *)malloc(sizeof(char) * capacity);
    if (!string) {
        return NO_MEMORY;
    }
    while (symbol != EOF) {
        if (symbol != ' ' && symbol != '\t' && symbol != '\n' && symbol != EOF) {
            string[size] = symbol;
            size++;
            if (size >= capacity) {
                capacity *= 2;
                char *tmp = NULL;
                if (!(tmp = (char *)realloc(string, sizeof(char) * capacity))) {
                    free(string);
                    return NO_MEMORY;
                }
                string = tmp;
            }
        }
        else {
            string[size] = '\0';
            if (!int_validation(string) && size) {
                free(string);
                return INVALID_INPUT;
            }
            if (size || arr->size == 0) {
                int new_number = atoi(string);
                if (push_backArray(arr, new_number) == NO_MEMORY) {
                    free(string);
                    return NO_MEMORY;
                }
            }
            size = 0;
            capacity = 2;
            char *tmp = NULL;
            if (!(tmp = (char *)realloc(string, sizeof(char) * capacity))) {
                free(string);
                return NO_MEMORY;
            }
            string = tmp;
        }
        symbol = fgetc(stream);
    }
    string[size] = '\0';
    if (!int_validation(string) && size) {
        return INVALID_INPUT;
    }
    if (size || arr->size == 0) {
        int new_number = atoi(string);
        if (push_backArray(arr, new_number) == NO_MEMORY) {
            free(string);
            return NO_MEMORY;
        }
    }
    free(string);
    return OK;
}

void saveArray(Array *arr, FILE *stream) {
    for (int i = 0; i < arr->size; i++) {
        fprintf(stream, "%d ", arr->data[i]);
    }
}

status_codes randArray(Array *arr, const int count, const int left_bound, const int right_bound) {
    if (!arr->data) {
        arr->data = (int *)malloc(sizeof(int) * arr->capacity);
        if (!arr->data) {
            return NO_MEMORY;
        }
    }
    for (int i = 0; i < count; i++) {
        if (push_backArray(arr, rand() % (right_bound - left_bound + 1) + left_bound) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes concatArray(Array *dest, Array *src) {
    dest->capacity += src->capacity;
    int *tmp = (int *)realloc(dest->data, sizeof(int) * dest->capacity);
    if (!tmp) {
        return NO_MEMORY;
    }
    dest->data = tmp;
    for (int i = dest->size; i < src->size + dest->size; i++) {
        dest->data[i] = src->data[i - dest->size];
    }
    dest->size += src->size;
    return OK;
}

status_codes freeArray(Array *arr) {
    arr->size = 0;
    arr->capacity = 2;
    int *tmp = NULL;
    if (!(tmp = (int *)realloc(arr->data, sizeof(int) * arr->capacity))) {
        return NO_MEMORY;
    }
    arr->data = tmp;
    return OK;
}

void removeArray(Array *arr, const int start, const int count) {
    for (int i = start; i < start + count; i++) {
        arr->data[i] = 0;
    }
    for (int i = start + count; i < arr->size; i++) {
        arr->data[i - count] = arr->data[i];
        arr->data[i] = 0;
    }
    arr->size -= count;
}

status_codes copyArray(Array *src, const int start, const int end, Array *dest) {
    for (int i = start; i <= end; i++) {
        if (push_backArray(dest, src->data[i]) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

void sort_increaseArray(Array *arr) {
    qsort(arr->data, arr->size, sizeof(int), (int(*)(const void *, const void *))comp_increase);
}

void sort_decreaseArray(Array *arr) {
    qsort(arr->data, arr->size, sizeof(int), (int(*)(const void *, const void *))comp_decrease);
}

void shuffleArray(Array *arr) {
    qsort(arr->data, arr->size, sizeof(int), (int(*)(const void *, const void *))comp_random);
}

void statsArray(Array *arr) {
    int max = arr->data[0], max_index = 0;
    int min = arr->data[0], min_index = 0;
    int max_count_elem = arr->data[0], max_count = 1;
    int summ = 0;
    for (int i = 0; i < arr->size; i++) {
        if (max < arr->data[i]) {
            max = arr->data[i];
            max_index = i;
        }
        if (min > arr->data[i]) {
            min = arr->data[i];
            min_index = i;
        }
        summ += arr->data[i];
        int curr_count = 0;
        for (int j = i + 1; j < arr->size; j++) {
            if (arr->data[i] == arr->data[j]) {
                curr_count++;
            }
        }
        if (curr_count > max_count || (curr_count == max_count && arr->data[i] > max_count_elem)) {
            max_count_elem = arr->data[i];
            max_count = curr_count;
        }
    }
    double avg = (double)summ / (double)arr->size;
    double max_deviation = -1.0;
    for (int i = 0; i < arr->size; i++) {
        if (fabs(arr->data[i] - avg) > max_deviation) {
            max_deviation = fabs(arr->data[i] - avg);
        }
    }
    printf("___________________________________\n");
    printf("\t%c stat\n", arr->name);
    printf("Max elem is %d in index %d\n", max, max_index);
    printf("Min elem is %d in index %d\n", min, min_index);
    printf("Array average is %f\n", avg);
    printf("Max deviation from average is %f\n", max_deviation);
    printf("___________________________________\n");
}

void printArray(Array *arr, const int start, const int end) {
    printf("%c: ", arr->name);
    for (int i = start; i <= end; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
}

status_codes addNode(Node **ptr, Array *arr) {
    if (!(*ptr)) {
        (*ptr) = (Node *)malloc(sizeof(Node));
        if (!(*ptr)) {
            return NO_MEMORY;
        }
        (*ptr)->data = arr;
        (*ptr)->left = NULL;
        (*ptr)->right = NULL;
        return OK;
    }
    if ((*ptr)->data->name > arr->name) {
        return addNode(&((*ptr)->left), arr);
    }
    if ((*ptr)->data->name < arr->name) {
        return addNode(&((*ptr)->right), arr);
    }
    return OK;
}

void findNode(Node *ptr, Array **result, char name) {
    if (!ptr) {
        return;
    }
    if (ptr->data->name == name) {
        (*result) = ptr->data;
        return;
    }
    if (ptr->data->name > name) {
        findNode(ptr->left, result, name);
    }
    else {
        findNode(ptr->right, result, name);
    }
}

void freeNode(Node *ptr) {
    if (!ptr) {
        return;
    }
    freeNode(ptr->left);
    freeNode(ptr->right);
    free(ptr->data->data);
    free(ptr->data);
    free(ptr);
}

status_codes loadProcessing(Node **root, const char *filename, const char array_name) {
    FILE *stream = fopen(filename, "r");
    if (!stream) {
        return NO_FILE;
    }
    Array *arr = NULL;
    findNode((*root), &arr, array_name);
    int new_flag = 0;
    if (!arr) {
        if (initArray(&arr, array_name) == NO_MEMORY) {
            return NO_MEMORY;
        }
        new_flag = 1;
    }
    else {
        arr->size = 0;
        arr->capacity = 2;
    }
    switch (loadArray(arr, stream)) {
        case OK:
            fclose(stream);
            break;
        case NO_MEMORY:
            fclose(stream);
            return NO_MEMORY;
        case INVALID_INPUT:
            fclose(stream);
            return INVALID_INPUT;
    }
    if (new_flag) {
        if (addNode(root, arr) == NO_MEMORY) {
            fclose(stream);
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes saveProcessing(Node *root, const char *filename, const char array_name) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return UNINITIALIZED_VALUE;
    }
    FILE *stream = fopen(filename, "w");
    if (!stream) {
        return NO_FILE;
    }
    saveArray(arr, stream);
    fclose(stream);
    return OK;
}

status_codes randProcessing(Node **root, const char array_name, int count, int left_bound, int right_bound) {
    Array *arr = NULL;
    findNode((*root), &arr, array_name);
    int new_flag = 0;
    if (!arr) {
        if (initArray(&arr, array_name) == NO_MEMORY) {
            return NO_MEMORY;
        }
        new_flag = 1;
    }
    else {
        arr->size = 0;
        arr->capacity = 2;
    }
    if (randArray(arr, count, left_bound, right_bound) == NO_MEMORY) {
        return NO_MEMORY;
    }
    if (new_flag) {
        if (addNode(root, arr) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes concatProcessing(Node **root, const char dest_name, const char src_name) {
    Array *dest = NULL;
    Array *src = NULL;
    findNode((*root), &dest, dest_name);
    findNode((*root), &src, src_name);
    if (!src) {
        return UNINITIALIZED_VALUE;
    }
    int new_flag = 0;
    if (!dest) {
        if (initArray(&dest, dest_name) == NO_MEMORY) {
            return NO_MEMORY;
        }
        new_flag = 1;
    }
    if (concatArray(dest, src) == NO_MEMORY) {
        return NO_MEMORY;
    }
    if (new_flag) {
        if (addNode(root, dest) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes freeProcessing(Node *root, const char array_name) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return UNINITIALIZED_VALUE;
    }
    if (freeArray(arr) == NO_MEMORY) {
        return NO_MEMORY;
    }
    return OK;
}

status_codes removeProcessing(Node *root, const char array_name, const int start, const int count) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return UNINITIALIZED_VALUE;
    }
    if (start + count >= arr->size || start < 0 || count < 0) {
        return OUT_OF_BOUNDS;
    }
    removeArray(arr, start, count);
    return OK;
}

status_codes copyProcessing(Node **root, const char src_name, const char dest_name, const int start, const int end) {
    if (end < start) {
        return INVALID_INPUT;
    }
    Array *src = NULL;
    findNode((*root), &src, src_name);
    if (!src) {
        return UNINITIALIZED_VALUE;
    }
    if (start >= src->size || end >= src->size || start < 0 || end < 0) {
        return OUT_OF_BOUNDS;
    }
    Array *dest = NULL;
    findNode((*root), &dest, dest_name);
    int new_flag = 0;
    if (!dest) {
        if (initArray(&dest, dest_name) == NO_MEMORY) {
            return NO_MEMORY;
        }
        new_flag = 1;
    }
    else {
        dest->size = 0;
        dest->capacity = 2;
    }
    if (copyArray(src, start, end, dest) == NO_MEMORY) {
        return NO_MEMORY;
    }
    if (new_flag) {
        if (addNode(root, dest) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes sortProcessing(Node *root, const char array_name, int sort_flag) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return UNINITIALIZED_VALUE;
    }
    (sort_flag == 0) ? sort_increaseArray(arr) : (sort_flag == 1) ? sort_decreaseArray(arr) : shuffleArray(arr);
    return OK;
}

status_codes statsProcessing(Node *root, const char array_name) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return UNINITIALIZED_VALUE;
    }
    statsArray(arr);
    return OK;
}

status_codes printProcessing(Node *root, const char array_name, int start, int end) {
    Array *arr = NULL;
    findNode(root, &arr, array_name);
    if (!arr) {
        return UNINITIALIZED_VALUE;
    }
    if (start == -1 && end == -1) {
        start = 0;
        end = arr->size - 1;
    }
    if (start >= arr->size || end >= arr->size || start < 0 || end < 0) {
        return OUT_OF_BOUNDS;
    }
    printArray(arr, start, end);
    return OK;
}

int int_validation(const char *number) {
    const size_t size = strlen(number);
    if (!size) return 0;
    if (!isdigit(number[0]) && number[0] != '-') return 0;
    for (size_t i = 1; i < size; i++) {
        if (!isdigit(number[i])) return 0;
    }
    return 1;
}

int comp_increase(int *first, int *second) {
    return *first - *second;
}

int comp_decrease(int *first, int *second) {
    return *second - *first;
}

int comp_random(int *first, int *second) {
    return rand() % 3 - 1;
}
//rand A 10 -10 10;
//stats A;
//copy A ,0, 4, B;
//print A, all;
//print B, all;
//remove A, 3, 5;
//concat A, B;
//print A, all;
//print B, all;
//free(A);
//free(B);
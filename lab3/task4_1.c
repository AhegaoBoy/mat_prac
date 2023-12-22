#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define STR_SIZE 256

typedef enum status_realloc {
    status_realloc_ok,
    status_realloc_fail
} status_realloc;

typedef enum status_code {
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum status_free {
    status_free_ok,
    status_free_fail,
} status_free;


typedef struct My_string {
    unsigned int size;
    char* data;
} My_string;


int my_strlen(const My_string* str);
My_string* String(const char* str);
void string_clear(My_string* str);
int my_strcpy(My_string** destination, const My_string* source);
My_string* my_strcpy_new(const My_string* source);
int my_strcmp(My_string* left, My_string* right);
int is_equal(My_string* left, My_string* right);
int concat_strings(My_string** left, const My_string* right);
void print_str(My_string* str);
status_realloc my_realloc(void** var, int size);
status_free free_all(int count, ...);

int main(int argc, char* argv[]) {
    My_string* str = String("wow meow");
    print_str(str);
    printf("%d\n", my_strlen(str));
    string_clear(str);
    free(str);
    My_string* str_a = NULL;
    My_string* str_b = String("kek cheburek");
    int res = my_strcpy(&str_a, str_b);
    printf("%d--\n", res);
    print_str(str_a);
    print_str(str_b);
    string_clear(str_a);
    free(str_a);
    string_clear(str_b);
    free(str_b);
    str_b = String("kolokol");
    My_string* str_c = my_strcpy_new(str_b);
    print_str(str_c);
    string_clear(str_c);
    free(str_c);
    string_clear(str_b);
    free(str_b);
    str_b = String("aa");
    str_c = String("bb");
    int c = concat_strings(&str_b, str_c);
    print_str(str_b);
    string_clear(str_b);
    free(str_b);
    string_clear(str_c);
    free(str_c);
    str_b = String("bb");
    str_c = String("bbb");
    printf("%d--\n", my_strcmp(str_b, str_c));
    string_clear(str_b);
    free(str_b);
    string_clear(str_c);
    free(str_c);
    return 0;
}

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_realloc_ok;
    } else {
        return status_realloc_fail;
    }
}

status_free free_all(int count, ...) {
    if (count < 1) {
        return status_free_fail;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
        ptr_to_free = NULL;
    }

    va_end(ptr);
    return status_free_ok;
}

int my_strlen(const My_string* str) {
    int len = 0;
    while(str->data[len] != '\0') len++;
    return len;
}



My_string* String(const char* str) {
    int _size = strlen(str);
    My_string* new = malloc(sizeof(My_string));
    new->data = malloc(sizeof(char) * (_size + 1));
    if (str) {
        for (int i = 0; i < _size; i++) {
            new->data[i] = str[i];
        }
        new->data[_size] = '\0';
    }
    new->size = _size;
    return new;
}



void string_clear(My_string* str) {
    if (!str) {
        return;
    }
    free(str->data);
    str->data = NULL;
    str->size = 0;
}



int my_strcpy(My_string** destination, const My_string* source) {
    if (!source) {
        return 1;
    }
    if (*destination == NULL) {
        *destination = String(source->data);
        return source->size;
    }
    int size_dest = (*destination)->size;
    int size_source = source->size;
    if ((*destination)->data) {
        string_clear(*destination);
    }
    (*destination) = String(source->data);
    return size_source;
}



My_string* my_strcpy_new(const My_string* source) {
    if (!source) {
        return NULL;
    }
    int _size = source->size;
    My_string* new = malloc(sizeof(My_string));
    new->data = malloc(sizeof(char) * (_size + 1));
    if (source->data) {
        for (int i = 0; i < _size; i++) {
            new->data[i] = source->data[i];
        }
        new->data[_size] = '\0';
    }
    new->size = _size;
    return new;
}



int my_strcmp(My_string* left, My_string* right) {
    int len_comp = left->size - right->size;
    if (!len_comp) {
        int i = 0;
        while (left->data[i] != '\0' && left->data[i] == right->data[i]) {
            i++;
        }
        return left->data[i] - right->data[i];
    }
    return len_comp;
}

int is_equal(My_string* left, My_string* right) {
    int i = 0;
    while (left->data[i] != '\0' && left->data[i] == right->data[i]) {
        i++;
    }
    return left->data[i] == right->data[i] && right->size == left->size;
}

int concat_strings(My_string** left, const My_string* right) {
    if (!right->size) {
        return 1;
    }

    int new_size = (*left)->size + right->size;
    status_realloc st_realloc = my_realloc(&(*left)->data, new_size + 1);

    if (st_realloc == status_realloc_fail) {
        return -1;
    }

    int left_index = (*left)->size;
    int right_index = 0;

    while (right_index < right->size) {
        (*left)->data[left_index++] = right->data[right_index++];
    }

    (*left)->data[new_size] = '\0';
    (*left)->size = new_size;

    return 1;
}

void print_str(My_string* str) {
    if (!str) {
        return;
    }
    printf("%s\n", str->data);
}
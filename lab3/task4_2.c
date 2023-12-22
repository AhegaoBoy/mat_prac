#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "string.h"

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

typedef struct Adress {
    My_string* city;
    My_string* street;
    int house;
    My_string* block;
    int flat;
    My_string* index;
} Adress;

typedef struct Mail {
    Adress ad;
    double weight;
    My_string* id;
    My_string* time_create;
    My_string* time_get;
} Mail;

typedef struct Post {
    Adress* cur_id;
    Mail* mails;
    int size;
    int capacity;
} Post;

typedef enum status_cmd {
    cmd_exit,
    cmd_error_alloc,
    cmd_invalid_parameter,
    cmd_sort,
    cmd_create_post,
    cmd_change_post,
    cmd_add_mail,
    cmd_remove_mail,
    cmd_print_expired,
    cmd_print_non_expired,
    cmd_table,
    cmd_find_mail
} status_cmd;

status_code free_storage(Post** posts, int size);
status_code add_post_storage(Post* new_post, Post* new, int* capacity, int* size);
status_cmd command(char** arg_one, char** arg_two, My_string** info);
status_code create_adress(Adress** new_adress, const My_string* info);
status_code create_mail(Mail** new_mail, const My_string* info);
status_code create_post(Post** new_post, const My_string* info);
status_code add_mail(Post** cur_post, const My_string* info);
status_code remove_mail(Post* cur_post, const My_string* id);
bool check_equal(Adress* left, Adress* right);
int free_adress(Adress* tmp);
int free_mail(Mail* tmp);
bool is_sorted(Mail* mails, int size);
Post* find_post(Post* posts, const My_string* id, int size);
void get_date(My_string* tmp, int* day, int* month, int* year, int* hour, int* minute, int* seconds);
Mail* find_mail(Post* _mails, My_string id, int size);
int compare_mails(const void* a, const void* b);
int compare_mails_id(const void* a, const void* b);
status_code find_expired_mails(Post* post, Mail** exp_mails, int* size);
status_code find_non_expired_mails(Post* post, Mail** exp_mails, int* size);
int is_expired(const Mail* a);
int compare_mails_date_create(const void* a, const void* b);
void print_mails(Mail* mails, int size);
void print_menu();
void print_cur_post(Post*);
void print_info_mail(Mail* mail);


        int main(int argc, char* argv[]) {
    Post* storage_posts = NULL;
    char* arg_one = NULL;
    char* arg_two = NULL;
    My_string* info = NULL;
    status_cmd st_cmd;
    status_code st_activ;
    int capacity_storage = 32;
    int size_storage = 0;
    storage_posts = (Post*)malloc(sizeof(Post*) * (capacity_storage));
    Post* current = NULL;
    Mail* tmp_mail = NULL;
    Mail* tmp_find = NULL;
    int tmp_size = 32;
    while (true) {
        Post* tmp_post = NULL;
        print_menu();
        print_cur_post(current);
        st_cmd = command(&arg_one, &arg_two, &info);
        switch (st_cmd) {
            case cmd_exit:
                break;
            case cmd_error_alloc:
                printf("Error alloc detected!!!\n");
                break;
            case cmd_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                break;
            case cmd_sort:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                qsort(current->mails, current->size, sizeof(Mail), compare_mails);
                print_mails(current->mails, current->size);
                break;
            case cmd_create_post:
                st_activ = create_post(&tmp_post, info);
                if (!current) current = tmp_post;
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    st_activ = add_post_storage(storage_posts, tmp_post, &capacity_storage, &size_storage);
                    if (st_activ == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    } else if (st_activ == code_invalid_parameter) {
                        printf("Invalid parameter detected!!!\n");
                    }
                }
                break;
            case cmd_change_post:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                tmp_post = find_post(storage_posts, info, size_storage);
                if (!tmp_post) {
                    printf("This post doesn`t exist\n");
                } else {
                    current = tmp_post;
                }
                break;
            case cmd_add_mail:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                st_activ = add_mail(&current, info);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                }
                break;
            case cmd_remove_mail:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                st_activ = remove_mail(current, info);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                }
                break;
            case cmd_print_expired:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                st_activ = find_expired_mails(current, &tmp_mail, &tmp_size);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    qsort(tmp_mail, tmp_size, sizeof(Mail), compare_mails_date_create);
                    print_mails(tmp_mail, tmp_size);
                    free(tmp_mail);
                    tmp_mail = NULL;
                    tmp_size = 32;
                }
                break;
            case cmd_print_non_expired:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                st_activ = find_non_expired_mails(current, &tmp_mail, &tmp_size);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    qsort(tmp_mail, tmp_size, sizeof(Mail), compare_mails_date_create);
                    print_mails(tmp_mail, tmp_size);
                    free(tmp_mail);
                    tmp_mail = NULL;
                    tmp_size = 32;
                }
                break;
            case cmd_table:
                if (current) {
                    print_mails(current->mails, current->size);
                } else {
                    printf("Current post is empty!\n");
                }
                break;
            case cmd_find_mail:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                if (!is_sorted(current->mails, current->size)) {
                    qsort(current->mails, current->size, sizeof(Mail), compare_mails_id);
                }
                tmp_find = find_mail(current, *info, current->size);
                if (!tmp_find) {
                    printf("This mail does not exist in this post!\n");
                } else {
                    print_info_mail(tmp_find);
                    tmp_find = NULL;
                }
                break;
            default:
                break;
        }
        string_clear(info);
        free(info);
        info = NULL;
        if (st_cmd == cmd_error_alloc) {
            break;
        }
        if (st_activ == code_error_alloc) {
            break;
        }
        if (st_cmd == cmd_exit) {
            break;
        }
    }
    free_storage(&storage_posts, size_storage);
    free(storage_posts);
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

status_code create_adress(Adress** new_adress, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }
    *new_adress = (Adress*)malloc(sizeof(Adress));
    if (*new_adress == NULL) {
        return code_error_alloc;
    }
    int _size = info->size;
    status_free st_free;
    char* _city = malloc(sizeof(char) * _size);
    if (!_city) {
        st_free = free_all(2, *new_adress, _city);
        return code_error_alloc;
    }
    char* _street = malloc(sizeof(char) * _size);
    if (!_street) {
        st_free = free_all(3, *new_adress, _city, _street);
        return code_error_alloc;
    }
    char* _block = malloc(sizeof(char) * _size);
    if (!_block) {
        st_free = free_all(4, *new_adress, _city, _street, _block);
        return code_error_alloc;
    }
    char* _index = malloc(sizeof(char) * _size);
    if (!_index) {
        st_free = free_all(5, *new_adress, _city, _street, _block, _index);
        return code_error_alloc;
    }
    int check;
    check = sscanf(info->data ,"%s %s %d %s %d %s", _city, _street, &(*new_adress)->house, _block, &(*new_adress)->flat, _index);
    if (check != 6) {
        st_free = free_all(5, *new_adress, _city, _street, _block, _index);
        return code_invalid_parameter;
    }

    if (strlen(_index) > 6) {
        st_free = free_all(5, *new_adress, _city, _street, _block, _index);
        return code_invalid_parameter;
    }


    (*new_adress)->city = String(_city);
    (*new_adress)->street = String(_street);
    (*new_adress)->block = String(_block);
    (*new_adress)->index = String(_index);
    st_free = free_all(4,  _city, _street, _block, _index);
    if (st_free == status_free_fail) {
        return code_error_alloc;
    }
    //printf("        %s %s %d %s %d %s\n", _city, _street, (*new_adress)->house, _block, (*new_adress)->flat, _index);
    return code_success;
}

bool check_valid_time(My_string* time) {
    int day, month, year, hour, minute, sec;
    get_date(time, &day, &month, &year, &hour, &minute, &sec);
    if (day > 31) return false;
    if (month > 12) return false;
    if (hour >= 24) return false;
    if (minute >= 60) return false;
    if (sec >= 60) return false;
    return true;
}

status_code create_mail(Mail** new_mail, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }
    const double epsilon = 1e-8;

    *new_mail = (Mail*)malloc(sizeof(Mail));
    if (*new_mail == NULL) {
        return code_error_alloc;
    }
    int _size = info->size;
    int check;
    Adress* new_ad = NULL;
    status_free st_free;
    status_code st_adress = create_adress(&new_ad, info);
    if (st_adress == code_error_alloc) {
        free(*new_mail);
        return code_error_alloc;
    }
    (*new_mail)->ad = *new_ad;
    //printf("        %s %s %d %s %d %s\n", (*new_mail)->ad.city->data, (*new_mail)->ad.street->data, (*new_mail)->ad.house, (*new_mail)->ad.block->data, (*new_mail)->ad.flat, (*new_mail)->ad.index->data);
    char* _id = malloc(sizeof(char) * _size);
    if (!_id) {
        free(*new_mail);
        return code_error_alloc;
    }
    char* information = strdup(info->data);
    for (int i = 0; i < 6; ++i) {
        sscanf(information, "%s", _id);
        information += strlen(_id) + 1;
    }
    char* _time_create_date = malloc(sizeof(char) * _size);
    if (!_time_create_date) {
        st_free = free_all(2, *new_mail, _id);
        return code_error_alloc;
    }
    char* _time_create_time = malloc(sizeof(char) * _size);
    if (!_time_create_time) {
        st_free = free_all(3, *new_mail, _id,_time_create_date);
        return code_error_alloc;
    }
    char* _time_get_date = malloc(sizeof(char) * _size);
    if (!_time_get_date) {
        st_free = free_all(4, *new_mail, _id, _time_create_date, _time_create_time);
        return code_error_alloc;
    }
    char* _time_get_time = malloc(sizeof(char) * _size);
    if (!_time_get_time) {
        st_free = free_all(5, *new_mail, _id,_time_create_date, _time_create_time, _time_get_date);
        return code_error_alloc;
    }
    double _weight;
    sscanf(information, "%lf %s %s %s %s %s", &_weight, _id, _time_create_date, _time_create_time, _time_get_date, _time_get_time);

    if (_weight < epsilon || strlen(_id) > 14) {
        st_free = free_all(5, *new_mail, _id,_time_create_date, _time_create_time, _time_get_date);
        return code_invalid_parameter;
    }

    (*new_mail)->weight = _weight;
    (*new_mail)->id = String(_id);
    My_string* time_cr_date = String(_time_create_date);
    My_string* time_cr_time = String(_time_create_time);
    My_string* time_get_date = String(_time_get_date);
    My_string* time_get_time = String(_time_get_time);
    My_string* null_string = String(" ");
    concat_strings(&time_cr_date, null_string);
    concat_strings(&time_cr_date, time_cr_time);
    concat_strings(&time_get_date, null_string);
    concat_strings(&time_get_date, time_get_time);
    if (!check_valid_time(time_cr_date)) {
        string_clear(time_cr_time);
        string_clear(null_string);
        string_clear(time_get_time);
        st_free = free_all(3, time_get_time, null_string, time_cr_time);
        st_free = free_all(5, *new_mail, _id,_time_create_date, _time_create_time, _time_get_date);
        return code_invalid_parameter;
    }
    (*new_mail)->time_create = time_cr_date;
    if (!check_valid_time(time_get_date) && time_get_date) {
        string_clear(time_cr_time);
        string_clear(null_string);
        string_clear(time_get_time);
        st_free = free_all(3, time_get_time, null_string, time_cr_time);
        st_free = free_all(5, *new_mail, _id,_time_create_date, _time_create_time, _time_get_date);
        return code_invalid_parameter;
    }
    if (!my_strcmp(null_string, time_get_date)) {
        (*new_mail)->time_get = String("");
        string_clear(time_get_date);
        free(time_get_date);
    } else {
        (*new_mail)->time_get = time_get_date;
    }
    string_clear(time_cr_time);
    string_clear(null_string);
    string_clear(time_get_time);
    st_free = free_all(8, time_get_time, null_string, _id, time_cr_time, _time_get_time, _time_create_date, _time_get_date, _time_create_time);
    return code_success;
}


status_code create_post(Post** new_post, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }
    (*new_post) = (Post*)malloc(sizeof(Post));
    if (*new_post == NULL) {
        return code_error_alloc;
    }
    Adress* new_ad = NULL;
    status_code st_adress = create_adress(&new_ad, info);
    if (st_adress == code_error_alloc) {
        free_adress(new_ad);
        return code_error_alloc;
    }
    (*new_post)->size = 0;
    (*new_post)->capacity = 32;
    (*new_post)->mails = (Mail*)malloc(sizeof(Mail) * 32);
    if (!(*new_post)->mails) {
        free(*new_post);
        return code_error_alloc;
    }
    (*new_post)->cur_id = new_ad;
    return code_success;
}

status_code add_post_storage(Post* new_post, Post* new, int* capacity, int* size) {
    if (!new) {
        return code_invalid_parameter;
    }
    /*
    if (*new_post == NULL) {
        (*new_post) = malloc(sizeof(Post*) * (*capacity));
        if (*new_post == NULL) {
            return code_error_alloc;
        }
    }
    */
    new_post[*size] = *new;
    (*size)++;
    status_realloc st_realloc;
    if (*size == *capacity - 1) {
        *capacity *= 2;
        st_realloc = my_realloc(new_post, sizeof(Post*) * (*capacity));
        if (st_realloc == status_realloc_fail) {
            return code_error_alloc;
        }
    }
    return code_success;
}

status_code add_mail(Post** cur_post, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }

    Mail* new_mail = NULL;
    status_code st_mail = create_mail(&new_mail, info);
    if (st_mail == code_error_alloc) {
        return code_error_alloc;
    }

    Mail* check = NULL;
    if (!(*cur_post)) {
        check = find_mail((*cur_post), *new_mail->id, (*cur_post)->size);
        if (check != NULL) {
            //print_str(info);
            //print_str(new_mail->id);
            free_mail(new_mail);
            return code_success;
        }
    }
    int index = (*cur_post)->size;
    (*cur_post)->mails[index] = *new_mail;
    ((*cur_post)->size)++;
    if ((*cur_post)->size == (*cur_post)->capacity) {
        (*cur_post)->capacity *= 2;
        status_realloc st_realloc = my_realloc(&(*cur_post)->mails, sizeof(Mail) * (*cur_post)->capacity);
        if (st_realloc == status_realloc_fail) {
            free(new_mail);
            return code_error_alloc;
        }
    }

    return code_success;
}

status_code remove_mail(Post* cur_post, const My_string* id) {
    if (!cur_post || !id || !id->size) {
        return code_invalid_parameter;
    }
    int index_to_remove = -1;
    for (int i = 0; i < cur_post->size; i++) {
        if (is_equal(cur_post->mails[i].id, id)) {
            index_to_remove = i;
            break;
        }
    }
    if (index_to_remove == -1) {
        return code_invalid_parameter;
    } else {
        string_clear(cur_post->mails[index_to_remove].id);
        string_clear(cur_post->mails[index_to_remove].time_create);
        string_clear(cur_post->mails[index_to_remove].time_get);
        string_clear(cur_post->mails[index_to_remove].ad.city);
        string_clear(cur_post->mails[index_to_remove].ad.street);
        string_clear(cur_post->mails[index_to_remove].ad.block);
        string_clear(cur_post->mails[index_to_remove].ad.index);
    }
    for (int i = index_to_remove; i < cur_post->size - 1; i++) {
        cur_post->mails[i] = cur_post->mails[i + 1];
    }
    cur_post->size--;
    return code_success;
}


bool check_equal(Adress* left, Adress* right) {
    return is_equal(left->city, right->city) && is_equal(left->street, right->street) && is_equal(left->block, right->block) && is_equal(left->index, right->index) && left->house == right->house && left->flat == right->flat;
}

int free_adress(Adress* tmp) {
    if (!tmp) {
        return 0;
    }
    if (tmp->city->data) string_clear(tmp->city);
    if (tmp->street->data) string_clear(tmp->street);
    if (tmp->block->data) string_clear(tmp->block);
    if (tmp->index->data) string_clear(tmp->index);
    status_free st_free = free_all(4, tmp->city, tmp->street, tmp->block, tmp->index);
    if (st_free == status_free_fail) {
        return -1;
    }
    free(tmp);
    tmp = NULL;
    return 1;
}

int free_mail(Mail* tmp) {
    if (!tmp) {
        return 0;
    }
    if (tmp->id->data) string_clear(tmp->id);
    if (tmp->time_create->data) string_clear(tmp->time_create);
    if (tmp->time_get->data) string_clear(tmp->time_get);
    status_free st_free = free_all(3, tmp->id, tmp->time_create, tmp->time_get);
    //if (st_free == status_free_fail) {
    // return -1;
    //}
    int check = free_adress(&tmp->ad);
    //free(tmp);
    return check + 1;
}


Post* find_post(Post* posts, const My_string* id, int size) {
    if (!id->size || !posts) {
        return NULL;
    }
    //print_str((*new_post)[i]->cur_id->city);
    Adress* check = NULL;
    status_code st_adress = create_adress(&check, id);
    if (st_adress == code_error_alloc) {
        return NULL;
    }
    int _size = size;
    Post* current = &posts[0];
    for (int i = 0; i < _size; i++) {
        current = &posts[i];
        if (check_equal(check, current->cur_id)) {
            free(check);
            return current;
        }
    }
    return NULL;
}

bool is_sorted(Mail* mails, int size) {
    for (int i = 0; i < size - 1; i++) {
        Mail left = mails[i];
        Mail right = mails[i + 1];
        if (my_strcmp(left.id, right.id) > 0) {
            return false;
        }
    }
    return true;
}

void get_date(My_string* tmp, int* day, int* month, int* year, int* hour, int* minute, int* seconds) {
    if (!tmp) {
        return;
    }
    sscanf(tmp->data, "%d:%d:%d %d:%d:%d", day, month, year, hour, minute, seconds);
}

Mail* find_mail(Post* _mails, My_string id, int size) {
    if (!_mails) {
        return NULL;
    }
    int left_ind = 0;
    int right_ind = size - 1;
    int mid_ind;
    while (left_ind <= right_ind) {
        mid_ind = (left_ind + right_ind) / 2;
        if (my_strcmp(&id, _mails->mails[mid_ind].id) < 0) {
            right_ind = mid_ind - 1;
        } else if (my_strcmp(&id, _mails->mails[mid_ind].id) > 0) {
            left_ind = mid_ind + 1;
        } else {
            return &_mails->mails[mid_ind];
        }
    }
    return NULL;
}

int compare_mails(const void* a, const void* b) {
    int diff_id = my_strcmp(((Mail*)a)->ad.index, ((Mail*)b)->ad.index);
    if (diff_id > 0) {
        return 1;
    } else if (diff_id < 0) {
        return -1;
    }
    return my_strcmp(((Mail*)a)->id, ((Mail*)b)->id);
}

int compare_mails_id(const void* a, const void* b) {
    return my_strcmp(((Mail*)a)->id, ((Mail*)b)->id);
}


status_code find_expired_mails(Post* post, Mail** exp_mails, int* size) {
    if (!post) {
        return code_invalid_parameter;
    }
    (*exp_mails) = malloc(sizeof(Mail) * (*size));
    if (*exp_mails == NULL) {
        return code_error_alloc;
    }
    int tmp_size = post->size;
    Mail current;
    int index = 0;
    int check;
    for (int i = 0; i < tmp_size; i++) {
        current = post->mails[i];
        check = is_expired(&current);
        if (check == 1) {
            (*exp_mails)[index] = current;
            index++;
            if (index == *size - 1) {
                *size *= 2;
                status_realloc st_realloc = my_realloc(exp_mails, *size);
            }
        }
        check = 0;
    }
    *size = index;
    return code_success;
}

status_code find_non_expired_mails(Post* post, Mail** exp_mails, int* size) {
    if (!post) {
        return code_invalid_parameter;
    }
    (*exp_mails) = malloc(sizeof(Mail) * (*size));
    if (*exp_mails == NULL) {
        return code_error_alloc;
    }
    int tmp_size = post->size;
    Mail current;
    int index = 0;
    int check;
    for (int i = 0; i < tmp_size; i++) {
        current = post->mails[i];
        check = is_expired(&current);
        if (check == -1) {
            (*exp_mails)[index] = current;
            index++;
            if (index == *size - 1) {
                *size *= 2;
                status_realloc st_realloc = my_realloc(exp_mails, *size);
            }
        }
        check = 0;
    }
    *size = index;
    return code_success;
}

int is_expired(const Mail* a) {
    My_string* left_date_get = a->time_get;
    My_string* null_string = String("");
    if (left_date_get->size == 0|| my_strcmp(left_date_get, null_string) == 0) {
        string_clear(null_string);
        free(null_string);
        return 1;
    }
    int l_day, l_month, l_year, l_hour, l_minute, l_sec;
    get_date(left_date_get, &l_day, &l_month, &l_year, &l_hour, &l_minute, &l_sec);
    time_t raw_time;
    struct tm* time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    int diff_year, diff_month, diff_day, diff_hour, diff_minute, diff_sec;
    diff_year = l_year - time_info->tm_year - 1900;
    if (diff_year > 0) {
        return 1;
    } else if (diff_year < 0) {
        return -1;
    }
    diff_month = l_month - time_info->tm_mon - 1;
    if (diff_month > 0) {
        return 1;
    } else if (diff_month < 0) {
        return -1;
    }
    diff_day = l_day - time_info->tm_mday;
    if (diff_day > 0) {
        return 1;
    } else if (diff_day < 0) {
        return -1;
    }
    diff_hour = l_hour - time_info->tm_hour;
    if (diff_hour > 0) {
        return 1;
    } else if (diff_hour < 0) {
        return -1;
    }
    diff_minute = l_minute - time_info->tm_min;
    if (diff_minute > 0) {
        return 1;
    } else if (diff_minute < 0) {
        return -1;
    }
    diff_sec = l_sec - time_info->tm_sec;
    if (diff_sec > 0) {
        return 1;
    } else if (diff_sec < 0) {
        return -1;
    }
    string_clear(null_string);
    free(null_string);
    return 0;
}



int compare_mails_date_create(const void* a, const void* b) {
    Mail* left = (Mail*)a;
    Mail* right = (Mail*)b;
    My_string* left_date_create = left->time_create;
    My_string* right_date_create = right->time_create;
    if (!left_date_create && !right_date_create) {
        return 0;
    }
    if (!left_date_create) {
        return -1;
    }
    if (!right_date_create) {
        return 1;
    }
    int l_day, l_month, l_year, l_hour, l_minute, l_sec;
    get_date(left_date_create, &l_day, &l_month, &l_year, &l_hour, &l_minute, &l_sec);
    if (l_year < 100) {
        l_year += (l_year < 70) ? 2000 : 1900;
    }

    struct tm left_tm = {
            .tm_sec = l_sec,
            .tm_min = l_minute,
            .tm_hour = l_hour,
            .tm_mday = l_day,
            .tm_mon = l_month - 1,
            .tm_year = l_year - 1900
    };
    time_t left_time_create = mktime(&left_tm);
    int r_day, r_month, r_year, r_hour, r_minute, r_sec;
    get_date(right_date_create, &r_day, &r_month, &r_year, &r_hour, &r_minute, &r_sec);
    if (r_year < 100) {
        r_year += (r_year < 70) ? 2000 : 1900;
    }
    struct tm right_tm = {
            .tm_sec = r_sec,
            .tm_min = r_minute,
            .tm_hour = r_hour,
            .tm_mday = r_day,
            .tm_mon = r_month - 1,
            .tm_year = r_year - 1900
    };
    time_t right_time_create = mktime(&right_tm);

    return (int)(left_time_create - right_time_create);
}

void print_mails(Mail* mails, int size) {
    if (!mails || !size) {
        return;
    }
    printf("|===================================================================================================|\n");
    printf("|       Id      |    Weight   |        Time create       |        Time get          |     Index     |\n");
    printf("|===================================================================================================|\n");
    Mail* current;
    for (int i = 0; i < size; i++) {
        current = &mails[i];
        printf("|%14s | %11lf | %24s | %24s |%14s |\n", current->id->data, current->weight, current->time_create->data, current->time_get->data, current->ad.index->data);
    }
    printf("|===================================================================================================|\n");
}

status_cmd command(char** arg_one, char** arg_two, My_string** info) {
    char* cmd = (char*)malloc(sizeof(char) * STR_SIZE);
    if (!cmd) {
        return cmd_error_alloc;
    }
    int index = 0;
    char symbol = getchar();
    while (!isspace(symbol)) {
        cmd[index++] = symbol;
        symbol = getchar();
    }
    cmd[index] = '\0';
    index = 0;
    if (!strcmp(cmd, "Exit")) {
        free(cmd);
        return cmd_exit;
    } else if (!strcmp(cmd, "Sort")) {
        free(cmd);
        return cmd_sort;
    } else if (!strcmp(cmd, "Table")) {
        free(cmd);
        return cmd_table;
    } else if (!strcmp(cmd, "Create")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        free(*arg_one);
        return cmd_create_post;
    } else if (!strcmp(cmd, "Find")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        free(*arg_one);
        return cmd_find_mail;
    } else if (!strcmp(cmd, "Change")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        free(*arg_one);
        return cmd_change_post;
    } else if (!strcmp(cmd, "Add")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        free(*arg_one);
        return cmd_add_mail;
    } else if (!strcmp(cmd, "Remove")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        free(*arg_one);
        return cmd_remove_mail;
    } else if (!strcmp(cmd, "Print")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        if (!strcmp("expired", *arg_one)) {
            free(*arg_one);
            return cmd_print_expired;
        }
        if (!strcmp("non-expired", *arg_one)) {
            free(*arg_one);
            return cmd_print_non_expired;
        }
    }
    free(cmd);
    return cmd_invalid_parameter;
}

status_code free_post(Post* post) {
    //if (free_adress(&post->cur_id) != 1) {
    //return code_invalid_parameter;
    //}
    int _size = post->size;
    Mail* current;
    status_free st_free;
    for (int i = 0; i < _size; i++) {
        current = &post->mails[i];
        if (current->id->data) string_clear(current->id);
        if (current->time_create->data) string_clear(current->time_create);
        if (current->time_get->data) string_clear(current->time_get);
        if (current->ad.city->data) string_clear(current->ad.city);
        if (current->ad.street->data) string_clear(current->ad.street);
        if (current->ad.block->data) string_clear(current->ad.block);
        if (current->ad.index->data) string_clear(current->ad.index);
        st_free = free_all(7, current->id, current->time_create, current->time_get, current->ad.city, current->ad.street, current->ad.block, current->ad.index);
        free(current);
        current = NULL;
    }
    free(post->mails);
    post->mails = NULL;
    post->size = 0;
    post->capacity = 1;
    return code_success;
}

status_code free_storage(Post** posts, int size) {
    if (!(posts)) {
        return code_invalid_parameter;
    }
    for (int i = 0; i < size; i++) {
        free_post(posts[i]);
    }
    return code_success;
}
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef enum errors
{
    MEMORY_ALLOCATION_ERROR = 1,
    WRONG_QUANTITY_OF_ARGUMENTS,
    FILE_OPEN_ERROR,
    EMPTY_SEPARATOR,
    WORD_NOT_FOUND,
    EMPTY_STRUCTURE,
    FINE
}status_code;

struct binary_tree
{
    char* string;
    int quantity;
    struct binary_tree* left;
    struct binary_tree* right;
};

struct List
{
    char* string;
    int quantity;
    struct List* next;
};

struct queue_node
{
    struct binary_tree* node;
    struct queue_node* next;
};

struct queue
{
    struct queue_node* first;
    struct queue_node* last;
};

char *get_string(int *len);

status_code array_of_separators(int argc, char** argv, char** separators);
status_code read_from_file(FILE* input, struct binary_tree** tree, int n,  char* separators);
status_code read_string(FILE* input, char** str, int n, char* separators);
status_code create_node(struct binary_tree** tree);
status_code insert_string(struct binary_tree** tree, char* string);
status_code find_n_the_most_frequency(struct binary_tree* tree, struct List** list, int n);
status_code find_word(struct binary_tree* tree, char* str, int* n);
status_code create_node_of_list(struct List** list, char* string, int quantity);
status_code insert_data_in_list(struct List** list, char* string, int quantity);
status_code print_list(struct List* list, int n);
status_code find_the_longest(struct binary_tree* tree, char** the_longest, int* length);
status_code find_the_shortest(struct binary_tree* tree, char** the_shortest, int* length);

status_code new_queue(struct queue** queue);
status_code push(struct queue** queue, struct binary_tree* node);
status_code pop(struct queue** queue, struct binary_tree** node);

status_code BFS(struct binary_tree* tree, struct queue** queue);
void clear_tree(struct binary_tree* tree);
void print_tree(struct binary_tree* tree, int depth);
void find_depth(struct binary_tree* tree, int depth, int* max);
void clear_list(struct List* list);
int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("WRONG QUANTITY 0F ARGUMENTS\n");
        return WRONG_QUANTITY_OF_ARGUMENTS;
    }

    FILE* input_file = NULL;
    input_file = fopen(argv[1], "r");
    if(input_file == NULL)
    {
        printf("FILE COULDN'T BEEN OPENED\n");
        return FILE_OPEN_ERROR;
    }
    status_code statusCode;
    char* separators;
    int quantity_of_separators;
    char* dialog_string;
    int len;
    struct binary_tree* tree = NULL;



    statusCode = array_of_separators(argc, argv, &separators);
    if(statusCode != FINE)
    {
        fclose(input_file);
        switch (statusCode) {
            case EMPTY_SEPARATOR:
                printf("YOU'VE INPUT EMPTY SEPARATOR\n");
                return EMPTY_SEPARATOR;
            case MEMORY_ALLOCATION_ERROR:
                printf("MEMORY ALLOCATION ERROR\n");
                return MEMORY_ALLOCATION_ERROR;
        }
    }
    quantity_of_separators = strlen(separators);
    statusCode = read_from_file(input_file, &tree, quantity_of_separators, separators);
    if(statusCode != FINE)
    {
        free(separators);
        fclose(input_file);
        printf("MEMORY ALLOCATION ERROR\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    fclose(input_file);
    printf("\tBuilt tree:\n");
    if(tree)print_tree(tree, 0);

    do {
        printf("\n\tWhat do you want to do?\nFind string - insert 'find'\nFind n with the most frequency - insert 'find_n'\nFind the longest - insert 'find_the_longest'\nFind the shortest - insert 'find_the_shortest'\nTo output depth of binary tree - insert 'depth'\nSave nodes of tree in file - insert 'save'\n");
        dialog_string = get_string(&len);
        if(strcmp(dialog_string, "find") == 0)
        {
            printf("Insert string you want to find in tree:");
            char* str_to_find;
            int len1;
            str_to_find = get_string(&len1);
            if(str_to_find == NULL)
            {
                clear_tree(tree);
                free(separators);
                free(dialog_string);
                free(separators);
                return MEMORY_ALLOCATION_ERROR;
            }
            int found;
            statusCode = find_word(tree, str_to_find, &found);
            if(statusCode != FINE)
            {
                printf("NO SUCH STRING\n");
                continue;
            }
            printf("String %s was found: %d\n", str_to_find, found);
            free(str_to_find);
        }
        else if(strcmp(dialog_string, "find_n") == 0)
        {
            printf("Insert quantity of strings you want to output:");
            int amount;
            scanf("%d", &amount);

            struct List* head = NULL;
            find_n_the_most_frequency(tree, &head, amount);
            printf("\tThe list of strings according to your request\n");
            if(print_list(head, amount) == EMPTY_STRUCTURE)
            {
                printf("THE TREE IS EMPTY\n");
                continue;
            }
            clear_list(head);

        }
        else if(strcmp(dialog_string, "find_the_longest") == 0)
        {
            char* the_longest = NULL;
            int len_the_longest = 0;
            if(find_the_longest(tree, &the_longest, &len_the_longest) == MEMORY_ALLOCATION_ERROR)
            {
                printf("MEMORY ALLOCATION ERROR\n");
                free(dialog_string);
                clear_tree(tree);
                free(separators);
                return MEMORY_ALLOCATION_ERROR;
            }

            if(the_longest == NULL)
            {
                printf("NO SUCH WORD\n");
                continue;
            }
            printf("The longest word in tree: %s\n", the_longest);
            free(the_longest);
        }
        else if(strcmp(dialog_string, "find_the_shortest") == 0)
        {
            char* the_shortest = NULL;
            int len_the_shortest = 1024;
            if(find_the_shortest(tree, &the_shortest, &len_the_shortest) == MEMORY_ALLOCATION_ERROR)
            {
                printf("MEMORY ALLOCATION ERROR\n");
                free(dialog_string);
                clear_tree(tree);
                free(separators);
                return MEMORY_ALLOCATION_ERROR;
            }

            if(the_shortest == NULL)
            {
                printf("NO SUCH WORD\n");
                continue;
            }
            printf("The shortest word in tree: %s\n", the_shortest);
            free(the_shortest);
        }
        else if(strcmp(dialog_string, "save") == 0)
        {
            char* path;
            int len_of_path;
            printf("Input path of file you want save tree data:");
            path = get_string(&len_of_path);
            if(path == NULL)
            {
                printf("MEMORY ALLOCATION ERROR\n");
                free(dialog_string);
                free(separators);
                clear_tree(tree);
                return MEMORY_ALLOCATION_ERROR;
            }
            FILE* dest = NULL;
            dest = fopen(path, "w");
            if(dest == NULL)
            {
                printf("COULDN'T OPEN FILE %s\n", path);
                free(path);
                free(dialog_string);
                clear_tree(tree);
                return FILE_OPEN_ERROR;
            }
            if(tree == NULL)
            {
                fclose(dest);
                struct binary_tree* for_null_tree = NULL;
                statusCode = read_from_file(input_file, &for_null_tree, quantity_of_separators, separators);
                if(statusCode != FINE)
                {
                    free(separators);
                    fclose(dest);
                    printf("MEMORY ALLOCATION ERROR\n");
                    return MEMORY_ALLOCATION_ERROR;
                }
                printf("\tTree after reading from file %s\n", path);
                continue;
            }

            struct queue* for_BFS = NULL;
            BFS(tree, &for_BFS);

            while(for_BFS->first!=NULL)
            {
                struct binary_tree* tmp;
                pop(&for_BFS, &tmp);
                for(int i = 0; i < tmp->quantity; ++i)
                    fprintf(dest,"%s ", tmp->string);
            }
            free(for_BFS);
            for_BFS = NULL;
            fclose(dest);
            struct binary_tree* new_tree = NULL;
            dest = fopen(path, "r");
            if(dest == NULL)
            {
                printf("COULDN'T OPEN FILE %s\n", path);
                free(path);
                free(dialog_string);
                clear_tree(tree);
                return FILE_OPEN_ERROR;
            }

            statusCode = read_from_file(input_file, &new_tree, quantity_of_separators, separators);
            if(statusCode != FINE)
            {
                free(separators);
                fclose(dest);
                printf("MEMORY ALLOCATION ERROR\n");
                return MEMORY_ALLOCATION_ERROR;
            }
            printf("\tTree after reading from file %s\n", path);
            print_tree(new_tree, 0);
            clear_tree(new_tree);
        }
        else if(strcmp(dialog_string, "depth") == 0)
        {
            int depth_of_tree = 0;
            find_depth(tree, 0, &depth_of_tree);
            printf("Depth of tree: %d\n", depth_of_tree);
        }
        else if(strcmp(dialog_string, "end") != 0)printf("Invalid command, try again\n\n");
        else break;
    }while(strcmp(dialog_string, "end") != 0);

    printf("ez thx4tutorial\n");
    clear_tree(tree);
    free(dialog_string);
    return 0;
}

status_code array_of_separators(int argc, char** argv, char** separators)
{
    (*separators) = (char*)malloc(sizeof(char) * (argc-1));
    if(*separators == NULL) return MEMORY_ALLOCATION_ERROR;
    int i;
    int len = 0;
    for(i = 2; i < argc; i++)
    {
        len = strlen(argv[i]);
        if(len == 2)
        {
            if(argv[i][0] == argv[i][1] && argv[i][1] == '\'')
            {
                (*separators) = strdup("");
                if(*separators == NULL)
                {
                    free(separators);
                    return MEMORY_ALLOCATION_ERROR;
                }
                break;
            }
        }
        if(len == 1 && i < argc - 1)
        {
            if(strlen(argv[i+1]) == 1)
            {
                if(argv[i][0] == '\'' && argv[i+1][0] == '\'')
                {
                    argc--;
                    argv[i+1] = argv[argc];
                    (*separators)[i-2] = ' ';
                    i++;
                }
            }
        }
        else if(len > 4 || len < 3 || argv[i][0] != '\'' || argv[i][len-1] != '\'')
        {
            free(separators);
            return EMPTY_SEPARATOR;
        }
        if(argv[i][1] == '\\')
        {
            switch(argv[i][2])
            {
                case 'n':
                {
                    (*separators)[i-2] = '\n';
                    break;
                }
                case 't':
                {
                    (*separators)[i-2] = '\t';
                    break;
                }
                case '\'':
                {
                    (*separators)[i-2] = '\\';
                    break;
                }
                default:
                {
                    printf("like... why\n");
                    free(*separators);
                    return 0;
                }
            }
        }
        else (*separators)[i-2] = argv[i][1];
    }
    if(*separators == NULL) return MEMORY_ALLOCATION_ERROR;
    (*separators)[argc-2] = '\0';
    return FINE;
}

char *get_string(int *len)
{
    *len = 0;
    int capacity = 1;
    char *s = (char*) malloc(sizeof(char));
    if(s == NULL) return NULL;

    char c = getchar();

    do{
        if(c!= ' ' && c != '\t' && c != '\n') s[(*len)++] = c;

        if (*len >= capacity) {
            capacity *= 2;
            s = (char*) realloc(s, capacity * sizeof(char));
        }

        c = getchar();
    }while (c != '\n' && c!='\0' && c!=' ' && c!='\t');

    s[*len] = '\0';

    return s;
}

status_code read_from_file(FILE* input, struct binary_tree** tree, int n, char* separators)
{
    int check;
    while(!feof(input))
    {
        char* string;
        check = read_string(input, &string,n, separators);
        if(check != FINE) return check;
        if(string[0] == '\0') continue;
        check = insert_string(tree, string);
        if(check != FINE) return check;

    }
    return FINE;
}

status_code read_string(FILE* input, char** str, int n, char* separators)
{
    int c = 1;
    char* string = (char*)malloc(sizeof(char));
    if(string == NULL) return MEMORY_ALLOCATION_ERROR;
    int len = 0;
    int capacity = 1;

    while(!feof(input))
    {
        c = getc(input);
        if(c == EOF)
        {
            strcpy(*str, string);
            return FINE;
        }
        for(int i = 0; i < n; ++i)
        {
            if(c == separators[i])
            {
                strcpy(*str, string);
                (*str)[len] = '\0';
                return FINE;
            }
        }
        if(len == capacity)
        {
            char* for_realloc = realloc(string, sizeof(char) * 2 * capacity);
            if(for_realloc == NULL)
            {
                free(string);
                return MEMORY_ALLOCATION_ERROR;
            }
            string = for_realloc;
            capacity<<=1;
        }
        string[len++] = c;
    }
    string[len] = '\0';
    return FINE;
}

status_code create_node_of_list(struct List** list, char* string, int quantity)
{
    (*list) = (struct List*)malloc(sizeof(struct List));
    if(*list == NULL) return MEMORY_ALLOCATION_ERROR;
    (*list)->quantity = quantity;
    (*list)->string = (char*)malloc(sizeof(char) * strlen(string));
    if((*list) -> string == NULL) return MEMORY_ALLOCATION_ERROR;
    strcpy((*list) -> string, string);
    (*list)->next = NULL;
    return FINE;
}

status_code create_node(struct binary_tree** tree)
{
    (*tree) = (struct binary_tree*)malloc(sizeof(struct binary_tree));
    if(tree == NULL)
    {
        return MEMORY_ALLOCATION_ERROR;
    }
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    (*tree)->quantity = 1;
    return FINE;
}

status_code insert_data_in_list(struct List** list, char* string, int quantity)
{
    struct List* head = *list;
    struct List* prev = NULL;
    struct List* new_node;
    int check_create_of_list_node = create_node_of_list(&new_node, string, quantity);
    if(check_create_of_list_node != FINE)
    {
        free(new_node->string);
        free(new_node);
        return check_create_of_list_node;
    }
    if(head == NULL)
    {
        *list = new_node;
        return FINE;
    }
    while(head != NULL)
    {
        if(quantity >= head -> quantity)
        {
            new_node -> next = head->next;
            head->next = new_node;
            return FINE;
        }
        else
        {
            prev = head;
            head = head -> next;
        }

    }
    new_node->next = NULL;
    prev ->next = new_node;
    return FINE;
}

status_code print_list(struct List* list, int n)
{
    if(list == NULL) return EMPTY_STRUCTURE;
    struct List* head = list;
    int iter = 0;
    while(iter != n && head != NULL)
    {
        printf("%s %d\n", head->string, head->quantity);
        head = head -> next;
        iter++;
    }
    return FINE;
}

void clear_list(struct List* list)
{
   while(list != NULL)
   {
        struct List* for_free = list;
        list = list->next;
        free(for_free -> string);
        free(for_free);
   }
}
status_code insert_string(struct binary_tree** tree, char* str)
{
    if(*tree == NULL)
    {
        create_node(tree);
        (*tree)->string = (char*)malloc(sizeof(char) * strlen(str));
        if(*tree == NULL) return MEMORY_ALLOCATION_ERROR;
        strcpy((*tree) -> string, str);
        return FINE;
    }

    struct binary_tree* root2 = *tree;
    struct binary_tree* parent = NULL;
    while(root2 != NULL)
    {
        parent = root2;
        if(strcmp(root2->string, str) < 0) root2 = root2 -> right;
        else if(strcmp(root2->string, str) > 0) root2 = root2 -> left;
        else
        {
            root2 -> quantity++;
            return FINE;
        }
    }
    struct binary_tree* tmp;
    create_node(&tmp);
    if(strcmp(parent->string, str) < 0)
    {
        tmp->string = (char*)malloc(sizeof(char) * strlen(str));
        if(tmp -> string == NULL) return MEMORY_ALLOCATION_ERROR;
        strcpy(tmp->string, str);
        parent->right = tmp;
        return FINE;
    }
    else
    {
        tmp->string = (char*)malloc(sizeof(char) * strlen(str));
        if(tmp -> string == NULL) return MEMORY_ALLOCATION_ERROR;
        strcpy(tmp->string, str);
        parent->left = tmp;
        return FINE;
    }
}

status_code find_n_the_most_frequency(struct binary_tree* tree, struct List** list, int n)
{
    if(tree == NULL) return FINE;

    find_n_the_most_frequency(tree->left, list, n);
    insert_data_in_list(list, tree->string, tree->quantity);
    find_n_the_most_frequency(tree->right, list, n);
}

status_code find_word(struct binary_tree* tree, char* str, int* n)
{
    struct binary_tree* root = tree;
    while(root != NULL)
    {
        if(strcmp(root->string, str) < 0) root = root->right;
        else if(strcmp(root->string, str) > 0) root = root -> left;
        else
        {
            (*n) = root ->quantity;
            return FINE;
        }
    }
    return WORD_NOT_FOUND;
}

status_code find_the_shortest(struct binary_tree* tree, char** the_shortest, int* length)
{
    if(tree == NULL) return FINE;
    find_the_shortest(tree->left, the_shortest, length);
    int len = strlen(tree->string);
    if(len < (*length))
    {
        if(*the_shortest != NULL) free(*the_shortest);
        (*the_shortest) = (char*)malloc(sizeof(char) * len);
        if(*the_shortest == NULL) return MEMORY_ALLOCATION_ERROR;
        strcpy(*the_shortest, tree->string);
        (*length) = len;
    }
    find_the_shortest(tree->right, the_shortest, length);
}

status_code find_the_longest(struct binary_tree* tree, char** the_longest, int* length)
{
    if(tree == NULL) return FINE;
    find_the_longest(tree->left, the_longest, length);
    int len = strlen(tree->string);
    if(len > (*length))
    {
        if(*the_longest != NULL) free(*the_longest);
        (*the_longest) = (char*)malloc(sizeof(char) * len);
        if(*the_longest == NULL) return MEMORY_ALLOCATION_ERROR;
        strcpy(*the_longest, tree->string);
        (*length) = len;
    }
    find_the_longest(tree->right, the_longest, length);
}

void clear_tree(struct binary_tree* tree)
{
    if(tree == NULL) return;
    clear_tree(tree -> left);
    clear_tree(tree -> right);
    free(tree -> string);
    free(tree);
}

void find_depth(struct binary_tree* tree, int depth, int* max)
{
    if(tree == NULL)
    {
        return;
    }
    find_depth(tree->left, depth++, max);
    if(depth > (*max)) (*max) = depth;
    find_depth(tree->right, ++depth, max);
}

void print_tree(struct binary_tree* tree, int depth)
{
    if(tree == NULL)
    {
        return;
    }
    for(int i = 0; i < depth; ++i) printf("\t");
    printf("%s %d\n", tree->string, tree->quantity);
    print_tree(tree->right, ++depth);
    print_tree(tree->left, depth++);
}

status_code new_queue(struct queue** queue)
{
    (*queue) = (struct queue*)malloc(sizeof(struct queue));
    if(*queue == NULL) return MEMORY_ALLOCATION_ERROR;
    (*queue)->first = NULL;
    (*queue)->last = NULL;
    return FINE;
}

status_code push(struct queue** queue, struct binary_tree* node)
{
    struct queue_node* new = (struct queue_node*)malloc(sizeof (struct queue_node));
    if(new == NULL) return MEMORY_ALLOCATION_ERROR;

    new->next = NULL;
    new->node = (struct binary_tree*)malloc(sizeof (struct binary_tree));
    if(new->node == NULL)
    {
        free(new);
        return MEMORY_ALLOCATION_ERROR;
    }

    new->node->string = (char*)malloc(sizeof (char) * strlen(node->string));
    if(new->node->string == NULL)
    {
        free(new->node);
        free(new);
        return MEMORY_ALLOCATION_ERROR;
    }
    strcpy(new->node->string, node->string);
    new->node->quantity = node->quantity;
    new->node->left = node->left;
    new->node->right = node->right;
    if((*queue)->first == NULL)
    {
        (*queue)->first = new;
        (*queue)->last = (*queue)->first;
        return FINE;
    }

    (*queue)->last->next = new;
    (*queue)->last = new;
    (*queue)->last->next = NULL;
    return FINE;
}

status_code pop(struct queue** queue, struct binary_tree** node)
{
    (*node) = (struct binary_tree*)malloc(sizeof(struct binary_tree));
    if(*node == NULL) return MEMORY_ALLOCATION_ERROR;
    struct queue_node* tmp = (*queue)->first;

    (*node)->string = (char*)malloc(sizeof(char) * strlen((*queue)->first->node->string));
    if((*node)->string == NULL) return MEMORY_ALLOCATION_ERROR;

    strcpy((*node)->string, (*queue)->first->node->string);
    (*node)->quantity = (*queue)->first->node->quantity;
    (*node)->right = (*queue)->first->node->right;
    (*node)->left = (*queue)->first->node->left;

    free((*queue)->first->node->string);
    free((*queue)->first->node);

    (*queue) -> first = (*queue) -> first -> next;
    free(tmp);
    return FINE;
}

status_code BFS(struct binary_tree* tree, struct queue** queue)
{
    struct queue* for_nodes;
    struct binary_tree* to_go = tree;
    new_queue(&for_nodes);
    new_queue(queue);
    push(&for_nodes, tree);

    while(for_nodes->first!=NULL)
    {
        struct binary_tree* tmp;
        pop(&for_nodes, &tmp);
        push(queue, tmp);

        if(tmp->left!=NULL) push(&for_nodes, tmp->left);
        if(tmp->right!=NULL) push(&for_nodes, tmp->right);
    }
    return FINE;
}
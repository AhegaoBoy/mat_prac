#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum errors
{
    MEMORY_ALLOCATION_ERROR = 1,
    WRONG_QUANTITY_OF_ARGUMENTS,
    FILE_OPEN_ERROR,
    NOT_BKT_SEQUENCE,
    FINE
}status_code;

struct tree
{
    char letter;
    struct tree* first_child;
    struct tree* brother;
};

struct stack
{
    char bkt;
    struct stack* next;
};

struct stack_of_tree_nodes
{
    struct tree* nodes;
    struct stack_of_tree_nodes* next;
};

struct list
{
    struct tree* node;
    struct list* next;
};

struct queue_node
{
    struct tree* tree_node;
    struct queue_node* next;
};

struct queue
{
    struct queue_node* first;
    struct queue_node* last;
};

status_code read_string_from_file(FILE* input, char** string);
status_code push(struct stack** stack, char c);
char pop(struct stack** stack);
status_code check_if_string_of_bkt_is_correct(struct stack** stack, char* string);
status_code push_back(struct stack_of_tree_nodes** stack, char root, char* children);
struct tree* popop(struct stack_of_tree_nodes** stack);
status_code rutiskhauzer_alogorythm(struct stack_of_tree_nodes** stack, char* sequence);
status_code create_parent_and_children_nodes(struct tree** node, char root, char* children);
status_code create_tree(struct tree** tree, struct stack_of_tree_nodes** stack);
status_code remove_substring(char** string, int start, int end, int* new_length);
void print_tree_in_file(FILE* output, struct tree* tree, int depth);

status_code new_queue(struct queue** queue);
status_code push_queue(struct queue** queue, struct tree* node);
struct tree* pop_queue(struct queue** queue);
status_code add_to_list(struct list** list, struct tree* node);
struct tree* find_lost_node_in_list(struct list** list, struct tree* node);
void clear_list(struct list** list);

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("WRONG QUANTITY OF ARGUMENTS, PLEASE, INPUT <OUTSOURCE FILE DIRECTORY> <RESULT FILE DIRECTORY>");
        return WRONG_QUANTITY_OF_ARGUMENTS;
    }

    FILE* input = NULL;
    input = fopen(argv[1], "r");
    if(input == NULL)
    {
        printf("FILE COULDN'T BEEN OPENED\n");
        return FILE_OPEN_ERROR;
    }

    char* string_of_bkt;
    struct stack* stack_of_bkt = NULL;
    struct tree* bxt_tree = NULL;
    struct stack_of_tree_nodes* stack_of_nodes = NULL;
    status_code check;

    check = read_string_from_file(input, &string_of_bkt);
    check = check_if_string_of_bkt_is_correct(&stack_of_bkt, string_of_bkt);
    check = rutiskhauzer_alogorythm(&stack_of_nodes, string_of_bkt);
    check = create_tree(&bxt_tree,&stack_of_nodes);
    if(check == FINE) printf("GOOD JOB\n");
    else printf("MAY BE IT WORKS MAY BE NOT\n");
    print_tree_in_file(input, bxt_tree, 0);
    fclose(input);
}

status_code read_string_from_file(FILE* input, char** string)
{
    char* str = (char*)malloc(sizeof(char));
    if(str == NULL)
        return MEMORY_ALLOCATION_ERROR;
    int len = 0;
    int capacity = 1;
    char c = 1;
    while(!feof(input))
    {
        c = getc(input);
        if(c == EOF)
        {
            (*string) = (char*)malloc(sizeof(char) * len);
            strcpy(*string, str);
            return FINE;
        }
        if(c == '\n')
        {
            strcpy(*string, str);
            return FINE;
        }
        if(len == capacity)
        {
            char* for_realloc = realloc(str, sizeof(char) * 2 * capacity);
            if(for_realloc == NULL)
            {
                free(str);
                return MEMORY_ALLOCATION_ERROR;
            }
            str = for_realloc;
            capacity<<=1;
        }
        str[len++] = c;
    }
    return FINE;
}

status_code push(struct stack** stack, char c)
{
    struct stack* node = (struct stack*)malloc(sizeof(struct stack));
    if(node == NULL) return MEMORY_ALLOCATION_ERROR;
    node->next = NULL;
    node->bkt = c;
    if(*stack == NULL)
    {
        (*stack) = node;
        return FINE;
    }

    node->next = (*stack);
    (*stack) = node;
    return FINE;
}

char pop(struct stack** stack)
{
    if((*stack)->next == NULL)
    {
        char c = (*stack)->bkt;
        free(*stack);
        (*stack) = NULL;
        return c;
    }
    struct stack* tmp = (*stack);
    (*stack) = (*stack)->next;
    char c = tmp->bkt;
    free(tmp);
    return c;
}

status_code check_if_string_of_bkt_is_correct(struct stack** stack, char* string)
{
    int length = strlen(string);
    for(int i = 0; i < length; ++i)
    {
        if(string[i] == '(' || string[i] == '{' || string[i] == '[')
        {
            status_code check = push(stack, string[i]);
            if(check != FINE) return check;
        }
        else
        {
            if(string[i] == ')')
            {
                char bkt = pop(stack);
                if(bkt != '(') return NOT_BKT_SEQUENCE;
            }
            else if(string[i] == ']')
            {
                char bkt = pop(stack);
                if(bkt != '[') return NOT_BKT_SEQUENCE;
            }
            else if(string[i] == '}')
            {
                char bkt = pop(stack);
                if(bkt != '{') return NOT_BKT_SEQUENCE;
            }
        }
    }
    if(*stack != NULL) return NOT_BKT_SEQUENCE;
    return FINE;
}

status_code create_parent_and_children_nodes(struct tree** node, char root, char* children)
{
    (*node) = (struct tree*)malloc(sizeof(struct tree));
    if(*node == NULL) return MEMORY_ALLOCATION_ERROR;
    (*node)->letter = root;
    (*node)->brother = NULL;
    (*node)->first_child = (struct tree*)malloc(sizeof(struct tree));
    struct tree* child = (*node)->first_child;

    int len = strlen(children);
    for(int i = 0; i < len; ++i)
    {
        child -> letter = children[i];
        child -> first_child = NULL;
        child->brother = (struct tree*)malloc(sizeof(struct tree));
        if(child->brother == NULL) return MEMORY_ALLOCATION_ERROR;
        child = child -> brother;
    }
    free(child->brother);
    child = child->brother;
    child = NULL;
    return FINE;
}

status_code push_back(struct stack_of_tree_nodes** stack, char root, char* children)
{
    struct stack_of_tree_nodes* node = (struct stack_of_tree_nodes*)malloc(sizeof(struct stack_of_tree_nodes));
    if(node == NULL) return MEMORY_ALLOCATION_ERROR;
    node->next = NULL;
    node->nodes = (struct tree*)malloc(sizeof(struct tree));

    if(node->nodes == NULL) return MEMORY_ALLOCATION_ERROR;
    create_parent_and_children_nodes(&(node->nodes), root, children);
    if(*stack == NULL)
    {
        (*stack) = node;
        return FINE;
    }
    node->next = (*stack);
    (*stack) = node;
    return FINE;
}

struct tree* popop(struct stack_of_tree_nodes** stack)
{
    if((*stack) -> next == NULL)
    {
        struct tree* tmp = (*stack)->nodes;
        free(*stack);
        (*stack) = NULL;
        return tmp;
    }
    struct stack_of_tree_nodes* top = (*stack);
    (*stack) = (*stack)->next;
    struct tree* result = top->nodes;
    free(top);
    return result;
}

status_code remove_substring(char** string, int start, int end, int* new_length)
{
    char* new_string = (char*)malloc(sizeof(char) * ((*new_length) - (end - start + 2)));
    if(new_string == NULL) return MEMORY_ALLOCATION_ERROR;
    for(int i = 0; i < start; ++i)
         new_string[i] = (*string)[i];
    for(int i = end + 1; i < *new_length; ++i)
        new_string[start + (i - end - 1)] = (*string)[i];
    strcpy(*string, new_string);
    (*new_length) = (*new_length) - (end - start + 1);
    free(new_string);
    return FINE;
}


status_code rutiskhauzer_alogorythm(struct stack_of_tree_nodes** stack, char* sequence)
{
    int length = strlen(sequence);
    if(length == 1)
    {
        struct tree* node = (struct tree*)malloc(sizeof(struct tree));
        if(node == NULL) return MEMORY_ALLOCATION_ERROR;
        node->brother = NULL;
        node->first_child = NULL;
        node->letter = sequence[0];

        struct stack_of_tree_nodes* tmp = (struct stack_of_tree_nodes*)malloc(sizeof(struct stack_of_tree_nodes));
        tmp->next = NULL;
        tmp->nodes = node;
        (*stack) = tmp;
    }

    while(length != 1)
    {
        int* array_of_coeffecients = (int*)malloc(sizeof(int) * length);
        if(array_of_coeffecients == NULL) return MEMORY_ALLOCATION_ERROR;
        char parent;
        int pos_of_max = 0;
        int prev_symbol = 0;
        int max = prev_symbol;
        if(length == 3)
        {
            struct tree* brothers = (struct tree*)malloc(sizeof(struct tree));
            if(brothers == NULL) return MEMORY_ALLOCATION_ERROR;

            brothers->letter = sequence[0];
            brothers->first_child = NULL;
            brothers->brother = (struct tree*)malloc(sizeof(struct tree));
            if(brothers->brother == NULL) return MEMORY_ALLOCATION_ERROR;
            brothers->brother->letter = sequence[2];
            brothers->brother->brother = NULL;
            brothers->first_child = NULL;

            struct stack_of_tree_nodes* tmp = (struct stack_of_tree_nodes*)malloc(sizeof(struct stack_of_tree_nodes));
            if(tmp == NULL) return MEMORY_ALLOCATION_ERROR;
            tmp->next = NULL;
            tmp->nodes = brothers;
            if(*stack == NULL)
                (*stack) = tmp;
            else
            {
                tmp->next = (*stack);
                (*stack) = tmp;
            }
            return FINE;

        }

        for(int i = 0; i < length; ++i)
        {
            if(sequence[i] == '(' || isalpha(sequence[i])) array_of_coeffecients[i] = ++prev_symbol;
            else if(sequence[i] == ')' || sequence[i] == ',' || sequence[i] == ' ') array_of_coeffecients[i] = --prev_symbol;

            if(array_of_coeffecients[i] > max)
            {
                max = array_of_coeffecients[i];
                pos_of_max = i;
                parent = sequence[i - 3];
            }
        }
        char* children = (char*)malloc(sizeof(char));
        int len = 0;
        int capacity = 1;
        int go = pos_of_max;
        do
        {
            if(capacity == len)
            {
                char* for_realloc = (char*)realloc(children, sizeof(char) * capacity * 2);
                if(for_realloc == NULL)
                {
                    free(children);
                    return MEMORY_ALLOCATION_ERROR;
                }
                children = for_realloc;
                capacity<<=1;
            }
            if(isalpha(sequence[go])) children[len++] = sequence[go++];
            else ++go;
        }while(sequence[go] != ')');
        children[len] = '\0';
        push_back(stack, parent, children);
        remove_substring(&sequence, pos_of_max - 2, go, &length);
    }
    return FINE;
}

status_code new_queue(struct queue** queue)
{
    (*queue) = (struct queue*)malloc(sizeof(struct queue));
    if(*queue == NULL) return MEMORY_ALLOCATION_ERROR;
    (*queue)->first = NULL;
    (*queue)->last = NULL;
    return FINE;
}

status_code push_queue(struct queue** queue, struct tree* node)
{
    struct queue_node* new_node = (struct queue_node*)malloc(sizeof(struct queue_node));
    if(new_node == NULL) return MEMORY_ALLOCATION_ERROR;
    new_node->tree_node = node;
    new_node->next = NULL;

    if((*queue)->last == NULL)
    {
       (*queue)->last = new_node;
       (*queue)->first = new_node;
    }
    else
    {
        (*queue)->last->next = new_node;
        (*queue)->last = new_node;
    }
    return FINE;
}

struct tree* pop_queue(struct queue** queue)
{
    struct queue_node* tmp = (*queue)->first;
    if(tmp == NULL) return NULL;
    struct tree* result = tmp->tree_node;
    (*queue)->first = (*queue)->first->next;
    free(tmp);
    return result;
}

status_code create_tree(struct tree** tree, struct stack_of_tree_nodes** stack)
{
    (*tree) = popop(stack);
    struct tree* tmp;
    struct queue* queue;
    struct list* list_for_lost_nodes = NULL;
    new_queue(&queue);
    if((*tree)->first_child == NULL && (*tree)->brother->first_child == NULL)
    {
        if(*stack == NULL) return FINE;
        struct tree* current_node = (*tree);
        while(current_node != NULL) {
            add_to_list(&list_for_lost_nodes, current_node);
            current_node = current_node->brother;
        }
        current_node = popop(stack);
        while(current_node != NULL)
        {
            push_queue(&queue, current_node);
            struct tree* for_children = current_node->first_child;
            while(for_children != NULL)
            {
                push_queue(&queue, for_children);
                for_children = for_children->brother;
            }
//            add_to_list(&list_for_lost_nodes, current_node);
            current_node = current_node->brother;
        }
        struct tree* build = pop_queue(&queue);
        struct tree* found_node = find_lost_node_in_list(&list_for_lost_nodes, build);
        found_node->first_child = build->first_child;
        free(build);
    }
    else
    {
        tmp = (*tree) -> first_child;
        while(tmp != NULL)
        {
            push_queue(&queue, tmp);
            if(tmp != NULL) add_to_list(&list_for_lost_nodes, tmp);
            tmp = tmp -> brother;
        }
    }


    while(*stack != NULL)
    {
        struct tree* ololo1 = popop(stack);
        //if(*stack == NULL) break;
        struct tree* ololo2;
        do
        {
            ololo2 = pop_queue(&queue);
            if(ololo1->letter == ololo2->letter) break;
            ololo2 = pop_queue(&queue);
            if(ololo2->brother == NULL && ololo2->first_child == NULL)
            {
                clear_list(&list_for_lost_nodes);
                list_for_lost_nodes = NULL;
                ololo2 = pop_queue(&queue);
            }

            if(ololo1->letter != ololo2->letter)
            {

                while(ololo2->brother != NULL)
                {
                    add_to_list(&list_for_lost_nodes, ololo2);
                    if(ololo2->brother->brother == NULL && ololo2->brother->first_child == NULL) break;
                    ololo2 = pop_queue(&queue);
                }
                struct tree* for_fun = find_lost_node_in_list(&list_for_lost_nodes, ololo1);
                if(for_fun != NULL)
                {
                    ololo2 = for_fun;
                    ololo2->first_child = ololo1->first_child;
                }
            }

        }while(ololo1->letter != ololo2->letter);


        tmp = ololo1->first_child;
        ololo2->first_child = tmp;
        while(tmp != NULL)
        {
            push_queue(&queue, tmp);
            if(tmp != NULL) add_to_list(&list_for_lost_nodes, tmp);
            tmp = tmp -> brother;
        }
        free(ololo1);
    }
    return FINE;
}

status_code add_to_list(struct list** list, struct tree* node)
{
    struct list* tmp = (*list);
    while(tmp != NULL)
    {
        if(tmp->node->letter == node->letter) return FINE;
        tmp = tmp->next;
    }
    struct list* new = (struct list*)malloc(sizeof(struct list));
    if(new == NULL) return MEMORY_ALLOCATION_ERROR;
    new->node = node;
    new->next = NULL;
    if(*list == NULL)
    {
        (*list) = new;
        return FINE;
    }
    new->next = *list;
    (*list) = new;
    return FINE;
}

struct tree* find_lost_node_in_list(struct list** list, struct tree* node)
{
    struct list* tmp = *list;
    while(tmp != NULL)
    {
        if(tmp->node->letter == node->letter) return tmp->node;
        tmp = tmp->next;
    }

}
void clear_list(struct list** list)
{
    while(*list != NULL)
    {
        struct list* for_free = *list;
        (*list) = (*list)->next;
        free(for_free);
    }
}

void print_tree_in_file(FILE* output, struct tree* tree, int depth)
{
    if(tree == NULL) return;
    for(int i = 0; i < depth; ++i) printf("\t");
    printf("%c\n", tree -> letter);
    print_tree_in_file(output, tree->first_child, depth + 1);
    print_tree_in_file(output, tree->brother, depth);
}

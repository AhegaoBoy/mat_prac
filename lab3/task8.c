#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    OK,
    NO_MEMORY,
    INVALID_PARAMETER,
} status_codes;

typedef struct Polynomial {
    int coefficient;
    int power;
    struct Polynomial * next;
} Polynomial;

void printPolynomial(Polynomial * head, int first_flag);
void freePolynomial(Polynomial * head);
status_codes push_forwardPolynomial(Polynomial ** head, int _coefficient, int _power);
status_codes push_backPolynomial(Polynomial * head, int _coefficient, int _power);
status_codes add(Polynomial ** result, Polynomial * first, Polynomial * second);
status_codes sub(Polynomial ** result, Polynomial * first, Polynomial * second);
status_codes copyPolynomial(Polynomial ** dest, Polynomial * src);
status_codes mult(Polynomial ** result, Polynomial * first, Polynomial * second);
void multconst(Polynomial * result, const int coef);
status_codes divmod(Polynomial ** div_result, Polynomial ** mod_result, Polynomial * first, Polynomial * second);
double binary_pow(double base, int power);
void eval(double *result, Polynomial * poly, const double value);
status_codes cmps(Polynomial ** result, Polynomial * first, Polynomial * second);
void diff(Polynomial * poly);
int isZero(Polynomial *poly);

int double_validation(const char * number) {
    const size_t size = strlen(number);
    if (!size) return 0;
    size_t index = 0;
    int e_count = 0, dot_count = 0;
    int fl = 0;
    while (index < size) {
        if (number[index] == 'e') e_count++;
        if (number[index] == '.') dot_count++;
        if (e_count > 1 || dot_count > 1 || (!isdigit(number[index]) && (number[index] != 'e' && number[index] != '.'))) return 0;
        if (isdigit(number[index])) fl = 1;
        index++;
    }
    return (1 && fl);
}

int int_validation(const char *number) {
    const size_t size = strlen(number);
    if (!size) {
        return 0;
    }
    if (!isdigit(number[0]) && number[0] != '-' && number[0] != '+') {
        return 0;
    }
    int digit_flag = isdigit(number[0]);
    for (size_t i = 1; i < size; i++) {
        if (!isdigit(number[i])) {
            return 0;
        }
        else {
            digit_flag = 1;
        }
    }
    return (1 && digit_flag);
}

status_codes deleteString(char **string, size_t *size, size_t *capacity) {
    (*size) = 0;
    (*capacity) = 2;
    char *tmp = NULL;
    if (!(tmp = (char *)realloc((*string), sizeof(char) * (*capacity)))) {
        free(*string);
        return NO_MEMORY;
    }
    (*string) = tmp;
    return OK;
}

status_codes addString(char **string, size_t *size, size_t *capacity, const char symbol) {
    (*string)[(*size)] = symbol;
    (*size)++;
    if ((*size) >= (*capacity)) {
        (*capacity) *= 2;
        char *tmp = NULL;
        if (!(tmp = (char *)realloc((*string), sizeof(char) * (*capacity)))) {
            free(*string);
            return NO_MEMORY;
        }
        (*string) = tmp;
    }
    return OK;
}

status_codes addPolynomialNode(char **number, size_t *number_size, size_t *number_capacity, Polynomial **poly, int *part_flag, int coefficient, int power, const char symbol) {
    if ((*part_flag) == 0) {
        if (!(*number_size)) {
            return addString(number, number_size, number_capacity, symbol);
        }
        (*number)[(*number_size)] = '\0';
        coefficient = atoi(*number);
        if (!int_validation(*number)) {
            coefficient = ((*number)[0] == '-') ? -1 : 1;
        }
        power = 0;
        if (!(*poly)) {
            if (push_forwardPolynomial(poly, coefficient, power) == NO_MEMORY) {
                free(*number);
                return NO_MEMORY;
            }
        }
        else if (push_backPolynomial(*poly, coefficient, power) == NO_MEMORY)  {
            free(*number);
            return NO_MEMORY;
        }
        if (deleteString(number, number_size, number_capacity) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    else {
        (*part_flag) = 0;
        (*number)[(*number_size)] = '\0';
        power = atoi(*number);
        if (!(*number_size)) {
            power = 1;
        }
        if (!(*poly) || (*poly)->power < power) {
            if (push_forwardPolynomial(poly, coefficient, power) == NO_MEMORY) {
                free(*number);
                return NO_MEMORY;
            }
        }
        else if (push_backPolynomial(*poly, coefficient, power) == NO_MEMORY)  {
            free(*number);
            return NO_MEMORY;
        }
        if (deleteString(number, number_size, number_capacity) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    return OK;
}

status_codes getPolynomial(const char *string, Polynomial **poly) {
    size_t string_size = strlen(string);
    if (string[string_size - 1] == '+' || string[string_size - 1] == '-') {
        return INVALID_PARAMETER;
    }
    size_t number_size = 0;
    size_t number_capacity = 2;
    char *number = (char *)malloc(sizeof(char) * number_capacity);
    if (!number) {
        return NO_MEMORY;
    }
    int coefficient = 0;
    int power = 0;
    int part_flag = 0;
    for (int i = 0; i < string_size; i++) {
        if (string[i] == '^') {
            if (string[i - 1] != 'x' || string[i + 1] == '-') {
                free(number);
                return INVALID_PARAMETER;
            }
            continue;
        }
        if (string[i] == 'x') {
            number[number_size] = '\0';
            coefficient = atoi(number);
            if (!int_validation(number)) {
                coefficient = (number[0] == '-') ? -1 : 1;
            }
            if (string[i + 1] != '^' && (i + 1) < string_size && string[i + 1] != '+' && string[i + 1] != '-') {
                free(number);
                return INVALID_PARAMETER;
            }
            if (deleteString(&number, &number_size, &number_capacity) == NO_MEMORY) {
                return NO_MEMORY;
            }
            part_flag = 1;
            continue;
        }
        if ((string[i] == '-' || string[i] == '+')) {
            if (string[i + 1] == '+' || string[i + 1] == '-') {
                free(number);
                return INVALID_PARAMETER;
            }
            if (addPolynomialNode(&number, &number_size, &number_capacity, poly, &part_flag, coefficient, power, string[i]) == NO_MEMORY) {
                return NO_MEMORY;
            }
        }
        else if (!isdigit(string[i])) {
            free(number);
            return INVALID_PARAMETER;
        }
        if (addString(&number, &number_size, &number_capacity, string[i]) == NO_MEMORY) {
            return NO_MEMORY;
        }

    }
    if (addPolynomialNode(&number, &number_size, &number_capacity, poly, &part_flag, coefficient, power, 0) == NO_MEMORY) {
        return NO_MEMORY;
    }
    free(number);
    return OK;
}

status_codes processDiff(char *token, const char *separators, Polynomial **summator) {
    token = strtok(NULL, separators);
    if (!token) {
        diff(*summator);
        printPolynomial(*summator, 1);
        return OK;
    }
    char polynomial[BUFSIZ];
    strcpy(polynomial, token);
    Polynomial *poly = NULL;
    status_codes status = getPolynomial(polynomial, &poly);
    if (status != OK) {
        freePolynomial(poly);
        return status;
    }
    diff(poly);
    freePolynomial(*summator);
    (*summator) = NULL;
    status = copyPolynomial(summator, poly);
    printPolynomial(*summator, 1);
    free(poly);
    return status;
}

status_codes processEval(char *token, const char *separators, Polynomial **summator) {
    token = strtok(NULL, separators);
    if (!token) {
        return INVALID_PARAMETER;
    }
    double result = 0;
    char first_argument[BUFSIZ];
    strcpy(first_argument, token);
    token = strtok(NULL, separators);
    if (!token) {
        if (!double_validation(first_argument)) {
            return INVALID_PARAMETER;
        }
        double arg = atof(first_argument);
        eval(&result, *summator, arg);
        printf("%.3f\n", result);
        return OK;
    }
    if (!double_validation(token)) {
        return INVALID_PARAMETER;
    }
    Polynomial *poly = NULL;
    status_codes status = getPolynomial(first_argument, &poly);
    if (status != OK) {
        freePolynomial(poly);
        return status;
    }
    eval(&result, poly, atof(token));
    printf("%.3f\n", result);
    freePolynomial(poly);
    return OK;
}

int command_validation(const char *command) {
    size_t command_size = strlen(command);
    int bracket_level = 0;
    for (int i = 0; i < command_size; i++) {
        if (command[i] == '(') {
            bracket_level++;
            if (command[i + 1] == ',' || bracket_level > 1) {
                return 0;
            }
        }
        if (command[i] == ')') {
            bracket_level--;
        }
    }
    return !bracket_level;
}

status_codes processCommand(char *command, Polynomial **summator) {
    if (!command_validation(command)) {
        return INVALID_PARAMETER;
    }
    int summator_flag = 0;
    const char *separators = "(),";
    char *token = strtok(command, separators);
    if (!token) {
        return INVALID_PARAMETER;
    }
    if (strcmp(token, "Diff") == 0) {
        return processDiff(token, separators, summator);
    }
    if (strcmp(token, "Eval") == 0) {
        return processEval(token, separators, summator);
    }
    char command_name[128];
    strcpy(command_name, token);
    token = strtok(NULL, separators);
    if (!token) {
        return INVALID_PARAMETER;
    }
    char first_polynomial[BUFSIZ];
    strcpy(first_polynomial, token);
    token = strtok(NULL, separators);
    if (!token) {
        summator_flag = 1;
    }
    char second_polynomial[BUFSIZ];
    Polynomial *first = NULL;
    Polynomial *second = NULL;
    status_codes status;
    if (!summator_flag) {
        strcpy(second_polynomial, token);
        status = getPolynomial(first_polynomial, &first);
        if (status != OK) {
            freePolynomial(first);
            freePolynomial(second);
            return status;
        }
        status = getPolynomial(second_polynomial, &second);
        if (status != OK) {
            freePolynomial(first);
            freePolynomial(second);
            return status;
        }
    }
    else {
        if (copyPolynomial(&first, *summator) == NO_MEMORY) {
            freePolynomial(first);
            freePolynomial(second);
            return NO_MEMORY;
        }
        freePolynomial(*summator);
        (*summator) = NULL;
        status = getPolynomial(first_polynomial, &second);
        if (status != OK) {
            freePolynomial(first);
            freePolynomial(second);
            return status;
        }
    }
    Polynomial *no_use = NULL;
    if (strcmp(command_name, "Add") == 0) {
        status = add(summator, first, second);
    }
    else if (strcmp(command_name, "Sub") == 0) {
        status = sub(summator, first, second);
    }
    else if (strcmp(command_name, "Mult") == 0) {
        status = mult(summator, first, second);
    }
    else if (strcmp(command_name, "Div") == 0) {
        if (isZero(second)) {
            freePolynomial(first);
            freePolynomial(second);
            return INVALID_PARAMETER;
        }
        if (isZero(first)) {
            freePolynomial(first);
            freePolynomial(second);
            freePolynomial(*summator);
            (*summator) = NULL;
            printPolynomial(*summator, 1);
            return OK;
        }
        status = divmod(summator, &no_use, first, second);
        freePolynomial(no_use);
    }
    else if (strcmp(command_name, "Mod") == 0) {
        if (isZero(second)) {
            freePolynomial(first);
            freePolynomial(second);
            return INVALID_PARAMETER;
        }
        if (isZero(first)) {
            freePolynomial(first);
            freePolynomial(second);
            freePolynomial(*summator);
            (*summator) = NULL;
            printPolynomial(*summator, 1);
            return OK;
        }
        status = divmod(&no_use, summator, first, second);
        freePolynomial(no_use);
    }
    else if (strcmp(command_name, "Cmps") == 0) {
        status = cmps(summator, first, second);
    }
    else {
        status = INVALID_PARAMETER;
    }
    freePolynomial(first);
    freePolynomial(second);
    printPolynomial(*summator, 1);
    return status;
}

status_codes getCommand(FILE *stream) {
    size_t command_size = 0;
    size_t command_capacity = 2;
    char *command = (char *)malloc(sizeof(char) * command_capacity);
    if (!command) {
        return NO_MEMORY;
    }
    Polynomial *summator = NULL;
    char symbol = fgetc(stream);
    int comment_level = 0;
    int short_comment = 0;
    while (symbol != EOF) {
        if (symbol == '[' && !short_comment) {
            comment_level++;
            if (comment_level > 1) {
                free(command);
                freePolynomial(summator);
                return INVALID_PARAMETER;
            }
        }
        else if (symbol == ']' && !short_comment) {
            comment_level--;
            if (comment_level < 0) {
                free(command);
                freePolynomial(summator);
                return INVALID_PARAMETER;
            }
        }
        if (symbol == '%' && !comment_level) {
            short_comment = 1;
        }
        else if (symbol == '\n') {
            short_comment = 0;
        }
        if (comment_level & 1 || short_comment == 1 || symbol == '[' || symbol == ']' || symbol == '%' || symbol == '\n' || symbol == ' ' || symbol == '\t') {
            symbol = fgetc(stream);
            continue;
        }
        if (symbol == ';') {
            command[command_size] = '\0';
            status_codes status = processCommand(command, &summator);
            if (status != OK) {
                free(command);
                freePolynomial(summator);
                return status;
            }
            if (deleteString(&command, &command_size, &command_capacity) == NO_MEMORY) {
                freePolynomial(summator);
                return NO_MEMORY;
            }
            symbol = fgetc(stream);
            continue;
        }
        command[command_size] = symbol;
        command_size++;
        if (command_size >= command_capacity) {
            command_capacity *= 2;
            char *tmp = NULL;
            if (!(tmp = (char *)realloc(command, sizeof(char) * command_capacity))) {
                free(command);
                freePolynomial(summator);
                return NO_MEMORY;
            }
            command = tmp;
        }
        symbol = fgetc(stream);
    }
    free(command);
    freePolynomial(summator);
    if (comment_level != 0) {
        return INVALID_PARAMETER;
    }
    return OK;
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("Wrong flag\n");
        return -1;
    }
    FILE *file = fopen(argv[1], "r");
    switch (getCommand(file)) {
        case OK: break;
        case NO_MEMORY:
            fclose(file);
            printf("No memory\n");
            return -1;
        case INVALID_PARAMETER:
            fclose(file);
            printf("Invalid parameter detected\n");
            return -1;
    }
    fclose(file);
    return 0;
}

void printPolynomial(Polynomial * head, int first_flag) {
    if (!head) {
        if (first_flag) {
            printf("0\n");
        }
        return;
    }
    if (head->coefficient == 0) {
        if (first_flag) {
            if (!head->next) {
                printf("0\n");
            }
            else {
                printPolynomial(head->next, 1);
            }
            return;
        }
        printPolynomial(head->next, 0);
        return;
    }
    int coef_flag = 0;
    if (head->coefficient) {
        if (head->coefficient > 0) {
            if (!first_flag) {
                printf(" + ");
            }
        }
        else {
            printf(" - ");
        }
        if (head->coefficient != 1) {
            printf("%d", abs(head->coefficient));
            coef_flag = 1;
        }
    }
    if (head->power && head->coefficient) {
        printf("x");
        if (head->power != 1) {
            printf("^%d", head->power);
        }
    }
    else if (!coef_flag) {
        printf("%d", head->coefficient);
    }
    printPolynomial(head->next, 0);
    if (first_flag) {
        printf("\n");
    }
}

void freePolynomial(Polynomial * head) {
    if (!head) {
        return;
    }
    freePolynomial(head->next);
    free(head);
}

status_codes push_forwardPolynomial(Polynomial ** head, int _coefficient, int _power) {
    if (*head && (*head)->power == _power) {
        (*head)->coefficient += _coefficient;
        return OK;
    }
    Polynomial * tmp = (Polynomial *)malloc(sizeof(Polynomial));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->coefficient = _coefficient;
    tmp->power = _power;
    tmp->next = (*head);
    (*head) = tmp;
    return OK;
}

status_codes push_backPolynomial(Polynomial * head, int _coefficient, int _power) {
    while (head->next && head->next->power >= _power) {
        head = head->next;
    }
    if (head->power == _power) {
        head->coefficient += _coefficient;
        return OK;
    }
    Polynomial * tmp = (Polynomial *)malloc(sizeof(Polynomial));
    if (!tmp) {
        return NO_MEMORY;
    }
    tmp->coefficient = _coefficient;
    tmp->power = _power;
    if (!head->next) {
        tmp->next = NULL;
    }
    else {
        tmp->next = head->next;
    }
    head->next = tmp;
    return OK;
}

status_codes add(Polynomial ** result, Polynomial * first, Polynomial * second) {
    while (1) {
        if (first && second) {
            if (first->power == second->power) {
                if (!(*result) || (*result)->power == first->power) {
                    if (push_forwardPolynomial(result, first->coefficient + second->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), first->coefficient + second->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }

                first = first->next;
                second = second->next;
            }
            else if (first->power > second->power) {
                if (!(*result) || (*result)->power == first->power) {
                    if (push_forwardPolynomial(result, first->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), first->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                first = first->next;
            }
            else if (second->power > first->power) {
                if (!(*result) || (*result)->power == second->power) {
                    if (push_forwardPolynomial(result, second->coefficient, second->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), second->coefficient, second->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                second = second->next;
            }
        }
        else if (!second && first) {
            if (!(*result) || (*result)->power == first->power) {
                if (push_forwardPolynomial(result, first->coefficient, first->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial((*result), first->coefficient, first->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            first = first->next;
        }
        else if (!first && second) {
            if (!(*result) || (*result)->power == second->power) {
                if (push_forwardPolynomial(result, second->coefficient, second->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial((*result), second->coefficient, second->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            second = second->next;
        }
        else {
            return OK;
        }
    }

}

status_codes sub(Polynomial ** result, Polynomial * first, Polynomial * second) {
    while (1) {
        if (first && second) {
            if (first->power == second->power) {
                if (!(*result) || (*result)->power == first->power) {
                    if (push_forwardPolynomial(result, first->coefficient - second->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), first->coefficient - second->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }

                first = first->next;
                second = second->next;
            }
            else if (first->power > second->power) {
                if (!(*result) || (*result)->power == first->power) {
                    if (push_forwardPolynomial(result, first->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), first->coefficient, first->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                first = first->next;
            }
            else if (second->power > first->power) {
                if (!(*result) || (*result)->power == second->power) {
                    if (push_forwardPolynomial(result, -second->coefficient, second->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                else {
                    if (push_backPolynomial((*result), -second->coefficient, second->power) == NO_MEMORY) {
                        return NO_MEMORY;
                    }
                }
                second = second->next;
            }
        }
        else if (!second && first) {
            if (!(*result) || (*result)->power == first->power) {
                if (push_forwardPolynomial(result, first->coefficient, first->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial((*result), first->coefficient, first->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            first = first->next;
        }
        else if (!first && second) {
            if (!(*result) || (*result)->power == second->power) {
                if (push_forwardPolynomial(result, -second->coefficient, second->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial((*result), -second->coefficient, second->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            second = second->next;
        }
        else {
            return OK;
        }
    }
}

status_codes copyPolynomial(Polynomial ** dest, Polynomial * src) {
    if (!src) {
        return OK;
    }
    if (!(*dest)) {
        if (push_forwardPolynomial(dest, src->coefficient, src->power) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    else {
        if (push_backPolynomial((*dest), src->coefficient, src->power) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    copyPolynomial(dest, src->next);
    return OK;
}

status_codes mult(Polynomial ** result, Polynomial * first, Polynomial * second) {
    Polynomial * copy = NULL;
    if (copyPolynomial(&copy, second) == NO_MEMORY) {
        return NO_MEMORY;
    }
    Polynomial * tmp = NULL;
    Polynomial * result_copy = NULL;
    while (first) {
        tmp = NULL;
        while (copy) {
            if (!tmp) {
                if (push_forwardPolynomial(&tmp, first->coefficient * copy->coefficient, first->power + copy->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            else {
                if (push_backPolynomial(tmp, first->coefficient * copy->coefficient, first->power + copy->power) == NO_MEMORY) {
                    return NO_MEMORY;
                }
            }
            Polynomial * tmp2 = copy->next;
            free(copy);
            copy = tmp2;

        }
        result_copy = NULL;
        if (copyPolynomial(&result_copy, (*result)) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(*result);
        (*result) = NULL;
        if (add(result, result_copy, tmp) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(result_copy);
        freePolynomial(copy);
        freePolynomial(tmp);
        if (copyPolynomial(&copy, second) == NO_MEMORY) {
            return NO_MEMORY;
        }
        if (!first->next) {
            break;
        }
        first = first->next;
    }
    freePolynomial(copy);
    return OK;
}

void multconst(Polynomial * result, const int coef) {
    if (!result) {
        return;
    }
    result->coefficient *= coef;
    multconst(result->next, coef);
}

status_codes divmod(Polynomial ** div_result, Polynomial ** mod_result, Polynomial * first, Polynomial * second) {
    Polynomial * copy = NULL;
    if (copyPolynomial(&copy, first) == NO_MEMORY) {
        return NO_MEMORY;
    }
    while (copy->power >= second->power) {
        int coefficient = copy->coefficient / second->coefficient;
        if (!coefficient) {
            (*mod_result) = copy;
            return OK;
        }
        Polynomial * to_mult = NULL;
        if (push_forwardPolynomial(&to_mult, coefficient, copy->power - second->power) == NO_MEMORY) {
            return NO_MEMORY;
        }
        if (!(*div_result)) {
            if (push_forwardPolynomial(div_result, coefficient, copy->power - second->power) == NO_MEMORY) {
                return NO_MEMORY;
            }
        }
        else {
            if (push_backPolynomial((*div_result), coefficient, copy->power - second->power) == NO_MEMORY) {
                return NO_MEMORY;
            }
        }
        Polynomial * mult_result_second = NULL;
        if (mult(&mult_result_second, to_mult, second) == NO_MEMORY) {
            return NO_MEMORY;
        }
        Polynomial * first_copy = NULL;
        if (copyPolynomial(&first_copy, copy) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(copy);
        copy = NULL;
        if (sub(&copy, first_copy, mult_result_second) == NO_MEMORY) {
            return NO_MEMORY;
        }
        freePolynomial(to_mult);
        freePolynomial(mult_result_second);
        freePolynomial(first_copy);
        if (copy->coefficient == 0) {
            if (!copy->next) {
                (*mod_result) = copy;
                return OK;
            }
            Polynomial * tmp = copy->next;
            free(copy);
            copy = tmp;
        }

    }
    (*mod_result) = copy;
    return OK;
}


double binary_pow(double base, int power) {
    if (power < 0) {
        base = 1.0 / base;
        power = abs(power);
    }
    if (power == 0) return 1;
    if (power % 2 == 1) return binary_pow(base, power - 1) * base;
    else {
        double tmp = binary_pow(base, power / 2);
        return tmp * tmp;
    }
}

void eval(double *result, Polynomial * poly, const double value) {
    if (!poly) {
        return;
    }
    (*result) += poly->coefficient * binary_pow(value, poly->power);
    eval(result, poly->next, value);
}

status_codes cmps(Polynomial ** result, Polynomial * first, Polynomial * second) {
    if (!first) {
        return OK;
    }
    Polynomial * second_base = NULL;
    if (first->power == 0) {
        if (push_forwardPolynomial(&second_base, 1, 0) == NO_MEMORY) {
            return NO_MEMORY;
        }
    }
    else {
        if (copyPolynomial(&second_base, second) == NO_MEMORY) {
            return NO_MEMORY;
        }
        for (int i = 0; i < first->power - 1; i++) {
            Polynomial * tmp = NULL;
            if (mult(&tmp, second_base, second) == NO_MEMORY) {
                return NO_MEMORY;
            }
            freePolynomial(second_base);
            second_base = NULL;
            if (copyPolynomial(&second_base, tmp) == NO_MEMORY) {
                return NO_MEMORY;
            }
            freePolynomial(tmp);
        }
    }
    multconst(second_base, first->coefficient);
    Polynomial * tmp2 = NULL;
    if (copyPolynomial(&tmp2, (*result)) == NO_MEMORY) {
        return NO_MEMORY;
    }
    freePolynomial(*result);
    (*result) = NULL;
    if (add(result, tmp2, second_base) == NO_MEMORY) {
        return NO_MEMORY;
    }
    freePolynomial(tmp2);
    freePolynomial(second_base);
    return cmps(result, first->next, second);
}

void diff(Polynomial * poly) {
    if (!poly) {
        return;
    }
    poly->coefficient *= poly->power;
    poly->power--;
    diff(poly->next);
}

int isZero(Polynomial *poly) {
    if (!poly) {
        return 1;
    }
    if (poly->coefficient != 0) {
        return 0;
    }
    return isZero(poly->next);
}
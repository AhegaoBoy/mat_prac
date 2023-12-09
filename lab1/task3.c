#define EQUATION_COEFFICIENTS 3
#define ROOTS 2

#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

double binary_power(double base, unsigned long long exponent);
double root_of_power(double x, int exponent, double eps);

int check_flag(char** word);
int is_number(const char* string);
int check_double(char** word);
void swap(double *a, double *b);
int partition(double** array, int low, int high, double epsilon);
void quicksort(double** array, int low, int high, double epsilon);
int generate_permutation(double** array, int array_length, double epsilon);
int solve_equation(double** permutation, double** solutions, int amount_of_solutions, double epsilon);

int main(int argc, char** argv) {
    if(argc < 2){
        printf("\nNo flags were given");
        return 0;
    }
    if(check_flag(&argv[1]) == 0){
        printf("\nInvalid flag: %s", argv[1]);
        return 0;
    }
    char flag = argv[1][1];
    double* permutation = NULL;
    double* roots = NULL;

    char* ptr = "";
    double epsilon = 0;
    int check = 0;
    int number_1 = 0;
    int number_2 = 0;

    double max = 0;
    double sum = 0;
    double side = 0;
    double hypotenuse = 0.0;

    switch (flag){
        case 'q':
        {
            if(argc != 6){
                printf("\nInvalid amount of arguments after flag.\nExpected: 4\nPut:%d", argc-2);
                return 0;
            }

            check = check_double(&argv[2]);
            switch(check) {
                case -1: {
                    printf("\nAccuracy can't be negative or zero.");
                    return 0;
                }
                case 0: {
                    printf("\nInvalid accuracy given.");
                    return 0;
                }
                case 1:
                {
                    ptr = argv[2];
                    epsilon = strtod(argv[2], &ptr);
                    if(!(is_number(argv[3]) && is_number(argv[4]) && is_number(argv[5]))){
                        printf("\nWrong coefficients given");
                        return 0;
                    }
                    permutation = (double*)malloc(sizeof(double)*EQUATION_COEFFICIENTS);
                    if(permutation == NULL){
                        printf("\nInternal error lol.");
                        free(permutation);
                        return 1;
                    }

                    for(int i = 0; i < EQUATION_COEFFICIENTS; i++){
                        if(!check_double(&argv[3 + i])){
                            printf("%s is not a valid float value ;(\n", argv[3+i]);
                            free(permutation);
                            return 0;
                        }
                        permutation[i] = strtod(argv[3 + i], &ptr);
                    }
                    roots = (double*)malloc(sizeof(double)*ROOTS);
                    if(roots == NULL){
                        printf("Internal Error sorry T_T\n");
                        free(permutation);
                        free(roots);
                        return 1;
                    }
                    quicksort(&permutation, 0, EQUATION_COEFFICIENTS - 1, epsilon);

                    printf("solutions:\n(%g) * x^2 + (%g) * x + (%g) = 0:\n", permutation[0], permutation[1], permutation[2]);
                    if(!solve_equation(&permutation, &roots, 2, epsilon)){
                        printf("both roots are within the field C of complex numbers\n");
                    }else{
                        printf("x_1 = %g\nx_2 = %g\n", roots[0], roots[1]);
                    }

                    while(generate_permutation(&permutation, EQUATION_COEFFICIENTS, epsilon)){
                        printf("\n(%g) * x^2 + (%g) * x + (%g) = 0:\n", permutation[0], permutation[1], permutation[2]);

                        if(!solve_equation(&permutation, &roots, 2, epsilon)){
                            printf("both roots are within the field C of complex numbers\n");
                        }else{
                            printf("x_1 = %g\nx_2 = %g\n", roots[0], roots[1]);
                        }
                    }
                    free(roots);
                    free(permutation);
                    return 0;
                }
            }
        }
        case 'm':
        {
            if(argc != 4){
                printf("\nInvalid amount of arguments after flag.\nExpected: 2\nPut:%d", argc-2);
                return 0;
            }
            if(!(is_number(argv[2]) && is_number(argv[3]))){
                printf("\nInvalid numbers given.");
                return 0;
            }
            number_1 = atoi(argv[2]);
            number_2 = atoi(argv[3]);
            printf("\n%d is%s a multiple for %d", number_1, (number_1 % number_2 == 0) ? "" : " not", number_2);
            return 0;

        }
        case 't':
        {
            if(argc != 6){
                printf("\nInvalid amount of arguments after flag.\nExpected: 4\nPut:%d", argc-2);
                return 0;
            }
            check = check_double(&argv[2]);
            switch(check) {
                case -1: {
                    printf("\nAccuracy can't be negative or zero.");
                    return 0;
                }
                case 0: {
                    printf("\nInvalid accuracy given.");
                    return 0;
                }
                case 1: {
                    ptr = argv[2];
                    epsilon = strtod(argv[2], &ptr);
                    max = 0.0;
                    sum = 0.0;
                    for (int i = 3; i <= 5; i++) {
                        check = check_double(&argv[i]);
                        switch (check) {
                            case -1: {
                                printf("\nSize of triangle side can't be negative or zero.");
                                return 0;
                            }
                            case 0: {
                                printf("\n%s is not a side size", argv[i]);
                            }
                            case 1: {
                                ptr = argv[2];
                                side = strtod(argv[i], &ptr);
                                max = (max - side > epsilon) ? max : side;
                                sum += binary_power(side, 2);
                                break;
                            }
                        }
                    }
                    hypotenuse = binary_power(max, 2);
                    sum -= hypotenuse;
                    printf("\nIt is %s for a triangle with given sides to exist.", fabs(sum - hypotenuse) < epsilon ? "possible" : "impossible");
                    return 0;

                }
            }
        }
        default:{
            printf("\nInvalid flag: %s", argv[1]);
            return 0;
        }
    }
}

double root_of_power(double x, int exponent, double eps){
    if(exponent > INT_MAX / 2 ){
        return INFINITY;
    }
    double value_current = 1;
    double value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ binary_power(value_current, exponent-1));
    while(fabs(value_current - value_next) > eps){
        value_current = value_next;
        value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ binary_power(value_current, exponent-1));
    }
    return value_next;
}

double binary_power(double base, unsigned long long exponent){
    double v = 1.0;
    while(exponent){
        if(exponent & 1){
            v *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return v;
}

int check_flag(char** word){
    if(((*word)[0] != '-' && (*word)[0] != '/') || strlen(*word) != 2) return 0;
    return 1;
}

int is_number(const char* string){
    size_t string_length = strlen(string);
    int i = 0;
    if((string)[0] == '-'){
        i = 1;
    }
    for( ; i < string_length; i++){
        if(!isdigit(string[i])) return 0;
    }
    return 1;
}

int check_double(char** word){
    double value;
    char* end;
    value = strtod(*word, &end);

    if(*end != 0){
        return 0;
    }
    if(value <= 0.0){
        return -1;
    }
    return 1;
}

void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

int partition(double** array, int low, int high, double epsilon) {

    double pivot = (*array)[high];
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        if(fabs((*array)[j] - pivot) < epsilon) continue;
        if ((*array)[j] < pivot) {
            i++;
            swap(&(*array)[i], &(*array)[j]);
        }
    }

    swap(&(*array)[i + 1], &(*array)[high]);
    return (i + 1);
}

void quicksort(double** array, int low, int high, double epsilon) {
    if (low < high) {
        int pivot = partition(array, low, high, epsilon);

        quicksort(array, low, pivot - 1, epsilon);
        quicksort(array, pivot + 1, high, epsilon);
    }
}

int generate_permutation(double** array, int array_length, double epsilon){
    int j = array_length - 2;
    while (j != -1 && ((*array)[j] > (*array)[j+1]) || fabs((*array)[j] - (*array)[j+1]) < epsilon) {
        j--;
    }
    if (j == -1){
        return 0;
    }

    int k = array_length - 1;
    while ((*array)[j] >= (*array)[k]) k--;
    swap(&(*array)[j], &(*array)[k]);

    int left = j + 1;
    int right = array_length - 1;
    while (left < right){
        left += 1;
        right -= 1;
        swap(&(*array)[left], &(*array)[right]);
    }
    return 1;
}

int solve_equation(double** permutation, double** solutions, int amount_of_solutions, double epsilon){
    double discriminant;
    double a = (*permutation)[0];
    double b = (*permutation)[1];
    double c = (*permutation)[2];

    double solution_1;
    double solution_2;

    if(a < epsilon){
        solution_1 = -c/b;
        solution_2 = solution_1;
    }else{
        discriminant = binary_power(b, 2) - 4 * a * c;
        if(discriminant < 0){
            return 0;
        }
        solution_1 = (-b + root_of_power(discriminant, 2, epsilon)) / (2*a);
        solution_2 = (-b - root_of_power(discriminant, 2, epsilon)) / (2*a);
    }
    (*solutions)[0] = solution_1;
    (*solutions)[1] = solution_2;
    return 1;
}
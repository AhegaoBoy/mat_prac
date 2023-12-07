#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

struct vector
{
    double* coordinates;
};

struct vector* find_max_mudulo(int, int, int**, double, double,  int*, int*, int*,...);
int** make_matrix(int n);
struct vector multiply_matrix_and_vector(int, int** , struct vector);
double euclidean_norm(int, double,  struct vector, int**, double);
double infinite_norm(int, double, struct vector, int**, double);
double bin_pow(double, int);
double get_root(double, double, double);
double gelder_norm(int, double,  struct vector, int**,  double);

int main()
{
    int n;
    double epsilon = 1e-6;
    printf("Insert volume of linear space:");
    scanf("%d", &n);
    printf("Insert matrix for Euclidean norm:\n");
    int** matrix = make_matrix(n);
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j) printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("Insert parametr p for Gelder norm:\n");
    double p;
    scanf("%lf", &p);

    int first, second, third;
    struct vector vector1;
    struct vector vector2;
    struct vector vector3;
    printf("Input vectors:\n");
    vector1.coordinates = (double*) malloc(sizeof(double)*n);
    for(int j = 0; j < n; ++j) scanf("%lf", &vector1.coordinates[j]);
    vector2.coordinates = (double*) malloc(sizeof(double)*n);
    for(int j = 0; j < n; ++j) scanf("%lf", &vector2.coordinates[j]);
    vector3.coordinates = (double*) malloc(sizeof(double)*n);
    for(int j = 0; j < n; ++j) scanf("%lf", &vector3.coordinates[j]);

    printf("\tCheck what you get\n");


    struct vector* max_vectors = find_max_mudulo(n, 3, matrix, p, epsilon, &first, &second, &third, euclidean_norm, vector1, vector2, vector3,
                                                 infinite_norm,vector1, vector2, vector3,  gelder_norm, vector1, vector2, vector3);
    if(max_vectors == NULL)
    {
        printf("MEMORY ALLOCATION ERROR\n");
        for(int i = 0; i < n; ++i) free(matrix[i]);
        free(matrix);
        return 1;
    }

    int pos = 0;
    for(int i = pos; i < first; ++i)
    {
        for(int j = 0; j < n; ++j) printf("%.lf ", max_vectors[i].coordinates[j]);
        free(max_vectors[i].coordinates);
        printf("\n");
    }
    pos = first;
    for(int i = pos; i < pos + second; ++i)
    {
        for(int j = 0; j < n; ++j) printf("%.lf ", max_vectors[i].coordinates[j]);
        free(max_vectors[i].coordinates);
        printf("\n");
    }
    pos += second;
    for (int i = pos; i < pos + third; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            printf("%.lf ", max_vectors[i].coordinates[j]);
        }
        free(max_vectors[i].coordinates);
        printf("\n");
    }
    free(max_vectors);
    for(int i = 0; i < n; ++i) free(matrix[i]);
    free(matrix);
    return 0;

}

int** make_matrix(int n)
{
    int** matrix = (int**)malloc(sizeof(int*) * n);
    if(matrix == NULL) return NULL;
    for(int i = 0; i < n; ++i)
    {
        matrix[i] = (int*)malloc(sizeof(int) * n);
        if(matrix[i] == NULL) return NULL;
        for(int j = 0; j < n; ++j ) scanf("%d", &matrix[i][j]);
    }
    return matrix;
}
struct vector multiply_matrix_and_vector(int n, int** matrix, struct vector vector)
{
    double* result_coordinates = (double*)malloc(sizeof(double) * n);
    struct vector result = {result_coordinates};
    for(int i = 0; i < n; ++i)
    {
        double sum = 0.0;
        for(int j = 0; j < n; ++j) sum += matrix[i][j] * vector.coordinates[j];
        result.coordinates[i] = sum;
    }
    return result;
}
double euclidean_norm(int n, double p,  struct vector vector, int** matrix, double epsilon)
{
    struct vector vector1 = multiply_matrix_and_vector(n, matrix, vector);
    double sum = 0;
    for(int i = 0; i < n; ++i) sum += vector1.coordinates[i] * vector.coordinates[i];
    double res = get_root(sum, 2.0, epsilon);
    return res;
}
double infinite_norm(int n, double p, struct vector vector, int** matrix,  double epsilon)
{
    double max = vector.coordinates[0];
    for(int i = 1; i < n; ++i)
    {
        if((max - vector.coordinates[i]) > epsilon) continue;
        else max = vector.coordinates[i];
    }
    return max;
}

double gelder_norm(int n, double p, struct vector vector, int** matrix,  double epsilon)
{
    double sum = 0;
    for(int i = 0; i < n; ++i)
    {
        sum += bin_pow(vector.coordinates[i], (int)p);
    }
    return get_root(sum, 1/p, epsilon);
}
double bin_pow (double a, int n)
{
    if (n == 0)
        return 1.0;
    if (n % 2 == 1)
        return bin_pow(a, n-1) * a;
    else {
        double b = bin_pow (a, n/2);
        return b * b;
    }
}

double get_root(double x, double exponent, double eps){
    if(exponent - 1.0 <=eps) return x;
    double value_current = 1;
    double value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ bin_pow(value_current, exponent-1));
    while(fabs(value_current - value_next) > eps){
        value_current = value_next;
        value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ bin_pow(value_current, exponent-1));
    }
    return value_next;
}

struct vector* find_max_mudulo(int n, int quantity, int** matrix, double p, double epsilon, int* first, int* second, int* third, ...)
{
    struct vector* max_vectors = (struct vector*)malloc(sizeof(struct vector));
    max_vectors[0].coordinates = (double*)malloc(sizeof(double) * n);
    int len = 0, capacity = 1;
    *first = 0; *second = 0; *third = 0;
    va_list  args;
    va_start(args,third);
    int quantity_of_functions_left = 3;
    while(quantity_of_functions_left > 0)
    {
        double max_value = 0.0;
        double(*count_function)(int, double,  struct vector, int**,  double) = va_arg(args, double(*)(int, double, struct vector, int**, double));
        int quantity_for_function = quantity;
        while(quantity_for_function > 0)
        {

            struct vector vector = va_arg(args, struct vector);
            double current_norm = count_function(n, p, vector, matrix, epsilon);
            if(fabs(current_norm - max_value) > epsilon)
            {
                for(int i = 0; i < n; ++i) max_vectors[len].coordinates[i] = vector.coordinates[i];
                switch (quantity_of_functions_left) {
                    case 3:
                        (*first) = 1;
                        break;
                    case 2:
                        (*second) = 1;
                        break;
                    case 1:
                        (*third) = 1;
                        break;
                }
                max_value = current_norm;
            }
            else
            {
                if(capacity == len)
                {
                    struct vector *for_realloc = (struct vector *) realloc(max_vectors,
                                                                           (sizeof(struct vector)) * capacity * 2);
                    if (for_realloc == NULL) {
                        for(int i = 0; i < len; ++i) free(max_vectors[i].coordinates);
                        free(max_vectors);
                        return NULL;
                    }
                    max_vectors = for_realloc;
                    capacity <<= 1;
                    for (int i = len; i < capacity; ++i) {
                        max_vectors[i].coordinates = (double *) malloc(sizeof(double) * n);
                        if (max_vectors[i].coordinates == NULL) {
                            for (int j = 0; j <= i; ++j) free(max_vectors[j].coordinates);
                            free(max_vectors);
                            return NULL;
                        }
                    }
                }
                if(len == 0)
                {
                    for(int i = 0; i < n; ++i) max_vectors[len].coordinates[i] = vector.coordinates[i];
                    switch (quantity_of_functions_left) {
                        case 3:
                            (*first) ++;
                            break;
                        case 2:
                            (*second) ++;
                            break;
                        case 1:
                            (*third) ++;
                            break;
                    }
                    max_value = current_norm;
                    len++;
                }
                else
                {

                    for(int i = 0; i < n; ++i) max_vectors[len].coordinates[i] = vector.coordinates[i];
                    switch (quantity_of_functions_left)
                    {
                        case 3:
                            (*first)++;
                            break;
                        case 2:
                            (*second)++;
                            break;
                        case 1:
                            (*first)++;
                            break;
                    }
                    len++;
                }
            }
            quantity_for_function--;
        }
        len++;
        if(capacity == len) {
            struct vector *for_realloc = (struct vector *) realloc(max_vectors,
                                                                   (sizeof(struct vector)) * capacity * 2);
            if (for_realloc == NULL) {
                for(int i = 0; i < len; ++i) free(max_vectors[i].coordinates);
                free(max_vectors);
            }
            max_vectors = for_realloc;
            capacity <<= 1;
            for (int i = len; i < capacity; ++i) {
                max_vectors[i].coordinates = (double *) malloc(sizeof(double) * n);
                if (max_vectors[i].coordinates == NULL) {
                    for (int j = 0; j <= i; ++j) free(max_vectors[j].coordinates);
                    free(max_vectors);
                    return NULL;
                }
            }
        }
        quantity_of_functions_left--;
    }
    for(int i = len+1; i <= capacity; ++i) free(max_vectors[i].coordinates);
    return max_vectors;
}
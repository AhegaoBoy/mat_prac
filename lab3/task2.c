#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define SPACE_VOLUME 3
struct vector
{
    double coordinates[SPACE_VOLUME];
};

struct vector* find_max_mudulo(int, int, int[SPACE_VOLUME][SPACE_VOLUME], double, double,  int*, int*, int*,...);
struct vector multiply_matrix_and_vector(int, int[SPACE_VOLUME][SPACE_VOLUME] , struct vector);
double euclidean_norm(int, double,  struct vector, int[SPACE_VOLUME][SPACE_VOLUME], double);
double infinite_norm(int, double, struct vector, int[SPACE_VOLUME][SPACE_VOLUME], double);
double bin_pow(double, int);
double get_root(double, double, double);
double gelder_norm(int, double,  struct vector, int[SPACE_VOLUME][SPACE_VOLUME],  double);

int main()
{

    double epsilon = 1e-6;
    int matrix[SPACE_VOLUME][SPACE_VOLUME] = {
                    {1, 0, 0},
                    {0, 1, 0},
                    {0, 0, 1}};


    double p_for_gelder = 2.0;

    int first = 0, second = 0, third = 0;
    struct vector vector1 = {.coordinates = {1.0, 2.0, 1.0}};
    struct vector vector2 = {.coordinates = {1.0, 2.0, 1.0}};
    struct vector vector3 = {.coordinates = {2.0, 1.0, 1.0}};




    printf("\tCheck what you get\n");


    struct vector* max_vectors = find_max_mudulo(SPACE_VOLUME, 3, matrix, p_for_gelder, epsilon, &first, &second, &third, euclidean_norm, vector1, vector2, vector3,
                                                 infinite_norm,vector1, vector2, vector3,  gelder_norm, vector1, vector2, vector3);
    if(max_vectors == NULL)
    {
        printf("MEMORY ALLOCATION ERROR\n");
        return 1;
    }

    int pos = 0;
    printf("vectors for eucledian norm:\n");
    for(int i = pos; i < first; ++i)
    {
        for(int j = 0; j < SPACE_VOLUME; ++j) printf("%.lf ", max_vectors[i].coordinates[j]);
        printf("\n");
    }
    printf("vectors for infinite norm:\n");
    pos = first;
    for(int i = pos; i < pos + second; ++i)
    {
        for(int j = 0; j < SPACE_VOLUME; ++j) printf("%.lf ", max_vectors[i].coordinates[j]);
        printf("\n");
    }
    printf("vectors for gelder norm:\n");
    pos += second;
    for (int i = pos; i < pos + third; ++i)
    {
        for (int j = 0; j < SPACE_VOLUME; ++j) printf("%.lf ", max_vectors[i].coordinates[j]);
        printf("\n");
    }

    free(max_vectors);
    return 0;

}

struct vector multiply_matrix_and_vector(int n, int matrix[SPACE_VOLUME][SPACE_VOLUME], struct vector vector)
{
    struct vector result;
    for(int i = 0; i < n; ++i)
    {
        double sum = 0.0;
        for(int j = 0; j < n; ++j) sum += matrix[i][j] * vector.coordinates[j];
        result.coordinates[i] = sum;
    }
    return result;
}
double euclidean_norm(int n, double p,  struct vector vector, int matrix[SPACE_VOLUME][SPACE_VOLUME], double epsilon)
{
    struct vector vector1 = multiply_matrix_and_vector(n, matrix, vector);
    double sum = 0;
    for(int i = 0; i < n; ++i) sum += vector1.coordinates[i] * vector.coordinates[i];
    double res = get_root(sum, 2.0, epsilon);
    return res;
}
double infinite_norm(int n, double p, struct vector vector, int matrix[SPACE_VOLUME][SPACE_VOLUME],  double epsilon)
{
    double max = vector.coordinates[0];
    for(int i = 1; i < n; ++i)
    {
        if((max - vector.coordinates[i]) > epsilon) continue;
        else max = vector.coordinates[i];
    }
    return max;
}

double gelder_norm(int n, double p, struct vector vector, int matrix[SPACE_VOLUME][SPACE_VOLUME],  double epsilon)
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

struct vector* find_max_mudulo(int n, int quantity, int matrix[SPACE_VOLUME][SPACE_VOLUME], double p, double epsilon, int* first, int* second, int* third, ...)
{
    struct vector* max_vectors = (struct vector*)malloc(sizeof(struct vector));
    if(max_vectors == NULL) return NULL;
    int len = 0, capacity = 1;
    *first = 0; *second = 0; *third = 0;
    va_list  args;
    va_start(args,third);
    int quantity_of_functions_left = 3;
    while(quantity_of_functions_left > 0)
    {
        double max_value = 0.0;
        double(*count_function)(int, double,  struct vector, int[SPACE_VOLUME][SPACE_VOLUME],  double) = va_arg(args, double(*)(int, double, struct vector, int[SPACE_VOLUME][SPACE_VOLUME], double));
        int vector_quantity = quantity;
        while(vector_quantity > 0)
        {

            struct vector vector = va_arg(args, struct vector);
            double current_norm = count_function(n, p, vector, matrix, epsilon);
            if(current_norm - max_value > epsilon || fabs(current_norm - max_value) < epsilon)
            {
                for(int i = 0; i < n; ++i) max_vectors[len].coordinates[i] = vector.coordinates[i];
                switch (quantity_of_functions_left) {
                    case 3:
                        (*first)++;
                        break;
                    case 2:
                        (*second)++;
                        break;
                    case 1:
                        (*third)++;
                        break;
                }
                max_value = current_norm;
                len++;
            }

            vector_quantity--;
        }
        quantity_of_functions_left--;
    }
    return max_vectors;
}
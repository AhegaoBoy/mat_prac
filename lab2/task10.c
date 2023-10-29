#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

double fact(int n)
{
    if(n == 1 || n == 0) return 1;
    long long res = 1;
    for(int i = 2; i <= n; ++i) res*=i;
    return res;
}
double C(int n, int k)
{
    return fact(n) / (fact(n - k) * fact(k));
}

void coefficients(int n, double** array_of_coefficients)
{
    for(int k = 0; k <= n; ++k)
    {
        (*array_of_coefficients)[k] = C(n, k);
    }
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

void recomposition_of_polynomial(double epsilon, double a, double** array_of_coefficients, int pow, ...)
{
    (*array_of_coefficients)= (double*)malloc(sizeof(double) * (pow + 1));
    if(array_of_coefficients == NULL)
    {
        //printf("MEMORY ALLOCATION ERROR\n");
        return;
    }
    int n = pow;
    double *start_polynomial = (double *) malloc(sizeof(double) * (pow + 1));
    if (start_polynomial == NULL)
    {
        free(*array_of_coefficients);
        return;
    }
    va_list args;
    va_start(args, pow);
    while(pow >= 0)
    {
        double coefficient_from_original = va_arg(args, double);
        start_polynomial[pow] = coefficient_from_original;
        pow--;
    }
    va_end(args);
    coefficients(n, array_of_coefficients);
    for (int i = 0; i <= n; ++i)
        (*array_of_coefficients)[i] *= bin_pow(a, i) * start_polynomial[0];
    for(int current_pow = 1; current_pow <= n; current_pow++)
    {
        double next_coefficient = start_polynomial[current_pow] - (*array_of_coefficients)[current_pow];
        double *current_polynomial = (double *) malloc(sizeof(double) * (n - current_pow + 1));
        if (current_polynomial == NULL) {
            free(*array_of_coefficients);
            free(start_polynomial);
            return;
        }


        coefficients(n - current_pow, &current_polynomial);
        for (int i = 0; i <= n - current_pow + 1; ++i) {
            current_polynomial[i] *= next_coefficient * bin_pow(a, i);
        }

        (*array_of_coefficients)[current_pow] = current_polynomial[0];
        for (int i = current_pow + 1; i <= n; ++i)
            (*array_of_coefficients)[i] += current_polynomial[i - current_pow];
    }

    free(start_polynomial);

}

int main()
{
    int pow = 2;
    double a = 0.0;
    double* array_of_coefficients =NULL;
    double epsilon = 1e-9;
    recomposition_of_polynomial(epsilon, a, &array_of_coefficients, pow, 2.0, 1.0, 1.0);
    if(array_of_coefficients == NULL)
    {
        printf("MEMORY ALLOCATION ERROR\n");
        return 1;
    }
    printf("RECOMPOSITION LOOKS LIKE:\n");
    for(int i = pow; i >= 0; i--)
        printf("+ %.5f * (x + %.2f)^%d ", array_of_coefficients[i], a, pow - i);
    free(array_of_coefficients);
    return 0;
}
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
    int n = pow--;
    int position = 1;
    va_list args;
    va_start(args, pow);
    double coefficient_from_original = va_arg(args, double);
    coefficients(n, array_of_coefficients);
    for(int i = 0; i <= n; ++i) {
        (*array_of_coefficients)[i] *= bin_pow(a, i) * coefficient_from_original;
    }
    while(pow >= 0)
    {
        coefficient_from_original = va_arg(args, double);
        double next_coefficient = coefficient_from_original - (*array_of_coefficients)[position];
        double* current_polynomial = (double*)malloc(sizeof(double) * (pow + 1));
        if(current_polynomial == NULL)
        {
            (*array_of_coefficients) = NULL;
            return;
        }


        coefficients(pow, &current_polynomial);
        for(int i = 0; i <= pow; ++i) {
            current_polynomial[i] *= next_coefficient * bin_pow(a, i);
        }

        (*array_of_coefficients)[position] = current_polynomial[0];
        for(int i = position + 1; i <= n; ++i)
            (*array_of_coefficients)[i] += current_polynomial[i - position];

        position++;
        pow--;

    }
    va_end(args);
}

int main()
{
    int pow = 6;
    double a = -2.0;
    double* array_of_coefficients = (double*)malloc(sizeof(double) * (pow + 1));
    if(array_of_coefficients == NULL)
    {
        printf("MEMORY ALLOCATION ERROR\n");
        return -1;
    }

    double epsilon = 1e-9;
    recomposition_of_polynomial(epsilon, a, &array_of_coefficients, pow, 1.0, -2.0, 10.0, 100.0, 0.0, 23232.0);
    if(array_of_coefficients == NULL)
    {
        printf("MEMORY ALLOCATION ERROR\n");
        return -1;
    }
    printf("RECOMPOSITION LOOKS LIKE:\n");
    for(int i = 0; i<=pow; ++i)
        printf("+ %.0f * (x + %.0f)^%d ", array_of_coefficients[i], a, pow - i);
    free(array_of_coefficients);
    return 0;
}
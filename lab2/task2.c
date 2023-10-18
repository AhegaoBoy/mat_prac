#include<stdio.h>
#include<stdarg.h>
#include<math.h>

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

double get_root(double x, double exponent, double eps)
{
    if(exponent - 1.0 <=eps) return x;
    double value_current = 1;
    double value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ bin_pow(value_current, exponent-1));
    while(fabs(value_current - value_next) > eps){
        value_current = value_next;
        value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ bin_pow(value_current, exponent-1));
    }
    return value_next;
}

double geometric_mean(int n,...)
{
    va_list args;
    va_start(args, n);
    int sqrt_pow = n;
    double multiplication = 1.0;
    double epsilon = 1e-6;
    while(n > 0)
    {
        multiplication *= va_arg(args, double);
        n--;
    }
    return get_root(multiplication, sqrt_pow, epsilon);
}

int main()
{
    printf("Работа процедуры, вычисляющей среднее геометрическое чисел: %.5f\n", geometric_mean(3, 2.0, 4.0, 8.0));
    printf("Работа процедуры, вычисляющей бинарное возведение вещественного числа в целую степень: %.5f\n", bin_pow(1.4, 2));
}
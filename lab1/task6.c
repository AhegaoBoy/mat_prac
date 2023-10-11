#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double f1(double x, double epsilon)
{
    return log(1 + x) / x;
}
double f2(double x, double epsilon)
{
    return exp(-x*x/2);
}
double f3(double x, double epsilon)
{
    return log(1/(1 - x));
}


double get_pow(double x, double st)
{
    double tmp = x;
    for(int i = 2; i<= st;++i) tmp*=x;
    return tmp;
}

double get_root(double x, double exponent, double eps){
    exponent = 1/exponent;
    if(exponent - 1.0 <=eps) return x;
    double value_current = 1;
    double value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ get_pow(value_current, exponent-1));
    while(fabs(value_current - value_next) > eps){
        value_current = value_next;
        value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ get_pow(value_current, exponent-1));
    }
    return value_next;
}

double f4(double x, double epsilon)
{
    return(get_root(x, x, epsilon));
}
double integral(double(*f)(double, double), double a, double b, double epsilon)
{
    double trapezoids = (b - a)/epsilon;
    double x0 = a + epsilon;
    double int_sum = 0;
    for(int i = 1; i<trapezoids; ++i)
    {
       int_sum += (f(x0, epsilon) + f(x0 + epsilon, epsilon))/2 * epsilon;
       x0 += epsilon; 
    }
    return int_sum;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Введите нужное количество аргументов\n");
        return 1;
    }
    char* end;
    double epsilon;
    epsilon= strtod(argv[1], &end);
    if(epsilon < 0)
    {
        printf("Введите положительный эпсилон\n");
        return 2;
    }
    printf("Первый интеграл равен: %.5f\n", integral(f1, 0, 1, epsilon));
    printf("Второй интеграл равен: %.5f\n", integral(f2, 0, 1, epsilon));
    printf("Третий интеграл равен: %.5f\n", integral(f3, 0, 1, epsilon));
    printf("Четвертый интеграл равен: %.5f\n", integral(f4, 0, 1, epsilon));
    return 0;
}
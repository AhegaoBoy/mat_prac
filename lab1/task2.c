#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double sum_for_stepen = 1.0; 
double next_element_of_sequence = 3.0;

double get_pow(double x, double st)
{
    double tmp = x;
    for(int i = 2; i<= st;++i) tmp*=x;
    return tmp;
}

int is_simple(double t)
{
    if((int)(t)%2 == 0 && (int)(t) != 2 || (int)(t) == 1) return 0;
    for(int i = 3; i*i<=t;i+=2)
        if((int)(t) % i == 0) return 0;
    return 1;
}


double get_root(double x, double exponent, double eps){
    if(exponent - 1.0 <=eps) return x;
    double value_current = 1;
    double value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ get_pow(value_current, exponent-1));
    while(fabs(value_current - value_next) > eps){
        value_current = value_next;
        value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ get_pow(value_current, exponent-1));
    }
    return value_next;
}



double get_power(double n) //get_power
{
    if((int)(n) < sum_for_stepen + next_element_of_sequence) return sum_for_stepen;
    else
    {
        sum_for_stepen+=next_element_of_sequence;
        next_element_of_sequence += 2.0;
        return sum_for_stepen;
    }
}





double factorial(double n)
{
    if(n < 0.5) return 1;
    double tmp = 1.0;
    for(int i = 2; i<=n; i++) tmp*=i;
    return tmp;
}




double lim_e(double n)
{
    double ex = 1 + 1/n;
    double tmp = get_pow(ex, n);
    return tmp;
}

double lim_pi(double n)
{
    double chislitel_1 = get_pow(2.0, 4*n) / get_pow(factorial(2*n), 2.0);
    double chislitel_2 = get_pow(factorial(n), 4.0) / n;
    return chislitel_1 * chislitel_2;
}

double lim_ln2(double n)
{
    double root = get_root(2.0, n, 0.000001);
    return n*(root - 1);
}

double lim_gamma(double n)
{
    double sum = 0.0;
    for(double i = 1; i<n;i+=1.0) sum += 1/i;
    return sum - log(n);
}

double lim(double (*f)(double), double epsilon)
{
    double iter = 2.0;
    double iter_previous = f(iter);
    iter += 1.0;
    double iter_current = f(iter);
    while(fabs(iter_current - iter_previous) > epsilon)
    {
        iter_previous = iter_current;
        iter_current = f(iter + 1.0);
        iter += 1.0;
    }
    return f(iter -= 2.0);
}



double sqrt_step(double x0)
{
    return x0 - x0*x0/2 + 1;
}

double limit_for_sqrt2(double n, double epsilon)
{
    double x0 = n;
    double xn = sqrt_step(x0);
    while(xn - x0 > epsilon)
    {
        x0 = xn;
        xn = sqrt_step(x0);
    }
    return xn;
}





double row_of_e(double n)
{
    return 1/factorial(n);
}

double row_of_pi(double n)
{
    int tmp = ((int) n - 0.5);
    if(tmp % 2 == 0) return 4/(2*n - 1);
    return -4/(2*n - 1);
}

double row_of_gamma(double n, double epsilon)
{
    if((int)(n) == (int)(sum_for_stepen + next_element_of_sequence)) 
    {
        sum_for_stepen += next_element_of_sequence;
        next_element_of_sequence += 2.0;
        return get_pow(-1.0, n)*2*epsilon;
    }      
    return 1/get_power(n) - 1/n;
}
double row_of_ln2(double n, double epsilon)
{
    int tmp = ((int) n - 0.5);
    if(tmp % 2 == 0) return 1/n;
    else return -1/n;
}

double stepen_of_2(double n)
{
    return 1/get_pow(2.0, n);
}





double sum_of_row(double f(double, double), double epsilon, double start)
{
    double step = start;
    double prev_summ = f(step, epsilon);
    step += 1.0;
    double next_summ = prev_summ +  f(step, epsilon);
    while(fabs(next_summ - prev_summ) > epsilon)
    {
        step+=1.0;
        prev_summ = next_summ;
        next_summ = prev_summ + f(step, epsilon);
    }
    return next_summ;
}



double e_eq(double x)
{
    return log(x) - 1;
}

double pi_eq(double x)
{
    return tan(x);
}

double ln2_eq(double x)
{
    return exp(x) - 2;
}

double sqrt2_eq(double x)
{
    return x*x - 2;
}

double multiplication = 1.0;
double lim_for_gamma(double t)
{
    double p = t;
    if(is_simple(p))multiplication *= 1 - 1.0/p;
    return multiplication * log(t);
    
}



double Dix(double (*f)(double), double left, double right, double eps)
{
    double mid;
    while(fabs(f(left) - f(right)) > eps)
    {
    mid = (left + right)/2.0;
    if(f(mid) > eps) right = mid;
    else if(f(mid) < eps) left = mid;
    }
    return mid;
}

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        printf("Введено неверное количество аргуметов\n");
        return 0;
    }
    char* end;
    double epsilon;
    epsilon= strtod(argv[1], &end);
    printf("\tПределы\nlimit of e: %.10f\n", lim((double(*)(double)) lim_e, epsilon));
    printf("limit of pi: %.10f\n", lim((double(*)(double)) lim_pi, epsilon));
    printf("limit of ln2: %.10f\n", lim((double(*)(double)) lim_ln2, epsilon));
    printf("limit of sqrt(2): %.10f\n", limit_for_sqrt2(-0.5, epsilon));
    printf("limit of gamma: %.10f\n\tРяды\n", lim((double(*)(double)) lim_gamma, epsilon));
    printf("Сумма ряда для числа e: %.10f\n", sum_of_row((double(*)(double, double)) row_of_e, epsilon, 0));
    printf("Сумма ряда для числа pi: %.10f\n", sum_of_row((double(*)(double, double)) row_of_pi, epsilon, 1));
    printf("Сумма ряда для числа ln2: %.10f\n", sum_of_row((double(*)(double, double)) row_of_ln2, epsilon, 1));
    double summ_of_stepen = sum_of_row((double(*)(double, double)) stepen_of_2, epsilon, 2);
    printf("Сумма ряда для числа sqrt(2): %.10f\n", get_root(2.0, 1/summ_of_stepen, epsilon));
    printf("Сумма ряда для gamma: %.10f\n\tУравнения\n",  sum_of_row((double(*)(double, double)) row_of_gamma, epsilon, 2) - 3.14*3.14/6.0);
    printf("Решение уравнения lnx = 1: %.10f\n", Dix((double(*)(double)) e_eq, 2.0, 3.0, epsilon));
    printf("Решение уравнения tan(x) = 0: %.10f\n", Dix((double(*)(double)) pi_eq, 3.0, 3.5, epsilon));
    printf("Решение уравнения e^x = 2: %.10f\n", Dix((double(*)(double)) ln2_eq, 0, 1, epsilon));
    printf("Решенеи уравнения x^2 = 2: %.10f\n", Dix((double(*)(double)) sqrt2_eq, 1.0, 2.0, epsilon));
    printf("Решение уравнения для gamma: %.10f\n", -log(lim((double(*)(double)) lim_for_gamma, epsilon)));
}
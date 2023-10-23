#include<stdio.h>
#include<math.h>

double function(double x)
{
    return (7*x - 3.14 * 4 / 3)/sin(x);
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

int main()
{
    double epsilon = 1e-6;
    printf("%.5f\n", Dix(function, 0.0, 1.0, epsilon));
}

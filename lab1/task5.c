#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

enum double_codes
{
    POSITIVE,
    NEGATIVE_OR_ZERO,
    NOT_A_NUMBER
};
int check_double(char** word);

double row_a(int n, double x, double current);
double row_b(int n, double x, double current);
double row_c(int n, double x, double current);
double row_d(int n, double x, double current);
double row(double row_(int n, double x, double current), double epsilon, double x);

int main(int argc, char** argv) {
    if(argc != 3)
    {
        printf("Invalid amount of arguments.\n");
        return 0;
    }
    double x = 0;
    double epsilon = 0;
    double precision = 0;
    char* ptr = "";
    int check_epsilon = check_double(&argv[1]);
    int check_x = check_double(&argv[2]);
    double (*func[])(int n, double epsilon, double x) = {row_a, row_b, row_c, row_d};

    switch(check_epsilon)
    {
        case NEGATIVE_OR_ZERO:
        {
            printf("Accuracy can not be negative or zero.\n");
            break;
        }
        case NOT_A_NUMBER:
        {
            printf("Given accuracy doesn't seem to be a valid float number.\n");
            break;
        }
        case POSITIVE:
        {
            if(check_x == NOT_A_NUMBER)
            {
                printf("Given value for x is not a valid float value.\n");
                break;
            }
            epsilon = strtod(argv[1], &ptr);
            x = strtod(argv[2], &ptr);
            precision = 1.0/epsilon;
            for(int i = 0; i < 4; i++)
            {
                printf("Row %c: ", (i + 'a'));
                if(fabs(x) >= 1.0 && (i == 2 || i == 3))
                {
                    printf("Row diverges on given value of x\n");
                }else
                {
                    printf("%g\n", floor(row(func[i], epsilon, x) * precision) / precision);
                }
            }
            break;
        }
    }
    return 0;
}

int check_double(char** word)
{
    double value;
    char* end;
    value = strtod(*word, &end);

    if(*end != 0)
    {
        return NOT_A_NUMBER;
    }
    if(value <= 0.0)
    {
        return NEGATIVE_OR_ZERO;
    }
    return POSITIVE;
}

double row_a(int n , double x, double current)
{
    if (n == 0) return 1;
    return current*x/n;
}
double row_b(int n, double x, double current)
{
    if(n == 0) return 1;
    return current * (-1) * x * x / ((2*n-1) * (2*n));
}
double row_c(int n, double x, double current)
{
    if(n == 0) return 1;
    for(int i = 3*(n-1)+1; i < 3*n; i++)
    {
        current /= i;
    }
    current = current * 3 * 3 *  n * n * x * x;
    return current;
}
double row_d(int n, double x, double current)
{
    n++;
    if(n == 1) return -(x * x)/2;
    current *= (2*n-1);
    current /= 2*n;
    current *= x * x * (-1);
    return current;
}

double row(double row_(int n, double x, double current), double epsilon, double x)
{
    double value_current = row_(0, x, 0);
    double value_next = row_(1, x, value_current);
    int n = 1;
    while(fabs(value_next) > epsilon)
    {
        n++;
        value_current += value_next;
        value_next = row_(n, x, value_next);
    }
    return value_next + value_current;
}
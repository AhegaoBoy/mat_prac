#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define VERTEX_AMOUNT 4
#define VALUE 2.718281828
#define DEGREE 1
enum exit_codes
{
    MEMORY_ALLOCATION_ERROR,
    CONVEX,
    CONCAVE,
};

typedef struct point_2d
{
    double x;
    double y;
} point_2d;

typedef struct polygon
{
    point_2d* points;
} polygon;

double binary_power(double base, unsigned long long exponent);
int is_convex(int vertex_amount, ...);
double polynomial(double value, int degree, ...);


int main() {
    int _convex = is_convex(VERTEX_AMOUNT, (point_2d){0.8, 0.8}, (point_2d){0,1}, (point_2d){1,1}, (point_2d){1,0});
    if(_convex == MEMORY_ALLOCATION_ERROR)
    {
        printf("memory_error\n");
        return 1;
    }
    printf("%s\n", _convex == CONVEX ? "convex" : "concave" );


    double poly = polynomial(VALUE, DEGREE, VALUE);
    printf("%lf\n", poly);
    return 0;
}
double binary_power(double base, unsigned long long exponent)
{
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


int is_convex(int vertex_amount, ...)
{
    va_list args;
    va_start(args, vertex_amount);

    point_2d* polygon = (point_2d*)malloc(sizeof(point_2d)*vertex_amount);
    if(polygon == NULL)
    {
        return MEMORY_ALLOCATION_ERROR;
    }

    for(int i = 0; i < vertex_amount; i++)
    {
        point_2d point = va_arg(args, point_2d);
        polygon[i] = point;
    }
    va_end(args);

    int i,j,k;
    int flag = 0;
    double z;

    if (vertex_amount < 3)
        return(0);

    for (i = 0; i < vertex_amount; i++)
    {
        j = (i + 1) % vertex_amount;
        k = (i + 2) % vertex_amount;

        z  = (polygon[j].x - polygon[i].x) * (polygon[k].y - polygon[j].y);
        z -= (polygon[j].y - polygon[i].y) * (polygon[k].x - polygon[j].x);
        if (z < 0)
            flag |= 1;
        else if (z > 0)
            flag |= 2;
        if (flag == 3)
            return(CONVEX);
    }
    if (flag != 0)
        return(CONCAVE);
    else
        return(CONVEX);
}

double polynomial(double value, int degree, ...)
{
    va_list args;
    va_start(args, degree);
    double result = 0;

    for(int exponent = degree; exponent >= 0; exponent--)
    {
        double coefficient = va_arg(args, double);
        result += coefficient * binary_power(value, exponent);
    }
    va_end(args);
    return result;
}
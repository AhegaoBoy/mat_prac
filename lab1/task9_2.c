#include<stdlib.h>
#include<stdio.h>
#include<time.h>

typedef enum Enum_Errors
{
    REALLOC_ERROR = 1
}Errors;
int compare (void const* elem1, void const* elem2)
{
    //int value1 = *((int const *)elem1);
    //int value2 = *((int const *)elem2);
    //return value1 - value2;
    
    return *((int const *)elem1) - *((int const *)elem2);
    
    //if(*elem1 - *elem2 < 0) return -1;
    //if(*elem1 - *elem2 == 0) return 0;
    //if(*elem1 - *elem2 > 0) return 1; 
}

int Binary_search(int length, int** array, int find)
{
    int a = 0;
    int b = length;
    int mid = -1;

    while(mid != (a+b) / 2)
    {
        mid = (a+b) / 2;
        if((*array)[mid] == find) return find;
        if((*array)[mid] < find)
        {
            a = mid;
            continue;
        }
        if((*array)[mid] > find)
        {
            b = mid;
            continue;
        }
    }
    return (*array)[mid];     
}
int main()
{
    srand(time(NULL));
    int* arrayA;
    int* arrayB;

    arrayA = (int*)malloc(sizeof(int));
    arrayB = (int*)malloc(sizeof(int));

    if(arrayA == NULL || arrayB == NULL)  perror("Память не выделилась для массива не выделилась");

    const int n = rand()%10000 + 9991;
    const int n1 = rand()%10000 + 9991;

    printf("%d---%d\n", n, n1);
    
    int * for_reallocA;
    for_reallocA = (int*)realloc(arrayA, n*sizeof(int));
    if (for_reallocA == NULL)
    {
        printf("Ошибка realloc\n");
        return REALLOC_ERROR;
    }
    arrayA = for_reallocA;
    free(for_reallocA);

    int* for_reallocB = (int*)realloc(arrayB, n*sizeof(int));
    if(for_reallocB == NULL)
    {
        printf("Ошибка realloc\n");
        return REALLOC_ERROR;
    }
    arrayB = for_reallocB;
    free(for_reallocB);


    for(int i = 0; i<n; ++i)  arrayA[i] = rand() % 1000 - 991;
    for(int i = 0; i<n1; ++i) arrayB[i] = rand() % 1000 - 991;
    
    int* arrayC;
    arrayC = (int*)malloc(sizeof(int));
    if(arrayC == NULL) perror("Память под результирующий массив не выделилась, иди нахуй");
    arrayC = (int*)realloc(arrayC, n*sizeof(int));
    qsort(arrayB, n, sizeof(int), (int (*)(const void *, const void *))compare);
    
    for(int i = 0; i<n; ++i)
    {
        int var = Binary_search(n, &arrayB, arrayA[i]);
        printf("%d\n", var);
        arrayC[i] = arrayA[i] + var;
    }
    
    for(int i = 0; i<n; ++i) printf("%d ",arrayA[i]);
    printf("\n-------------------------------------------------\n");
    for(int i = 0; i < n1; ++i) printf("%d ", arrayB[i]);
    printf("\n-------------------------------------------------\n");
    for(int i = 0; i < n; ++i) printf("%d ", arrayC[i]);
    return 0;
}
#include <math.h>
#include <stdio.h>
int main()
{
    printf("%lld,%lld", (long long int)(1010 / pow(10, (int)(4 / 2))),
        (long long int)(1010 % (int)pow(10, (int)(4 / 2))));
}

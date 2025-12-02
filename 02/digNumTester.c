#include <math.h>
#include <stdio.h>
int main()
{
    for (long long int i = 0; i < 1011; i += 1) {
        int digNum = 1;
        while ((long long int)(i / pow(10, digNum)))
            digNum++;
        printf("%5lld: %d\n", i, digNum);
    }
}

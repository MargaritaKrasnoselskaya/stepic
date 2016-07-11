#include <iostream>
#include <math.h>
#include <cstdlib>
using namespace std;

int main()
{

    long n;
    cin>>n;
    n=n*n;
    int val;
    for (long i =0; i<n; i++)
    {
        val = rand()%10000;
        cout<<val<<" ";
    }
    return 0;
}

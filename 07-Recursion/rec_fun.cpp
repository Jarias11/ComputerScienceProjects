#include <ostream>
#include <fstream>
#include <iostream>
#include "rec_fun.h"
using namespace std;

//decimal to binary
binary_print(ostream& outs, unsigned int n)
{
if( n == 0 )
{
outs << n;
return;
}

binary_print(outs, n/2 );

outs << n % 2;
}

// star pattern
triangle(ostream& outs, unsigned int m, unsigned int n)
{
for(int i=m;i<=n;i++){
for(int j=0;j<i;j++){
outs << "*";
}
outs << endl;
}
for(int i=n;i>=m;i--){
for(int j=i;j>0;j--){
outs << "*";
}
outs << endl;
}
}

//pow function
pow(double x, int n)
{
double result=1;
for(int i=1;i<=n;i++){
result*=x;
}
return result;
}

indented_sentences(size_t m, size_t n)
{
if (m>n) return; //base case   
else{
for (int q = 1; q <m; q++)   
cout <<" ";//print spaces
cout<<"This was written by calling number "<<m<< endl;
//recursively call for m+1 line
//for bottom part
indented_sentences(m +1, n);
for (int q = 1; q <m; q++) //to print spaces
cout <<" ";
cout<<"This was ALSO written by calling number"<<m<<endl;

}
}

#include <iostream>
using namespace std;
int main()
{int i,r=0,s=5527,v,a;cin>>i;
if(i)do{
cin>>v;
a=(v>0?v:-1*v);
if(a<s)r=v,s=a;
if(r==-1*v)r=s;
}while(cin.good());
cout<<r<<"\n";}

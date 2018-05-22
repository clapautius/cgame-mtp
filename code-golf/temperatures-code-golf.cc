#include <iostream>
#define C std::cin
int main(){int r=0,s=5527,v,a;C>>v;if(v)do{C>>v;a=(v>0?v:-v);a<s&&(r=v,s=a);r==-v&&(r=s);}while(C.good());std::cout<<r;}

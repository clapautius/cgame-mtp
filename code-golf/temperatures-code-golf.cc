#include <iostream>
#define C std::cin
int main(){int r=0,s=5527,v,a;C>>a;for(C>>v;a&&C;a=(v>0?v:-v),a<s&&(r=v,s=a),r==-v&&(r=s),C>>v);std::cout<<r;}

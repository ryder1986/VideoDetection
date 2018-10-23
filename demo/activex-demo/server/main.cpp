#include <iostream>
#include "tool.h"
#include "configmanager.h"
#include "app.h"
using namespace std;
//LogFile Tool1::log_file1;//open this when need log
int main()
{

  //  stdin = freopen("data.in","r",stdin);
    //stdout = freopen("data.out","w",stdout);
   // stdout =
//    freopen("data.err","w",stderr);

//    FILE* fd=freopen("data.out","w",stdout);
    //    FILE* fd1=freopen("data.out1","w",fd);

 //   freopen("data.err","w",stderr);
    fflush(NULL);
    cout<<"Video Detection ";
    prt(info,"Start!!!");
    ConfigManager config;
    App app(&config);
    app.start();
    while(1)
    {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    return 0;
}


#include <iostream>
#include "server.h"
#include "tool.h"
using namespace std;
void handle_msg(char * da,int len)
{
    prt(info,"get ->> %s",da);
}
#include "test1.h"

int main()
{
    prt(info,"start");
  //  Test1 t1;
 Test2 t2;
//    TcpClient client("192.168.1.219",6789,handle_msg);
    SLEEP_HERE
    return 0;
}


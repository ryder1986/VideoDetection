#include <iostream>
#include<configmanager.h>
#include "app.h"
using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    ConfigManager cfg;
    App app(&cfg);
    return 0;
}

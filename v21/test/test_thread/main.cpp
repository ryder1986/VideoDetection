#include <iostream>
#include "tool.h"
#include "testclass1.h"
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    Timer2 async_task;
    TestClass1 t1;
    //t1.loop();
    async_task.AsyncWait(0,bind(&TestClass1::loop,&t1));
    cout << "Bye World!" << endl;

    [t1](int a){ cout << " lamda!" << endl;}(333);
    auto x = [](int a)->char{cout << a << endl;return 's';}(897);
    cout << x << endl;


    thread([](){ cout << "thread World!" << endl;}).detach();
    while(1){

    }
    return 0;
}

#ifndef TESTCLASS1_H
#define TESTCLASS1_H

#include <thread>
#include <iostream>
#include "tool.h"
using namespace std;
class TestClass1
{
public:
    TestClass1();
    int loop()
    {
        while (true) {
            prt(info,"loop");
            this_thread::sleep_for(std::chrono::microseconds(1000000));
            return 7;
        }

    }
};

#endif // TESTCLASS1_H

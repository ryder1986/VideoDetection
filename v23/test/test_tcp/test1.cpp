#include "test1.h"

Test1::Test1():client("192.168.1.219",6789,bind(&Test1::handle_msg,this,placeholders::_1,placeholders::_2))
{
    ;
}


#include <iostream>
#include "tool.h"
#include "jsonpacket.h"
using namespace std;
#define TEST_STR1 "{\"name\":\"mike\"}"
#define TEST_STR2 "\
{\
    \"name\":\"mike\",\
    \"books\":\
    [\
        {\
            \"book1\":\"english\"\
        },\
        {\
            \"book2\":\"math\"\
        }\
    ]\
}"
void test_str1()
{
    JsonPacket pkt1(string(TEST_STR1));
    prt(info,"%s",pkt1.str().data());
    prt(info,"%s",pkt1.get("name").to_string().data());
}
void test_str2()
{
    try{
    JsonPacket pkt7(string(TEST_STR2));
    }
    catch(exception e){
    prt(info,"333");
    }
      JsonPacket pkt2(string(TEST_STR2));
    try{
        prt(info,"%s",pkt2.str().data());
        prt(info,"%s",pkt2.get_string("name").data());
  //      prt(info,"%s",JsonPacket(pkt2.get("books").to_array().front()).\
                get("book1").to_string().data());
    }
    catch(Exception e){
        prt(info,"get exception1 %s",e.what());
    }


    JsonPacket pkt3;
   pkt3=pkt2;
    // pkt3="pkt2";
     prt(info,"pkt3 %s",pkt3.str().data());

    //try
     {

     //    JsonPacket pp(pkt2.get("books").to_array().front());
//         JsonPacket pp(pkt2.get("books").to_array());
//               prt(info,"-> %s",pp.str().data());
//        prt(info,"%s",pkt2.str().data());
//        prt(info,"%s",pkt2.get("name").to_string().data());
        prt(info,"%s",JsonPacket(pkt2.get("books").to_array().front()).\
                get("book1").to_string().data());
    }
//    catch(Exception e){
//        prt(info,"get exception1 %s",e.what());
//    }
}

int main()
{
    prt(info,"test start");
    try{
        test_str2();
    }
    catch(exception e)
    {
        prt(info," ex");
    }
    prt(info,"test end");
    return 0;
}

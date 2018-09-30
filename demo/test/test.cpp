#include "test.h"
#include "configmanager.h"
#include "tool.h"
#include "json/json.h"
Test::Test()
{

}
int  main()
{

    ConfigManager cm;

    prt(info,"%s",cm.get_config().str().data());
    JsonPacket pkt1=cm.get_config();
    JsonPacket pkt2;



    vector <int> ia1;
    ia1.push_back(1);
    ia1.push_back(2);
    ia1.push_back(3);
    pkt2.add("intarray",ia1);

    vector <int> vi= pkt2.get("intarray").to_int_array();
    for(int t:vi){
          prt(info,"%d",t);
    }


        vector <string> ia2;
    string s1("str1");
    string s2("str2");
    string s3("str3");
    ia2.push_back(s1);
    ia2.push_back(s2);
    ia2.push_back(s3);
 pkt2.add("strarray",ia2);
 vector <string> vs= pkt2.get("strarray").to_string_array();
 for(string t:vs){
       prt(info,"%s", t.data());
 }

    return 1;
}

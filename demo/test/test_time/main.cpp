#include <iostream>

using namespace std;
#include "tool.h"



#include <sys/time.h>


inline  char *get_sql_time1()
{
    static char buf2[100];// = "2000-01-01-00:00:00";
    char fmt[] = "%Y-%m-%d %H:%M:%S";
    time_t tt;
    tt=time(NULL);
      cout<<  ctime(&tt);

    tm*  lt=localtime(&tt);
    cout<<asctime(lt);
    if (strftime(buf2, 100,fmt, lt) != NULL) {
        prt(info,"err");
    }else{
        prt(info,"ok ###: %s",buf2);


    }
    return buf2;
}
inline long get_time1()
{
    time_t tt;
    struct timeval tv;
    tt=time(NULL);
    tm*lt=   localtime(&tt);
  cout<<  ctime(&tt);

    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000+tv.tv_usec/1000;
}
#include <strstream>
#include <vector>
int main()
{

    vector<int > ss[10];
   vector<int > &t=   ss[2];
   t.push_back(123);
   t.push_back(123);
   cout<<ss[2].front()<<endl;
   for(int tmp=0;tmp<10;tmp++)
   {
 vector<int > &t=   ss[tmp];
        cout<<t.size()<<" ";
   }


    strstream str1;
    str1<<11;

    cout<<str1.str()<<endl;

    //str1<<"22";
   cout<<str1.str()<<endl;
  //  get_time1();
        cout<<get_sql_time1()<<endl;
      cout << "Hello World!" << endl;
    return 0;
}

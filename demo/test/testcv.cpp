#include <opencv2/highgui.hpp>
#include "iostream"
using namespace std;
using namespace cv;
bool end_with_str(string str,string end_str)
{
    bool ret=false;
    if(str.size()>end_str.size()){
       string st= str.substr(str.size()-end_str.size(),end_str.size());
       return st==end_str;
    }
    return ret;
}
int main()
{
    string str("/root/test.png");

  bool ret=end_with_str(str,"ts.png");
  cout<<ret<<endl;

   Mat frame=  imread("/root/test.png");
   Rect r(0,0,100,100);
 Mat frame1=frame(r);
  imshow("",frame);
  waitKey(0);
     std::cout<<"enss"<<endl;
    return 0;
}

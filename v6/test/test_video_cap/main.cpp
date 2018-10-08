#include <iostream>
#include "ffmpegvideocapture.h"
#include "dahuavideocapture.h"
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
   // FfmpegVideoCapture cap("rtsp://192.168.1.95:554/av0_1");
    DahuaVideoCapture cap("rtsp://192.168.1.95:554/av0_1");
      Mat mt;
    while(1){
        if(cap.read(mt)){
        imshow("",mt);
        waitKey(10);
        }
    }

    return 0;
}

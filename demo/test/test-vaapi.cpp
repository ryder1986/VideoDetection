#include <iostream>
#include "player.h"
#include "tool.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;
int main()
{

    Player p;
    Mat pic;
    while (1) {


        if(p.get(pic))
        {
              prt(info,"get a pic ok");
              imshow("",pic);
              waitKey(100);
        }else{

             prt(info,"get a pic fail");
        }
        this_thread::sleep_for(chrono::microseconds(1000000));
    }
    return 0;

}

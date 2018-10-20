#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/video/video.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>


#include "tool.h"
#include "datapacket.h"
using namespace std;
using namespace cv;
#define LABLE_PROCESSOR_C4 "C4"
#define LABLE_PROCESSOR_DUMMY "Dummy"

#ifdef WITH_CUDA
#define LABLE_PROCESSOR_PVD "Pvd"
#define LABLE_PROCESSOR_FVD "Fvd"
#define LABLE_PROCESSOR_MVD "Mvd"
#endif
namespace VideoProcessorNS{
typedef struct args{
    double scale_ratio;
    int scan_step;
    Rect area;
    int no;
    string ratio;

}arg_t;
}
using namespace VideoProcessorNS;
class VideoProcessor
{

protected:
    //  arg_t arg;
public:
    string alg_rst;
#if 1
    VideoProcessor()
    {

    }
#else
    VideoProcessor(DataPacket pkt):JsonDataDealer(pkt)
    {

    }
#endif
    virtual ~VideoProcessor()
    {

    }

    int get_id()
    {
        return channel_id;
    }
    void set_config(DataPacket pkt)
    {
        //        private_data.scale_ratio=string2f(pkt.get_string("ratio"));
        //        private_data.scan_step=pkt.get_int("step");
        //        vector <DataPacket> area=pkt.get_array_packet("detect_area");
        //        private_data.area=area_2_rect(area);
        //        private_data.area.x=0;
        //        private_data.area.y=0;
        //        private_data.area.width=640;
        //        private_data.area.height=480;
    }

    virtual bool modify_processor(JsonPacket p)
    {
        return true;
    }

    DataPacket get_config()
    {
        DataPacket pkt;
        //        pkt.set_int("step",private_data.scan_step);
        //        pkt.set_string("ratio",f2string(private_data.scale_ratio));
        return pkt;
    }
    //    virtual  bool process( Mat img)
    //    {
    //        return false;
    //    }
    //    virtual  bool process( int t)
    //    {
    //        return false;
    //    }
    //    virtual bool process(Mat img_src,vector<Rect> &rects)
    //    {

    //    }
    virtual bool process(Mat img_src,JsonPacket &pkt)=0;

    virtual bool process_whole_pic(Mat img_src,JsonPacket &pkt,Rect rct)
    {

    }

    //    {
    //        prt(info,"actual processor needed");
    //    }
    //    virtual bool process(Mat img_src,vector<Rect> &rects,Rect detect_area)
    //    {

    //        return false;
    //    }

    virtual  string get_rst()
    {
        string ba;
        return ba ;
    }

    virtual void init()
    {
    }

    //    DataPacket get_config()
    //    {
    //        DataPacket ret;
    //        encode(ret);
    //        return ret;
    //    }
    Rect area_2_rect(vector<DataPacket> area)
    {
        int x_min=10000;
        int y_min=10000;
        int x_max=0;
        int y_max=0;
        for(DataPacket pkt: area) {
            int x=pkt.get_int("x");
            int y=pkt.get_int("y");
            //              int x= v.toObject()["x"].toInt();
            //            int y= v.toObject()["y"].toInt();
            if(x<x_min)
                x_min=x;
            if(x>x_max)
                x_max=x;
            if(y<y_min)
                y_min=y;
            if(y>y_max)
                y_max=y;
        }
        return Rect(x_min,y_min,x_max-x_min,y_max-y_min);
    }

protected:
    int channel_id;
private:

};




class DummyProcessorOutputData:public JsonData{

public:
    vector<VdPoint> Points;
    int Radii;
    DummyProcessorOutputData()
    {
    }
    DummyProcessorOutputData(JsonPacket str):JsonData(str)
    {
        decode();
    }
    DummyProcessorOutputData(vector<VdPoint> o,int r):Radii(r)
    {
        Points.assign(o.begin(),o.end());
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(Points);
        DECODE_INT_MEM(Radii);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(Points);
        ENCODE_INT_MEM(Radii);
    }
};
class DummyProcessorInputData:public JsonData{

public:
    bool Horizon;
    bool Vertical;
    int Radii;
    DummyProcessorInputData(bool h,bool v,int r):Horizon(h),Vertical(v),Radii(r)
    {
        encode();
    }
    DummyProcessorInputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    DummyProcessorInputData()
    {

    }
    void decode()
    {
        DECODE_BOOL_MEM(Horizon);
        DECODE_BOOL_MEM(Vertical);
        DECODE_INT_MEM(Radii);
    }
    void encode()
    {
        ENCODE_BOOL_MEM(Horizon);
        ENCODE_BOOL_MEM(Vertical);
        ENCODE_INT_MEM(Radii);
    }
};


class C4ProcessorOutputData:public JsonData{

public:
    vector <VdRect> Rects;
    C4ProcessorOutputData()
    {
    }
    C4ProcessorOutputData(JsonPacket str):JsonData(str)
    {
        decode();
    }
    C4ProcessorOutputData(vector <VdRect> o):Rects(o)
    {
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(Rects);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(Rects);
    }
};
class C4ProcessorInputData:public JsonData{

public:
    double ratio;
    int scan_step;
    C4ProcessorInputData()
    {
    }
    C4ProcessorInputData(JsonPacket str):JsonData(str)
    {
        decode();
    }
    C4ProcessorInputData(int st,double ra):scan_step(st),ratio(ra)
    {
        encode();
    }
    void decode()
    {
        DECODE_INT_MEM(scan_step);
        DECODE_DOUBLE_MEM(ratio);
    }
    void encode()
    {
        ENCODE_INT_MEM(scan_step);
        ENCODE_DOUBLE_MEM(ratio);
    }
};

class PvdProcessorInputData:public JsonData{
public:
    vector <VdPoint> DetectLine;
    PvdProcessorInputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    PvdProcessorInputData(VdPoint p1,VdPoint p2)
    {
        DetectLine.push_back(p1);
        DetectLine.push_back(p2);
        encode();
    }
    PvdProcessorInputData()
    {

    }
    bool set_point(VdPoint new_p,int index)
    {
        bool ret=false;
        if(index>0&&index<=2){
            ret=true;
            DetectLine[index-1]=VdPoint(new_p.x,new_p.y);
            encode();
        }
        return ret;
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(DetectLine);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(DetectLine);
    }
};

class PvdProcessorOutputData:public JsonData{
public:
    vector <ObjectRect> PvdDetectedObjects;
    int PersonFlow1;
    int PersonFlow2;
    int CurrentPersionCount;
    PvdProcessorOutputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    PvdProcessorOutputData(    vector <ObjectRect> objs,
                               int p1,
                               int p2,
                               int cnt)
    {
        PvdDetectedObjects.assign(objs.begin(),objs.end());
        PersonFlow1=p1;
        PersonFlow2=p2;;
        CurrentPersionCount=cnt;
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(PvdDetectedObjects);
        DECODE_INT_MEM(PersonFlow1);
        DECODE_INT_MEM(PersonFlow2);
        DECODE_INT_MEM(CurrentPersionCount);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(PvdDetectedObjects);
        ENCODE_INT_MEM(PersonFlow1);
        ENCODE_INT_MEM(PersonFlow2);
        ENCODE_INT_MEM(CurrentPersionCount);
    }
};


class BaseLineJsonData:public JsonData{
public:
    VdPoint BeginPoint;// start point
    VdPoint EndPoint;// end point
    int Length; // real length
    BaseLineJsonData(VdPoint pt1, VdPoint pt2, int len):BeginPoint(pt1),EndPoint(pt2),Length(len)
    {
        encode();
    }
    BaseLineJsonData(JsonPacket p):JsonData(p)
    {
        decode();
    }
    BaseLineJsonData()
    {

    }
    void decode()
    {
        DECODE_JSONDATA_MEM(BeginPoint);
        DECODE_JSONDATA_MEM(EndPoint);
        DECODE_INT_MEM(Length);
    }
    void encode()
    {
        ENCODE_JSONDATA_MEM(BeginPoint);
        ENCODE_JSONDATA_MEM(EndPoint);
        ENCODE_INT_MEM(Length);
    }
};
class LaneDataJsonData:public JsonData{
public:
    int LaneNo;// lane name index;


    vector <VdPoint> LaneArea; // whole rect
    vector <VdPoint> NearArea; // near rect
    vector <VdPoint> FarArea; // far rect
    LaneDataJsonData(JsonPacket p):JsonData(p)
    {
        decode();
    }
    LaneDataJsonData(int ln, vector <VdPoint> fa,vector <VdPoint> na,vector <VdPoint> la):
        LaneNo(ln),
        FarArea(fa),
        NearArea(na),
        LaneArea(la)
    {
        encode();
    }

    void set_point(VdPoint p,int index)
    {
        if(index<=4)
            LaneArea[index-1]=p;
        if(index>4&&index<=4*2)
            NearArea[(index-1)%4]=p;
        if(index>4*2&&index<=4*3)
            FarArea[(index-1)%4]=p;
        encode();
    }
    int get_valid_x(int x1,int y1,int x2,int y2,int valid_y)
    {
        int valid_x=x1;
        if(y1!=y2)
            valid_x=(x1-x2)*(valid_y-y1)/(y1-y2)+x1;
        return valid_x;
    }
    void adjust_lane_points(VdPoint p1,VdPoint p2, bool left_line)
    {
        int i=0;
        if(left_line){
            i=0; LaneArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,LaneArea[i].y),LaneArea[i].y);
            i=3; LaneArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,LaneArea[i].y),LaneArea[i].y);

            i=0; NearArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,NearArea[i].y),NearArea[i].y);
            i=3; NearArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,NearArea[i].y),NearArea[i].y);

            i=0; FarArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,FarArea[i].y),FarArea[i].y);
            i=3; FarArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,FarArea[i].y),FarArea[i].y);
            /////
#if 1
            i=1; LaneArea[i]=VdPoint(LaneArea[i].x,LaneArea[i-1].y);
            i=2; LaneArea[i]=VdPoint(LaneArea[i].x,LaneArea[i+1].y);

            i=1; NearArea[i]=VdPoint(NearArea[i].x,NearArea[i-1].y);
            i=2; NearArea[i]=VdPoint(NearArea[i].x,NearArea[i+1].y);

            i=1; FarArea[i]=VdPoint(FarArea[i].x,FarArea[i-1].y);
            i=2; FarArea[i]=VdPoint(FarArea[i].x,FarArea[i+1].y);

#endif

        }else{
            i=1; LaneArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,LaneArea[i].y),LaneArea[i].y);
            i=2; LaneArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,LaneArea[i].y),LaneArea[i].y);

            i=1; NearArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,NearArea[i].y),NearArea[i].y);
            i=2; NearArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,NearArea[i].y),NearArea[i].y);

            i=1; FarArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,FarArea[i].y),FarArea[i].y);
            i=2; FarArea[i]=VdPoint(get_valid_x(p1.x,p1.y,p2.x,p2.y,FarArea[i].y),FarArea[i].y);

#if 1
            i=0; LaneArea[i]=VdPoint(LaneArea[i].x,LaneArea[i+1].y);
            i=3; LaneArea[i]=VdPoint(LaneArea[i].x,LaneArea[i-1].y);

            i=0; NearArea[i]=VdPoint(NearArea[i].x,NearArea[i+1].y);
            i=3; NearArea[i]=VdPoint(NearArea[i].x,NearArea[i-1].y);

            i=0; FarArea[i]=VdPoint(FarArea[i].x,FarArea[i+1].y);
            i=3; FarArea[i]=VdPoint(FarArea[i].x,FarArea[i-1].y);

#endif
        }
    }
    void set_point_adjust(VdPoint p,int index)
    {
        VdPoint p_e;
        int  rect_index=(index-1)/4+1;
        bool adjust_left_line;
        int tmp=(index-1)%4;
        if(tmp==1||tmp==2){
            adjust_left_line=false;
        }else{
            adjust_left_line=true;
        }
        if(rect_index==1){
            LaneArea[tmp]=p;
            if(tmp==1){
                VdPoint e(LaneArea[tmp+1]);
                p_e=e;
            }
            if(tmp==2){
                VdPoint e(LaneArea[tmp-1]);
                p_e=e;
            }
            if(tmp==0){
                VdPoint e(LaneArea[tmp+3]);
                p_e=e;
            }
            if(tmp==3){
                VdPoint e(LaneArea[tmp-3]);
                p_e=e;
            }
        }
        if(rect_index==2){
            NearArea[tmp]=p;
            if(tmp==1){
                VdPoint e(NearArea[tmp+1]);
                p_e=e;
            }
            if(tmp==2){
                VdPoint e(NearArea[tmp-1]);
                p_e=e;
            }
            if(tmp==0){
                VdPoint e(NearArea[tmp+3]);
                p_e=e;
            }
            if(tmp==3){
                VdPoint e(NearArea[tmp-3]);
                p_e=e;
            }
        }
        if(rect_index==3){
            FarArea[tmp]=p;
            if(tmp==1){
                VdPoint e(FarArea[tmp+1]);
                p_e=e;
            }
            if(tmp==2){
                VdPoint e(FarArea[tmp-1]);
                p_e=e;
            }
            if(tmp==0){
                VdPoint e(FarArea[tmp+3]);
                p_e=e;
            }
            if(tmp==3){
                VdPoint e(FarArea[tmp-3]);
                p_e=e;
            }
        }

        adjust_lane_points(p,p_e,adjust_left_line);
        encode();
    }
    void decode()
    {
        DECODE_INT_MEM(LaneNo);
        DECODE_JSONDATA_ARRAY_MEM(FarArea);
        DECODE_JSONDATA_ARRAY_MEM(NearArea);
        DECODE_JSONDATA_ARRAY_MEM(LaneArea);

    }
    void encode()
    {
        ENCODE_INT_MEM(LaneNo);
        ENCODE_JSONDATA_ARRAY_MEM(FarArea);
        ENCODE_JSONDATA_ARRAY_MEM(NearArea);
        ENCODE_JSONDATA_ARRAY_MEM(LaneArea);
    }
};
class FvdProcessorInputData:public JsonData{
public:
    vector <VdPoint> BasicCoil;// standard rect
    BaseLineJsonData BaseLine;// a line can adjust car real length
    int NearPointDistance;//distance to camera
    int FarPointDistance;
    vector <LaneDataJsonData> LaneData; // lane info
    FvdProcessorInputData(){}
    FvdProcessorInputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    FvdProcessorInputData(  vector <VdPoint> bl,BaseLineJsonData be,int i1, int i2, vector <LaneDataJsonData>  ld):
        BasicCoil(bl), BaseLine(be), NearPointDistance(i1), FarPointDistance(i2), LaneData(ld)

    {
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(BasicCoil);
        DECODE_JSONDATA_MEM(BaseLine);
        DECODE_INT_MEM(NearPointDistance);
        DECODE_INT_MEM(FarPointDistance);
        DECODE_JSONDATA_ARRAY_MEM(LaneData);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(BasicCoil);
        ENCODE_JSONDATA_MEM(BaseLine);
        ENCODE_INT_MEM(NearPointDistance);
        ENCODE_INT_MEM(FarPointDistance);
        ENCODE_JSONDATA_ARRAY_MEM(LaneData);
    }
};
class LaneOutputJsonData:public JsonData{
public:
    int LaneNo; // lane name index
    int QueueLength;// length of queue
    VdPoint StartQueuePoint;// queue start point
    VdPoint EndQueuePoint;// queue end point
    int LaneVehicleNumber;// lane car count now
    int VehicleFlow;// flow total
    int VehicleSpeed;// near rect car's speed
    int NearActualLength;// near rect real size
    int FarActualLength;// far rect real size
    bool FarCarExist;
    bool NearCarExist;
    LaneOutputJsonData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    LaneOutputJsonData(int lo,int qh,VdPoint st,VdPoint et,int lr,int vw,int vd,int nh,int fh,bool fe,bool ne):
        LaneNo(lo),
        QueueLength(qh),
        StartQueuePoint(st),
        EndQueuePoint(et),
        LaneVehicleNumber(lr),
        VehicleFlow(vw),
        VehicleSpeed(vd),
        NearActualLength(nh),
        FarActualLength(fh),FarCarExist(fe),NearCarExist(ne)
    {
        encode();
    }
    void decode()
    {
        DECODE_INT_MEM(LaneNo);
        DECODE_INT_MEM(QueueLength);
        DECODE_JSONDATA_MEM(StartQueuePoint);
        DECODE_JSONDATA_MEM(EndQueuePoint);
        DECODE_INT_MEM(LaneVehicleNumber);
        DECODE_INT_MEM(VehicleFlow);
        DECODE_INT_MEM(VehicleSpeed);
        DECODE_INT_MEM(NearActualLength);
        DECODE_INT_MEM(FarActualLength);
        DECODE_BOOL_MEM(FarCarExist);
        DECODE_BOOL_MEM(NearCarExist);
    }
    void encode()
    {
        ENCODE_INT_MEM(LaneNo);
        ENCODE_INT_MEM(QueueLength);
        ENCODE_JSONDATA_MEM(StartQueuePoint);
        ENCODE_JSONDATA_MEM(EndQueuePoint);
        ENCODE_INT_MEM(LaneVehicleNumber);
        ENCODE_INT_MEM(VehicleFlow);
        ENCODE_INT_MEM(VehicleSpeed);
        ENCODE_INT_MEM(NearActualLength);
        ENCODE_INT_MEM(FarActualLength);
        ENCODE_BOOL_MEM(FarCarExist);
        ENCODE_BOOL_MEM(NearCarExist);
    }
};
//class VdRect:public JsonData
//{
//public:
//	int x;
//	int y;
//	int w;
//	int h;
//	VdRect(JsonPacket pkt):JsonData(pkt)
//	{
//		decode();
//	}
//	VdRect(int x,int y,int w, int h):x(x),y(y),w(w),h(h)
//	{
//		encode();
//	}
//	VdRect()
//	{

//	}
//	void decode()
//	{
//		DECODE_INT_MEM(x);
//		DECODE_INT_MEM(y);
//		DECODE_INT_MEM(w);
//		DECODE_INT_MEM(h);
//	}
//	void encode()
//	{
//		ENCODE_INT_MEM(x);
//		ENCODE_INT_MEM(y);
//		DECODE_INT_MEM(w);
//		DECODE_INT_MEM(h);
//	}
//};
class LinePoint:public JsonData{
public:
	VdPoint StartPoint;
	VdPoint EndPoint;
	LinePoint(JsonPacket p):JsonData(p)
	{
		decode();
	}
	LinePoint(VdPoint pt1,VdPoint pt2):StartPoint(pt1),EndPoint(pt2)
	{
		encode();
	}
	void decode()
	{
        DECODE_JSONDATA_MEM(StartPoint);
        DECODE_JSONDATA_MEM(EndPoint);
	}
	void encode()
	{
        ENCODE_JSONDATA_MEM(StartPoint);
        ENCODE_JSONDATA_MEM(EndPoint);
	}
};
class DegreeJsonData:public JsonData{
public:
    int DegreePointsY;
    int DegreeValue;
    DegreeJsonData(JsonPacket p):JsonData(p)
    {
        decode();
    }
    DegreeJsonData(int a,int b):DegreePointsY(a),DegreeValue(b)
    {
        encode();
    }
    void decode()
    {
        DECODE_INT_MEM(DegreePointsY);
        DECODE_INT_MEM(DegreeValue);
    }
    void encode()
    {
        ENCODE_INT_MEM(DegreePointsY);
        ENCODE_INT_MEM(DegreeValue);
    }
};


class FvdProcessorOutputData:public JsonData{
public:
    vector <ObjectRect> FvdDetectedObjects;//all rects of car
    int CurrentVehicleNumber; // cars number count on screen now;
    int Visibility;// visiable or not
    int VideoState;// video state
    vector <LaneOutputJsonData> LaneOutputData;// output
    vector <DegreeJsonData> DegreeData; // on  lane points

    FvdProcessorOutputData(JsonPacket p):JsonData(p)
    {
        decode();
    }
    FvdProcessorOutputData(vector <ObjectRect> fs, int cr, int vy, int ve,  vector <LaneOutputJsonData> la,  vector <DegreeJsonData> da):
        FvdDetectedObjects(fs),
        CurrentVehicleNumber(cr),
        Visibility(vy),
        VideoState(ve),
        LaneOutputData(la),
        DegreeData(da)
    {
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(FvdDetectedObjects);
        DECODE_INT_MEM(CurrentVehicleNumber);
        DECODE_INT_MEM(Visibility);
        DECODE_INT_MEM(VideoState);
        DECODE_JSONDATA_ARRAY_MEM(LaneOutputData);
        DECODE_JSONDATA_ARRAY_MEM(DegreeData);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(FvdDetectedObjects);
        ENCODE_INT_MEM(CurrentVehicleNumber);
        ENCODE_INT_MEM(Visibility);
        ENCODE_INT_MEM(VideoState);
        ENCODE_JSONDATA_ARRAY_MEM(LaneOutputData);
        ENCODE_JSONDATA_ARRAY_MEM(DegreeData);
    }
};

class EventRegion:public JsonData{
public:
    enum MvdEvent{
        OVER_SPEED=1,
        REVERSE_DRIVE,
        STOP_INVALID,
        NO_PEDESTRIANTION,
        DRIVE_AWAY,
        CONGESTION,
        AbANDON_OBJECT
    };
    vector <VdPoint> Vers;
    int Type;
    int Direction;
    EventRegion(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    EventRegion(vector <VdPoint> vs, int t):Vers(vs),Type(t)
    {
        encode();
    }
    void decode()
    {
        DECODE_INT_MEM(Type);
        DECODE_JSONDATA_ARRAY_MEM(Vers);
		DECODE_INT_MEM(Direction);
    }
    void encode()
    {
        ENCODE_INT_MEM(Type);
        ENCODE_JSONDATA_ARRAY_MEM(Vers);
		DECODE_INT_MEM(Direction);
    }
};

class EventRegionObjectOutput:public JsonData{
public:
    vector <VdPoint> Vers;
    int Type;
	int EventID;
    EventRegionObjectOutput(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    EventRegionObjectOutput(vector <VdPoint> vs, int t, int id):Vers(vs),Type(t),EventID(id)
    {
        encode();
    }
    void decode()
    {
        DECODE_INT_MEM(Type);
        DECODE_INT_MEM(EventID);
        DECODE_JSONDATA_ARRAY_MEM(Vers);
    }
    void encode()
    {
        ENCODE_INT_MEM(Type);
        ENCODE_INT_MEM(EventID);
        ENCODE_JSONDATA_ARRAY_MEM(Vers);
    }
};
class MvdProcessorInputData:public JsonData{
public:
    vector <VdPoint> BasicCoil;// standard rect
    BaseLineJsonData BaseLine;// a line can adjust car real length
    int NearPointDistance;//distance to camera
    int FarPointDistance;
    vector <LaneDataJsonData> LaneData; // lane info
    vector <VdPoint> DetectLine;
    vector <EventRegion> Events;
  //   OppositeDirDriveRegion;
    MvdProcessorInputData(){}
    MvdProcessorInputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
//    MvdProcessorInputData(  vector <VdPoint> bl,BaseLineJsonData be,int i1, int i2, vector <LaneDataJsonData>  ld, vector <VdPoint>de):
//        BasicCoil(bl), BaseLine(be), NearPointDistance(i1), FarPointDistance(i2), LaneData(ld),DetectLine(de)

//    {
//        encode();
//    }
    MvdProcessorInputData(  vector <VdPoint> bl,BaseLineJsonData be,int i1, int i2,
                            vector <LaneDataJsonData>  ld, vector <VdPoint>de
                            ,vector <EventRegion> es):
        BasicCoil(bl), BaseLine(be), NearPointDistance(i1), FarPointDistance(i2),
        LaneData(ld),DetectLine(de),Events(es)

    {
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(BasicCoil);
        DECODE_JSONDATA_MEM(BaseLine);
        DECODE_INT_MEM(NearPointDistance);
        DECODE_INT_MEM(FarPointDistance);
        DECODE_JSONDATA_ARRAY_MEM(LaneData);
        DECODE_JSONDATA_ARRAY_MEM(DetectLine);
        DECODE_JSONDATA_ARRAY_MEM(Events);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(BasicCoil);
        ENCODE_JSONDATA_MEM(BaseLine);
        ENCODE_INT_MEM(NearPointDistance);
        ENCODE_INT_MEM(FarPointDistance);
        ENCODE_JSONDATA_ARRAY_MEM(LaneData);
        ENCODE_JSONDATA_ARRAY_MEM(DetectLine);
        ENCODE_JSONDATA_ARRAY_MEM(Events);
    }
    void add_lane(JsonPacket pkt)
    {
        LaneDataJsonData dt(pkt);
        LaneData.push_back(dt);
        encode();
    }
    void del_lane()
    {
        // LaneDataJsonData dt(pkt);
        LaneData.erase(LaneData.end()-1);
        encode();
    }
    void append_event(EventRegion e)
    {
        Events.push_back(e);
        encode();
    }
    void del_event()
    {
        Events.erase(Events.begin()+Events.size());
        decode();
    }
    bool set_point(VdPoint new_p,int index)
    {
        bool ret=false;
        if(index>0&&index<=2){
            ret=true;
            DetectLine[index-1]=VdPoint(new_p.x,new_p.y);
            encode();
        }
        //int lane_cout=LaneData.size();
        if(index>2){
            index-=2;
            int lane_index=(index-1)/12+1;
            index=(index-1)%12+1;

            LaneDataJsonData d=LaneData[lane_index-1];
#if 0
            d.set_point(new_p,index);
#else
            d.set_point_adjust(new_p,index);
#endif
            LaneData[lane_index-1]=d;

            encode();
            ret=true;
        }
        return ret;
    }
};
class MvdProcessorOutputData:public JsonData{
public:
    vector <ObjectRect> MvdDetectedObjects;//all rects of car
    int CurrentVehicleNumber; // cars number count on screen now;
    int Visibility;// visiable or not
    int VideoState;// video state
    vector <LaneOutputJsonData> LaneOutputData;// output
    vector <DegreeJsonData> DegreeData; // on  lane points
    int PersonFlow1;
    int PersonFlow2;
    int CurrentPersionCount;


    vector <EventRegionObjectOutput> EventObjects;
//	vector<VdRect> InvalidStopData;
//	vector<VdRect> ReverseDriveData;
//	vector<VdRect> DriveAwayData;
//	vector<VdRect> NoPedestrainData;
//	vector<LinePoint> CongestionData;
//	vector<VdRect>	AbandonedObjectData;

    MvdProcessorOutputData(JsonPacket p):JsonData(p)
    {
        decode();
    }
    MvdProcessorOutputData()
    {
    }
    MvdProcessorOutputData(vector <ObjectRect> fs, int cr, int vy, int ve,
                           vector <LaneOutputJsonData> la,  vector <DegreeJsonData> da,int p1,int p2,int ct,
     vector <EventRegionObjectOutput> eo    ):
        MvdDetectedObjects(fs),
        CurrentVehicleNumber(cr),
        Visibility(vy),
        VideoState(ve),
        LaneOutputData(la),
        DegreeData(da),
        PersonFlow1(p1),
        PersonFlow2(p2),
        CurrentPersionCount(ct),
        EventObjects(eo)
    {
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(MvdDetectedObjects);
        DECODE_INT_MEM(CurrentVehicleNumber);
        DECODE_INT_MEM(Visibility);
        DECODE_INT_MEM(VideoState);
        DECODE_JSONDATA_ARRAY_MEM(LaneOutputData);
        DECODE_JSONDATA_ARRAY_MEM(DegreeData);

        DECODE_INT_MEM(PersonFlow1);
        DECODE_INT_MEM(PersonFlow2);
        DECODE_INT_MEM(CurrentPersionCount);

        DECODE_JSONDATA_ARRAY_MEM(EventObjects);
//		DECODE_JSONDATA_ARRAY_MEM(InvalidStopData);
//		DECODE_JSONDATA_ARRAY_MEM(ReverseDriveData);
//		DECODE_JSONDATA_ARRAY_MEM(DriveAwayData);
//		DECODE_JSONDATA_ARRAY_MEM(NoPedestrainData);
//		DECODE_JSONDATA_ARRAY_MEM(CongestionData);
//		DECODE_JSONDATA_ARRAY_MEM(AbandonedObjectData);

    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(MvdDetectedObjects);
        ENCODE_INT_MEM(CurrentVehicleNumber);
        ENCODE_INT_MEM(Visibility);
        ENCODE_INT_MEM(VideoState);
        ENCODE_JSONDATA_ARRAY_MEM(LaneOutputData);
        ENCODE_JSONDATA_ARRAY_MEM(DegreeData);

        ENCODE_INT_MEM(PersonFlow1);
        ENCODE_INT_MEM(PersonFlow2);
        ENCODE_INT_MEM(CurrentPersionCount);

        ENCODE_JSONDATA_ARRAY_MEM(EventObjects);

//        ENCODE_JSONDATA_ARRAY_MEM(InvalidStopData);
//        ENCODE_JSONDATA_ARRAY_MEM(ReverseDriveData);
//        ENCODE_JSONDATA_ARRAY_MEM(DriveAwayData);
//        ENCODE_JSONDATA_ARRAY_MEM(NoPedestrainData);
//        ENCODE_JSONDATA_ARRAY_MEM(CongestionData);
//        ENCODE_JSONDATA_ARRAY_MEM(AbandonedObjectData);
    }
};
#endif // VIDEOPROCESSOR_H

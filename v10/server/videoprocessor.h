#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H
#include "tool.h"
#include "opencv2/core.hpp"
#include "jsonpacket.h"
using namespace std;
using namespace cv;
#define LABEL_PROCESSOR_DUMMY "Dummy"
#define LABEL_PROCESSOR_MVD "Mvd"
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

    virtual bool modify_processor(JsonPacket p)
    {
        return true;
    }

    virtual bool process(Mat img_src,JsonPacket &pkt)=0;

    virtual bool process_whole_pic(Mat img_src,JsonPacket &pkt,Rect rct)
    {

    }
    virtual  string get_rst()
    {
        string ba;
        return ba ;
    }

    virtual void init()
    {
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
    template <typename A,typename B,typename C>
    void draw(int offx,int offy,
              A draw_line,
              B draw_circle,C draw_text)
    {
        for(VdPoint p:Points){
            draw_circle(VdPoint(p.x+offx,p.y+offy),Radii,PaintableData::Colour::Red,2);
        }
    }

};
class DummyProcessorInputData:public JsonData,public PaintableData{

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
    static DummyProcessorInputData get_dummy_test_data()
    {
        DummyProcessorInputData d(true,false,17);
        return d;
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
        OVER_SPEED,
        REVERSE_DRIVE,
        STOP_INVALID,
        NO_PEDESTRIANTION,
        DRIVE_AWAY
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
class MvdProcessorInputData:public JsonData,public PaintableData{
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
    MvdProcessorInputData(JsonPacket pkt,PaintableData pd)
    {
        seizing=pd.seizing;
        point_index=pd.point_index;
        event_type=pd.event_type;
        ori_pnt=pd.ori_pnt;
        config=pkt;
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
        //    DECODE_JSONDATA_ARRAY_MEM(Events);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(BasicCoil);
        ENCODE_JSONDATA_MEM(BaseLine);
        ENCODE_INT_MEM(NearPointDistance);
        ENCODE_INT_MEM(FarPointDistance);
        ENCODE_JSONDATA_ARRAY_MEM(LaneData);
        ENCODE_JSONDATA_ARRAY_MEM(DetectLine);
        //   ENCODE_JSONDATA_ARRAY_MEM(Events);
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
    //    void append_event(EventRegion e)
    //    {
    //        Events.push_back(e);
    //        encode();
    //    }
    //    void del_event()
    //    {
    //        Events.erase(Events.begin()+Events.size());
    //        decode();
    //    }
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

    bool press(VdPoint pnt)
    {
//        if((point_index=p_on_v(ExpectedAreaVers,pnt))){
//            seizing=true;
//            event_type=PaintableData::Event::MoveVer;
//            return true;
//        }
//        if(p_on_vl(ExpectedAreaVers,pnt)){
//            seizing=true;
//            ori_pnt=pnt;
//            event_type=PaintableData::Event::MoveAll;
//            return true;
//        }
        prt(info,"mvd press")
                return false;
    }
    bool right_press(VdPoint pnt)
    {
        //  seizing=true;
        prt(info,"mvd right press")
                return false;
    }
    bool move(VdPoint pnt)
    {
        return false;
    }
    bool double_click(VdPoint pnt)
    {

        return false;
    }
    void release()
    {
        seizing=false;

    }
    template <typename A>
    void draw_vers_line(vector<VdPoint> ps,A draw_line)
    {
        //  draw_line(ExpectedAreaVers.front(),ExpectedAreaVers.back(),PaintableData::Colour::Red,2);
        if(ps.size()>=4){
            for(int i=0;i<ps.size()-1;i++){
                draw_line(ps[i],ps[i+1],PaintableData::Colour::Blue,2);
            }
            draw_line(ps.front(),ps.back(),PaintableData::Colour::Blue,2);

        }
    }

    template <typename A,typename B,typename C>
    void draw(int offx,int offy,
              A draw_line,
              B draw_circle,C draw_text)
    {
        for(LaneDataJsonData r: LaneData)
        {
            for(int i=0;i<r.FarArea.size();i++){
                r.FarArea[i].x+=offx;
                r.FarArea[i].y+=offy;
            }
            draw_vers_line(r.FarArea,draw_line);

            for(int i=0;i<r.NearArea.size();i++){
                r.NearArea[i].x+=offx;
                r.NearArea[i].y+=offy;
            }
            draw_vers_line(r.NearArea,draw_line);


//            for(int i=0;i<r.LaneArea.size();i++){
//                r.LaneArea[i].x+=offx;
//                r.LaneArea[i].y+=offy;
//            }
//            draw_vers_line(r.LaneArea,draw_line);
        }
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
    vector<VdRect> InvalidStopData;
    vector<VdRect> ReverseDriveData;
    vector<VdRect> DriveAwayData;
    vector<VdRect> NoPedestrainData;
    vector<LinePoint> CongestionData;
    vector<VdRect>	AbandonedObjectData;
    MvdProcessorOutputData(JsonPacket p):JsonData(p)
    {
        decode();
    }
    MvdProcessorOutputData()
    {
    }
    MvdProcessorOutputData(vector <ObjectRect> fs, int cr, int vy, int ve,
                           vector <LaneOutputJsonData> la,  vector <DegreeJsonData> da,int p1,int p2,int ct,
                           vector<VdRect> is_rct, vector<VdRect> rd_rct,vector<VdRect> da_rct, vector<VdRect> np_rct, vector<LinePoint> cpt, vector<VdRect> ao_rct):
        MvdDetectedObjects(fs),
        CurrentVehicleNumber(cr),
        Visibility(vy),
        VideoState(ve),
        LaneOutputData(la),
        DegreeData(da),
        PersonFlow1(p1),
        PersonFlow2(p2),
        CurrentPersionCount(ct),
        InvalidStopData(is_rct),
        ReverseDriveData(rd_rct),
        DriveAwayData(da_rct),
        NoPedestrainData(np_rct),
        CongestionData(cpt),
        AbandonedObjectData(ao_rct)
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
        DECODE_JSONDATA_ARRAY_MEM(InvalidStopData);
        DECODE_JSONDATA_ARRAY_MEM(ReverseDriveData);
        DECODE_JSONDATA_ARRAY_MEM(DriveAwayData);
        DECODE_JSONDATA_ARRAY_MEM(NoPedestrainData);
        DECODE_JSONDATA_ARRAY_MEM(CongestionData);
        DECODE_JSONDATA_ARRAY_MEM(AbandonedObjectData);

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
        ENCODE_JSONDATA_ARRAY_MEM(InvalidStopData);
        ENCODE_JSONDATA_ARRAY_MEM(ReverseDriveData);
        ENCODE_JSONDATA_ARRAY_MEM(DriveAwayData);
        ENCODE_JSONDATA_ARRAY_MEM(NoPedestrainData);
        ENCODE_JSONDATA_ARRAY_MEM(CongestionData);
        ENCODE_JSONDATA_ARRAY_MEM(AbandonedObjectData);
    }
    template <typename A>
    void draw_rect(VdRect rect ,A draw_line )
    {
        VdPoint p1(rect.x,rect.y);
        VdPoint p2(rect.x+rect.w,rect.y);
        VdPoint p3(rect.x+rect.w,rect.y+rect.h);
        VdPoint p4(rect.x,rect.y+rect.h);

        draw_line(p1,p2,PaintableData::Colour::Red,2);
        draw_line(p2,p3,PaintableData::Colour::Red,2);
        draw_line(p3,p4,PaintableData::Colour::Red,2);
        draw_line(p1,p4,PaintableData::Colour::Red,2);


    }

    template <typename A,typename B,typename C>
    void draw(int offx,int offy,
              A draw_line,
              B draw_circle,C draw_text)
    {
        //        for(VdPoint p:Points){
        //            draw_circle(VdPoint(p.x+offx,p.y+offy),Radii,PaintableData::Colour::Red,2);
        //        }

        vector <VdRect>rcts;
        for(ObjectRect r:MvdDetectedObjects){
            rcts.push_back(VdRect(r.x+offx,r.y+offy,r.w,r.h));
        }
        for(VdRect r:rcts)
            draw_rect(r,draw_line);
    }
};
#endif // VIDEOPROCESSOR_H

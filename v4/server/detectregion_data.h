#ifndef DETECTREGION_DATA_H
#define DETECTREGION_DATA_H
#include "tool.h"
#include "jsonpacket.h"
class DetectRegionInputData:public JsonData,public PaintableData
{

public:
    string SelectedProcessor;
    vector <VdPoint>ExpectedAreaVers;
    JsonPacket ProcessorData;//TODO: dynamic binding
    DetectRegionInputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    DetectRegionInputData(JsonPacket pdata,string type,vector<VdPoint> vers)
        :ProcessorData(pdata),SelectedProcessor(type),ExpectedAreaVers(vers)
    {
        encode();
    }

    void set_point(VdPoint p,int index)
    {
        ExpectedAreaVers[index-1]=p;
        encode();
    }

    void set_points(vector <VdPoint> p)
    {
        ExpectedAreaVers=p;
        encode();
    }

    void set_processor_data(JsonPacket p)
    {
        ProcessorData=p;
        encode();
    }

    void set_processor(string p,JsonPacket pd)
    {
        SelectedProcessor=p;
        ProcessorData=pd;
        encode();
    }
    void operator =(DetectRegionInputData dt)
    {
        config=dt.config;
        decode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
        DECODE_PKT_MEM(ProcessorData);
        DECODE_STRING_MEM(SelectedProcessor);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
        ENCODE_STRING_MEM(SelectedProcessor);
        ENCODE_PKT_MEM(ProcessorData);
    }
    static DetectRegionInputData get_region_test_data(JsonPacket pkt,string SelectedProcessor)
    {

        vector <VdPoint>ExpectedAreaVers;
        ExpectedAreaVers.push_back(VdPoint(0,0));
        ExpectedAreaVers.push_back(VdPoint(640,0));
        ExpectedAreaVers.push_back(VdPoint(640,480));
        ExpectedAreaVers.push_back(VdPoint(0,480));

        DetectRegionInputData rd(pkt,SelectedProcessor,ExpectedAreaVers);
        return rd;
    }

    virtual bool press(VdPoint pnt)
    {
        if((point_index=p_on_v(ExpectedAreaVers,pnt))){
            seizing=true;
            event_type=PaintableData::Event::MoveVer;
            return true;
        }
        if(p_on_vl(ExpectedAreaVers,pnt)){
            seizing=true;
            ori_pnt=pnt;
            event_type=PaintableData::Event::MoveAll;
            return true;
        }
        return true;
    }
    virtual bool move(VdPoint pnt)
    {
        if(seizing){
            switch (event_type) {
            case Event::MoveVer:
                ExpectedAreaVers[point_index-1]=pnt;
                break;
            case Event::MoveAll:
            {
                int offx=pnt.x-ori_pnt.x;
                int offy=pnt.y-ori_pnt.y;
                ori_pnt=pnt;
                int i=0;
                int sz=ExpectedAreaVers.size();
                for(i=0;i<sz;i++){
                    ExpectedAreaVers[i]=VdPoint(ExpectedAreaVers[i].x+offx,ExpectedAreaVers[i].y+offy);
                }
                break;
            }
            default:
                break;
            }
            encode();
            return true;

        }
        return true;
    }
    virtual bool double_click(VdPoint pnt)
    {
        return true;
    }
    virtual void release()
    {
        release_event();
    }
    template <typename A,typename B>
    void draw(A draw_line, B draw_circle)
    {
        for(int i=1;i<ExpectedAreaVers.size();i++){
            if(!seizing)
                draw_line(ExpectedAreaVers[i-1],ExpectedAreaVers[i],PaintableData::Colour::Blue,2);
            else
                draw_line(ExpectedAreaVers[i-1],ExpectedAreaVers[i],PaintableData::Colour::Red,2);
        }
        if(!seizing)
            draw_line(ExpectedAreaVers.front(),ExpectedAreaVers.back(),PaintableData::Colour::Blue,2);
        else
            draw_line(ExpectedAreaVers.front(),ExpectedAreaVers.back(),PaintableData::Colour::Red,2);

    }
};
class ProcessorDataJsonDataRequest:public JsonData{
public:
    string SelectedProcessor;
    JsonPacket ProcessorData;
    ProcessorDataJsonDataRequest(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    ProcessorDataJsonDataRequest(string pro,JsonPacket d)
    {
        ProcessorData=d;
        SelectedProcessor=pro;
        encode();
    }
    void decode()
    {
        DECODE_STRING_MEM(SelectedProcessor);
        DECODE_PKT_MEM(ProcessorData);
    }
    void encode()
    {
        ENCODE_STRING_MEM(SelectedProcessor);
        ENCODE_PKT_MEM(ProcessorData);
    }
};
class AreaVersJsonDataRequest:public JsonData{
public:
    vector <VdPoint>ExpectedAreaVers;
    AreaVersJsonDataRequest(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    AreaVersJsonDataRequest(vector <VdPoint> vs)
    {
        ExpectedAreaVers.assign(vs.begin(),vs.end());
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
    }
};
class DetectRegionOutputData:public JsonData
{
public:
    VdRect DetectionRect;
    JsonPacket Result;
    DetectRegionOutputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }

    DetectRegionOutputData(JsonPacket rst ,VdRect rct)
    {
        DetectionRect=rct;
        Result=rst;
        encode();
    }

    void decode()
    {
        DECODE_PKT_MEM(DetectionRect);
        DECODE_PKT_MEM(Result);
    }

    void encode()
    {
        ENCODE_JSONDATA_MEM(DetectionRect);
        ENCODE_PKT_MEM(Result);
    }

};


#endif // DETECTREGION_DATA_H

#ifndef CAMERA_DATA_H
#define CAMERA_DATA_H
#include "tool.h"
#include "jsonpacket.h"
#include "detectregion_data.h"
class CameraInputData:public JsonData,PaintableData
{
public:
    enum OP{
        CHANGE_URL=1,
        INSERT_REGION,
        DELETE_REGION,
        MODIFY_REGION
    };
    string Url;
    vector <DetectRegionInputData >DetectRegion;
    CameraInputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    CameraInputData()
    {
    }
    CameraInputData(vector <DetectRegionInputData> regions,string url):DetectRegion(regions),Url(url)
    {
        encode();
    }
    void set_region(JsonPacket data,int index)
    {
        DetectRegion[index-1]=data;
        encode();
    }
    void insert_region(JsonPacket data,int index)
    {
        vector <DetectRegionInputData >::iterator begin=DetectRegion.begin();
        DetectRegion.insert(begin+index,data);
        encode();
    }
    void change_url(string u)
    {
        Url=u;
        encode();
    }
    void change_url(JsonPacket url)
    {
        Url=url.get("Url").to_string();
        encode();
    }
    void delete_region(int index)
    {
        vector <DetectRegionInputData >::iterator begin=DetectRegion.begin();
        DetectRegion.erase(begin+index-1);
        encode();
    }
    void decode()
    {
        DECODE_STRING_MEM(Url);
        DECODE_JSONDATA_ARRAY_MEM(DetectRegion);
    }
    void encode()
    {
        ENCODE_STRING_MEM(Url);
        ENCODE_JSONDATA_ARRAY_MEM(DetectRegion);
    }
    virtual bool press(VdPoint pnt)
    {
        for(int i=0;i<DetectRegion.size();i++){
           DetectRegionInputData &dt= DetectRegion[i];
           dt.press(pnt);
        }
        return true;
    }
    virtual bool right_press(VdPoint pnt)
    {
        for(int i=0;i<DetectRegion.size();i++){
           DetectRegionInputData &dt= DetectRegion[i];
         //  dt.right_press(pnt);
        }
        return true;
    }
    virtual bool move(VdPoint pnt)
    {
        for(int i=0;i<DetectRegion.size();i++){
           DetectRegionInputData &dt= DetectRegion[i];
           dt.move(pnt);
        }
        return true;
    }
    virtual bool double_click(VdPoint pnt)
    {
        for(int i=0;i<DetectRegion.size();i++){
           DetectRegionInputData &dt= DetectRegion[i];
           dt.double_click(pnt);
        }
        return true;
    }
    virtual void release()
    {
        for(int i=0;i<DetectRegion.size();i++){
           DetectRegionInputData &dt= DetectRegion[i];
           dt.release();
        }

    }
    void modify_rect(RequestPkt pkt)
    {

    }
    bool release(RequestPkt &req)
    {
        RequestPkt req1;
        for(int i=0;i<DetectRegion.size();i++){
           DetectRegionInputData &dt= DetectRegion[i];
           if(dt.release(req1)){
                req=get_request_pkt(CameraInputData::MODIFY_REGION,i+1,req1.data());
                return true;
           }
        }
        return false;
    }

    template <typename A,typename B>
    void draw(
            A draw_line,
            B draw_circle)
    {
        int sz=DetectRegion.size();
        for(int i=0;i<sz;i++){
           DetectRegionInputData &dt= DetectRegion[i];
           dt.draw(draw_line,draw_circle);
        }
    }
    static CameraInputData get_camera_test_data(vector <DetectRegionInputData> regions,string url)
    {
        CameraInputData rd(regions,url);
        return rd;
    }
};

class CameraOutputData:public JsonData
{
public:
    vector<DetectRegionOutputData> DetectionResult;
    int Timestamp;
    CameraOutputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    CameraOutputData()
    {
    }
    CameraOutputData(vector <DetectRegionOutputData> regions,int ts):DetectionResult(regions),Timestamp(ts)
    {
        encode();
    }
    void decode()
    {
        DECODE_INT_MEM(Timestamp);
        DECODE_JSONDATA_ARRAY_MEM(DetectionResult);
    }
    void encode()
    {
        ENCODE_INT_MEM(Timestamp);
        ENCODE_JSONDATA_ARRAY_MEM(DetectionResult);
    }

    template <typename A,typename B,typename C>
    void draw(CameraInputData data,
            A draw_line,
            B draw_circle,C draw_text)
    {
        int sz=DetectionResult.size();
        for(int i=0;i<sz;i++){
           DetectRegionOutputData &dt= DetectionResult[i];
           dt.draw(draw_line,draw_circle,draw_text,data.DetectRegion[i]);
        }
    }
};
class UrlJsonDataRequest:public JsonData
{
public:
    string Url;
    UrlJsonDataRequest(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    UrlJsonDataRequest(string url)
    {
        Url=url;
        encode();
    }
    void decode()
    {
        DECODE_STRING_MEM(Url);
    }
    void encode()
    {
        ENCODE_STRING_MEM(Url);
    }
};

#endif // CAMERA_DATA_H

#ifndef CAMERA_DATA_H
#define CAMERA_DATA_H
#include "tool.h"
#include "jsonpacket.h"
#include "detectregion_data.h"
class CameraInputData:public JsonData
{
public:
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

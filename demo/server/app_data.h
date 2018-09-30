#ifndef APP_DATA_H
#define APP_DATA_H
#include "tool.h"
#include "jsonpacket.h"
#include "camera_data.h"
class AppInputData:public JsonObject{
public:
    enum Operation{
        GET_CONFIG=1,
        SET_CONFIG,
        INSERT_CAMERA,
        MODIFY_CAMERA,
        DELETE_CAMERA,
        OPEN_CAMERA_DATA,
        CLOSE_CAMERA_DATA,
        HEART_PKT,
        REBOOT
    };
    vector <CameraInputData> CameraData;
    string DeviceName;
    AppInputData(JsonPacket pkt):JsonObject(pkt,"DeviceConfig")
    {
        decode();
    }
    AppInputData()
    {

    }
    void modify_camera(CameraInputData data,int index)
    {
        if(index>0&&index<=CameraData.size()){
            CameraData[index-1]=data;
        }else{
            prt(info," cams unchange with index %d", CameraData.size(),index);
        }
        encode();
    }

    void insert_camera(CameraInputData data,int index)
    {
        if(index>=0&&index<=CameraData.size()){
            prt(info," cams  size  %d", CameraData.size());
            vector <CameraInputData>::iterator it=CameraData.begin();
            CameraData.insert(it+index,data);
            prt(info," cams new size  %d", CameraData.size());
        }else{
            prt(info," cams size  %d,unchange with index %d", CameraData.size(),index);
        }
        encode();
    }

    void delete_camera(int index)
    {
        if(index>0&&index<=CameraData.size()){
            vector <CameraInputData>::iterator it=CameraData.begin();
            CameraData.erase(it+index-1);
            prt(info,"del ok,cams new size  %d", CameraData.size());
        }else{
            prt(info," cams size  %d,unchange with index %d", CameraData.size(),index);
        }
        encode();
    }
    void decode()
    {
        DECODE_STRING_MEM(DeviceName);
        DECODE_JSONDATA_ARRAY_MEM(CameraData);
    }
    void encode()
    {
        ENCODE_STRING_MEM(DeviceName);
        ENCODE_JSONDATA_ARRAY_MEM(CameraData);
    }
};

class AppOutputData:public JsonData{
public:
    int CameraIndex;
    CameraOutputData CameraOutput;
    AppOutputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }

    AppOutputData(int index,CameraOutputData camout):CameraIndex(index),CameraOutput(camout)
    {
        encode();
    }

    void decode()
    {
        DECODE_INT_MEM(CameraIndex);
        DECODE_JSONDATA_MEM(CameraOutput);
    }
    void encode()
    {
        ENCODE_INT_MEM(CameraIndex);
        ENCODE_JSONDATA_MEM(CameraOutput);
    }
};

#endif // APP_DATA_H

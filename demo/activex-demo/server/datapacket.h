#ifndef PVD_H
#define PVD_H
#include <cstring>
#include <json/json.h>
#include <json/value.h>
#include "configmanager.h"
using namespace std;
using  namespace Json ;
typedef  Value JsonValue;
class DataPacket{
public:
    DataPacket(string json_data)
    {
        JsonValue v;
        Reader r;
        bool rst=r.parse(json_data,v);
        if(!rst){
            prt(info,"parse err");
        }
        val=v;
    }

    DataPacket(JsonValue v)
    {
        val = v;
    }

    DataPacket()
    {
    }

    DataPacket(vector<JsonValue>ar)
    {
        JsonValue v;
        int sz=ar.size();
        for(int i=0;i<sz;i++){
            v[i]=ar[i];
        }
        val=v;
    }

    DataPacket(vector<DataPacket> ar)
    {
        JsonValue v;
        int sz=ar.size();
        for(int i=0;i<sz;i++){
            v[i]=ar[i].val;
        }
        val=v;
    }

    void set_name_check(string name)
    {
        bool rst=val[name].isNull();
        if(rst){
            prt(info,"%s not exist,adding",name.data());
        }
    }
    void get_name_check(string name)
    {
        bool rst=val[name].isNull();
        if(rst){
            prt(info,"%s not exist,fail to get",name.data());
        }
    }

    void name_check(string name,string str)
    {
        bool rst=val[name].isNull();
        if(rst){
            prt(info,"json key:%s(%s) miss",name.data(),str.data());
        }
    }
    template <typename T>
    void set(string name,T value)
    {
        name_check(name,typeid(value).name());
        val[name]=value;
    }
    DataPacket get(string name)
    {
        name_check(name,"get");
        return DataPacket(val[name]);
    }

    int to_int(DataPacket p)
    {
        return p.val.asInt();
    }
    int to_int()
    {
        return val.asInt();
    }
    string to_string(DataPacket p)
    {
        return p.val.asString();
    }

    string to_string()
    {
        return val.asString();
    }

    DataPacket to_pkt(DataPacket p)
    {
        return p;
    }

    DataPacket to_pkt()
    {
        return DataPacket(val);
    }

    vector <DataPacket> to_array(DataPacket p)
    {
        JsonValue v=p.val;
        vector<DataPacket>  ar;
        int sz=v.size();
        for(int i=0;i<sz;i++){
            ar.push_back( v[i]);
        }
        return ar;
    }
    vector <DataPacket> to_array()
    {
        JsonValue v=val;
        vector<DataPacket>  ar;
        int sz=v.size();
        for(int i=0;i<sz;i++){
            ar.push_back( v[i]);
        }
        return ar;
    }
    void set_int(string name,int v)
    {
        val[name]=v;
    }


    void  set_bool(string name ,bool v)
    {

        val[name]=v;
    }


    void set_string(string name,string v)
    {

        val[name]=v;
    }

    void set_value(string name,JsonValue value)
    {

        val[name]=value;
    }
    void set_pkt(string name,DataPacket pkt)
    {

        val[name]=pkt.value();
    }

    JsonValue get_value(string name)
    {
        name_check(name,"get value");
        return val[name];
    }
    DataPacket get_pkt(string name)
    {

        return DataPacket(val[name]);
    }

    int get_int(string name)
    {

        return get_value(name).asInt();
    }

    bool get_bool(string name)
    {

        return get_value(name).asBool();
    }

    string get_string(string name)
    {

        return string(get_value(name).asString());
    }

    vector<JsonValue>  get_array(string name)
    {
        JsonValue v=get_value(name);
        vector<JsonValue>  ar;
        int sz=v.size();
        for(int i=0;i<sz;i++){
            ar.push_back( v[i]);
        }
        return ar;
    }
    vector<DataPacket>  get_array_packet(string name)
    {
        JsonValue v=get_value(name);
        vector<DataPacket>  ar;
        int sz=v.size();
        for(int i=0;i<sz;i++){
            ar.push_back( v[i]);
        }
        return ar;
    }
    void set_array_packet(string name,vector<DataPacket> ar)
    {
        set_value(name,DataPacket(ar).value());
    }
    vector <JsonValue>  array_value()
    {
        int sz=val.size();
        vector <JsonValue> vec;
        for(int i=0;i<sz;i++)
        {
            vec.push_back(val[i]);
        }
        return vec;
    }
    vector <DataPacket>  array_packet()
    {
        int sz=val.size();
        vector <DataPacket> vec;
        for(int i=0;i<sz;i++)
        {
            vec.push_back(val[i]);
        }
        return vec;
    }
    string data()
    {
        FastWriter  w;
        return  w.write(val);
    }

    //    JsonValue value()
    //    {
    //        return val;
    //    }
private:
    JsonValue value()
    {
        return val;
    }

    JsonValue val;
};

#if 0
class Pvd{
private:
    Pvd()
    {
        server_port=12345;
        client_data_port=12346;
    }
    Pvd(string data)
    {
        DataPacket pkt(data);
        server_port=pkt.get_int("server_port");
        client_data_port=pkt.get_int("client_data_port");
        alg_hog_file1=pkt.get_string("alg_hog_file1");
        alg_c4_file1=pkt.get_string("alg_c4_file1");
        alg_c4_file2=pkt.get_string("alg_c4_file2");
        config_file=pkt.get_string("config_file");
        kalman_lost_frame_threhold=pkt.get_int("kalman_lost_frame_threhold");
        kalman_trace_len=pkt.get_int("kalman_trace_len");
    }

public:
    int server_port;
    int client_data_port;
    string alg_hog_file1;
    string alg_c4_file1;
    string alg_c4_file2;
    string config_file;
    int kalman_lost_frame_threhold;
    int kalman_trace_len;
    static Pvd& get_instance()//ensure only 1 copy of Pvd exist in memory.
    {//TODO: lock 4 multi-thread ?
#if 1
        static FileDatabase server_setting("res/server.json");
        static Pvd pvd(server_setting.load());
#else
        static Pvd pvd;
#endif
        return pvd;
    }
    enum LENGTH_FIXED_VALUE{
        STR_LENGTH=100,
        PATH_LENGTH=1000,
        BUFFER_LENGTH=1000,
        BUFFER_MAX_LENGTH=3000
    };
    enum CMD{
        GET_CONFIG=1,
        SET_CONFIG,
        INSERT_CAMERA,
        DELETE_CAMERA,
        MOD_CAMERA_ATTR,
        MOD_CAMERA_ALG,
        CAM_OUTPUT_OPEN,
        CAM_OUTPUT_CLOSE,
        HEART,
        REBOOT,
        MOD_DEVICE_ATTR
    };
    enum RETURN{
        OK=0,
        PARSE_ERROR,
        NEED_UPDATE,
        INVALID_VALUE
    };
    enum PORTS{
        //        SERVER_PORT=12345,
        //        SERVER_DATA_OUTPUT_PORT=12346,
        CLIENT_REPORTER_PORT=12347,
        SERVER_REPORTER_PORT=12348
    };
};

#endif
#if 0
examples:
1.config file: a demo example of config.json
{
                   "cameras": [
{
               "channel": [
{
               "selected_alg":"pvd_c4",
               "pvd_c4": {
               "channel_id":1,
               "step":2,
               "ratio":"0.8",
               "detect_area": [
{
               "x": 119,
               "y": 332
               },
{
               "x": 324,
               "y": 312
               },
{
               "x": 628,
               "y": 462
               },
{
               "x": 225,
               "y": 474
               }
               ]
               }
               }
  ],
    "camera_id": 2,
    "camera_ip": "192.168.1.97",
    "camera_port": 5000,
    "direction": 3,
    "password": "admin",
    "url": "rtsp://192.168.1.97:554/av0_1",
    "url1": "rtsp://192.168.1.216:8554/test1",
    "user_name": "admin"
    }
    ],
    "deviceID": 0,
    "device_name": "110",
    "ntp_ip": "192.168.1.3",
    "ntp_port": 1111,
    "signal_machine_ip": "192.168.1.2",
    "signal_machine_port": 9999
    }
    2.server.json: read when startup
{
                 "server_port":12345,
                 "client_data_port":12346,
                 "alg_hog_file1":"res/hogcascade_pedestrians.xml",
                 "alg_c4_file1":"res/combined.txt.model",
                 "alg_c4_file2":"res/combined2.txt.model",
                 "config_file":"res/config.json",
                 "kalman_lost_frame_threhold":5,
                 "kalman_trace_len":100
                 }
  3.protocal: cmd sent from client


  #endif
  #endif // PD_H

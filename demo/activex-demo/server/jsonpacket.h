#ifndef JSONPACKET_H
#define JSONPACKET_H

#include <cstring>
#include <json/json.h>
#include <json/value.h>
#include <tool.h>
using namespace std;
using  namespace Json ;

class JsonPacket:public Value
{

public:
    typedef  Value JsonValue1;
    typedef  Value JsonValue;
    JsonPacket(string str)
    {
        JsonValue v;
        Reader r;
        bool rst=r.parse(str,v);
        if(!rst){
            prt(info,"parse err");
        }else
            val=v;
    }
    void set(string str)
    {
        JsonValue v;
        Reader r;
        bool rst=r.parse(str,v);
        if(!rst){
            prt(info,"parse err");
        }else
            val=v;
    }
    void set(vector<JsonPacket> ar)
    {
        JsonValue v;
        int sz=ar.size();
        for(int i=0;i<sz;i++){
            v[i]=ar[i].val;
        }
        val=v;
    }
    JsonPacket()
    {
    }
    void clear()
    {
        val.clear();
    }

    JsonPacket(vector<JsonPacket> ar)
    {
        JsonValue v;
        int sz=ar.size();
        for(int i=0;i<sz;i++){
            v[i]=ar[i].val;
        }
        val=v;
    }
public:
    JsonPacket(JsonValue v)
    {
        val=v;
    }
    //    JsonPacket(const JsonPacket &v)
    //    {
    //        val=v.value();
    //    }
public:
    JsonValue obj()
    {
        return val;
    }

    string str()
    {
#if 1
        //     FastWriter  w;
        FastWriter  w;
        //   StyledWriter  w;
        return  w.write(val);
#else
        return val.asString();
#endif
    }

    template <typename T>
    void set(string name,T value)
    {
        if(!name_exist(name,"set")){
            prt(info,"setting key: %s fail",name.data());
            //print_backstrace();
        }
        else
            val[name]=value;
#if 0
        check_type(val[name]);
#endif
    }
    void set(string name,JsonPacket p)
    {
        if(!name_exist(name,"set")){
            prt(info,"setting name: %s fail",name.data());
            //print_backstrace();
        }
        else
            val[name]=p.val;
    }

    void set(string name,vector<JsonPacket> pkts)
    {
        val[name].clear();
        if(!name_exist(name,"set")){
            prt(info,"setting name: %s fail",name.data());
            //print_backstrace();

        }
        else
        {
            for(JsonPacket p:pkts)
                val[name].append(p.value());
        }
    }
    void add(string name,vector<JsonPacket> pkts)
    {
        val[name].clear();
        if(!name_exist(name,"add")){

        }
        else
        {
            //   prt(info,"adding exist name: %s ",name.data());
            //print_backstrace();
        }
        for(JsonPacket p:pkts)
            val[name].append(p.value());
    }
    void add(string name,JsonPacket pkt)
    {
        val[name].clear();
        if(!name_exist(name,"add")){

        }
        else
        {
            //   prt(info,"adding exist name: %s ",name.data());
            //print_backstrace();
        }
        val[name]=pkt.obj();

    }
    template <typename T>
    void add(string name,T value)
    {
        if(!name_exist(name,"add")){
            val[name].clear();
        }
        else{
            //   prt(info,"adding exist key: %s ,already exist",name.data());
            //print_backstrace();
        }
        val[name]=value;


#if 0
        check_type(val[name]);
#endif
    }
    template <typename T>
    void add(string name,vector<T> va)
    {
        if(!name_exist(name,"add")){
            val[name].clear();
        }
        else{
            //   prt(info,"adding exist key: %s ,already exist",name.data());
            //print_backstrace();
        }
        for(T t:va)
            val[name].append(t);


#if 0
        check_type(val[name]);
#endif
    }
    //    void add(string name,JsonPacket p)
    //    {
    //        if(!name_exist(name,"add"))
    //            val[name]=p.val;
    //        else{
    //            prt(info,"adding key: %s fail,already exist",name.data());
    //        }
    //    }

    JsonPacket get(string name)
    {
        if(name_exist(name,"get"))
            return JsonPacket(val[name]);
        else
            return JsonPacket();
    }
    int to_int()
    {
        if(val.empty()){
            //assert(false);
            //int t=1/0;//sig fault deal to error.

          //  prt(info,"to int error :no content,%d",t);
            print_backstrace();
            return 0;
        }
        if(!val.isInt()){
            prt(info,"get error type:not Int");
            return 0;
        }
        return val.asInt();
    }
    vector <int> to_int_array()
    {
        if(val.empty()){
            prt(info,"to int error :no content");
            print_backstrace();
            return  vector <int>();
        }
        int sz=val.size();
        vector <int> rt;
        for(int i=0;i<sz;i++){
            rt.push_back(val[i].asInt());
        }
        return rt;
    }
    double to_double()
    {
        if(val.empty()){
            prt(info,"to  double :no content");
            return 0;
        }
        if(!val.isDouble()){
            prt(info,"get error type:not Double");
            return 0;
        }
        return val.asDouble();
    }
    vector <double>to_double_array()
    {
        if(val.empty()){
            prt(info,"to  double :no content");
            return  vector <double>();
        }
        int sz=val.size();
        vector <double> rt;
        for(int i=0;i<sz;i++){
            rt.push_back(val[i].asDouble());
        }
        return rt;
    }
    bool to_bool()
    {
        if(val.empty()){
            prt(info,"to  bool :no content");
            return false;
        }
        if(!val.isBool()){
            prt(info,"get error type:not Bool");
            return false;
        }
        return val.asBool();
    }
    vector<  bool >to_bool_array()
    {
        if(val.empty()){
            prt(info,"to  bool :no content");
            return  vector<  bool >();
        }
        vector <bool> rt;
      int  sz=val.size();
        for(int i=0;i<sz;i++){
            rt.push_back(val[i].asBool());
        }
        return rt;
    }
    string to_string()
    {
        if(val.empty()){
            prt(info,"to  string :no content");
            return string();
        }
        if(!val.isString()){
            prt(info,"get error type:not String");
            return string();
        }
        return val.asString();
    }
   vector<  string> to_string_array()
    {
        if(val.empty()){
            prt(info,"to  string :no content");
            return  vector<  string> ();
        }
        int sz=val.size();
        vector <string> rt;
        for(int i=0;i<sz;i++){
            rt.push_back(val[i].asString());
        }
        return rt;
    }
    void show_value_type(JsonValue val)
    {
        prt(info,"show value");
        if(val.isArray()){  prt(info,"type:array");}
        if(val.isString()){  prt(info,"type:str");}
        if(val.empty()){  prt(info,"type:empty");}
        if(val.isBool()){  prt(info,"type:bool");}
        if(val.isInt()){  prt(info,"type:int");}
        if(val.isObject()){  prt(info,"type:obj");}
        prt(info,"show value done");

    }
    vector<JsonPacket> to_array()
    {
        vector<JsonPacket>  ar;
        if(val.empty()){
            //prt(info,"to array :empty");

            return ar;
        }
        if(!val.isArray()){
            prt(info,"get error type:not array");
            show_value_type(val);
            return ar;
        }
        JsonValue v=val;

        int sz=v.size();
        for(int i=0;i<sz;i++){
            ar.push_back( v[i]);
        }
        return ar;
    }

    JsonValue &value()
    {
        return val;
    }
    bool is_null()
    {
        return val.isNull();
    }
private:
    bool name_exist(string name,string str)
    {
        bool rst=true;
        JsonValue v=val;
        rst=v[name].isNull();
        if(rst&&(str=="get"||str=="set")){
            //prt(info," (%s) not exist or no data",name.data());
            //print_backstrace();
            return false;
        }
        return true;
    }
    void check_type(JsonValue v)
    {
        if(v.isArray())   prt(info,"isArray");
        if(v.isBool())   prt(info,"isBool");
        if(v.isDouble())   prt(info,"isDouble");
        if(v.isInt())   prt(info,"isInt");
        if(v.isInt64())   prt(info,"isInt64");
        if(v.isIntegral())   prt(info,"isIntegral");
        if(v.isNull())   prt(info,"isNull");
        if(v.isNumeric())   prt(info,"isNumeric");
        if(v.isObject())   prt(info,"isObject");
        if(v.isString())   prt(info,"isString");
        if(v.isUInt())   prt(info,"isUInt");
        if(v.isUInt64())   prt(info,"isUInt64");
    }
private:
    JsonValue val;

};


class JsonData{
protected:
    JsonPacket config;
public:
    JsonData(JsonPacket pkt)
    {
        config=pkt;
        // prt(info,"json string: %s",config.str().data());
    }
    JsonData()
    {

    }
    virtual void encode()=0;
    virtual void decode()=0;
    JsonPacket data()
    {
        return config;
    }
};
class JsonObject{
private:
    string title;
protected:
    JsonPacket config;
public:
    JsonObject(JsonPacket pkt,string tt):title(tt)
    {
        config=pkt.get(title);

    }
    JsonObject()
    {

    }
    JsonObject(string tt):title(tt)
    {

    }
    virtual void encode()=0;
    virtual void decode()=0;
    JsonPacket data()
    {
        JsonPacket p;
        p.add(title,config);
        return p;
    }
};
template<typename TP>
class VdData{
protected:
    TP private_data;
public:
    VdData(TP d):private_data(d)
    {
    }
    virtual ~VdData()
    {

    }

    TP get_data()
    {
        return private_data;
    }
};

#define DECODE_INT_MEM(mem) {this->mem=config.get(#mem).to_int();}
#define DECODE_STRING_MEM(mem) {this->mem=config.get(#mem).to_string();}
#define DECODE_DOUBLE_MEM(mem) {this->mem=config.get(#mem).to_double();}
#define DECODE_BOOL_MEM(mem) {this->mem=config.get(#mem).to_bool();}

#define DECODE_INT_ARRAY_MEM(mem) {this->mem=config.get(#mem).to_int_array();}
#define DECODE_STRING_ARRAY_MEM(mem) {this->mem=config.get(#mem).to_string_array();}
#define DECODE_DOUBLE_ARRAY_MEM(mem) {this->mem=config.get(#mem).to_double_array();}
#define DECODE_BOOL_ARRAY_MEM(mem) {this->mem=config.get(#mem).to_bool_array();}


#define DECODE_PKT_MEM(mem) {this->mem=config.get(#mem);}
#define DECODE_PKT_ARRAY_MEM(mem)  {auto tmp=config.get(#mem).to_array();this->mem.assign(tmp.begin(),tmp.end());}
#define DECODE_JSONDATA_MEM(mem) {this->mem=config.get(#mem);}
#define DECODE_JSONDATA_ARRAY_MEM(mem)  {auto tmp=config.get(#mem).to_array();this->mem.assign(tmp.begin(),tmp.end());}

#define ENCODE_MEM(mem) {config.add(#mem,this->mem);}

#define ENCODE_INT_MEM(mem) {ENCODE_MEM(mem);}
#define ENCODE_STRING_MEM(mem) {ENCODE_MEM(mem);}
#define ENCODE_DOUBLE_MEM(mem) {ENCODE_MEM(mem);}
#define ENCODE_BOOL_MEM(mem) {ENCODE_MEM(mem);}


#define ENCODE_INT_ARRAY_MEM(mem) {ENCODE_MEM(mem);}
#define ENCODE_STRING_ARRAY_MEM(mem) {ENCODE_MEM(mem);}
#define ENCODE_DOUBLE_ARRAY_MEM(mem) {ENCODE_MEM(mem);}
#define ENCODE_BOOL_ARRAY_MEM(mem) {ENCODE_MEM(mem);}


#define ENCODE_PKT_MEM(mem) {config.add(#mem,this->mem);}
#define ENCODE_PKT_ARRAY_MEM(mem) {config.add(#mem,mem);}
#define ENCODE_JSONDATA_MEM(mem) {config.add(#mem,this->mem.data());}
#define ENCODE_JSONDATA_ARRAY_MEM(mem) { vector<JsonPacket> pkts;\
    for(auto tmp1:this->mem){pkts.push_back(tmp1.data());}\
    config.add(#mem,pkts);}


class RequestPkt:public JsonData
{
public:
    int Operation;
    int Index;
    JsonPacket Argument;

    RequestPkt(int op,int index , JsonPacket data,JsonPacket rt=JsonPacket()):Operation(op),Index(index),Argument(data)
    {
        encode();
    }
    RequestPkt(JsonPacket p):JsonData(p)
    {
        decode();
    }
    void encode()
    {
        ENCODE_INT_MEM(Index);
        ENCODE_INT_MEM(Operation);
        ENCODE_PKT_MEM(Argument);
    }
    void decode()
    {
        DECODE_INT_MEM(Operation);
        DECODE_INT_MEM(Index);
        DECODE_PKT_MEM(Argument);
    }

};
class ReplyPkt:public JsonData
{
public:
    bool Ret;
    int Operation;
    JsonPacket Data;
    ReplyPkt(bool p,int o,JsonPacket pkt):Ret(p),Operation(o),Data(pkt)
    {
        encode();
    }
    ReplyPkt()
    {

    }
    ReplyPkt(string ret):JsonData(ret)
    {
        decode();
    }
    void encode()
    {
        ENCODE_BOOL_MEM(Ret);
        ENCODE_INT_MEM(Operation);
        ENCODE_PKT_MEM(Data);
    }
    void decode()
    {
        DECODE_BOOL_MEM(Ret);
        DECODE_INT_MEM(Operation);
        DECODE_PKT_MEM(Data);
    }

};

class VdRect:public JsonData
{
public:
    int x;
    int y;
    int w;
    int h;
    VdRect(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    VdRect(int x1,int y1,int w1,int h1):x(x1),y(y1),w(w1),h(h1)
    {
        encode();
    }
    VdRect(){}
    void decode()
    {
        DECODE_INT_MEM(x);
        DECODE_INT_MEM(y);
        DECODE_INT_MEM(w);
        DECODE_INT_MEM(h);

    }
    void encode()
    {
        ENCODE_INT_MEM(x);
        ENCODE_INT_MEM(y);
        ENCODE_INT_MEM(w);
        ENCODE_INT_MEM(h);
    }

};
class ObjectRect:public JsonData
{
public:
    int x;
    int y;
    int w;
    int h;
    string label;
    int confidence_rate;
    ObjectRect(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    ObjectRect(int x1,int y1,int w1,int h1,string l,int c):x(x1),y(y1),w(w1),h(h1),label(l),confidence_rate(c)
    {
        encode();
    }
    ObjectRect(){}
    void decode()
    {
        DECODE_INT_MEM(x);
        DECODE_INT_MEM(y);
        DECODE_INT_MEM(w);
        DECODE_INT_MEM(h);
        DECODE_STRING_MEM(label);
        DECODE_INT_MEM(confidence_rate);
    }
    void encode()
    {
        ENCODE_INT_MEM(x);
        ENCODE_INT_MEM(y);
        ENCODE_INT_MEM(w);
        ENCODE_INT_MEM(h);
        ENCODE_STRING_MEM(label);
        ENCODE_INT_MEM(confidence_rate);
    }

};
class VdPoint:public JsonData
{
public:
    int x;
    int y;
    VdPoint(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    VdPoint(int x,int y):x(x),y(y)
    {
        encode();
    }
    VdPoint()
    {

    }
    void decode()
    {
        DECODE_INT_MEM(x);
        DECODE_INT_MEM(y);
    }
    void encode()
    {
        ENCODE_INT_MEM(x);
        ENCODE_INT_MEM(y);
    }
};

template <typename T>
vector<JsonPacket> obj_2_pkt_array(T jd)
{
    vector<JsonPacket> v;
    for(JsonData d:jd){
        v.push_back(d.data());
    }
    return v;
}
template <typename T>
JsonPacket obj_2_pkt(T jd)
{

    return jd.data();
}
#endif // JSONPACKET_H

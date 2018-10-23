#ifndef PVDOBJECT_H
#define PVDOBJECT_H

#include "tool.h"
#include "datapacket.h"

//#define GET_INT_VALUE_FROM_PKT(mem,container) {container.get_int(#mem)}

//#define GET_STRING_VALUE_FROM_PKT(mem,container) (container.get_string(#mem))
//#define GET_ARRAY_VALUE_FROM_PKT(mem,container) (container.get_array_packet(#mem))
//#define GET_VALUE_FROM_PKT(mem,container) (container.get_pkt(#mem))


//#define SET_STRING_VALUE_FROM_PKT(mem,container,v) (container.set_string(#mem,v))
//#define SET_ARRAY_VALUE_FROM_PKT(mem,container,v) (container.set_array_packet(#mem,v))
//#define SET_VALUE_FROM_PKT(mem,container,v) (container.set_pkt(#mem,v))



//#define GET_INT_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_int(#mem);}
//#define GET_STRING_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_string(#mem);}
//#define GET_ARRAY_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_array_packet(#mem);}
//#define GET_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_pkt(#mem);}

//#define SET_INT_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_int(#mem,container->mem);}
//#define SET_STRING_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_string(#mem,container->mem);}
//#define SET_ARRAY_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_array_packet(#mem,container->mem);}
//#define SET_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_pkt(#mem,container->mem);}


//#define GET_INT(mem) {this->mem=config.get_int(#mem);}
//#define GET_STRING(mem) {this->mem=config.get_string(#mem);}
//#define GET_ARRAY(mem) {this->mem=config.get_array_packet(#mem);}
//#define GET_VALUE(mem) {this->mem=config.get_pkt(#mem);}

//#define GET_INT1(mem) {config.get_int(#mem)}
//#define GET_STRING1(mem) {config.get_string(#mem)}
//#define GET_ARRAY1(mem) (config.get_array_packet(#mem))
//#define ASSIGN_ARRAY(mem) mem.assign((GET_ARRAY1(mem)).begin(),(GET_ARRAY1(mem)).end());
//#define ASSIGN_ARRAY1(mem) { vector <DataPacket> p= GET_ARRAY1(mem); mem.assign(p.begin(),p.end());};
//#define GET_VALUE1(mem) (mem,config.get_pkt(#mem))

//#define SET_INT(mem) {config.set_int(#mem,this->mem);}
//#define SET_STRING(mem) {config.set_string(#mem,this->mem);}
//#define SET_ARRAY(mem) {config.set_array_packet(#mem,this->mem);}
//#define SET_VALUE(mem) {config.set_pkt(#mem,this->mem);}


//#define DECODE_PKT_MEM(mem)  {this->mem=config.get_pkt(#mem);}

//#define DECODE_INT_MEM(mem) {this->mem=config.get_int(#mem);}
//#define DECODE_STRING_MEM(mem) {this->mem=config.get_string(#mem);}
#define DECODE_INT_MEM(mem) {this->mem=config.get(#mem).to_int();}
#define DECODE_STRING_MEM(mem) {this->mem=config.get(#mem).to_string();}
#define DECODE_PKT_MEM(mem) {this->mem=config.get(#mem).to_pkt();}
#define DECODE_OBJ_ARRAY_MEM(mem)  {auto tmp=config.get(#mem).to_array();this->mem.assign(tmp.begin(),tmp.end());}


//#define DECODE_OBJ_ARRAY_MEM(mem)  {auto tmp=config.get_array_packet(#mem);prt(info,"%d@@",tmp.size());this->mem.assign(tmp.begin(),tmp.end());}


template<typename TP>
class VdData{
protected:
    TP private_data;
public:
    VdData(TP d):private_data(d)
    {
        //private_data.decode();
    }
};

class JsonData{
protected:
    DataPacket config;
public:
    JsonData(DataPacket pkt)
    {
        config=pkt;
    }
    JsonData()
    {

    }

    // virtual void encode()=0;
    DataPacket data()
    {
        // encode();
        return config;
    }

};
//class JsonDataArray{
//protected:
//    vector <DataPacket> config;
//public:
//    JsonDataArray(vector <DataPacket> pkt)
//    {
//        config=pkt;
//    }
//    virtual void decode()=0;
//    virtual void encode()=0;
//    vector <DataPacket>  data()
//    {
//        return config;
//    }

//};
#endif // PVDOBJECT_H

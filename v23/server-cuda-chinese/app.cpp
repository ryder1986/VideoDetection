#include "app.h"
#include "socket.h"
App::App():watch_dog(bind(&App::check_point,this))
{

}
App::App(ConfigManager *p_config_manager):str_stream(""),watch_dog(bind(&App::check_point,this)),
    VdData(DeviceConfigData(p_config_manager->get_config()).DeviceConfig.data()),lservice(),p_cm(p_config_manager),udp_fd(0)
{
    stream_cmd=NULL;
    restart_all();
    sql_need_connect=true;
    p_count_thread=new thread(bind(&App::count_fun1,this));
    static Tcpserver server_cmd(stream_cmd,
                                12345,
                                bind(&App::process_client_cmd,
                                     this,placeholders::_1,
                                     placeholders::_2,
                                     placeholders::_3
                                     )
                                );
    quit_count=false;
    watch_dog.start(1000*60);//do check every 1 minute
}

App::~App()
{
    watch_dog.stop();
    if(udp_fd>0)
        close(udp_fd);

}
//deal with clients command
void App::process_client_cmd(Session *clt, char *data, int len)
{
    str_stream.append(string(data,len));
    string valid_buf;
    valid_buf.clear();
    string ret_str;
    bool process_ret;
    while(JsonStr::get_valid_buf(str_stream,valid_buf)) {//Get valid json object, TODO:we only check {} matches, we should check json grammar
        prt(info,"recive object--> %s(%d bytes left)",valid_buf.data(),str_stream.size());
        RequestPkt event(valid_buf);
        ReplyPkt ret_pkt;
        client_tmp_ip=clt->ip();
        process_ret=process_event(event,ret_pkt);
        ret_str=ret_pkt.data().str();
        clt->send(ret_str.data(),ret_str.length());
    }
}

void App::process_camera_data(Camera *camera, CameraOutputData data)
{
    vector<Camera *>::iterator itr;
    int idx=0;
    itr=cms.begin();
    for( idx=0;idx<cms.size();idx++){
        if(*(idx+itr)==camera)
            break;
    }
    if(idx<cms.size()){
        //prt(info,"process camera %d",idx+1);
    }else{
        prt(info,"process invalid camera index %d, sz %d",idx,cms.size());
        return;
    }
    insert_database(data,idx+1,cms[idx]->screenshot);
    if(udp_fd<=0)
        udp_fd=Socket::UdpCreateSocket(5000);
    AppOutputData rst(idx+1,data);
    if(stream_cmd)
        for(Session *ss:*stream_cmd)
        {
            //prt(info,"send  %d bytes --> %s",rst.data().str().size(),rst.data().str().data());
            Socket::UdpSendData(udp_fd,ss->ip().data(),12349,rst.data().str().data(),rst.data().str().length());
        }
    for(DestClient dst:dest_clients){
        Socket::UdpSendData(udp_fd,dst.get_ip().data(),12349,rst.data().str().data(),rst.data().str().length());
    }

}

bool App::process_event(RequestPkt e, ReplyPkt &r)
{
    bool ret=false;
    prt(info,"handle cmd type %d",e.Operation);
    switch(e.Operation){
    case AppInputData::Operation::GET_CONFIG:
    {
        JsonPacket cfg=p_cm->get_config();//get config
        ReplyPkt rp(true,AppInputData::Operation::GET_CONFIG,cfg);
        r=rp;
        ret=true;
        break;
    }
    case AppInputData::Operation::SET_CONFIG:
    {
        p_cm->set_config(e.Argument.str());//get config
        prt(info,"set config with string:\n %s",e.Argument.str().data());
        AppInputData dt(DeviceConfigData(p_cm->get_config()).DeviceConfig.data());
        private_data=AppInputData(dt);
        restart_all();
        ret=true;
        ReplyPkt p(ret,AppInputData::Operation::SET_CONFIG,JsonPacket());
        r=p;
        break;
    }
    case AppInputData::Operation::INSERT_CAMERA:
    {
        if( add_camera(e.Index,e.Argument)){
            save_data();
            ret=true;
        }
        ReplyPkt p(ret,AppInputData::Operation::MODIFY_CAMERA,JsonPacket());
        r=p;
        break;
    }
    case AppInputData::Operation::MODIFY_CAMERA:
    {
        if(mod_camera(e.Index,e.Argument)){
            save_data();
            ret=true;
        }
        ReplyPkt p(ret,AppInputData::Operation::MODIFY_CAMERA,JsonPacket());
        r=p;
        break;
    }
    case AppInputData::Operation::DELETE_CAMERA:
    {
        ret=false;

#if 0
        Timer2 async_task;

        async_task.AsyncWait(0,bind(&App::del_camera,this,placeholders::_1),e.Index);
        save_data();
        ret=true;
#else
        if(del_camera(e.Index)){
            save_data();
            ret=true;
        }
#endif
        ReplyPkt p(ret,AppInputData::Operation::DELETE_CAMERA,JsonPacket());
        r=p;
        break;
    }
    case AppInputData::Operation::OPEN_CAMERA_DATA:
    {
        bool exist=false;
        for(DestClient dst:dest_clients ){
            if(dst.get_ip()==client_tmp_ip)
                exist=true;
        }
        if(!exist)
            dest_clients.push_back(DestClient(client_tmp_ip));
        ret=true;
        ReplyPkt p(ret,AppInputData::Operation::OPEN_CAMERA_DATA,JsonPacket());
        r=p;
        break;
    }
    case AppInputData::Operation::CLOSE_CAMERA_DATA:
    {
        bool exist=false;
        vector<DestClient>::iterator  bg= dest_clients.begin();
        for(int i=0;i<dest_clients.size();i++ ){
            DestClient dst=dest_clients[i];
            if(dst.get_ip()==client_tmp_ip)
                dest_clients.erase(bg+i);
        }
        ret=true;
        ReplyPkt p(ret,AppInputData::Operation::CLOSE_CAMERA_DATA,JsonPacket());
        r=p;
        break;
    }
    case AppInputData::Operation::HEART_PKT:
    {
        ret=true;
        break;
    }
    case AppInputData::Operation::REBOOT:
    {
        ret=true;
        break;
    }
    default:
        prt(info,"unknow cmd %d",e.Operation);
        break;
    }
    return ret;
}

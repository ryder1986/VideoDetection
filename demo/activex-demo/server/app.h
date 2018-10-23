#ifndef APP_H
#define APP_H

#include "server.h"
#include "tool.h"
#include "camera.h"
#include "configmanager.h"
#include "app_data.h"
#include <mysql/mysql.h>
#include <list>
#define MAX_CAM_NUM 100
class DestClient{
public:

    DestClient(string i):ip(i)
    {

    }

    string get_ip()
    {
        return ip;
    }

private:
    string ip;
};
class DatabaseInstance
{
public:
    static DatabaseInstance &get_instance()
    {
        static DatabaseInstance ins;
        return ins;
    }
    bool connect()
    {
        if(!mysql_real_connect(sql,host.data(),user.data(),passwd.data(),NULL,3306,0,0)){
            sql=NULL;
            const char *err= mysql_error(sql);
            prt(info,"connect database error: %s" ,err);
            mysql_close(sql);
            return false;
        }else{
            prt(info,"connect database %s ok",db.data());
            int ret=  mysql_select_db(sql,db.data());
            prt(info,"ok use %d",ret);
            return true;
        }
    }

    bool connect(string host,string user,string passwd,string db)
    {
        if(!mysql_real_connect(sql,host.data(),user.data(),passwd.data(),NULL,3306,0,0)){
            sql=NULL;
            const char *err= mysql_error(sql);
            prt(info,"connect database error: %s" ,err);
            mysql_close(sql);
            return false;
        }else{
            prt(info,"connect database %s ok",db.data());
            int ret=  mysql_select_db(sql,db.data());
            prt(info,"ok use %d",ret);
            return true;
        }
    }
    void print_result()
    {
    }
    void print_table()
    {
        if(!query_flag){
            prt(info,"eror:can not print table  because query failed");
            return ;
        }
#if 1
        //   MYSQL_FIELD *field;
        if(!resid)
            return;
        while((field = mysql_fetch_field(resid)))
        {
            printf(" %s ", field->name);
        }
#endif

        cout<<endl;
        // query("select * from  table1");
        //  resid = mysql_store_result(sql);
        MYSQL_ROW row;
        unsigned int num_fields;
        unsigned int i;

        num_fields = mysql_num_fields(resid);
        // return ;
        while ((row = mysql_fetch_row(resid)))
        {
            unsigned long *lengths;
            lengths = mysql_fetch_lengths(resid);
            for(i = 0; i < num_fields; i++)
            {
                //  field = mysql_fetch_field(resid);
                //  printf("field   %p\n", field);
                // printf("field name %s\n", field->name);
                printf("[%.*s] ", (int) lengths[i],
                       row[i] ? row[i] : "NULL");
            }
            printf("\n");
        }
    }
    void query(char *statement)
    {

        //mysql_free_result(resid );
        int mysqlret = mysql_real_query(sql,statement,(unsigned int)strlen(statement));
        if(mysqlret!=0)
        {
            query_flag=false;
            //       abort();
            //  assert(false);
            const char *err_str=  mysql_error(sql);
            prt(info,"error: %s" , err_str);
        }else{
            query_flag=true;
            resid = mysql_store_result(sql);
        }
    }
    ~DatabaseInstance()
    {
        mysql_close(sql);
    }
private:
    DatabaseInstance(char *host,char *user,char*passwd,char *db):
        host(host),user(user),passwd(passwd),db(db),sql(NULL),query_flag(false),need_connect(true)
    {
        sql=mysql_init(NULL);
    }

    DatabaseInstance()
    {
        sql=mysql_init(NULL);
    }
    string host;
    string user;
    string passwd;
    string db;
    MYSQL *sql;
    MYSQL_RES *resid;
    MYSQL_FIELD *field;
    bool query_flag;
    bool need_connect;
    // MYSQL_ROW row_str_list;
};
#include <strstream>
class App:public VdData<AppInputData>
{
public:
    //define operation for request pkt
    //    enum Operation{
    //        GET_CONFIG=1,
    //        SET_CONFIG,
    //        INSERT_CAMERA,
    //        MODIFY_CAMERA,
    //        DELETE_CAMERA,
    //        OPEN_CAMERA_DATA,
    //        CLOSE_CAMERA_DATA,
    //        HEART_PKT,
    //        REBOOT
    //    };
    App(ConfigManager *p);
    ~App();

    void start()
    {
    }
private:
    //#define COUNT_SECONDS 10
#define COUNT_SECONDS 300
    void count_fun()
    {
        int camera_size=private_data.CameraData.size();
        int count_time=COUNT_SECONDS;//10s
        int person_count[100];
        int car_count[100];
        int speed_count[100];


        int old_exist[100];//max lane num
        memset(old_exist,0,100);
        while(  (get_time_second()%6)){
            this_thread::sleep_for(chrono::microseconds(1000000));
        }
        while(!quit_count){


            if(count_time--==1){

                prt(info,"time up , start cal ---------------------------->");

                for(int loop_cams=0;loop_cams<camera_size;loop_cams++){
                       prt(info,"cam %d",loop_cams);
                    CameraInputData cd= private_data.CameraData[loop_cams];
                    int region_size=cd.DetectRegion.size();
                    vector< MvdProcessorOutputData>  &cam_out=outputs[loop_cams];
                    for(int loop_regions=0;loop_regions<region_size;loop_regions++){
                          prt(info,"region %d",loop_regions);
                        DetectRegionInputData dr=  cd.DetectRegion[loop_regions];
                        MvdProcessorInputData mvddata(  dr.ProcessorData);
                        int lane_size=mvddata.LaneData.size();
                        if(lane_size>=100){
                            prt(info,"max lane  100 ,err");
                            continue;
                        }

                        DatabaseInstance &ins=get_database();

                        for(int i=0;i<cam_out.size();i++){
                                  prt(info,"frame %d",loop_regions);
                            MvdProcessorOutputData tmp=cam_out[i];
                            // tmp.CongestionData
                            for(int j=0;j<tmp.LaneOutputData.size();j++){
                                if(old_exist[j]==0&&tmp.LaneOutputData[j].NearCarExist)
                                {
                                    car_count[j]++;
                                    speed_count[j]+=tmp.LaneOutputData[j].VehicleSpeed;

                                }
                                old_exist[j]=tmp.LaneOutputData[j].NearCarExist;

                            }


                            //      person_count+=tmp.PersonFlow1;
                        }
                        for(int j=0;j<mvddata.LaneData.size();j++){

                            int vs_record_id=3;
                            string vs_sst=get_sql_time();
                            strstream tmp1;


                            tmp1<<COUNT_SECONDS;

                            char buf_tmp[100];
                            memset(buf_tmp,0,100);
                            sprintf(buf_tmp,"%d",COUNT_SECONDS);
                            string vs_sp(buf_tmp);
                            string vs_analyce_id="2";
                            string vs_savenue="沙河西路";
                            string vs_CameraID="1";

                            string vs_LaneID="12";
                            //   string vs_VSum="";car_count
                            string vs_VPSum="0";
                            string vs_VNSum="0";
                            string vs_VTSum="0";

                            string vs_VBSum="0";
                            //  string vs_VCSum="0";
                            string vs_VMSum="0";
                            string vs_VKSum="0";


                            if(car_count[j]){
                                prt(info,"car num %d, speed total %d",car_count[j],speed_count[j]);




                                int average_speed=speed_count[j]/car_count[j];
                                memset(buf_tmp,0,100);
                                sprintf(buf_tmp,"%d",average_speed);
                                //  tmp2<<average_speed;
                            }else{
                                  memset(buf_tmp,0,100);
                                    sprintf(buf_tmp,"%d",0);
                            }
                            string vs_ASpeed(buf_tmp);

                            string vs_ATime="1";
                            string vs_ASpace="1";
                            string vs_AVM="1";
                            string vs_AOccupy="1";
                            string vs_RState="1";

                            //                    char buf[100];
                            //                    memset(buf,0,100);
                            //                    sprintf(buf,"%s",);

                            stringstream stream;

                            stream<< "INSERT INTO VS ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `LaneID`, `VSum`, `VPSum`, `VNSum`, `VTSum`, `VBSum`, `VCSum`, `VMSum`, `VKSum`, `ASpeed`, `ATime`, `ASpace`, `AVM`, `AOccupy`,`RState`) VALUES";
                            stream<< "(";
                            stream<< "'";

                            stream<<vs_record_id;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";



                            stream<<vs_sst;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_sp;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_analyce_id;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_savenue;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_CameraID;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";



                            stream<<vs_LaneID;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";

                            prt(info,"car : %d",car_count[j]);
                            //  stream<<vs_VSum;
                            stream<<  car_count[j];
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_VPSum;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_VNSum;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";

                            stream<<vs_VTSum;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_VBSum;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";

                            //  stream<<vs_VCSum;
                            stream<<car_count[j];
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_VMSum;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_VKSum;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_ASpeed;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";

                            stream<<vs_ATime;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_ASpace;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_AVM;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_AOccupy;
                            stream<< "'";
                            stream<< ",";
                            stream<< "'";


                            stream<<vs_RState;
                            stream<< "'";
                            stream<< ");";
                            //stream<<

                            prt(info,"%s",stream.str().data());
                            char buf1[1000];
                            memset(buf1,0,1000);
                            sprintf(buf1,"%s",stream.str().data());

                            prt(info,"inserting database cam %d, region %d, lane %d",loop_cams,loop_regions,j);
                            ins.query(buf1);


                            speed_count[j]=0;
                            car_count[j]=0;

                        }





//                        int record_id=1;//no
//                        string sst;// start time. date type
//                        //  string count_period="10";//s
//                        string device_id="123";
//                        string location_name;
//                        string camera_id;
//                        string region_id;
//                        string person_count;
//                        string normal_direction_person_count;
//                        string reverse_direction_person_count;
//                        string person_density;


                        //      ins.query(" INSERT INTO PS ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `RegionID`, `PSum`, `PPSum`, `PNSum`, `PDensity`) \
                        //         VALUES \
                        //        ('2', '2018-10-17 16:34:07', '90', '1', '沙河西路', '2', '2', '15', '10', '5', '40');");
                //        ins.query(" INSERT INTO PS ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `RegionID`, `PSum`, `PPSum`, `PNSum`, `PDensity`) \
                       //           VALUES \
                       //           ('2', '2018-10-17 16:34:07', '90', '1', '沙河西路', '2', '2', '15', '10', '5', '40');");


                                //                                int vs_record_id=3;
                                //                        string vs_sst=get_sql_time();
                                //                        strstream tmp1;


                                //                        tmp1<<COUNT_SECONDS;

                                //                        char buf_tmp[100];
                                //                        memset(buf_tmp,0,100);
                                //                        sprintf(buf_tmp,"%d",COUNT_SECONDS);
                                //                        string vs_sp(buf_tmp);
                                //                        string vs_analyce_id="2";
                                //                        string vs_savenue="沙河西路";
                                //                        string vs_CameraID="1";

                                //                        string vs_LaneID="12";
                                //                        //   string vs_VSum="";car_count
                                //                        string vs_VPSum="0";
                                //                        string vs_VNSum="0";
                                //                        string vs_VTSum="0";

                                //                        string vs_VBSum="0";
                                //                        //  string vs_VCSum="0";
                                //                        string vs_VMSum="0";
                                //                        string vs_VKSum="0";


                                //                        if(car_count){
                                //                            prt(info,"car num %d, speed total %d",car_count,speed_count);




                                //                            int average_speed=speed_count[j]/car_count[j];
                                //                            memset(buf_tmp,0,100);
                                //                            sprintf(buf_tmp,"%d",average_speed);
                                //                            //  tmp2<<average_speed;
                                //                        }
                                //                        string vs_ASpeed(buf_tmp);

                                //                        string vs_ATime="1";
                                //                        string vs_ASpace="1";
                                //                        string vs_AVM="1";
                                //                        string vs_AOccupy="1";
                                //                        string vs_RState="1";

                                //                        //                    char buf[100];
                                //                        //                    memset(buf,0,100);
                                //                        //                    sprintf(buf,"%s",);

                                //                        stringstream stream;

                                //                        stream<< "INSERT INTO VS ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `LaneID`, `VSum`, `VPSum`, `VNSum`, `VTSum`, `VBSum`, `VCSum`, `VMSum`, `VKSum`, `ASpeed`, `ATime`, `ASpace`, `AVM`, `AOccupy`,`RState`) VALUES";
                                //                        stream<< "(";
                                //                        stream<< "'";

                                //                        stream<<vs_record_id;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";



                                //                        stream<<vs_sst;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_sp;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_analyce_id;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_savenue;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_CameraID;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";



                                //                        stream<<vs_LaneID;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        //  stream<<vs_VSum;
                                //                        stream<<  car_count;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_VPSum;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_VNSum;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";

                                //                        stream<<vs_VTSum;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_VBSum;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";

                                //                        //  stream<<vs_VCSum;
                                //                        stream<<car_count;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_VMSum;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_VKSum;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_ASpeed;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";

                                //                        stream<<vs_ATime;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_ASpace;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_AVM;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_AOccupy;
                                //                        stream<< "'";
                                //                        stream<< ",";
                                //                        stream<< "'";


                                //                        stream<<vs_RState;
                                //                        stream<< "'";
                                //                        stream<< ");";
                                //                        //stream<<

                                //                        prt(info,"%s",stream.str().data());
                                //                        char buf1[1000];
                                //                        memset(buf1,0,1000);
                                //                        sprintf(buf1,"%s",stream.str().data());
                                //                        ins.query(buf1);

                                //                        count_time=COUNT_SECONDS;
                                //                        speed_count=0;
                                //                        car_count=0;

                                //    ins.query( "INSERT INTO VS ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `LaneID`, `VSum`, `VPSum`, `VNSum`, `VTSum`, `VBSum`, `VCSum`, `VMSum`, `VKSum`, `ASpeed`, `ATime`, `ASpace`, `AVM`, `AOccupy`,`RState`) VALUES\
                                //             ( '2', '2018-10-07 10:39:34', '300', '2', '沙河西路', '1', '2', '15', '10', '5', '1', '2', '3', '4', '5', '65.45', '60.45', '93.7', '48.66', '43.78','1');");

                    }//loop regions
                }//loop cams
                for(int i=0;i<100;i++)
                    outputs[i].clear();

                  count_time=COUNT_SECONDS;
                    prt(info," stop cal ---------------------------->");
            }

            this_thread::sleep_for(chrono::microseconds(1000000));
        }
    }
    bool process_event(RequestPkt e,ReplyPkt &r);
    void process_client_cmd(Session *clt,char *data,int len);
    void client_data_request(Session *clt, char *data, int len);
    void process_camera_data(Camera *clt,CameraOutputData);
    Camera* new_cam(CameraInputData data)
    {
        return new Camera(data,bind(&App::process_camera_data,
                                    this,placeholders::_1,
                                    placeholders::_2));
    }
    void restart_all()
    {
        stop_cams();
        start_cams();
    }
    void start_cams()
    {
        for(CameraInputData p:private_data.CameraData){
            cms.push_back(new_cam( p));
        }
    }
    void stop_cams()
    {
        for(Camera *c:cms){
            delete c;
        }
        cms.clear();
    }
    bool add_camera(int index,CameraInputData data)//after who ?  0~size
    {
        bool ret=false;
        if(0<=index&&index<=cms.size()){
            Camera *c=new_cam( (data));
            vector<Camera*>::iterator it=cms.begin();
            cms.insert(it+index,c);
            private_data.insert_camera(data,index);
            ret=true;
        }
        return ret;
    }
    bool del_camera(int index)//delete who ? 1~size
    {
        if(1<=index&&index<=cms.size()){
            delete cms[index-1];
            vector<Camera*>::iterator it=cms.begin();
            cms.erase(it+index-1);
            private_data.delete_camera(index);
            return true;
        }
        return false;
    }
    bool mod_camera(int index,RequestPkt pkt)//delete who ? 1~size
    {
        bool ret=false;
        if(1<=index&&index<=cms.size()){
            vector<Camera*>::iterator it=cms.begin();
            Camera *c=cms[index-1];
            if(c->modify(pkt)){
                private_data.modify_camera(c->get_data(),index);
                ret=true;
            }
        }
        return ret;
    }
    DatabaseInstance  &  get_database()
    {
        DatabaseInstance &ins=DatabaseInstance::get_instance();
        string user="root";
        string passwd="root";
        string db="AIPD";
        string host="localhost";
        string fn;
        fn.append("/ftphome/pic/");


        if(sql_need_connect){
            ins.connect(host,user,passwd,db);
            sql_need_connect=false;
        }
        return ins;
    }
    void get_tis_str(char *buf,char *time, int type)
    {

      //  "INSERT INTO TIS ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `RegionID`, `TEType`, `TEPAddr`, `TEVAddr`)\
      //   VALUES  ( '3', '2018-03-07 17:46:24', '60', '32', '大学城', '12', '12', '21', 'pic_path', 'video_path');"

        sprintf(buf," INSERT INTO TIS ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `RegionID`, `TEType`, `TEPAddr`, `TEVAddr`)\
         VALUES  ( '3', '%s', '60', '32', '大学城', '12', '12', '%d', 'pic_path', 'video_path')",time,type);
    }
    void single_insert(EventRegionObjectOutput data,Mat frame)
    {
        //if(mvddata.DriveAwayData.size()>0){
            if(1){
                  prt(info,"get incident");
            DatabaseInstance &ins=DatabaseInstance::get_instance();
            string user="root";
            string passwd="root";
            string db="AIPD";
            string host="localhost";
            string fn;
            fn.append("/ftphome/pic/");

            stringstream  stream;
            stream<<get_time_point_ms();
            fn.append(stream.str());
            fn.append(".png");
            imwrite(fn,frame);
            if(sql_need_connect){
                ins.connect(host,user,passwd,db);
                sql_need_connect=false;
            }
            static char bf[1000];
            memset(bf,0,1000);
            get_tis_str(bf,get_sql_time(),1);
            prt(info,"start qurey");
            ins.query(bf);
            prt(info,"stop qurey");
        }
    }
    void insert_pic(Mat frame)
    {

            if(1){
                  prt(info,"get incident");

            string user="root";
            string passwd="root";
            string db="AIPD";
            string host="localhost";
            string fn;
            fn.append("/ftphome/pic/");

            stringstream  stream;
            stream<<get_time_point_ms();
            fn.append(stream.str());
            fn.append(".png");
            imwrite(fn,frame);
            prt(info,"get pic");


        }
    }
    void insert_database(CameraOutputData data,int index,Mat frame)
    {
#ifdef WITH_CUDA

        if(index<1)
        {
            prt(info,"err in inserting database");
            //           / return -1;
        }
        CameraInputData input=cms[index-1]->get_data();
        #if 0
        for(int i=0;i<data.DetectionResult.size();i++){
            DetectRegionOutputData d=data.DetectionResult[i];
            DetectRegionInputData  id= input.DetectRegion[i];
            JsonPacket rst=d.Result;
            //   prt(info," %s ",id.SelectedProcessor.data());
            if(id.SelectedProcessor==LABLE_PROCESSOR_MVD){
                MvdProcessorOutputData mvddata(rst);
                outputs[index-1].push_back(mvddata);
                vector<MvdProcessorOutputData> &ops=outputs[index-1];
                ops.push_back(mvddata);
                // prt(info," %s ",mvddata.data().str().data());


                int new_event_flag[7];
                memset(new_event_flag,0,7*sizeof(int));
                for( EventRegionObjectOutput  eo:  mvddata.EventObjects){
                   new_event_flag[eo.Type-1]++;
                }
                for(int k=0;k<7;k++){
                    if(new_event_flag[k]>old_event_flag[k]){
                        for(int t=0;t<mvddata.EventObjects.size();t++){
                            if(mvddata.EventObjects[t].Type==k){
                                single_insert(mvddata.EventObjects[t],frame);
                                break;
                            }
                        }

                    }
                       old_event_flag[k]=new_event_flag[k];
                }

//                if(mvddata.DriveAwayData.size()>0){
//                    prt(info,"get incident");
//                    DatabaseInstance &ins=DatabaseInstance::get_instance();
//                    string user="root";
//                    string passwd="root";
//                    string db="AIPD";
//                    string host="localhost";
//                    string fn;
//                    fn.append("/ftphome/pic/");

//                    stringstream  stream;
//                    stream<<get_time_point_ms();
//                    fn.append(stream.str());
//                    fn.append(".png");
//                    imwrite(fn,frame);
//                    if(sql_need_connect){
//                        ins.connect(host,user,passwd,db);
//                        sql_need_connect=false;
//                    }
//                    static char bf[1000];
//                    memset(bf,0,1000);
//                    get_tis_str(bf,get_sql_time(),1);
//                    prt(info,"start qurey");
//                    ins.query(bf);
//                      prt(info,"stop qurey");
//                    ins.query("INSERT INTO TIS \
//                              ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `RegionID`, `TEType`, `TEPAddr`, `TEVAddr`)\
//                              VALUES\
//                              ( '3', '2018-03-07 17:46:24', '60', '32', '大学城', '12', '12', '21', 'pic_path', 'video_path');");

//                            ins.query(" INSERT INTO PS ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `RegionID`, `PSum`, `PPSum`, `PNSum`, `PDensity`) \
//                                      VALUES \
//                                      ('2', '2018-10-17 16:34:07', '90', '1', '沙河西路', '2', '2', '15', '10', '5', '40');");


             //   }
             }




            //  .PersonFlow2;
            //  mvddata.

        }
        #else
        for(int i=0;i<data.DetectionResult.size();i++){
            DetectRegionOutputData d=data.DetectionResult[i];
            DetectRegionInputData  id= input.DetectRegion[i];
            JsonPacket rst=d.Result;
            //   prt(info," %s ",id.SelectedProcessor.data());
            if(id.SelectedProcessor==LABLE_PROCESSOR_MVD){
                MvdProcessorOutputData mvddata(rst);
                for(EventRegionObjectOutput eo:mvddata.EventObjects){
                    if(!exist_in_last(eo)){
                        database_insert_tis(get_sql_time(),eo.Type);
                        insert_pic(frame);
                    }
                }
                last_events.clear();
                last_events=mvddata.EventObjects;
                //outputs[index-1].push_back(mvddata);
                //vector<MvdProcessorOutputData> &ops=outputs[index-1];
                //ops.push_back(mvddata);
            }
        }
        #endif
#endif

    }
    inline bool exist_in_last(EventRegionObjectOutput event)
    {
        for(EventRegionObjectOutput e:last_events){
            if(e.EventID==event.EventID&&e.Type==event.Type){
                return true;
            }
        }
        return false;
    }
    void database_insert_tis(const char *sql_time,int type)
    {
        prt(info,"event %d",type);
        DatabaseInstance &ins=DatabaseInstance::get_instance();

        string user="root";
        string passwd="root";
        string db="AIPD";
        string host="localhost";
        if(sql_need_connect){
            ins.connect(host,user,passwd,db);
            sql_need_connect=false;
        }
//        ins.query("INSERT INTO TIS \
//        ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `RegionID`, `TEType`, `TEPAddr`, `TEVAddr`)\
//           VALUES\
//        ( '3', '2018-03-07 17:46:24', '60', '32', 'abc', '12', '12', '21', 'pic_path', 'video_path');");

        char buf[1000];memset(buf,0,1000);
        sprintf(buf,"INSERT INTO TIS \
                ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `RegionID`, `TEType`, `TEPAddr`, `TEVAddr`)\
                   VALUES\
                ( '3', '%s', '60', '32', 'abcde', '12', '12', '%d', 'pic_path', 'video_path');",sql_time,type);
        ins.query(buf);
    }

    void count_fun1()
    {
        int camera_size=private_data.CameraData.size();
        int count_time=COUNT_SECONDS;//10s
        int person_count[100];
        int car_count[100];
        int speed_count[100];


        int old_exist[100];//max lane num
        memset(old_exist,0,100);
        while((get_time_second()%6)){
            this_thread::sleep_for(chrono::microseconds(1000000));
        }
        while(!quit_count){
            if(count_time--==1){
                prt(info,"time up , start cal ---------------------------->");
                for(int loop_cams=0;loop_cams<camera_size;loop_cams++){
                    prt(info,"cam %d",loop_cams);
                    CameraInputData cd= private_data.CameraData[loop_cams];
                    int region_size=cd.DetectRegion.size();
                    vector< MvdProcessorOutputData>  &cam_out=outputs[loop_cams];
                    for(int loop_regions=0;loop_regions<region_size;loop_regions++){
                          prt(info,"region %d",loop_regions);
                        DetectRegionInputData dr=  cd.DetectRegion[loop_regions];
                        MvdProcessorInputData mvddata(  dr.ProcessorData);
                        int lane_size=mvddata.LaneData.size();
                        if(lane_size>=100){
                            prt(info,"max lane  100 ,err");
                            continue;
                        }

                        DatabaseInstance &ins=get_database();

                        for(int i=0;i<cam_out.size();i++){
                                  prt(info,"frame %d",loop_regions);
                            MvdProcessorOutputData tmp=cam_out[i];
                            // tmp.CongestionData
                            for(int j=0;j<tmp.LaneOutputData.size();j++){
                                if(old_exist[j]==0&&tmp.LaneOutputData[j].NearCarExist)
                                {
                                    car_count[j]++;
                                    speed_count[j]+=tmp.LaneOutputData[j].VehicleSpeed;

                                }
                                old_exist[j]=tmp.LaneOutputData[j].NearCarExist;
                            }
                        }
                        for(int j=0;j<mvddata.LaneData.size();j++){
                            database_insert_vs(j,speed_count,car_count);
                        }


                     }//loop regions
                }//loop cams
                for(int i=0;i<100;i++)
                    outputs[i].clear();

                  count_time=COUNT_SECONDS;
                    prt(info," stop cal ---------------------------->");
            }

            this_thread::sleep_for(chrono::microseconds(1000000));
        }
    }

#define INSERT_DB_MEM(xxx)       \
    stream<< "'";\
    stream<<xxx;\
    stream<< "'";
    void database_insert_vs(int lane_index,int speed_count[],int car_count[])
    {
        int vs_record_id=3;

        //strstream tmp1;tmp1<<COUNT_SECONDS;
        char buf_tmp[100];memset(buf_tmp,0,100);sprintf(buf_tmp,"%d",COUNT_SECONDS);
        string vs_sp(buf_tmp);
        string vs_sst=get_sql_time();
        string vs_analyce_id="2";
        string vs_savenue="12345";
        string vs_CameraID="1";
        string vs_LaneID="12";
        string vs_VSum="";//car_count
        string vs_VPSum="0";
        string vs_VNSum="0";
        string vs_VDSum="0";
        string vs_VBSum="0";
        string vs_VCSum="0";//car_count
        string vs_VMSum="0";
        string vs_VKSum="0";
        if(car_count[lane_index]){
            prt(info,"car num %d, speed total %d",car_count[lane_index],speed_count[lane_index]);
            int average_speed=speed_count[lane_index]/car_count[lane_index];
            memset(buf_tmp,0,100);
            sprintf(buf_tmp,"%d",average_speed);
            //  tmp2<<average_speed;
        }else{
              memset(buf_tmp,0,100);
                sprintf(buf_tmp,"%d",0);
        }
        string vs_ASpeed(buf_tmp);
        string vs_ATime="1";
        string vs_ASpace="1";
        string vs_AVM="1";
        string vs_AOccupy="1";
        string vs_RState="1";
        DatabaseInstance &ins=DatabaseInstance::get_instance();
        stringstream stream;
        stream<< "INSERT INTO VS \
            ( `RecordID`, `SST`, `SP`, `AnalyceID`, `SAvenue`, `CameraID`, `LaneID`,\
              `VSum`, `VPSum`, `VNSum`, `VTSum`, `VBSum`, `VCSum`, `VMSum`,\
              `VKSum`, `ASpeed`, `ATime`, `ASpace`, `AVM`, `AOccupy`,`RState`) VALUES";
        stream<< "(";

        INSERT_DB_MEM(vs_record_id) stream<< ",";
        INSERT_DB_MEM(vs_sst) stream<< ",";
        INSERT_DB_MEM(vs_sp) stream<< ",";
        INSERT_DB_MEM(vs_analyce_id) stream<< ",";
        INSERT_DB_MEM(vs_savenue) stream<< ",";
        INSERT_DB_MEM(vs_CameraID) stream<< ",";
        INSERT_DB_MEM(vs_LaneID) stream<< ",";


        INSERT_DB_MEM(vs_VSum) stream<< ",";
        INSERT_DB_MEM(vs_VPSum) stream<< ",";
        INSERT_DB_MEM(vs_VNSum) stream<< ",";
        INSERT_DB_MEM(vs_VDSum) stream<< ",";
        INSERT_DB_MEM(vs_VBSum) stream<< ",";
        INSERT_DB_MEM(vs_VCSum) stream<< ",";
        INSERT_DB_MEM(vs_VMSum) stream<< ",";

        INSERT_DB_MEM(vs_VKSum) stream<< ",";
        INSERT_DB_MEM(vs_ASpeed) stream<< ",";
        INSERT_DB_MEM(vs_ATime) stream<< ",";
        INSERT_DB_MEM(vs_ASpace) stream<< ",";
        INSERT_DB_MEM(vs_AVM) stream<< ",";
        INSERT_DB_MEM(vs_AOccupy) stream<< ",";
        INSERT_DB_MEM(vs_RState)
        stream<< ");";

        prt(info,"%s",stream.str().data());
        if(stream.str().size()>1000){
            prt(info,"sql too long");
            return;
        }
        char buf1[1000];
        memset(buf1,0,1000);
        sprintf(buf1,"%s",stream.str().data());
        ins.query(buf1);
    }
private:
    vector <Session*> *stream_cmd;//clients who connected to our server
    string str_stream;//
    vector <Camera*> cms;
    LocationService lservice;
    ConfigManager *p_cm;
    vector <DestClient > dest_clients;
    string client_tmp_ip;
    int udp_fd;
    bool sql_need_connect;
    thread *p_count_thread;

    //vector <MvdProcessorOutputData> outputs[100];//support 100 cameras
    vector <Mat> frames;
    bool quit_count;
    vector <EventRegionObjectOutput> last_events;
    vector <MvdProcessorOutputData> outputs[MAX_CAM_NUM];//support 100 cameras

    //int old_event_flag[7];
};

#endif // APP_H

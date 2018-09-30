#ifndef DATABASE_H
#define DATABASE_H
#include <mysql/mysql.h>
#include "tool.h"


using namespace std;

class Database
{
public:
    Database(char *host,char *user,char*passwd,char *db):
        host(host),user(user),passwd(passwd),db(db),sql(NULL),query_flag(false)
    {
        sql=mysql_init(NULL);
        //        if(!mysql_real_connect(sql,host,user,passwd,db,3306,0,0)){
        //            // mysql_free_result(res);
        //               sql=NULL;
        ////            const char *err= mysql_error(sql);
        ////            prt(info,"error: %s" ,err);
        ////            mysql_close(sql);

        //        }else{
        //            prt(info,"ok connect");
        //        }
    }
    bool connect()
    {
        if(!mysql_real_connect(sql,host,user,passwd,NULL,3306,0,0)){
            //   if(!mysql_real_connect(sql,host,user,passwd,db,3306,0,0)){
            // mysql_free_result(res);

            //  prt(info,"flag %d",               sql->client_flag);
            sql=NULL;
            //            const char *err= mysql_error(sql);
            //            prt(info,"error: %s" ,err);
            //            mysql_close(sql);
            return false;
        }else{
            prt(info,"ok connect");
            mysql_select_db(sql,db);
            return true;
        }
    }
    void print_result()
    {


        // resid = mysql_store_result(sql);


        // row_str_list = mysql_fetch_field(resid);
        //  prt(info,"rows count %d",           resid->row_count);
        //        int c1;
        //        sscanf(row_str_list[0],"%d",&c1);

        //        int row_cnt=mysql_num_rows(resid);
        //        for(int i=0;i<row_cnt;i++){
        //              row_str_list = mysql_fetch_row(resid);
        //            prt(info,"row %d:  %s",        i,   row_str_list[i]);
        //        }

    }
    void print_table()
    {


        //         int row_cnt=mysql_num_rows(resid);
        //         prt(info,"now table1 has %d rows",row_cnt);
        //         row_str_list = mysql_fetch_row(resid);
        //         row_str_list = mysql_fetch_field(res_field);

        query("select * from  table1");
      //  query("select * from  table1 where col1=343");
        if(!query_flag){
            prt(info,"eror:can not print table  because query failed");
            return ;
        }
    //     query("select * from  table1");
     //   resid = mysql_store_result(sql);
     //    mysql_free_result(resid );
       //  query("select * from  table1");
     //   resid = mysql_store_result(sql);
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

        mysql_free_result(resid );
        int mysqlret = mysql_real_query(sql,statement,(unsigned int)strlen(statement));
        if(mysqlret!=0)
        {
            query_flag=false;
     //       abort();
            assert(false);
            const char *err_str=  mysql_error(sql);
            prt(info,"error: %s" , err_str);
        }else{
            query_flag=true;
            resid = mysql_store_result(sql);
            //            row = mysql_fetch_row(resid);
            //            int num;
            //            sscanf(row[0],"%lld",&num);
            //            prt(info,"%d",num);
            //  mysql_free_result(resid );
        }
    }
    ~Database()
    {
        mysql_close(sql);
    }
private:
    char *host;
    char *user;
    char *passwd;
    char *db;
    MYSQL *sql;
    MYSQL_RES *resid;
    MYSQL_FIELD *field;
    bool query_flag;
    // MYSQL_ROW row_str_list;
};

#endif // DATABASE_H

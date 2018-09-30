#include <iostream>
#include <mysql/mysql.h>
using namespace std;
#include "tool.h"
#include "database.h"
void insert_table1(Database &d)
{
     data.query("create table table1 (col1 int,col2 double);");
     data.query("insert into table1 (col1 ,col2 )values(343,5.35);");
   // data.query("select * from  table1");
 }
int main()
{


    char *host="localhost";
    //    char *host="192.168.1.219";
    char *user="root";
    char *passwd="root";
    //  char *db="AIPD";
    // char *db="database1";
    char *db="database1";
    //  char *db="test";
    Database data(host,user,passwd,db);
    if(data.connect()){
        // data.query("select * from  table1");
     //   data.query("drop table table1");
        // data.query("create database database3");
      //   data.query("create table table1 (col1 int,col2 double);");
      //  data.query("insert into table1 (col1 ,col2 )values(343,5.35);");
       // data.query("insert into table1 (col1 ,col2 )values(344,5.355);");
       // data.query("delete  from  table1 where col1='343' ");
      // data.query("delete  from  table1 ");


       // data.query("select * from  table1");

        insert_table1(data);
        data.print_table();
      //  data.query("select * from  table1 ");
       // data.print_result();
    }else{
        prt(info,"connect fail");
    }
    // data.query("SET CHARACTER SET utf8");
    //  data.query("select * from  test1");

    return 0;
    cout << "Hello World!" << endl;
    MYSQL *sql;
    MYSQL_RES *resid;
    MYSQL_ROW row;
    sql=mysql_init(NULL);


    if(!mysql_real_connect(sql,host,user,passwd,db,3306,0,0)){
        // mysql_free_result(res);
        mysql_close(sql);
        prt(info,"error: %s" , mysql_error(sql));
        return 1;
    }else{
        prt(info,"ok connect");
    }


    //  char* query="SET CHARACTER SET utf8";
    char* query="select * from  test1";
    int mysqlret = mysql_real_query(sql,query,(unsigned int)strlen(query));
    if(mysqlret!=0)
    {
        //mysql_free_result(res);
        mysql_close(sql);
        prt(info,"err query");
        return 1;
        // prt(info,"ret %d\n",mysqlret);

    }else{
        prt(info,"ok query");
        resid = mysql_store_result(sql);
        row = mysql_fetch_row(resid);
        int num;
        sscanf(row[0],"%lld",&num);
        prt(info,"%d",num);
        mysql_free_result(resid );
    }

    //    sprintf(cmd,"insert into device_warn_table values ('',%d,'%010lu','%s','%s','%s',%d,'%s',0,'')",
    //		sql->sblx,sql->sbid,sql->sbip,sql->sbmc,sql->bjsj,sql->bjlb,sql->bjzt);
    mysql_close(sql);
    prt(info,"psswd %s",sql->passwd);
    return 0;
}

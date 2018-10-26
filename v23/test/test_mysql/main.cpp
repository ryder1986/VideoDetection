#include <iostream>
#include "tool.h"
using namespace std;
//#include <mysql.h>
//#include "sql.h"
#include <QtSql>

static bool createConnection()
{
QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
db.setHostName("192.168.1.4");
db.setDatabaseName("datainfo");       //这里输入你的数据库名
db.setUserName("root");
db.setPassword("myadmin");   //这里输入你的密码
if (!db.open()) {
    prt(info,"db fail");
//QMessageBox::critical(0, QObject::tr("无法打开数据库"),
//"无法创建数据库连接！ ", QMessageBox::Cancel);
return false;
}else{
      prt(info,"db ok");
} //

// 如果 MySQL 数据库中已经存在同名的表， 那么下面的代码不会执行
QSqlQuery query(db);
// 使数据库支持中文
query.exec("SET NAMES 'Latin1'");
// 创建 course 表
query.exec("create table course (id int primary key, "
"name varchar(20), teacher varchar(20))");
query.exec("insert into course values(0, '数学', '刘老师')");
query.exec("insert into course values(1, '英语', '张老师')");
query.exec("insert into course values(2, '计算机', '李老师')");
return true;
}

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}

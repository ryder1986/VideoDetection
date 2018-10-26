#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include "tool.h"
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  //  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
db.setHostName("192.168.1.14");
db.setDatabaseName("datainfo");       //这里输入你的数据库名
db.setUserName("roo11t");
db.setPassword("myadmin111");   //这里输入你的密码
if (!db.open()) {
    prt(info,"db fail");
//QMessageBox::critical(0, QObject::tr("无法打开数据库"),
//"无法创建数据库连接！ ", QMessageBox::Cancel);
return false;
}else{
      prt(info,"db ok");
      QSqlQuery query("select * from event", db);

        if (query.next())
      {
             QString stringXml = query.value(0).toString();
             qDebug()<<stringXml;
           //do somethingQT数据库错误

        }else{
            prt(info,"db  no data");

        }

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

//https://blog.csdn.net/kilotwo/article/details/79248657

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

  //  createConnection();
    MainWindow w;
    w.show();

    return a.exec();
}

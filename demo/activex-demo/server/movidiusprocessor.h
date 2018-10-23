#ifndef MOVIDIUSPROCESSOR_H
#define MOVIDIUSPROCESSOR_H

#include <Python.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <thread>
#include <mutex>
#include "conversion.h"
using namespace cv;
using namespace std;


template <typename T>
class py_arg_t{
public:
    py_arg_t(T d,string n):data(d),des(n)
    {}
    T data;
    string des;
};
template <typename... T>
void set_item(PyObject* pArgs,int index)
{
}
template <class... T,typename HEAD>
void set_item(PyObject* pArgs,int index,HEAD arg,T...args)
{
    int sz=sizeof...(args);
    PyTuple_SetItem(pArgs, index++, Py_BuildValue(((string)arg.des).data(),arg.data));
    set_item(pArgs,index,args...);
}
template <class... T>
PyObject * call_py(string fun_name,PyObject *pDict,T...args )
{
    PyObject*pFunc,*pArgs;
    // printf("sz %d\n",sizeof...(args));
    pFunc = PyDict_GetItemString(pDict, fun_name.data());
    if ( !pFunc || !PyCallable_Check(pFunc) ) {
        printf("can't find function [%s]",fun_name.data());
        // getchar();
        //return -1;
    }
    int sz=sizeof...(args);
    pArgs = PyTuple_New(sz);
    set_item(pArgs,0,args...);
    //    for(int i=0;i<sz;i++)
    //        PyTuple_SetItem(pArgs, i, Py_BuildValue(args[i].des,args[i].data));
    PyObject*ret=PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    return ret;
}
PyObject * convert(Mat frame);
#if 0
class MovidiusProcessor
{
    vector <Rect> result;
    PyObject* obj;
    NDArrayConverter *p_cvt;
public:
    static MovidiusProcessor &get_instance()
    {
        static MovidiusProcessor pro;
        return pro;
    }

    void process(Mat frame,vector <Rect> &rst)
    {
        printf(" (%x)processing  request a frame \n",this);fflush(NULL);
        frame_lock.lock();
        printf(" (%x)processing  1\n",this);fflush(NULL);
        result.clear();
        //        printf("----------- cvt -----------\n");fflush(NULL);
        //        //  NDArrayConverter cvt;

        //        //obj = cvt.toNDArray(frame);
        //        printf("----------- cvt done-----------\n");fflush(NULL);





        //py_arg_t<PyObject*> test_arg(convert(frame),"O");

        //        NDArrayConverter cvt;
        printf(" (%x)processing  1.1\n",this);fflush(NULL);

        PyObject* obj;
        obj = p_cvt->toNDArray(frame);








        py_arg_t<PyObject*> test_arg(obj,"O");


#if 0
        //call_py("process",pDict,test_arg);
        call_py("process123",pDict);
#else

        PyObject* rect_data;
        PyObject* ret_objs;


        //rect_data= call_py("process",pDict,test_arg);





        py_arg_t<int> arg_w(frame.cols,"l");
        py_arg_t<int> arg_h(frame.rows,"l");

        rect_data= call_py("process1",pDict,test_arg,arg_w,arg_h);












        PyArg_Parse(rect_data, "O!", &PyList_Type, &ret_objs);
        int size=PyList_Size(ret_objs);
        //    int size=0;
        printf("-----------get object rects: %d-----------\n",size/4);

        rst.clear();
        int i,j;
        for(i=0;i<size/4;i++){
            for(j=0;j<4;j++){
                int t;
                //PyArg_Parse(rect_data, "i", &PyList_Type, &ret_objs[i*4+j]);
                //   PyArg_Parse(&ret_objs[i*4+j], "i", &t);

                //   PyList_GetItem(ret_objs,i*4+j);
                t=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+j));
                printf("%d,",t);
            }

            int x,y,w,h;
            x=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+0));
            y=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+1));
            w=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+2));
            h=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+3));






            printf("-----\n");
            rst.push_back(Rect(x,y,w,h));
        }
        printf(" (%x)processing  3\n",this);fflush(NULL);
#endif
        frame_lock.unlock();
    }

    vector <Rect> get_rects()
    {
        return result;
    }

private:
    MovidiusProcessor()
    {
        // Py_Initialize();
        init();
        p_cvt=new NDArrayConverter();
        //         // 检查初始化是否成功
        //         if ( !Py_IsInitialized() ) {
        //          //   return -1;
        //         }
        //         pName = PyString_FromString("run1");
        //         //  pName = PyString_FromString("run");
        //         pModule = PyImport_Import(pName);
        //         if ( !pModule ) {
        //             printf("can't find .py");
        //             getchar();
        //          //   return -1;
        //         }
        //         pDict = PyModule_GetDict(pModule);
        //         if ( !pDict ) {
        //            // return -1;
        //         }



    }
    ~MovidiusProcessor()
    {
        delete p_cvt;
        Py_DECREF(pName);
        // Py_DECREF(pArgs);
        Py_DECREF(pModule);
        Py_DECREF(pDict);

        // 关闭Python
        Py_Finalize();
        call_py("release",pDict);
    }

    void init()
    {
        Py_Initialize();
        if ( !Py_IsInitialized() ) {
            printf("init err\n");
        }else{
            printf("init ok\n");
        }
        printf("finding ...\n");

        //          PyRun_SimpleString( "import sys");
        //          PyRun_SimpleString("sys.path.append('./')");

        pName = PyString_FromString("movidius");


        if(!pName){
            printf("finding err \n");fflush(NULL);
        }else{
            printf("finding ok \n");fflush(NULL);
        }

        pModule = PyImport_Import(pName);
        if ( !pModule ) {
            printf("can't find .py");fflush(NULL);
        }else{
            printf("py found\n");fflush(NULL);
        }
        pDict = PyModule_GetDict(pModule);
        if ( !pDict ) {
            printf("can't find dict");fflush(NULL);
        }else{
            printf("dict found\n");fflush(NULL);
        }
        //  this_thread::sleep_for(chrono::microseconds(3000000));
        call_py("init",pDict);

    }

    mutex frame_lock;
    PyObject *pName,*pModule,*pDict;

};
#else



class MovidiusProcessor
{
    vector <Rect> result;
    PyObject* obj;
    NDArrayConverter *p_cvt;
    PyThreadState *st;
public:
    static MovidiusProcessor &get_instance()
    {
        static MovidiusProcessor pro;
        return pro;
    }

    void process(Mat frame,vector <Rect> &rst)
    {
      //  printf(" (%x)processing  request a frame \n",this);fflush(NULL);
        frame_lock.lock();

        result.clear();
        PyObject* obj;

        obj = p_cvt->toNDArray(frame);

        py_arg_t<PyObject*> test_arg(obj,"O");
       //  printf("convert done "); ;fflush(NULL);
        PyObject* rect_data;
        PyObject* ret_objs;
        py_arg_t<int> arg_w(frame.cols,"l");
        py_arg_t<int> arg_h(frame.rows,"l");

     //   GILLock gil;
        rect_data= call_py("process1",pDict,test_arg,arg_w,arg_h);
      //  printf("end call py\n"); ;fflush(NULL);
        PyArg_Parse(rect_data, "O!", &PyList_Type, &ret_objs);
      //  printf("---------parse done---------\n");;fflush(NULL);
//   GILLock gil1;
   //PyEval_ReleaseLock();
        int size=PyList_Size(ret_objs);
        prt(info,"sz %d",size);
#if 1
        // GILLock gil1;


    //    int size=PyList_Size(ret_objs);
        //    int size=0;
        printf("-----------get object rects: %d-----------\n",size/4);;fflush(NULL);
  //      printf("---------1---------\n");;fflush(NULL);

        rst.clear();
        int i,j;
        for(i=0;i<size/4;i++){
            for(j=0;j<4;j++){
          //      int t;
                //PyArg_Parse(rect_data, "i", &PyList_Type, &ret_objs[i*4+j]);
                //   PyArg_Parse(&ret_objs[i*4+j], "i", &t);

                //   PyList_GetItem(ret_objs,i*4+j);
            //    t=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+j));
             //   printf("%d,",t);
            }

            int x,y,w,h;
            x=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+0));
            y=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+1));
            w=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+2));
            h=PyInt_AsLong(PyList_GetItem(ret_objs,i*4+3));
           // printf("-----\n");
            rst.push_back(Rect(x,y,w,h));
        }
     //   printf("---------2---------\n");;fflush(NULL);
       // printf(" (%x)processing  3\n",this);fflush(NULL);
        /**************************以下加入需要调用的python脚本代码  End***********************/
//            Py_UNBLOCK_THREADS;
//            Py_END_ALLOW_THREADS;

//PyEval_ReleaseLock();
// PyEval_ReleaseLock() ;
   //   PyEval_RestoreThread(st);

       // release();
//        Py_UNBLOCK_THREADS;
//        Py_END_ALLOW_THREADS;

#endif
        frame_lock.unlock();
    }

    vector <Rect> get_rects()
    {
        return result;
    }

private:
    MovidiusProcessor()
    {
        printf("  mvnc processor \n");fflush(NULL);

           init();
// init_thread_py();
    }
    ~MovidiusProcessor()
    {

            release();
//release_thread_py();

    }
    void release()
    {
//        delete p_cvt;
        call_py("release",pDict);
        Py_DECREF(pName);
        // Py_DECREF(pArgs);
        Py_DECREF(pModule);
        Py_DECREF(pDict);

        // 关闭Python
    //    Py_Finalize();

    }
    void *init_py()
    {

            Py_Initialize(); //初始化Python环境
            if ( !Py_IsInitialized() ) //检测是否初始化成功
            {
                return NULL;
            }
            else
            {
                PyEval_InitThreads();     //开启多线程支持
                int nInit = PyEval_ThreadsInitialized();  //检测线程支持是否开启成功
                if ( nInit )
                {
               //     PyEval_SaveThread();  //因为调用PyEval_InitThreads成功后，当前线程就拥有了GIL，释放当前线程的GIL，
                }
            }

    }

    void init_thread_py()
    {
        // 初始化
        Py_Initialize();
        // 初始化线程支持
        PyEval_InitThreads();
        // 启动子线程前执行，为了释放PyEval_InitThreads获得的全局锁，否则子线程可能无法获取到全局锁。
        PyEval_ReleaseThread(PyThreadState_Get());
    }

    void release_thread_py()
    {
        // 保证子线程调用都结束后
        PyGILState_Ensure();
        Py_Finalize();
    }

    void init()
    {
     //   init_thread_py();
 //          init_py();
        Py_Initialize();
        PyEval_InitThreads();
        PyEval_ReleaseLock();
//      int nInit = PyEval_ThreadsInitialized();
       // if ( !Py_IsInitialized() ) {
         //   printf("init err\n");
       // }else{
         //   printf("init ok\n");
       // }
     //   init_thread_py();
        p_cvt=new NDArrayConverter();
        printf("finding1 ------------------->...\n");

        //          PyRun_SimpleString( "import sys");
        //          PyRun_SimpleString("sys.path.append('./')");

        //pName = PyString_FromString("movidius");
        pName = PyString_FromString("video_new_run");


        if(!pName){
            printf("finding err \n");fflush(NULL);
        }else{
            printf("finding ok \n");fflush(NULL);
        }

        pModule = PyImport_Import(pName);
        if ( !pModule ) {
            printf("can't find .py");fflush(NULL);
        }else{
            printf("py found\n");fflush(NULL);
        }
        pDict = PyModule_GetDict(pModule);
        if ( !pDict ) {
            printf("can't find dict");fflush(NULL);
        }else{
            printf("dict found\n");fflush(NULL);
        }
        //  this_thread::sleep_for(chrono::microseconds(3000000));
        call_py("init",pDict);
        printf("init done \n");fflush(NULL);
//        while(1)
//            ;
    }

    mutex frame_lock;
    PyObject *pName,*pModule,*pDict;

};


#endif
#endif // MOVIDIUSPROCESSOR_H

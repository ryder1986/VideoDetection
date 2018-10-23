#ifndef PVDPROCESSOR_H
#define PVDPROCESSOR_H

#include "videoprocessor.h"
#include "jsonpacket.h"
#include "algorithm.h"
//class PvdProcessorInputData:public JsonData{
//public:
//    vector <VdPoint> DetectLine;
//    PvdProcessorInputData(JsonPacket pkt):JsonData(pkt)
//    {
//        decode();
//    }
//    void decode()
//    {
//        DECODE_JSONDATA_ARRAY_MEM(DetectLine);
//    }
//    void encode()
//    {
//        ENCODE_JSONDATA_ARRAY_MEM(DetectLine);
//    }
//};

//class PvdProcessorOutputData:public JsonData{
//public:
//    vector <ObjectRect> PvdDetectedObjects;
//    int PersonFlow1;
//    int PersonFlow2;
//    int CurrentPersionCount;
//    PvdProcessorOutputData()
//    {

//    }
//    void decode()
//    {
//        DECODE_JSONDATA_ARRAY_MEM(PvdDetectedObjects);
//        DECODE_INT_MEM(PersonFlow1);
//        DECODE_INT_MEM(PersonFlow2);
//        DECODE_INT_MEM(CurrentPersionCount);
//     }
//    void encode()
//    {
//        ENCODE_JSONDATA_ARRAY_MEM(PvdDetectedObjects);
//        ENCODE_INT_MEM(PersonFlow1);
//        ENCODE_INT_MEM(PersonFlow2);
//        ENCODE_INT_MEM(CurrentPersionCount);
//    }
//};


class PvdProcessor : public VideoProcessor
{
	typedef struct args{
		double scale_ratio;
		int scan_step;
		cv::Rect area;
		int no;
	}arg_t;
	arg_t arg;
	m_args *p_cfg;
public:
    PvdProcessor(JsonPacket pkt)
    {
		PvdProcessorInputData da(pkt);
		CPoint DetLine[2];
		
		DetLine[0].x = da.DetectLine[0].x;
		DetLine[0].y = da.DetectLine[0].y;
		DetLine[1].x = da.DetectLine[1].x;
		DetLine[1].y = da.DetectLine[1].y;		
        p_cfg=alg_mem_malloc();
		PvdArithInit(p_cfg->pCfgs, DetLine);
    }

    ~PvdProcessor()
    {
		alg_mem_free(p_cfg);
        p_cfg=NULL;
    }
    bool process(Mat img_src,JsonPacket &pkt)
    {
		int i = 0;
		int x = 0, y = 0, w = 0, h = 0, c;
		string names;
        IplImage* img = new IplImage(img_src);
     //   PvdArithProc(img, p_cfg->pCfgs, &p_cfg->p_outbuf->PVDoutbuf);

        int PersonFlow1 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[0];
        int PersonFlow2 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[1];
        int CurrentPersionCount = p_cfg->p_outbuf->PVDoutbuf.uPersonTotalSum;
		 vector <ObjectRect> PvdDetectedObjects;
		for( i = 0; i < p_cfg->p_outbuf->PVDoutbuf.uPersonTotalSum; i++) 
		{
            x = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.x;
            y = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.y;
            w = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.width;
            h = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.height;
            c = p_cfg->p_outbuf->PVDoutbuf.detObj[i].prob;
            char name_tmp[50];memset(name_tmp,0,50);
            strcpy(name_tmp,(char*)p_cfg->p_outbuf->PVDoutbuf.detObj[i].names);
            names.assign(name_tmp);
			ObjectRect  obj = ObjectRect(x, y, w, h, names, c);
            PvdDetectedObjects.push_back(obj);

		}
            PvdProcessorOutputData out(PvdDetectedObjects,PersonFlow1,PersonFlow2,CurrentPersionCount);
          //  prt(info,"get detected rst %d",  out.PvdDetectedObjects.size());
		pkt=out.data();
    }
};

#endif // PVDPROCESSOR_H

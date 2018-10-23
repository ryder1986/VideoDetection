#ifndef MVDPROCESSOR_H
#define MVDPROCESSOR_H

#include "videoprocessor.h"
#include "jsonpacket.h"
#include "algorithm.h"
#include <stdlib.h>
#define TEST_CLIENT
#undef TEST_CLIENT
#define  SAVE_VIDEO 
#ifdef SAVE_VIDEO
Mat imgSave;
#endif
#define TEST_PROGRAM
#undef TEST_PROGRAM
class MvdProcessor : public VideoProcessor
{
    m_args *p_cfg;
public:
	bool ArithInit_test(ALGCFGS *pCfgs, CPoint* ptDetLine, FVDDETECTCFG *pDownDetectCfg, INCIDENTDETECTCFG * pDownIncidentCfg, ALGPARAMS *pParams)
	{
		int i = 0;
		//PVD��ʼ��
		pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[0].x = ptDetLine[0].x;
		pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[0].y = ptDetLine[0].y;
		pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[1].x = ptDetLine[1].x;
		pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[1].y = ptDetLine[1].y;
		//FVD��ʼ��
		pCfgs->LaneAmount = pDownDetectCfg->uLaneTotalNum;//������
		//�жϳ��������Ƿ���ȷ
		for(i = 0; i < pCfgs->LaneAmount; i++)	
		{
			memcpy( (void*)pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea, (void*)pDownDetectCfg->EachLaneCfg[i].NearArea, 4 * sizeof(CPoint) );
			memcpy( (void*)pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea, (void*)pDownDetectCfg->EachLaneCfg[i].FarArea, 4 * sizeof(CPoint) );
			memcpy( (void*)pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea, (void*)pDownDetectCfg->EachLaneCfg[i].LaneArea, 4 * sizeof(CPoint) );
		}
		pCfgs->gThisFrameTime = 0; 
		////////////////////////////////////////////////////detect params
		pCfgs->target_id = 1;
		pCfgs->targets_size = 0;
		memset(pCfgs->targets, 0, MAX_TARGET_NUM * sizeof(CTarget));
		memset(pCfgs->detClasses, 0 , MAX_CLASSES * sizeof(CDetClass));
		for( i = 0; i < MAX_LANE; i++)
		{
			pCfgs->uDetectInSum[i] = 0;
			pCfgs->uDetectOutSum[i] = 0;
			pCfgs->uDetectVehicleSum[i] = 0;
			memset(pCfgs->detBoxes, 0, 20 * sizeof(CRect1));
			memset(pCfgs->uStatQuePos[i], 0, 6 * sizeof(Uint16));//ͳ���Ŷӳ���
			pCfgs->uVehicleQueueLength[i] = 0;
		}
		memset(pCfgs->detNum, 0, MAX_LANE * sizeof(Uint16));
		pCfgs->Visbility = FALSE;//�ܼ���
		pCfgs->VideoException = FALSE;//��Ƶ״̬
		pCfgs->bMaskLaneImage = FALSE;
		//��ͨ�¼������Ϣ��ʼ��
		memset(pCfgs->incident_targets, 0, MAX_TARGET_NUM * sizeof(CTarget));//��ͨ�¼��������Ŀ��
		pCfgs->incident_target_id = 1;
		pCfgs->incident_targets_size = 0;
		pCfgs->uIllegalParkNum = 0;
		memset(pCfgs->IllegalParkBox, 0, MAX_INCIDENT_NUM * sizeof(CRect1));//��ֹͣ��
		pCfgs->uOppositeDirDriveNum = 0;
		memset(pCfgs->OppositeDirDriveBox, 0, MAX_INCIDENT_NUM * sizeof(CRect1));//��ֹ����
		memset(pCfgs->direction, 0, MAX_REGION_NUM * sizeof(int));//�������з���
		memset(pCfgs->bCongestion, FALSE, MAX_LANE * sizeof(bool));//��������ӵ��
		pCfgs->uOffLaneNum = 0;
		memset(pCfgs->OffLaneBox, 0, MAX_INCIDENT_NUM * sizeof(CRect1));//ƫ�복��
		pCfgs->uNoPersonAllowNum = 0;
		memset(pCfgs->NoPersonAllowBox, 0, MAX_INCIDENT_NUM * sizeof(CRect1));//Υ������
		pCfgs->bMaskIncidentImage = FALSE;//��ͨ�¼���ģͼ��
		return	TRUE;
	}
	bool RestParams_test(ALGCFGS *pCfgs, CPoint* ptDetLine, FVDDETECTCFG *pDownDetectCfg, INCIDENTDETECTCFG * pDownIncidentCfg, ALGPARAMS *pParams)//���ò���
	{
		int i = 0;
		//PVD��ʼ��
		pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[0].x = ptDetLine[0].x;
		pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[0].y = ptDetLine[0].y;
		pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[1].x = ptDetLine[1].x;
		pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[1].y = ptDetLine[1].y;
		//FVD��ʼ��
		pCfgs->LaneAmount = pDownDetectCfg->uLaneTotalNum;//������
		//�жϳ��������Ƿ���ȷ
		for(i = 0; i < pCfgs->LaneAmount; i++)	
		{
			memcpy( (void*)pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea, (void*)pDownDetectCfg->EachLaneCfg[i].NearArea, 4 * sizeof(CPoint) );
			memcpy( (void*)pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea, (void*)pDownDetectCfg->EachLaneCfg[i].FarArea, 4 * sizeof(CPoint) );
			memcpy( (void*)pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea, (void*)pDownDetectCfg->EachLaneCfg[i].LaneArea, 4 * sizeof(CPoint) );
		}
		pCfgs->gThisFrameTime = 0; 
		////////////////////////////////////////////////////detect params
		pCfgs->target_id = 1;
		pCfgs->targets_size = 0;
		memset(pCfgs->targets, 0, MAX_TARGET_NUM * sizeof(CTarget));
		memset(pCfgs->detClasses, 0 , MAX_CLASSES * sizeof(CDetClass));
		for( i = 0; i < MAX_LANE; i++)
		{
			pCfgs->uDetectInSum[i] = 0;
			pCfgs->uDetectOutSum[i] = 0;
			pCfgs->uDetectVehicleSum[i] = 0;
			memset(pCfgs->detBoxes, 0, 20 * sizeof(CRect1));
			memset(pCfgs->uStatQuePos[i], 0, 6 * sizeof(Uint16));//ͳ���Ŷӳ���
			pCfgs->uVehicleQueueLength[i] = 0;
		}
		memset(pCfgs->detNum, 0, MAX_LANE * sizeof(Uint16));
		pCfgs->Visbility = FALSE;//�ܼ���
		pCfgs->VideoException = FALSE;//��Ƶ״̬
		pCfgs->bMaskLaneImage = FALSE;
		//��ͨ�¼������Ϣ��ʼ��
		memset(pCfgs->incident_targets, 0, MAX_TARGET_NUM * sizeof(CTarget));//��ͨ�¼��������Ŀ��
		pCfgs->incident_target_id = 1;
		pCfgs->incident_targets_size = 0;
		pCfgs->uIllegalParkNum = 0;
		memset(pCfgs->IllegalParkBox, 0, MAX_INCIDENT_NUM * sizeof(CRect1));//��ֹͣ��
		pCfgs->uOppositeDirDriveNum = 0;
		memset(pCfgs->OppositeDirDriveBox, 0, MAX_INCIDENT_NUM * sizeof(CRect1));//��ֹ����
		memset(pCfgs->direction, 0, MAX_REGION_NUM * sizeof(int));//�������з���
		memset(pCfgs->bCongestion, FALSE, MAX_LANE * sizeof(bool));//��������ӵ��
		pCfgs->uOffLaneNum = 0;
		memset(pCfgs->OffLaneBox, 0, MAX_INCIDENT_NUM * sizeof(CRect1));//ƫ�복��
		pCfgs->uNoPersonAllowNum = 0;
		memset(pCfgs->NoPersonAllowBox, 0, MAX_INCIDENT_NUM * sizeof(CRect1));//Υ������
		pCfgs->bMaskIncidentImage = FALSE;//��ͨ�¼���ģͼ��
		return	TRUE;

	}
	Uint16 ArithProc_test(IplImage* img, OUTBUF* outBuf, ALGCFGS *pCfgs, ALGPARAMS	*pParams)
	{
		int i, j;
		if(img->width <= 0 || img->height <= 0)
		{
			printf("img cannot be zero!\n");
			return 0;
		}
		//�������ݴ�С
		pCfgs->m_iHeight = (img->height > FULL_ROWS)? FULL_ROWS : img->height;
		pCfgs->m_iWidth = (img->width > FULL_COLS)? FULL_COLS : img->width;

		pCfgs->gThisFrameTime++;
		//PVD����
		//�������������
		for(i = 0; i < MAX_DIRECTION_NUM; i++)
		{
			outBuf->PVDoutbuf.uPersonSum[i] = pCfgs->uPersonSum[i];
			outBuf->PVDoutbuf.uBicycleSum[i] = pCfgs->uBicycleSum[i];
			outBuf->PVDoutbuf.uMotorbikeSum[i] = pCfgs->uMotorbikeSum[i];
		}
		//����

		outBuf->PVDoutbuf.detObj[0].class_id = 0;
		outBuf->PVDoutbuf.detObj[0].box.x = rand() % img->width;
		outBuf->PVDoutbuf.detObj[0].box.y = rand() % img->height;
		outBuf->PVDoutbuf.detObj[0].box.width = MIN(rand() % img->width + 1, img->width - 1 - outBuf->PVDoutbuf.detObj[0].box.x);
		outBuf->PVDoutbuf.detObj[0].box.height = MIN(rand() % img->height + 1, img->height - 1 - outBuf->PVDoutbuf.detObj[0].box.y);;
		outBuf->PVDoutbuf.detObj[0].prob = 80;
		strcpy(outBuf->PVDoutbuf.detObj[0].names, "P");	
		outBuf->PVDoutbuf.uObjNum = 1;

		//FVD����
		outBuf->FVDoutbuf.VideoException = 1;//��Ƶ�쳣
		outBuf->FVDoutbuf.Visbility = 1;//�ܼ���
		outBuf->FVDoutbuf.uLaneNum = pCfgs->LaneAmount;//������
		for( i = 0; i < pCfgs->LaneAmount; i++)
		{
			outBuf->FVDoutbuf.uEachLaneData[i].LaneNo = i;//������
			outBuf->FVDoutbuf.uEachLaneData[i].uVehicleFlow = 0;//������

			outBuf->FVDoutbuf.uEachLaneData[i].uRegionVehicleNumber = rand() % 5;//��������
			outBuf->FVDoutbuf.uEachLaneData[i].bFlowRegionState = 0;//���복��״̬
			outBuf->FVDoutbuf.uEachLaneData[i].uVehicleDirection = 0;//�������з���
			outBuf->FVDoutbuf.uEachLaneData[i].uVehicleSpeed = 0;//����
			outBuf->FVDoutbuf.uEachLaneData[i].uVehicleType = 0;//������
			outBuf->FVDoutbuf.uEachLaneData[i].uVehicleHeadtime = 0;//��ͷʱ��
			outBuf->FVDoutbuf.uEachLaneData[i].uVehicleQueueLength = 0;//�Ŷӳ���
			outBuf->FVDoutbuf.uEachLaneData[i].QueLine[0].x = 0;//�Ŷӳ�����
			outBuf->FVDoutbuf.uEachLaneData[i].QueLine[0].y = 0;
			outBuf->FVDoutbuf.uEachLaneData[i].QueLine[1].x = 0;
			outBuf->FVDoutbuf.uEachLaneData[i].QueLine[1].y = 0;   
			outBuf->FVDoutbuf.uEachLaneData[i].uLastVehicleLength = 0;//���һ������λ��
			outBuf->FVDoutbuf.uEachLaneData[i].LastVehiclePos[0].x = 0;//�Ŷӳ�����
			outBuf->FVDoutbuf.uEachLaneData[i].LastVehiclePos[0].y = 0;
			outBuf->FVDoutbuf.uEachLaneData[i].LastVehiclePos[1].x = 0;
			outBuf->FVDoutbuf.uEachLaneData[i].LastVehiclePos[1].y = 0;
			outBuf->FVDoutbuf.uEachLaneData[i].IsCarInTailFlag = 0;//β������ռ�б�־
			outBuf->FVDoutbuf.uActualDetectLength[i] = 3;//��������ʵ�ʳ���
			outBuf->FVDoutbuf.uActualTailLength[i] = 3;//ռ����Ȧʵ�ʳ���

		}
		//����
		outBuf->FVDoutbuf.detObj[0].class_id = 2;
		outBuf->FVDoutbuf.detObj[0].box.x = rand() % img->width;
		outBuf->FVDoutbuf.detObj[0].box.y = rand() % img->height;
		outBuf->FVDoutbuf.detObj[0].box.width = MIN(rand() % img->width + 1, img->width - 1 - outBuf->FVDoutbuf.detObj[0].box.x);;
		outBuf->FVDoutbuf.detObj[0].box.height = MIN(rand() % img->width + 1, img->width - 1 - outBuf->FVDoutbuf.detObj[0].box.x);;
		outBuf->FVDoutbuf.detObj[0].prob = 90;
		strcpy(outBuf->FVDoutbuf.detObj[0].names, "C");
		outBuf->FVDoutbuf.uObjNum = 1;//���������
		outBuf->FVDoutbuf.udetVehicleTotalNum = 1;//��������ڳ�������
		outBuf->FVDoutbuf.udetCarTotalNum = 1; //car num
		outBuf->FVDoutbuf.udetBusTotalNum = 0; //bus num
		outBuf->FVDoutbuf.udetTruckTotalNum = 0; //truck num

		return 1;
	}
    MvdProcessor(JsonPacket pkt)
    {
        int i = 0, j = 0;

        p_cfg = alg_mem_malloc();
        MvdProcessorInputData da(pkt);

        //pvd
        CPoint DetLine[2];
        DetLine[0].x = da.DetectLine[0].x;
        DetLine[0].y = da.DetectLine[0].y;
        DetLine[1].x = da.DetectLine[1].x;
        DetLine[1].y = da.DetectLine[1].y;

        //fvd
        for(i = 0; i < da.BasicCoil.size(); i++)
        {
            p_cfg->pDetectCfg->FvdDetectCfg.BasicCoil[i].x = da.BasicCoil[i].x;
            p_cfg->pDetectCfg->FvdDetectCfg.BasicCoil[i].y = da.BasicCoil[i].y;
        }

        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[0].x = da.BaseLine.BeginPoint.x;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[0].y = da.BaseLine.BeginPoint.y;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[1].x = da.BaseLine.EndPoint.x;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[1].y = da.BaseLine.EndPoint.y;
        p_cfg->pDetectCfg->FvdDetectCfg.base_length = da.BaseLine.Length;

        p_cfg->pDetectCfg->FvdDetectCfg.near_point_length = da.NearPointDistance;
        p_cfg->pDetectCfg->FvdDetectCfg.far_point_length = da.FarPointDistance;

        p_cfg->pDetectCfg->FvdDetectCfg.uLaneTotalNum = da.LaneData.size();
        for(i = 0; i < da.LaneData.size(); i++)
        {
            p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneNo = da.LaneData[i].LaneNo;
            for(j = 0; j < da.LaneData[i].FarArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[j].x = da.LaneData[i].FarArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[j].y = da.LaneData[i].FarArea[j].y;
            }
            for(j = 0; j < da.LaneData[i].NearArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[j].x = da.LaneData[i].NearArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[j].y = da.LaneData[i].NearArea[j].y;
            }
            for(j = 0; j < da.LaneData[i].LaneArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[j].x = da.LaneData[i].LaneArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[j].y = da.LaneData[i].LaneArea[j].y;
            }

        }
		//incident
		int uOppositeDirDriveRegionNum = 0, uIllegalParkRegionNum = 0, uNoPersonAllowRegionNum = 0, uNoEntryRegionNum = 0;
		for(i = 0; i < da.Events.size(); i++)//��ͨ�¼�
		{
            if(da.Events[i].Type == 1)//����
			{
                for(j = 0; j < da.Events[i].Vers.size(); j++)
				{
					p_cfg->pDetectCfg->IncidentDetectCfg.OppositeDirDriveRegion[uOppositeDirDriveRegionNum].detRegion[j].x = da.Events[i].Vers[j].x;
					p_cfg->pDetectCfg->IncidentDetectCfg.OppositeDirDriveRegion[uOppositeDirDriveRegionNum].detRegion[j].y = da.Events[i].Vers[j].y;
				}
				p_cfg->pDetectCfg->IncidentDetectCfg.direction[uOppositeDirDriveRegionNum] = da.Events[i].Direction;
				uOppositeDirDriveRegionNum++;
			}
            if(da.Events[i].Type == 2)//��ֹͣ������
			{
                for(j = 0; j < da.Events[i].Vers.size(); j++)
				{
					p_cfg->pDetectCfg->IncidentDetectCfg.IllegalParkRegion[uIllegalParkRegionNum].detRegion[j].x = da.Events[i].Vers[j].x;;
					p_cfg->pDetectCfg->IncidentDetectCfg.IllegalParkRegion[uIllegalParkRegionNum].detRegion[j].y = da.Events[i].Vers[j].y;
				}
				uIllegalParkRegionNum++;
			}
            if(da.Events[i].Type == 3)//��ֹ����
			{
				for(j = 0; j < da.Events[i].Vers.size(); j++)
				{
                    p_cfg->pDetectCfg->IncidentDetectCfg.NoPersonAllowRegion[uNoPersonAllowRegionNum].detRegion[j].x = da.Events[i].Vers[j].x;
                    p_cfg->pDetectCfg->IncidentDetectCfg.NoPersonAllowRegion[uNoPersonAllowRegionNum].detRegion[j].y = da.Events[i].Vers[j].y;
				}
				uNoPersonAllowRegionNum++;
			}
            if(da.Events[i].Type == 4)//ʻ������
			{
				for(j = 0; j < da.Events[i].Vers.size(); j++)
				{
					p_cfg->pDetectCfg->IncidentDetectCfg.NoEntryRegion[uNoEntryRegionNum].detRegion[j].x = da.Events[i].Vers[j].x;
					p_cfg->pDetectCfg->IncidentDetectCfg.NoEntryRegion[uNoEntryRegionNum].detRegion[j].y = da.Events[i].Vers[j].y;
				}
				uNoEntryRegionNum++;
			}
		}
		p_cfg->pDetectCfg->IncidentDetectCfg.uIllegalParkRegionNum = uIllegalParkRegionNum;//��ֹͣ��������
		p_cfg->pDetectCfg->IncidentDetectCfg.uOppositeDirDriveRegionNum = uOppositeDirDriveRegionNum;//��ֹ����������
		p_cfg->pDetectCfg->IncidentDetectCfg.uNoEntryRegionNum = uNoEntryRegionNum;//ʻ��������
		p_cfg->pDetectCfg->IncidentDetectCfg.uNoPersonAllowRegionNum = uNoPersonAllowRegionNum;//��ֹ������

#ifndef TEST_CLIENT
#ifdef TEST_PROGRAM
		ArithInit_test(p_cfg->pCfgs, DetLine, &p_cfg->pDetectCfg->FvdDetectCfg, &p_cfg->pDetectCfg->IncidentDetectCfg, p_cfg->pParams);
#else
        ArithInit(p_cfg->pCfgs, DetLine, &p_cfg->pDetectCfg->FvdDetectCfg, &p_cfg->pDetectCfg->IncidentDetectCfg, p_cfg->pParams);
#endif
#endif
    }
    ~MvdProcessor()
    {
        prt(info,"############free start");
#ifndef TEST_CLIENT
        alg_mem_free(p_cfg);
#endif
        prt(info,"############free done");
    }

    virtual bool modify_processor(JsonPacket p)
    {
        int i,j;
        MvdProcessorInputData da(p);
        //pvd
        CPoint DetLine[2];
        DetLine[0].x = da.DetectLine[0].x;
        DetLine[0].y = da.DetectLine[0].y;
        DetLine[1].x = da.DetectLine[1].x;
        DetLine[1].y = da.DetectLine[1].y;

        //fvd
        for(i = 0; i < da.BasicCoil.size(); i++)
        {
            p_cfg->pDetectCfg->FvdDetectCfg.BasicCoil[i].x = da.BasicCoil[i].x;
            p_cfg->pDetectCfg->FvdDetectCfg.BasicCoil[i].y = da.BasicCoil[i].y;
        }

        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[0].x = da.BaseLine.BeginPoint.x;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[0].y = da.BaseLine.BeginPoint.y;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[1].x = da.BaseLine.EndPoint.x;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[1].y = da.BaseLine.EndPoint.y;
        p_cfg->pDetectCfg->FvdDetectCfg.base_length = da.BaseLine.Length;

        p_cfg->pDetectCfg->FvdDetectCfg.near_point_length = da.NearPointDistance;
        p_cfg->pDetectCfg->FvdDetectCfg.far_point_length = da.FarPointDistance;

        p_cfg->pDetectCfg->FvdDetectCfg.uLaneTotalNum = da.LaneData.size();
        for(i = 0; i < da.LaneData.size(); i++)
        {
            p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneNo = da.LaneData[i].LaneNo;
            for(j = 0; j < da.LaneData[i].FarArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[j].x = da.LaneData[i].FarArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[j].y = da.LaneData[i].FarArea[j].y;
            }
            for(j = 0; j < da.LaneData[i].NearArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[j].x = da.LaneData[i].NearArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[j].y = da.LaneData[i].NearArea[j].y;
            }
            for(j = 0; j < da.LaneData[i].LaneArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[j].x = da.LaneData[i].LaneArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[j].y = da.LaneData[i].LaneArea[j].y;
            }

        }



        //incident
		int uOppositeDirDriveRegionNum = 0, uIllegalParkRegionNum = 0, uNoPersonAllowRegionNum = 0, uNoEntryRegionNum = 0;
		for(i = 0; i < da.Events.size(); i++)//��ͨ�¼�
		{
            if(da.Events[i].Type == 1)//����
			{
                for(j = 0; j < da.Events[i].Vers.size(); j++)
				{
					p_cfg->pDetectCfg->IncidentDetectCfg.OppositeDirDriveRegion[uOppositeDirDriveRegionNum].detRegion[j].x = da.Events[i].Vers[j].x;
					p_cfg->pDetectCfg->IncidentDetectCfg.OppositeDirDriveRegion[uOppositeDirDriveRegionNum].detRegion[j].y = da.Events[i].Vers[j].y;
				}
				p_cfg->pDetectCfg->IncidentDetectCfg.direction[uOppositeDirDriveRegionNum] = da.Events[i].Direction;
				uOppositeDirDriveRegionNum++;
			}
            if(da.Events[i].Type == 2)//��ֹͣ������
			{
                for(j = 0; j < da.Events[i].Vers.size(); j++)
				{
					p_cfg->pDetectCfg->IncidentDetectCfg.IllegalParkRegion[uIllegalParkRegionNum].detRegion[j].x = da.Events[i].Vers[j].x;;
					p_cfg->pDetectCfg->IncidentDetectCfg.IllegalParkRegion[uIllegalParkRegionNum].detRegion[j].y = da.Events[i].Vers[j].y;
				}
				uIllegalParkRegionNum++;
			}
            if(da.Events[i].Type == 3)//��ֹ����
			{
				for(j = 0; j < da.Events[i].Vers.size(); j++)
				{
                    p_cfg->pDetectCfg->IncidentDetectCfg.NoPersonAllowRegion[uNoPersonAllowRegionNum].detRegion[j].x = da.Events[i].Vers[j].x;
                    p_cfg->pDetectCfg->IncidentDetectCfg.NoPersonAllowRegion[uNoPersonAllowRegionNum].detRegion[j].y = da.Events[i].Vers[j].y;
				}
				uNoPersonAllowRegionNum++;
			}
            if(da.Events[i].Type == 4)//ʻ������
			{
				for(j = 0; j < da.Events[i].Vers.size(); j++)
				{
					p_cfg->pDetectCfg->IncidentDetectCfg.NoEntryRegion[uNoEntryRegionNum].detRegion[j].x = da.Events[i].Vers[j].x;
					p_cfg->pDetectCfg->IncidentDetectCfg.NoEntryRegion[uNoEntryRegionNum].detRegion[j].y = da.Events[i].Vers[j].y;
				}
				uNoEntryRegionNum++;
			}
		}
		p_cfg->pDetectCfg->IncidentDetectCfg.uIllegalParkRegionNum = uIllegalParkRegionNum;//��ֹͣ��������
		p_cfg->pDetectCfg->IncidentDetectCfg.uOppositeDirDriveRegionNum = uOppositeDirDriveRegionNum;//��ֹ����������
		p_cfg->pDetectCfg->IncidentDetectCfg.uNoEntryRegionNum = uNoEntryRegionNum;//ʻ��������
		p_cfg->pDetectCfg->IncidentDetectCfg.uNoPersonAllowRegionNum = uNoPersonAllowRegionNum;//��ֹ������
#ifndef TEST_CLIENT
#ifdef TEST_PROGRAM
		RestParams_test(p_cfg->pCfgs, DetLine, &p_cfg->pDetectCfg->FvdDetectCfg, &p_cfg->pDetectCfg->IncidentDetectCfg, p_cfg->pParams);
#else
        RestParams(p_cfg->pCfgs, DetLine, &p_cfg->pDetectCfg->FvdDetectCfg, &p_cfg->pDetectCfg->IncidentDetectCfg,  p_cfg->pParams);//���ò���
#endif
#endif
        return true;
    }
    virtual bool process_whole_pic(Mat img_src,JsonPacket &pkt,Rect rct)
    {
        img_src.copyTo(imgSave);
        //imshow("1",img_src);
        // prt(info,"%d %d",img_src.rows,img_src.cols);
        int i = 0;
        int x = 0, y = 0, w = 0, h = 0, c;
        string names;
        IplImage img = IplImage(img_src);
        CRect1 r1;
        r1.x=rct.x;
        r1.y=rct.y;
        r1.width=rct.width;
        r1.height=rct.height;
#ifndef TEST_CLIENT
  //      ArithProc_whole(&img, r1, p_cfg->p_outbuf, p_cfg->pCfgs, p_cfg->pParams);
#endif
        //pvd
        int PersonFlow1 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[0];
        int PersonFlow2 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[1];
        int CurrentPersionCount = p_cfg->p_outbuf->PVDoutbuf.uPersonTotalSum;

        int BicycleFlow1 = p_cfg->p_outbuf->PVDoutbuf.uBicycleSum[0];
        int BicycleFlow2 = p_cfg->p_outbuf->PVDoutbuf.uBicycleSum[1];
        int CurrentBicycleCount = p_cfg->p_outbuf->PVDoutbuf.uBicycleTotalSum;

        int MotorbikeFlow1 = p_cfg->p_outbuf->PVDoutbuf.uMotorbikeSum[0];
        int MotorbikeFlow2 = p_cfg->p_outbuf->PVDoutbuf.uMotorbikeSum[1];
        int CurrentMotorbikeCount = p_cfg->p_outbuf->PVDoutbuf.uBicycleTotalSum;
        //fvd
        vector <ObjectRect> MvdDetectedObjects;
        for( i = 0; i <p_cfg->p_outbuf->FVDoutbuf.uObjNum; i++)
        {
            x = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.x;
            y = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.y;
            w = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.width;
            h = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.height;
            c = p_cfg->p_outbuf->FVDoutbuf.detObj[i].prob;
            char names_tmp[50];
            strcpy(names_tmp, p_cfg->p_outbuf->FVDoutbuf.detObj[i].names);
            names=string(names_tmp);
            ObjectRect  obj = ObjectRect(x, y, w, h, names, c);
            MvdDetectedObjects.push_back(obj);
        }
        int CurrentVehicleNumber = p_cfg->p_outbuf->FVDoutbuf.uObjNum;
        int Visibility = p_cfg->p_outbuf->FVDoutbuf.Visbility;
        int VideoState = p_cfg->p_outbuf->FVDoutbuf.VideoException;

        vector <LaneOutputJsonData> LaneOutputData;
        for( i = 0;  i <p_cfg->p_outbuf->FVDoutbuf.uLaneNum; i++)
        {
            int laneNo, queLen, vehiNum, flow, speed,Farcarexist, Nearcarexist;
            laneNo = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].LaneNo;
            queLen = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleQueueLength;
            vehiNum = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uRegionVehicleNumber;
            flow = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleFlow;
            speed = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleSpeed;
            VdPoint pt1( p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[0].x,
                    p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[0].y);
            VdPoint pt2(p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[1].x,
                    p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[1].y );
			Farcarexist = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].IsCarInTailFlag;
			Nearcarexist = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].bFlowRegionState;

			LaneOutputJsonData detLaneData = LaneOutputJsonData(laneNo, queLen, pt1, pt2, vehiNum, flow, speed
				,p_cfg->p_outbuf->FVDoutbuf.uActualDetectLength[i],
				p_cfg->p_outbuf->FVDoutbuf.uActualTailLength[i],Farcarexist,Nearcarexist);
            LaneOutputData.push_back(detLaneData);
        }
        vector <DegreeJsonData> DegreeData; // on  lane points
        for( i = 0; i < 20; i++)
        {
            DegreeJsonData d(p_cfg->p_outbuf->FVDoutbuf.uDegreePoint[i][0],p_cfg->p_outbuf->FVDoutbuf.uDegreePoint[i][1]);
            DegreeData.push_back(d);

        }

//        MvdProcessorOutputData out(MvdDetectedObjects,
//                                   CurrentVehicleNumber,
//                                   Visibility,
//                                   VideoState,
//                                   LaneOutputData,
//                                   DegreeData,
//                                   PersonFlow1,
//                                   PersonFlow2,
//                                   CurrentPersionCount);
//        pkt=out.data();
        //   prt(info,"sending %s",pkt.str().data());
#ifdef SAVE_VIDEO
#ifndef TEST_CLIENT
        //�������
        rectangle(imgSave, cvRect(r1.x, r1.y, r1.width, r1.height), Scalar(0, 255 ,255), 2, 8, 0 );
        //��������
        for( i = 0; i < p_cfg->p_outbuf->FVDoutbuf.uObjNum; i++)
        {
            Rect objRect;
            int c;
            objRect.x = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.x;
            objRect.y = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.y;
            objRect.width = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.width;
            objRect.height = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.height;
            c = p_cfg->p_outbuf->FVDoutbuf.detObj[i].prob;
            char str[30];
            sprintf(str, " %d", c);
            char str1[50];
            strcpy(str1, p_cfg->p_outbuf->FVDoutbuf.detObj[i].names);
            strcat(str1, str);
            putText(imgSave, str1, Point(objRect.x,objRect.y - 15),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,255),1,8);
            rectangle(imgSave, objRect, Scalar(0, 0 ,255), 2, 8, 0 );
        }
        for( i = 0; i < p_cfg->p_outbuf->PVDoutbuf.uObjNum; i++)//���ˡ����г���Ħ�г�����
        {
            Rect objRect;
            int c;
            objRect.x = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.x;
            objRect.y = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.y;
            objRect.width = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.width;
            objRect.height = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.height;
            c = p_cfg->p_outbuf->PVDoutbuf.detObj[i].prob;
            char str[30];
            sprintf(str, " %d", c);
            char str1[50];
            strcpy(str1, p_cfg->p_outbuf->PVDoutbuf.detObj[i].names);
            strcat(str1, str);
            putText(imgSave, str1, Point(objRect.x,objRect.y - 15),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,255),1,8);
            rectangle(imgSave, objRect, Scalar(0, 0 ,255), 2, 8, 0 );
        }
        int CurrentCarCount = p_cfg->pCfgs->ResultMsg.FvdResultInfo.udetCarTotalNum;
        int CurrentBusCount = p_cfg->pCfgs->ResultMsg.FvdResultInfo.udetBusTotalNum;
        int CurrentTruckCount = p_cfg->pCfgs->ResultMsg.FvdResultInfo.udetTruckTotalNum;
        //��ǰ�˳���
        char str2[50], str3[50],str4[50],str5[50];
        sprintf(str2, "Vehicle:%d", CurrentVehicleNumber);
        putText(imgSave, str2, Point(0,30),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        sprintf(str3, "Person:%d   ", CurrentPersionCount);
        sprintf(str4, "Bicycle:%d   ", CurrentBicycleCount);
        sprintf(str5, "Motorbike:%d", CurrentMotorbikeCount);
        strcat(str3,str4);
        strcat(str3,str5);
        putText(imgSave, str3, Point(0,80),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        char str6[50],str7[50],str8[50];
        sprintf(str6, "Car:%d   ", CurrentCarCount);
        sprintf(str7, "Bus:%d   ", CurrentBusCount);
        sprintf(str8, "Truck:%d", CurrentTruckCount);
        strcat(str6,str7);
        strcat(str6,str8);
        char str9[50],str10[50],str11[50],str12[50],str13[50],str14[50];
        sprintf(str9, "Person North: %d ", PersonFlow1);
        sprintf(str10, "South: %d", PersonFlow2);
        strcat(str9,str10);
        sprintf(str11, "Bicycle North: %d ", BicycleFlow1);
        sprintf(str12, "South: %d", BicycleFlow2);
        strcat(str11,str12);
        sprintf(str13, "Motorbike North: %d ", MotorbikeFlow1);
        sprintf(str14, "South: %d", MotorbikeFlow2);
        strcat(str13,str14);
        putText(imgSave, str9, Point(0,130),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        putText(imgSave, str11, Point(0,180),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        putText(imgSave, str13, Point(0,230),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        //pvd�����
        CPoint detLline[2];
        detLline[0].x = p_cfg->pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[0].x + +rct.x;
        detLline[0].y = p_cfg->pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[0].y + rct.y;
        detLline[1].x = p_cfg->pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[1].x + rct.x;
        detLline[1].y = p_cfg->pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[1].y + rct.y;
        line(imgSave, Point(detLline[0].x,detLline[0].y),Point(detLline[1].x,detLline[1].y), Scalar(255, 255 ,0), 3, 8, 0 );
        for(i = 0; i < p_cfg->pDetectCfg->FvdDetectCfg.uLaneTotalNum; i++)
        {
            CPoint m_ptend[12];
            /*m_ptend[0]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[0];
            m_ptend[1]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[1];
            m_ptend[2]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[3];
            m_ptend[3]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[2];
            m_ptend[4]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[0];
            m_ptend[5]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[1];
            m_ptend[6]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[3];
            m_ptend[7]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[2];
            m_ptend[8]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[0];
            m_ptend[9]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[1];
            m_ptend[10]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[3];
            m_ptend[11]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[2]; */
            m_ptend[0]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea[0];
            m_ptend[1]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea[1];
            m_ptend[2]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea[3];
            m_ptend[3]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea[2];
            m_ptend[4]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea[0];
            m_ptend[5]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea[1];
            m_ptend[6]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea[3];
            m_ptend[7]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea[2];
            m_ptend[8]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea[0];
            m_ptend[9]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea[1];
            m_ptend[10]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea[3];
            m_ptend[11]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea[2];
            line(imgSave, Point(m_ptend[0].x + rct.x,m_ptend[0].y+ rct.y),Point(m_ptend[1].x+ rct.x,m_ptend[1].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            line(imgSave, Point(m_ptend[0].x + rct.x,m_ptend[0].y+ rct.y),Point(m_ptend[2].x+ rct.x,m_ptend[2].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            line(imgSave, Point(m_ptend[1].x+ rct.x,m_ptend[1].y+ rct.y),Point(m_ptend[3].x+ rct.x,m_ptend[3].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            line(imgSave, Point(m_ptend[2].x + rct.x,m_ptend[2].y+ rct.y),Point(m_ptend[3].x+ rct.x,m_ptend[3].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            if(p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].calarflag)
            {
                line(imgSave, Point(m_ptend[8].x+ rct.x,m_ptend[8].y+ rct.y),Point(m_ptend[9].x+ rct.x,m_ptend[9].y+ rct.y), Scalar(0, 0 ,255), 3, 8, 0 );
                line(imgSave, Point(m_ptend[10].x+ rct.x,m_ptend[10].y+ rct.y),Point(m_ptend[11].x+ rct.x,m_ptend[11].y+ rct.y), Scalar(0, 0 ,255), 3, 8, 0 );
            }
            else
            {
                line(imgSave, Point(m_ptend[8].x+ rct.x,m_ptend[8].y+ rct.y),Point(m_ptend[9].x+ rct.x,m_ptend[9].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
                line(imgSave, Point(m_ptend[10].x+ rct.x,m_ptend[10].y+ rct.y),Point(m_ptend[11].x+ rct.x,m_ptend[11].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            }
            line(imgSave, Point(p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].QueLine[0].x,p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].QueLine[0].y),Point(p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].QueLine[1].x,p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].QueLine[1].y), Scalar(0, 255 ,0), 3, 8, 0 );
            if(p_cfg->pCfgs->IsCarInTail[i])
            {
                line(imgSave, Point(m_ptend[4].x+ rct.x,m_ptend[4].y+ rct.y),Point(m_ptend[5].x+ rct.x,m_ptend[5].y+ rct.y), Scalar(0, 0 ,255), 3, 8, 0);
                line(imgSave, Point(m_ptend[6].x+ rct.x,m_ptend[6].y+ rct.y),Point(m_ptend[7].x+ rct.x,m_ptend[7].y+ rct.y), Scalar(0, 0 ,255), 3, 8, 0);
            }
            else
            {
                line(imgSave, Point(m_ptend[4].x+ rct.x,m_ptend[4].y+ rct.y),Point(m_ptend[5].x+ rct.x,m_ptend[5].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0);
                line(imgSave, Point(m_ptend[6].x+ rct.x,m_ptend[6].y+ rct.y),Point(m_ptend[7].x+ rct.x,m_ptend[7].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0);
            }
            char str4[50], str5[50];
            sprintf(str4, "%d ", p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].uRegionVehicleNumber);//��������
            sprintf(str5, "%d ", p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].uVehicleFlow);//������
            putText(imgSave, str4, Point(i * 80 ,280),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
            putText(imgSave, str5, Point(i * 80 ,330),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);

        }
    //    namedWindow("detect",CV_WINDOW_NORMAL);
   //     imshow("detect", imgSave);
    //    waitKey(1);
#endif
#endif
    }
    inline VdRect crect_2_vdrect(CRect1 r)
    {
        return VdRect(r.x,r.y,r.width,r.height);
    }
    inline VdRect line_2_vdrect(VdPoint p1,VdPoint p2)
    {
        return VdRect(p1.x,p1.y,p2.x,p2.y);
    }
    bool process(Mat img_src,JsonPacket &pkt)
    {

        img_src.copyTo(imgSave);
        //imshow("1",img_src);
        //prt(info,"%d %d",img_src.rows,img_src.cols);
        int i = 0;
        int x = 0, y = 0, w = 0, h = 0, c;
        string names;
        IplImage img = IplImage(img_src);
#ifdef TEST_PROGRAM
		ArithProc_test(&img, p_cfg->p_outbuf, p_cfg->pCfgs, p_cfg->pParams);
#else
        ArithProc(&img, p_cfg->p_outbuf, p_cfg->pCfgs, p_cfg->pParams);
#endif

        //pvd
        int PersonFlow1 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[0];
        int PersonFlow2 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[1];
        int CurrentPersionCount = p_cfg->p_outbuf->PVDoutbuf.uPersonTotalSum;
        //fvd
        vector <ObjectRect> MvdDetectedObjects;
        for( i = 0; i <p_cfg->p_outbuf->FVDoutbuf.uObjNum; i++)
        {
            x = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.x;
            y = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.y;
            w = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.width;
            h = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.height;
            c = p_cfg->p_outbuf->FVDoutbuf.detObj[i].prob;
            char names_tmp[50];
            strcpy(names_tmp, p_cfg->p_outbuf->FVDoutbuf.detObj[i].names);
            names=string(names_tmp);
            ObjectRect  obj = ObjectRect(x, y, w, h, names, c);
            MvdDetectedObjects.push_back(obj);
        }
        int CurrentVehicleNumber = p_cfg->p_outbuf->FVDoutbuf.uObjNum - p_cfg->p_outbuf->PVDoutbuf.uPersonTotalSum;
        int Visibility = p_cfg->p_outbuf->FVDoutbuf.Visbility;
        int VideoState = p_cfg->p_outbuf->FVDoutbuf.VideoException;

        vector <LaneOutputJsonData> LaneOutputData;
        for( i = 0;  i <p_cfg->p_outbuf->FVDoutbuf.uLaneNum; i++)
        {
            int laneNo, queLen, vehiNum, flow, speed, Farcarexist, Nearcarexist;
            laneNo = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].LaneNo;
            queLen = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleQueueLength;
            vehiNum = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uRegionVehicleNumber;
            flow = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleFlow;
            speed = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleSpeed;
            VdPoint pt1( p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[0].x,
                    p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[0].y);
            VdPoint pt2(p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[1].x,
                    p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[1].y );
			Farcarexist = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].IsCarInTailFlag;
			Nearcarexist = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].bFlowRegionState;

            LaneOutputJsonData detLaneData = LaneOutputJsonData(laneNo, queLen, pt1, pt2, vehiNum, flow, speed
                                                                ,p_cfg->p_outbuf->FVDoutbuf.uActualDetectLength[i],
                                                                p_cfg->p_outbuf->FVDoutbuf.uActualTailLength[i],Farcarexist,Nearcarexist);
            LaneOutputData.push_back(detLaneData);
        }
        vector <DegreeJsonData> DegreeData; // on  lane points
        for( i = 0; i < 20; i++)
        {
            DegreeJsonData d(p_cfg->p_outbuf->FVDoutbuf.uDegreePoint[i][0],p_cfg->p_outbuf->FVDoutbuf.uDegreePoint[i][1]);
            DegreeData.push_back(d);
        }

		/*if(p_cfg->pCfgs->gThisFrameTime % 100 == 0)
		{

            prt(info,"%d",p_cfg->pCfgs->gThisFrameTime);
			p_cfg->p_outbuf->INCIDENToutbuf.uIllegalParkNum = rand() % 3;
			p_cfg->p_outbuf->INCIDENToutbuf.uOppositeDirDriveNum = rand() % 3;
			p_cfg->p_outbuf->INCIDENToutbuf.uOffLaneNum = rand() % 3;
			p_cfg->p_outbuf->INCIDENToutbuf.uNoPersonAllowNum = rand() % 3;
			p_cfg->p_outbuf->INCIDENToutbuf.uCongestionNum = rand() % 3;
			p_cfg->p_outbuf->INCIDENToutbuf.uAbandonedObjectNum = rand() % 3;

			for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uIllegalParkNum; i++)//ͣ��
			{
				p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].x = rand() % 640;
				p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].y = rand() % 480;
				p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].width = MIN(rand() % 640 + 1, 639 - p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].x);
				p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].height = MIN(rand() % 480 + 1, 479 - p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].y);
			}
			for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uOppositeDirDriveNum; i++)//����
			{
				p_cfg->p_outbuf->INCIDENToutbuf.OppositeDirDriveBox[i].x = rand() % 640;
				p_cfg->p_outbuf->INCIDENToutbuf.OppositeDirDriveBox[i].y = rand() % 480;
				p_cfg->p_outbuf->INCIDENToutbuf.OppositeDirDriveBox[i].width = MIN(rand() % 640 + 1, 639 - p_cfg->p_outbuf->INCIDENToutbuf.OppositeDirDriveBox[i].x);
				p_cfg->p_outbuf->INCIDENToutbuf.OppositeDirDriveBox[i].height = MIN(rand() % 480 + 1, 479 - p_cfg->p_outbuf->INCIDENToutbuf.OppositeDirDriveBox[i].y);
			}
			for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uOffLaneNum; i++)//ʻ��
			{
				p_cfg->p_outbuf->INCIDENToutbuf.OffLaneBox[i].x = rand() % 640;
				p_cfg->p_outbuf->INCIDENToutbuf.OffLaneBox[i].y = rand() % 480;
				p_cfg->p_outbuf->INCIDENToutbuf.OffLaneBox[i].width = MIN(rand() % 640 + 1, 639 - p_cfg->p_outbuf->INCIDENToutbuf.OffLaneBox[i].x);
				p_cfg->p_outbuf->INCIDENToutbuf.OffLaneBox[i].height = MIN(rand() % 480 + 1, 479 - p_cfg->p_outbuf->INCIDENToutbuf.OffLaneBox[i].y);
			}
			for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uNoPersonAllowNum; i++)//��ֹ����
			{
				p_cfg->p_outbuf->INCIDENToutbuf.NoPersonAllowBox[i].x = rand() % 640;
				p_cfg->p_outbuf->INCIDENToutbuf.NoPersonAllowBox[i].y = rand() % 480;
				p_cfg->p_outbuf->INCIDENToutbuf.NoPersonAllowBox[i].width = MIN(rand() % 640 + 1, 639 - p_cfg->p_outbuf->INCIDENToutbuf.NoPersonAllowBox[i].x);
				p_cfg->p_outbuf->INCIDENToutbuf.NoPersonAllowBox[i].height = MIN(rand() % 480 + 1, 479 - p_cfg->p_outbuf->INCIDENToutbuf.NoPersonAllowBox[i].y);
			}
			for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uCongestionNum; i++)
			{
				p_cfg->p_outbuf->INCIDENToutbuf.CongestionLine[i][0].x = rand() % 640;
				p_cfg->p_outbuf->INCIDENToutbuf.CongestionLine[i][0].y = rand() % 480;
				p_cfg->p_outbuf->INCIDENToutbuf.CongestionLine[i][1].x = MIN(rand() % 640, 639);
				p_cfg->p_outbuf->INCIDENToutbuf.CongestionLine[i][1].y = MIN(rand() % 640, 479);
			}
			for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uAbandonedObjectNum; i++)
			{
				p_cfg->p_outbuf->INCIDENToutbuf.AbandonedObject[i].x = rand() % 640;
				p_cfg->p_outbuf->INCIDENToutbuf.AbandonedObject[i].y = rand() % 480;
				p_cfg->p_outbuf->INCIDENToutbuf.AbandonedObject[i].width = MIN(rand() % 640 + 1, 639 - p_cfg->p_outbuf->INCIDENToutbuf.AbandonedObject[i].x);
				p_cfg->p_outbuf->INCIDENToutbuf.AbandonedObject[i].height = MIN(rand() % 480 + 1, 479 - p_cfg->p_outbuf->INCIDENToutbuf.AbandonedObject[i].y);
			}

		}
		else
		{
			p_cfg->p_outbuf->INCIDENToutbuf.uIllegalParkNum = 0;
			p_cfg->p_outbuf->INCIDENToutbuf.uOppositeDirDriveNum = 0;
			p_cfg->p_outbuf->INCIDENToutbuf.uOffLaneNum = 0;
			p_cfg->p_outbuf->INCIDENToutbuf.uNoPersonAllowNum = 0;
			p_cfg->p_outbuf->INCIDENToutbuf.uCongestionNum = 0;
			p_cfg->p_outbuf->INCIDENToutbuf.uAbandonedObjectNum = 0;




		}*/


        vector <EventRegionObjectOutput> EventObjects;
		//incident 
		//��ͣ
		for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uIllegalParkNum; i++)
		{
			vector<VdPoint> Vers;
			for(int j = 0; j < 4; j++)
			{
                Vers.push_back(VdPoint(p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].IncidentBox[j].x,
                                       p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].IncidentBox[j].y));
			}
            //id = p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].uIncidentID;
            EventObjects.push_back(EventRegionObjectOutput(Vers,EventRegion::STOP_INVALID,p_cfg->p_outbuf->INCIDENToutbuf.IllegalParkBox[i].uIncidentID));
		}
		//����
		for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uOppositeDirDriveNum; i++)
		{
			vector<VdPoint> Vers;
			for(int j = 0; j < 4; j++)
			{
                Vers.push_back(VdPoint(p_cfg->p_outbuf->INCIDENToutbuf.OppositeDirDriveBox[i].IncidentBox[j].x,
                                       p_cfg->p_outbuf->INCIDENToutbuf.OppositeDirDriveBox[i].IncidentBox[j].y));
			}
            EventObjects.push_back(EventRegionObjectOutput(Vers,EventRegion::REVERSE_DRIVE,p_cfg->p_outbuf->INCIDENToutbuf.OppositeDirDriveBox[i].uIncidentID));
		}
		//ʻ��
		for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uOffLaneNum; i++)
		{
			vector<VdPoint> Vers;
			for(int j = 0; j < 4; j++)
			{
                Vers.push_back(VdPoint(p_cfg->p_outbuf->INCIDENToutbuf.OffLaneBox[i].IncidentBox[j].x,
                                       p_cfg->p_outbuf->INCIDENToutbuf.OffLaneBox[i].IncidentBox[j].y));
			}
            EventObjects.push_back(EventRegionObjectOutput(Vers,EventRegion::DRIVE_AWAY,p_cfg->p_outbuf->INCIDENToutbuf.OffLaneBox[i].uIncidentID));

		}
		//��ֹ����
		for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uNoPersonAllowNum; i++)
		{
			vector<VdPoint> Vers;
			for(int j = 0; j < 4; j++)
			{
                Vers.push_back(VdPoint(p_cfg->p_outbuf->INCIDENToutbuf.NoPersonAllowBox[i].IncidentBox[j].x,
                                       p_cfg->p_outbuf->INCIDENToutbuf.NoPersonAllowBox[i].IncidentBox[j].y));
			}
            EventObjects.push_back(EventRegionObjectOutput(Vers,EventRegion::NO_PEDESTRIANTION,p_cfg->p_outbuf->INCIDENToutbuf.NoPersonAllowBox[i].uIncidentID));

		}
		//ӵ��
		for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uCongestionNum; i++)
		{
			vector<VdPoint> Vers;
			for(int j = 0; j < 4; j++)
			{
                Vers.push_back(VdPoint(p_cfg->p_outbuf->INCIDENToutbuf.CongestionBox[i].IncidentBox[j].x,
                                       p_cfg->p_outbuf->INCIDENToutbuf.CongestionBox[i].IncidentBox[j].y));
			}
            EventObjects.push_back(EventRegionObjectOutput(Vers, EventRegion::CONGESTION,p_cfg->p_outbuf->INCIDENToutbuf.CongestionBox[i].uIncidentID));

		}
		//������
        //prt(info,"alg rst %d", p_cfg->p_outbuf->INCIDENToutbuf.uAbandonedObjectNum);
		for( i = 0; i < p_cfg->p_outbuf->INCIDENToutbuf.uAbandonedObjectNum; i++)
		{
			vector<VdPoint> Vers;
			for(int j = 0; j < 4; j++)
			{
                Vers.push_back(VdPoint(p_cfg->p_outbuf->INCIDENToutbuf.AbandonedObject[i].IncidentBox[j].x,
                                       p_cfg->p_outbuf->INCIDENToutbuf.AbandonedObject[i].IncidentBox[j].y));
			}
            EventObjects.push_back(EventRegionObjectOutput(Vers,EventRegion::AbANDON_OBJECT,p_cfg->p_outbuf->INCIDENToutbuf.AbandonedObject[i].uIncidentID));

		}

        MvdProcessorOutputData out(MvdDetectedObjects,
                                   CurrentVehicleNumber,
                                   Visibility,
                                   VideoState,
                                   LaneOutputData,
                                   DegreeData,
                                   PersonFlow1,
                                   PersonFlow2,
                                   CurrentPersionCount,
                                   EventObjects);
        pkt=out.data();
    }
};


#endif // MVDPROCESSOR_H

//========================================
// 
// �`�揈��
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "draw-info.h"
#include "PutObject/polygon2D.h"
#include "PutObject/polygon3D.h"
#include "PutObject/model.h"
#include "PutObject/text2D.h"
#include "PutObject/text3D.h"
#include "../../RNmain.h"
#include "../Mechanical/memory.h"

//****************************************
// �N���X��`
//****************************************
// �`��N���X
class CDrawMgr {
public:
	//----------------------------------------
	// �萔�錾
	//----------------------------------------
	static const UShort POLYGON2D_ALLOC_BASE_POWER = 8;
	static const UShort POLYGON3D_ALLOC_BASE_POWER = 8;
	static const UShort SCREEN_PRIORITY_MAX        = 8;
	// 0 �J�����|���S������
	// 1 free
	// 2 free
	// 3 free
	// 4 free
	// 5 free
	// 6 free
	// 7 �f�o�b�O���O
	static const UShort SCREEN_PRIORITY_DEBUG_LOG = 7;

	//----------------------------------------
	// �N���X��`
	//----------------------------------------
	// �`���񑍊��N���X
	class CDrawInfoSum {
	public:
		// [[[ �֐��錾 ]]]
		CDrawInfoSum  ();
		~CDrawInfoSum ();
		void Release  (void);
		void Overwrite(CDrawInfoSum* pOvr);

		// [[[ �ϐ��錾 ]]]
		CModel::CDrawInfo**     m_model;
		UShort                  m_modelNum;
		CPolygon3D::CDrawInfo** m_polygon3D;
		UShort                  m_polygon3DNum;
		CPolygon2D::CDrawInfo** m_polygon2D;
		UShort                  m_polygon2DNum;
	};

	// �o�^����񑍊��N���X
	class CRegistInfoSum {
	public:
		// [[[ �֐��錾 ]]]
		CRegistInfoSum ();
		~CRegistInfoSum();
		void InitAlloc (void);
		void ReAlloc   (void);
		void Release   (void);

		// [[[ �ϐ��錾 ]]]
		CPolygon2D::CRegistInfo** m_polygon2DRegistInfos;
		UShort                    m_polygon2DRegistInfoNum;
		UShort                    m_polygon2DRegistInfoAllocPower;
		UShort                    m_polygon2DRegistInfoAllocNum;
		CPolygon3D::CRegistInfo** m_polygon3DRegistInfos;
		UShort                    m_polygon3DRegistInfoNum;
		UShort                    m_polygon3DRegistInfoAllocPower;
		UShort                    m_polygon3DRegistInfoAllocNum;
		CText2D::CRegistInfo**    m_text2DRegistInfos;
		UShort                    m_text2DRegistInfoNum;
		UShort                    m_text2DRegistInfoAllocPower;
		UShort                    m_text2DRegistInfoAllocNum;
		CText3D::CRegistInfo**    m_text3DRegistInfos;
		UShort                    m_text3DRegistInfoNum;
		UShort                    m_text3DRegistInfoAllocPower;
		UShort                    m_text3DRegistInfoAllocNum;
		CModel::CRegistInfo**     m_modelRegistInfos;
		UShort                    m_modelRegistInfoNum;
		UShort                    m_modelRegistInfoAllocPower;
		UShort                    m_modelRegistInfoAllocNum;
	};

	//========== [[[ �֐��錾 ]]]
	CDrawMgr                                ();
	~CDrawMgr                               ();
	void                     Init           (const UShort& priorityMax);
	void                     Uninit         (void);
	void                     Update         (void);
	void                     StartDraw      (Device& device);
	void                     EndDraw        (Device& device);
	void                     Release        (void);
	CPolygon2D::CRegistInfo* PutPolygon2D   (const UShort& priority, const bool& isOnScreen);
	CPolygon3D::CRegistInfo* PutPolygon3D   (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	CText2D::CRegistInfo*    PutText2D      (const UShort& priority, const Pos2D& pos, const float& angle, const bool& isOnScreen);
	CText3D::CRegistInfo*    PutText3D      (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	CModel::CRegistInfo*     PutModel       (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	const UShort&            GetPriorityMax (void) { return m_priorityMax; }
	UShort                   GetPolygon2DNum(void) { UShort num = 0; for (UShort cnt = 0; cnt < m_priorityMax; num += m_drawInfoSum[cnt].m_polygon2DNum, cnt++); return num; }
	UShort                   GetPolygon3DNum(void) { UShort num = 0; for (UShort cnt = 0; cnt < m_priorityMax; num += m_drawInfoSum[cnt].m_polygon3DNum, cnt++); return num; }
	UShort                   GetModelNum    (void) { UShort num = 0; for (UShort cnt = 0; cnt < m_priorityMax; num += m_drawInfoSum[cnt].m_modelNum    , cnt++); return num; }

private:
	//========== [[[ �֐��錾 ]]]
	void                     Draw                      (Device& device, CCamera* camera, const bool& isOnScreen);
	void                     PutBasedRegistInfo        (CRegistInfoSum& resistInfoSum, const UShort& priority, const bool& isOnScreen);
	void                     ExecutionDraw             (Device& device, CCamera* camera, CDrawInfoSum*& drawInfo, Matrix& viewMtx, const bool& isOnSreen);
	void                     AssignVertexInfo          (void);
	void                     ConvRegistInfoToDrawInfo  (CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum, Device& device);
	void                     ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum);
	void                     ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum);
	CPolygon2D::CRegistInfo* RegistPolygon2D           (CRegistInfoSum& resistInfo);
	CPolygon3D::CRegistInfo* RegistPolygon3D           (CRegistInfoSum& resistInfo);
	CText2D::CRegistInfo*    RegistText2D              (CRegistInfoSum& resistInfo);
	CText3D::CRegistInfo*    RegistText3D              (CRegistInfoSum& resistInfo);
	CModel::CRegistInfo*     RegistModel               (CRegistInfoSum& resistInfo);
	bool                     CheckPriority             (const UShort& priority, const bool& isOnScreen) { return (priority >= 0 && priority < (isOnScreen ? SCREEN_PRIORITY_MAX : m_priorityMax)); }

	//========== [[[ �ϐ��錾 ]]]
	CRegistInfoSum* m_resistInfoSum;
	CRegistInfoSum* m_resistInfoSumScreen;
	CDrawInfoSum*   m_drawInfoSum;				// �`����
	CDrawInfoSum*   m_drawInfoSumOvr;			// �`����(�㏑��)
	CDrawInfoSum*   m_drawInfoSumScreen;		// �X�N���[���`����
	CDrawInfoSum*   m_drawInfoSumScreenOvr;		// �X�N���[���`����(�㏑��)
	UShort          m_priorityMax;
	UShort          m_reAllocCount;
};
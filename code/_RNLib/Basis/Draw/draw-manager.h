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
class _RNC_DrawMgr {
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
	static const UShort SCREEN_PRIORITY_DEBUG_LOG  = 7;

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
		_RNC_Model::CDrawInfo**     m_model;
		UShort                      m_modelNum;
		_RNC_Polygon3D::CDrawInfo** m_polygon3D;
		UShort                      m_polygon3DNum;
		_RNC_Polygon2D::CDrawInfo** m_polygon2D;
		UShort                      m_polygon2DNum;
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
		_RNC_Polygon2D::CRegistInfo** m_polygon2DRegistInfos;
		UShort                        m_polygon2DRegistInfoNum;
		UShort                        m_polygon2DRegistInfoAllocPower;
		UShort                        m_polygon2DRegistInfoAllocNum;
		_RNC_Polygon3D::CRegistInfo** m_polygon3DRegistInfos;
		UShort                        m_polygon3DRegistInfoNum;
		UShort                        m_polygon3DRegistInfoAllocPower;
		UShort                        m_polygon3DRegistInfoAllocNum;
		_RNC_Text2D::CRegistInfo**    m_text2DRegistInfos;
		UShort                        m_text2DRegistInfoNum;
		UShort                        m_text2DRegistInfoAllocPower;
		UShort                        m_text2DRegistInfoAllocNum;
		_RNC_Text3D::CRegistInfo**    m_text3DRegistInfos;
		UShort                        m_text3DRegistInfoNum;
		UShort                        m_text3DRegistInfoAllocPower;
		UShort                        m_text3DRegistInfoAllocNum;
		_RNC_Model::CRegistInfo**     m_modelRegistInfos;
		UShort                        m_modelRegistInfoNum;
		UShort                        m_modelRegistInfoAllocPower;
		UShort                        m_modelRegistInfoAllocNum;
	};

	//========== [[[ �֐��錾 ]]]
	_RNC_DrawMgr                                    ();
	~_RNC_DrawMgr                                   ();
	void                         Init               (const UShort& priorityMax);
	void                         Uninit             (void);
	void                         Update             (void);
	void                         StartDraw          (Device& device);
	void                         EndDraw            (Device& device);
	void                         Release            (void);
	_RNC_Polygon2D::CRegistInfo* PutPolygon2D       (const UShort& priority, const bool& isOnScreen);
	_RNC_Polygon3D::CRegistInfo* PutPolygon3D       (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	_RNC_Text2D::CRegistInfo*    PutText2D          (const UShort& priority, const Pos2D& pos, const float& angle, const bool& isOnScreen);
	_RNC_Text3D::CRegistInfo*    PutText3D          (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	_RNC_Model::CRegistInfo*     PutModel           (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	void                         SetIsDrawPolygon2D (const bool& isOnScreen, const bool& isDraw) { m_isDrawPolygon2D [isOnScreen] = isDraw; }
	void                         SetIsDrawPolygon3D (const bool& isOnScreen, const bool& isDraw) { m_isDrawPolygon3D [isOnScreen] = isDraw; }
	void                         SetIsDrawModel     (const bool& isOnScreen, const bool& isDraw) { m_isDrawModel     [isOnScreen] = isDraw; }
	void                         SetIsDrawStaticMesh(const bool& isOnScreen, const bool& isDraw) { m_isDrawStaticMesh[isOnScreen] = isDraw; }
	bool&                        GetIsDrawPolygon2D (const bool& isOnScreen) { return m_isDrawPolygon2D [isOnScreen]; }
	bool&                        GetIsDrawPolygon3D (const bool& isOnScreen) { return m_isDrawPolygon3D [isOnScreen]; }
	bool&                        GetIsDrawModel     (const bool& isOnScreen) { return m_isDrawModel     [isOnScreen]; }
	bool&                        GetIsDrawStaticMesh(const bool& isOnScreen) { return m_isDrawStaticMesh[isOnScreen]; }
	const UShort&                GetPriorityMax     (void) { return m_priorityMax; }
	UShort                       GetPolygon2DNum    (void) { UShort num = 0; for (UShort cnt = 0; cnt < m_priorityMax; num += m_drawInfoSum[cnt].m_polygon2DNum, cnt++); return num; }
	UShort                       GetPolygon3DNum    (void) { UShort num = 0; for (UShort cnt = 0; cnt < m_priorityMax; num += m_drawInfoSum[cnt].m_polygon3DNum, cnt++); return num; }
	UShort                       GetModelNum        (void) { UShort num = 0; for (UShort cnt = 0; cnt < m_priorityMax; num += m_drawInfoSum[cnt].m_modelNum    , cnt++); return num; }

private:
	//========== [[[ �֐��錾 ]]]
	void                         Draw                      (Device& device, CCamera* camera, const bool& isOnScreen);
	void                         PutBasedRegistInfo        (CRegistInfoSum& resistInfoSum, const UShort& priority, const bool& isOnScreen);
	void                         ExecutionDraw             (Device& device, CCamera* camera, CDrawInfoSum*& drawInfo, Matrix& viewMtx, const bool& isOnSreen);
	void                         AssignVertexInfo          (void);
	void                         ConvRegistInfoToDrawInfo  (CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum, Device& device);
	void                         ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum);
	void                         ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum);
	_RNC_Polygon2D::CRegistInfo* RegistPolygon2D           (CRegistInfoSum& resistInfo);
	_RNC_Polygon3D::CRegistInfo* RegistPolygon3D           (CRegistInfoSum& resistInfo);
	_RNC_Text2D::CRegistInfo*    RegistText2D              (CRegistInfoSum& resistInfo);
	_RNC_Text3D::CRegistInfo*    RegistText3D              (CRegistInfoSum& resistInfo);
	_RNC_Model::CRegistInfo*     RegistModel               (CRegistInfoSum& resistInfo);
	bool                         CheckPriority             (const UShort& priority, const bool& isOnScreen) { return (priority >= 0 && priority < (isOnScreen ? SCREEN_PRIORITY_MAX : m_priorityMax)); }

	//========== [[[ �ϐ��錾 ]]]
	CRegistInfoSum* m_resistInfoSum;
	CRegistInfoSum* m_resistInfoSumScreen;
	CDrawInfoSum*   m_drawInfoSum;				// �`����
	CDrawInfoSum*   m_drawInfoSumOvr;			// �`����(�㏑��)
	CDrawInfoSum*   m_drawInfoSumScreen;		// �X�N���[���`����
	CDrawInfoSum*   m_drawInfoSumScreenOvr;		// �X�N���[���`����(�㏑��)
	bool            m_isDrawPolygon2D [2];
	bool            m_isDrawPolygon3D [2];
	bool            m_isDrawModel     [2];
	bool            m_isDrawStaticMesh[2];
	UShort          m_priorityMax;
	UShort          m_reAllocCount;
};
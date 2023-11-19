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
	static const UShort REGIST_ALLOC_BASE_POWER    = 8;	// 2�̉������Ƃ��邩
	static const UShort WAIT_MILLISECONDS_MIN      = 1;
	static const UShort WAIT_MILLISECONDS_MAX      = 5;

	//----------------------------------------
	// �񋓌^��`
	//----------------------------------------
	// �������
	enum class PROCESS_STATE {
		REGIST_ACCEPT,			// �o�^��t
		REGIST_INFO_APPLY_WAIT,	// �o�^���K�p�҂�
		DRAW_INFO_SWAP_WAIT,	// �`�������ւ��҂�
	};

	//----------------------------------------
	// �N���X��`
	//----------------------------------------
	// �`���񑍊��N���X
	class CDrawInfoSum {
	public:
		// [[[ �֐��錾 ]]]
		CDrawInfoSum();
		~CDrawInfoSum();
		void Release(void);
		void Overwrite(CDrawInfoSum* pOvr);

		// [[[ �ϐ��錾 ]]]
		CDrawInfoBase** m_drawInfos;
		int             m_drawInfoNum;
	};

	//----------------------------------------
	// �o�^�����
	//----------------------------------------
	// �o�^����񑍊�
	class CRegistInfoSum {
	public:
		// [[[ �֐��錾 ]]]
		CRegistInfoSum();
		~CRegistInfoSum();
		void InitAlloc(void);
		void ReAlloc(void);
		void Release(void);

		// [[[ �ϐ��錾 ]]]
		CPolygon2D::CRegistInfo** m_polygon2DRegistInfos;
		UShort                    m_polygon2DRegistInfoNum;
		UShort                    m_polygon2DRegistInfoAllocPower;
		UShort                    m_polygon2DRegistInfoAllocNum;
		UShort                    m_polygon2DDrawNum;
		CPolygon3D::CRegistInfo** m_polygon3DRegistInfos;
		UShort                    m_polygon3DRegistInfoNum;
		UShort                    m_polygon3DRegistInfoAllocPower;
		UShort                    m_polygon3DRegistInfoAllocNum;
		UShort                    m_polygon3DDrawNum;
		CText2D::CRegistInfo**    m_text2DRegistInfos;
		UShort                    m_text2DRegistInfoNum;
		UShort                    m_text2DRegistInfoAllocPower;
		UShort                    m_text2DRegistInfoAllocNum;
		UShort                    m_text2DDrawNum;
		CText3D::CRegistInfo**    m_text3DRegistInfos;
		UShort                    m_text3DRegistInfoNum;
		UShort                    m_text3DRegistInfoAllocPower;
		UShort                    m_text3DRegistInfoAllocNum;
		UShort                    m_text3DDrawNum;
		CModel::CRegistInfo**     m_modelRegistInfos;
		UShort                    m_modelRegistInfoNum;
		UShort                    m_modelRegistInfoAllocPower;
		UShort                    m_modelRegistInfoAllocNum;
		UShort                    m_modelDrawNum;
	};

	//========== [[[ �֐��錾 ]]]
	static PROCESS_STATE&  GetProcessState       (void) { return ms_processState; }
	static CRegistInfoSum& GetRegistInfoSum      (void) { return ms_resistInfoSum[0]; }
	static CRegistInfoSum& GetRegistInfoSumScreen(void) { return ms_resistInfoSumScreen[0]; }
	static CDrawInfoSum&   GetDrawInfoSum        (void) { return ms_drawInfoSum[0]; }
	static CDrawInfoSum&   GetDrawInfoSumScreen  (void) { return ms_drawInfoSumScreen[0]; }
	static const UShort&   GetPriorityMax        (void) { return ms_priorityMax; }
	CDrawMgr();
	~CDrawMgr();
	void Init(const UShort& priorityMax);
	void Uninit(void);
	void Update(void);
	void Release(void);
	UShort& GetWaitMilliseconds(void) { return m_waitMilliseconds; }
	void ResetWaitMilliseconds(void) { m_waitMilliseconds = WAIT_MILLISECONDS_MIN; }
	void StartRegistInfoApplyWait(void);
	bool StartDraw(void);
	void Draw(Device& device, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen);
	CPolygon2D::CRegistInfo* PutPolygon2D(const UShort& priority, const Pos3D& pos, const float& angle, const bool& isOnScreen);
	CPolygon2D::CRegistInfo* PutPolygon2D(const UShort& priority, const bool& isOnScreen);
	CPolygon3D::CRegistInfo* PutPolygon3D(const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	CText2D::CRegistInfo*    PutText2D   (const UShort& priority, const Pos3D& pos, const float& angle, const bool& isOnScreen);
	CText3D::CRegistInfo*    PutText3D   (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	CModel::CRegistInfo*     PutModel    (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);

private:
	//========== [[[ �֐��錾 ]]]
	static void MainLoop(void);
	static void PutBasedRegistInfo(CRegistInfoSum& resistInfoSum, const UShort& priority, const bool& isOnScreen);
	static void ConvRegistInfoToDrawInfo(CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum);
	void AssignVertexInfo(void);
	void ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum);
	void ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum);
	CPolygon2D::CRegistInfo* RegistPolygon2D(CRegistInfoSum& resistInfo);
	CPolygon3D::CRegistInfo* RegistPolygon3D(CRegistInfoSum& resistInfo);
	CText2D::CRegistInfo*    RegistText2D   (CRegistInfoSum& resistInfo);
	CText3D::CRegistInfo*    RegistText3D   (CRegistInfoSum& resistInfo);
	CModel::CRegistInfo*     RegistModel    (CRegistInfoSum& resistInfo);

	//========== [[[ �ϐ��錾 ]]]
	static PROCESS_STATE   ms_processState;
	static CRegistInfoSum* ms_resistInfoSum;
	static CRegistInfoSum* ms_resistInfoSumScreen;
	static CDrawInfoSum*   ms_drawInfoSum;				// �`����
	static CDrawInfoSum*   ms_drawInfoSumOvr;			// �`����(�㏑��)
	static CDrawInfoSum*   ms_drawInfoSumScreen;		// �X�N���[���`����
	static CDrawInfoSum*   ms_drawInfoSumScreenOvr;		// �X�N���[���`����(�㏑��)
	static std::thread     ms_mainLoopTh;				// ���C�����[�v�X���b�h
	static UShort          ms_priorityMax;
	UShort m_reAllocCount;
	UShort m_oldDrawFPS;
	UShort m_waitMilliseconds;
};
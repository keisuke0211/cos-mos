//========================================
// 
// 描画処理
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
// クラス定義
//****************************************
// 描画クラス
class CDrawMgr {
public:
	//----------------------------------------
	// 定数宣言
	//----------------------------------------
	static const UShort POLYGON2D_ALLOC_BASE_POWER = 8;
	static const UShort POLYGON3D_ALLOC_BASE_POWER = 8;
	static const UShort REGIST_ALLOC_BASE_POWER    = 8;	// 2の何乗を基準とするか
	static const UShort WAIT_MILLISECONDS_MIN      = 1;
	static const UShort WAIT_MILLISECONDS_MAX      = 5;

	//----------------------------------------
	// 列挙型定義
	//----------------------------------------
	// 処理状態
	enum class PROCESS_STATE {
		REGIST_ACCEPT,			// 登録受付
		REGIST_INFO_APPLY_WAIT,	// 登録情報適用待ち
		DRAW_INFO_SWAP_WAIT,	// 描画情報入れ替え待ち
	};

	//----------------------------------------
	// クラス定義
	//----------------------------------------
	// 描画情報総括クラス
	class CDrawInfoSum {
	public:
		// [[[ 関数宣言 ]]]
		CDrawInfoSum();
		~CDrawInfoSum();
		void Release(void);
		void Overwrite(CDrawInfoSum* pOvr);

		// [[[ 変数宣言 ]]]
		CDrawInfoBase** m_drawInfos;
		int             m_drawInfoNum;
	};

	//----------------------------------------
	// 登録情報情報
	//----------------------------------------
	// 登録情報情報総括
	class CRegistInfoSum {
	public:
		// [[[ 関数宣言 ]]]
		CRegistInfoSum();
		~CRegistInfoSum();
		void InitAlloc(void);
		void ReAlloc(void);
		void Release(void);

		// [[[ 変数宣言 ]]]
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

	//========== [[[ 関数宣言 ]]]
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
	//========== [[[ 関数宣言 ]]]
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

	//========== [[[ 変数宣言 ]]]
	static PROCESS_STATE   ms_processState;
	static CRegistInfoSum* ms_resistInfoSum;
	static CRegistInfoSum* ms_resistInfoSumScreen;
	static CDrawInfoSum*   ms_drawInfoSum;				// 描画情報
	static CDrawInfoSum*   ms_drawInfoSumOvr;			// 描画情報(上書き)
	static CDrawInfoSum*   ms_drawInfoSumScreen;		// スクリーン描画情報
	static CDrawInfoSum*   ms_drawInfoSumScreenOvr;		// スクリーン描画情報(上書き)
	static std::thread     ms_mainLoopTh;				// メインループスレッド
	static UShort          ms_priorityMax;
	UShort m_reAllocCount;
	UShort m_oldDrawFPS;
	UShort m_waitMilliseconds;
};
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
		CDrawInfoBase** ms_drawInfos;
		int             ms_drawInfoNum;
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
		CPolygon2D::CRegistInfo* m_polygon2DRegistInfos;
		UShort                   m_polygon2DRegistInfoNum;
		UShort                   m_polygon2DRegistInfoAllocPower;
		UShort                   m_polygon2DRegistInfoAllocNum;
		CPolygon3D::CRegistInfo* m_polygon3DRegistInfos;
		UShort                   m_polygon3DRegistInfoNum;
		UShort                   m_polygon3DRegistInfoAllocPower;
		UShort                   m_polygon3DRegistInfoAllocNum;
		CText2D::CRegistInfo*    m_text2DRegistInfos;
		UShort                   m_text2DRegistInfoNum;
		UShort                   m_text2DRegistInfoAllocPower;
		UShort                   m_text2DRegistInfoAllocNum;
		CText3D::CRegistInfo*    m_text3DRegistInfos;
		UShort                   m_text3DRegistInfoNum;
		UShort                   m_text3DRegistInfoAllocPower;
		UShort                   m_text3DRegistInfoAllocNum;
		CModel::CRegistInfo*     m_modelRegistInfos;
		UShort                   m_modelRegistInfoNum;
		UShort                   m_modelRegistInfoAllocPower;
		UShort                   m_modelRegistInfoAllocNum;
	};

	//========== [[[ 関数宣言 ]]]
	static PROCESS_STATE GetProcessState(void) { return ms_processState; }
	CDrawMgr();
	~CDrawMgr();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Release(void);
	bool StartDraw(void);
	void Draw(Device& device, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen);
	CPolygon2D::CRegistInfo* PutPolygon2D(const D3DXVECTOR3& pos, const float& angle, const bool& isOnScreen);
	CPolygon3D::CRegistInfo* PutPolygon3D(const Matrix& mtx, const bool& isOnScreen);
	CText2D::CRegistInfo*    PutText2D   (const D3DXVECTOR3& pos, const float& angle, const bool& isOnScreen);
	CText3D::CRegistInfo*    PutText3D   (const Matrix& mtx, const bool& isOnScreen);
	CModel::CRegistInfo*     PutModel    (const Matrix& mtx, const bool& isOnScreen);

private:
	//========== [[[ 構造体定義 ]]]
	// カメラ描画順情報
	struct CameraDrawOrderInfo {
		short    ID        = 0;
		UShort*  orderList = NULL;
		Pos3D    posV      = INITPOS3D;
		Vector3D nor       = INITVECTOR3D;
	};
	struct CameraDrawOrderInfoSum {
		CameraDrawOrderInfo* cameraDrawOrderInfos   = NULL;
		UShort               cameraDrawOrderInfoNum = 0;
	};

	//========== [[[ 関数宣言 ]]]
	static void MainLoop(void);
	static void PutBasedRegistInfo(CRegistInfoSum& resistInfoSum, const bool& isOnScreen);
	static void ConvRegistInfoToDrawInfo(CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum);
	static void SortDrawInfo(CDrawInfoSum& drawInfoSum, CameraDrawOrderInfo& cameraDrawOrderInfo);
	void AssignVertexInfo(void);
	void ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum);
	void ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum);
	CPolygon2D::CRegistInfo* RegistPolygon2D(CRegistInfoSum& resistInfo);
	CPolygon3D::CRegistInfo* RegistPolygon3D(CRegistInfoSum& resistInfo);
	CText2D::CRegistInfo*    RegistText2D   (CRegistInfoSum& resistInfo);
	CText3D::CRegistInfo*    RegistText3D   (CRegistInfoSum& resistInfo);
	CModel::CRegistInfo*     RegistModel    (CRegistInfoSum& resistInfo);

	//========== [[[ 変数宣言 ]]]
	static PROCESS_STATE          ms_processState;
	static CRegistInfoSum         ms_resistInfoSum;
	static CRegistInfoSum         ms_resistInfoSumScreen;
	static CDrawInfoSum           ms_drawInfoSum;				// 描画情報
	static CDrawInfoSum           ms_drawInfoSumOvr;			// 描画情報(上書き)
	static CDrawInfoSum           ms_drawInfoSumScreen;			// スクリーン描画情報
	static CDrawInfoSum           ms_drawInfoSumScreenOvr;		// スクリーン描画情報(上書き)
	static CameraDrawOrderInfoSum ms_cameraDrawOrderInfoSum;	// カメラの描画順情報
	static CameraDrawOrderInfoSum ms_cameraDrawOrderInfoSumOvr;	// カメラの描画順情報(上書き)
	static std::thread            ms_mainLoopTh;				// メインループスレッド
	UShort m_reAllocCount;
};
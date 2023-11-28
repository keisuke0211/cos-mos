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
		CModel::CDrawInfo**     m_model;
		int                     m_modelNum;
		CPolygon3D::CDrawInfo** m_polygon3D;
		int                     m_polygon3DNum;
		CPolygon2D::CDrawInfo** m_polygon2D;
		int                     m_polygon2DNum;
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
	static const UShort& GetPriorityMax (void) { return ms_priorityMax; }
	static UInt          GetPolygon2DNum(void) { UInt num = 0; for (int cnt = 0; cnt < ms_priorityMax; num += ms_drawInfoSum[cnt].m_polygon2DNum, cnt++); return num; }
	static UInt          GetPolygon3DNum(void) { UInt num = 0; for (int cnt = 0; cnt < ms_priorityMax; num += ms_drawInfoSum[cnt].m_polygon3DNum, cnt++); return num; }
	static UInt          GetModelNum    (void) { UInt num = 0; for (int cnt = 0; cnt < ms_priorityMax; num += ms_drawInfoSum[cnt].m_modelNum    , cnt++); return num; }
	static void StartDraw(Device& device);
	static void EndDraw(Device& device);
	CDrawMgr();
	~CDrawMgr();
	void Init(const UShort& priorityMax);
	void Uninit(void);
	void Update(void);
	void Release(void);
	CPolygon2D::CRegistInfo* PutPolygon2D(const UShort& priority, const bool& isOnScreen);
	CPolygon3D::CRegistInfo* PutPolygon3D(const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	CText2D::CRegistInfo*    PutText2D   (const UShort& priority, const Pos2D& pos, const float& angle, const bool& isOnScreen);
	CText3D::CRegistInfo*    PutText3D   (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);
	CModel::CRegistInfo*     PutModel    (const UShort& priority, const Matrix& mtx, const bool& isOnScreen);

private:
	//========== [[[ 関数宣言 ]]]
	static void PutBasedRegistInfo(CRegistInfoSum& resistInfoSum, const UShort& priority, const bool& isOnScreen);
	static void ConvRegistInfoToDrawInfo(CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum, Device& device);
	static void Draw(Device& device, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen);
	static void ExecutionDraw(Device& device, const short& cameraID, const bool& isCameraClipping, CDrawInfoSum*& drawInfo, Matrix& viewMtx);
	static void AssignVertexInfo(void);
	static void ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum);
	static void ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum);
	CPolygon2D::CRegistInfo* RegistPolygon2D(CRegistInfoSum& resistInfo);
	CPolygon3D::CRegistInfo* RegistPolygon3D(CRegistInfoSum& resistInfo);
	CText2D::CRegistInfo*    RegistText2D   (CRegistInfoSum& resistInfo);
	CText3D::CRegistInfo*    RegistText3D   (CRegistInfoSum& resistInfo);
	CModel::CRegistInfo*     RegistModel    (CRegistInfoSum& resistInfo);

	//========== [[[ 変数宣言 ]]]
	static CRegistInfoSum* ms_resistInfoSum;
	static CRegistInfoSum* ms_resistInfoSumScreen;
	static CDrawInfoSum*   ms_drawInfoSum;				// 描画情報
	static CDrawInfoSum*   ms_drawInfoSumOvr;			// 描画情報(上書き)
	static CDrawInfoSum*   ms_drawInfoSumScreen;		// スクリーン描画情報
	static CDrawInfoSum*   ms_drawInfoSumScreenOvr;		// スクリーン描画情報(上書き)
	static UShort          ms_priorityMax;
	UShort m_reAllocCount;
};
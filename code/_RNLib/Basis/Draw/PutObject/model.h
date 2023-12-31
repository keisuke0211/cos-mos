//========================================
// 
// モデルの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../camera.h"
#include "../draw-info.h"
#include "../draw-state.h"
#include "../regist-info.h"
#include "../../Mechanical/regist.h"

//****************************************
// クラス定義
//****************************************
// モデルクラス
class _RNC_Model : public CRegist {
public:
	//========== [[[ 構造体定義 ]]]
	struct Vertex3DInfo {
		Pos3D    pos;
		Pos3D    worldPos;
		Rot3D    rot;
		Rot3D    worldRot;
		Normal3D nor;
		Normal3D worldNor;
		Pos2D    texPos;
	};

	//========== [[[ クラス定義 ]]]
	// データクラス
	class CData {
	public:
		// [[[ 構造体定義 ]]]
		struct VtxData {
			bool* isMats = NULL;
		};
		struct MatData {
			Color  col    = COLOR_WHITE;
			ULong  vtxNum = 0;
			ULong* idxes  = NULL;
			ULong  idxNum = 0;
		};

		// [[[ 関数宣言 ]]]
		CData();
		~CData();
		void Release(void);

		// [[[ 変数宣言 ]]]
		short*   m_texIdxes;
		Texture* m_texes;
		VtxData* m_vtxDatas;
		ULong    m_vtxNum;
		ULong*   m_idxes;
		ULong    m_idxNum;
		Buffer   m_matBuff;
		MatData* m_matDatas;
		UShort   m_matNum;
		UShort*  m_faceMatIdxes;
		ULong    m_faceNum;
		Mesh     m_mesh;
		Mesh*    m_outLineMeshs;
		float    m_radiusMax;
	};

	// 描画情報クラス
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ 関数宣言 ]]]
		CDrawInfo();
		~CDrawInfo();

		// [[[ 変数宣言 ]]]
		static Material    ms_outLineMat;
		Matrix             m_mtx;
		Material*          m_mats;
		Texture*           m_texes;
		UShort             m_matNum;
		Mesh               m_mesh;
		Mesh               m_outLineMesh;
		bool               m_isScaling;
		bool               m_isZTest;
		bool               m_isLighting;
		INTERPOLATION_MODE m_interpolationMode;
		float              m_radiusMax;
	};

	// 登録情報クラス
	class CRegistInfo : public CRegistInfoBase {
	public:
		// [[[ 関数宣言 ]]]
		CRegistInfo();
		~CRegistInfo();
		CRegistInfo* SetClippingCamera      (CCamera& camera); 
		CRegistInfo* SetClippingCamera      (const short& ID);
		CRegistInfo* SetCol                 (const Color& col);
		CRegistInfo* SetTex                 (const short& texIdx);
		CRegistInfo* SetZTest               (const bool& isZTest);
		CRegistInfo* SetLighting            (const bool& isLighting);
		CRegistInfo* SetOutLineIdx          (const UShort& outLineIdx);
		CRegistInfo* SetBrightnessOfEmissive(const float& brightnessOfEmissive);
		CRegistInfo* SetInterpolationMode   (const _RNC_DrawState::INTERPOLATION_MODE& interpolationMode);

	private:
		// [[[ 友達宣言 ]]]
		friend class _RNC_DrawMgr;
		friend class _RNC_Model;

		// [[[ 関数宣言 ]]]
		void         ClearParameter(void);
		CDrawInfo*   ConvToDrawInfo(Device& device);
		CRegistInfo* SetModel      (const short& modelIdx);
		CRegistInfo* SetMtx        (const Matrix& mtx);

		// <<< 基本情報 >>>
		Matrix             m_mtx;
		short              m_modelIdx;

		// <<< 見た目情報 >>>
		Color              m_col;
		short              m_texIdx;
		short              m_outLineIdx;
		float              m_brightnessOfEmissive;

		// <<< 描画情報設定 >>>
		bool               m_isZTest;
		bool               m_isLighting;
		INTERPOLATION_MODE m_interpolationMode;
	};

	//========== [[[ 関数宣言 ]]]
	_RNC_Model               ();
	~_RNC_Model              ();
	void         Init        (void);
	void         Uninit      (void);
	void         Update      (void);
	void         Release     (void);
	short        Load        (const char* loadPath, short idx = NONEDATA);
	CData&       GetData     (const short& idx) { return *m_datas[idx]; }
	void         StoreVtxInfo(const Matrix& modelMtx, const short& modelIdx, Vertex3DInfo** vtxInfos, const short& matIdx = NONEDATA);
	CRegistInfo* Put         (const UShort& priority, const short& modelIdx, const Matrix& mtx,                                        const bool& isOnScreen = false);
	CRegistInfo* Put         (const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot,                       const bool& isOnScreen = false);
	CRegistInfo* Put         (const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const bool& isOnScreen = false);

private:
	//========== [[[ 関数宣言 ]]]
	void ExecutionStoreVtxInfo(Vertex3DInfo* vtx, const Matrix& modelMtx, const ULong& cntVtx, BYTE*& vtxBuff, const DWORD& dwSizeFVF, const DWORD& texCoordOffset);

	//========== [[[ 変数宣言 ]]]
	CData** m_datas;
};
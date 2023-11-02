//========================================
// 
// モデルの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../draw-info.h"
#include "../../Mechanical/regist.h"

//****************************************
// クラス定義
//****************************************
// モデルクラス
class CModel : public CRegist {
public:
	//========== [[[ クラス定義 ]]]
	// データクラス
	class CData {
	public:
		// [[[ 関数宣言 ]]]
		CData();
		~CData();
		void Release(void);

		// [[[ 変数宣言 ]]]
		short*         m_texIdxs;
		LPD3DXMESH     m_mesh;
		LPD3DXMESH     m_outLineMesh;
		LPD3DXBUFFER   m_matBuff;
		unsigned short m_matNum;
		float          m_radiusMax;
	};

	// 描画情報クラス
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ 関数宣言 ]]]
		CDrawInfo();
		~CDrawInfo();
		void Draw       (Device& device, const Matrix& viewMtx);
		void SetMaterial(Device& device, Material* mat, const Color& col);

		// [[[ 変数宣言 ]]]
		Matrix m_mtx;
		Color  m_col;
		short  m_modelIdx;
		short  m_texIdx;
		bool   m_isZTest;
		bool   m_isLighting;
		bool   m_isOutLine;
		float  m_brightnessOfEmissive;
		float  m_distance;
	};

	// 登録情報クラス
	class CRegistInfo {
	public:
		// [[[ 関数宣言 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		CDrawInfo* ConvToDrawInfo(void);
		CRegistInfo* SetMtx                 (const Matrix& mtx);
		CRegistInfo* SetCol                 (const Color& col);
		CRegistInfo* SetModel               (const short& modelIdx);
		CRegistInfo* SetTex                 (const short& texIdx);
		CRegistInfo* SetZTest               (const bool& isZTest);
		CRegistInfo* SetLighting            (const bool& isLighting);
		CRegistInfo* SetOutLine             (const bool& isOutLine);
		CRegistInfo* SetBrightnessOfEmissive(const float& brightnessOfEmissive);
		CRegistInfo* SetPriority            (const short& priority);

	private:
		// [[[ 変数宣言 ]]]
		Matrix m_mtx;
		Color  m_col;
		short  m_modelIdx;
		short  m_texIdx;
		bool   m_isZTest;
		bool   m_isLighting;
		bool   m_isOutLine;
		float  m_brightnessOfEmissive;
		short  m_priority;
	};

	//========== [[[ 関数宣言 ]]]
	CModel();
	~CModel();
	void         Release(void);
	short        Load   (const char* loadPath, short idx = NONEDATA);
	CData&       GetData(const short& idx) { return m_datas[idx]; }
	CRegistInfo* Put    (const Matrix& mtx, const short& modelIdx, const bool& isOnScreen = false);
	CRegistInfo* Put    (const Pos3D& pos, const Rot3D& rot, const short& modelIdx, const bool& isOnScreen = false);
	CRegistInfo* Put    (const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const short& modelIdx, const bool& isOnScreen = false);

private:
	//========== [[[ 変数宣言 ]]]
	CData* m_datas;
};
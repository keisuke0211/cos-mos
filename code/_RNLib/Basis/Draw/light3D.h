//========================================
// 
// ライト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/string.h"
#include "../Mechanical/object.h"

//****************************************
// クラス定義
//****************************************
// ライト3Dクラス
class CLight3D : CObject {
public:
	//========== [[[ 構造体定義 ]]]
	// 環境光情報構造体
	struct LinearLight {
		Matrix rotMtx = INITMATRIX;
		Color  col    = COLOR_WHITE;
	};
	// フォグ情報
	struct FogInfo {
		UShort start = 500;
		UShort end   = 500;
	};

	//========== [[[ 関数宣言 ]]]
	static void AllDisable(Device& device);
	CLight3D                      (const String& loadPath);
	~CLight3D                     ();
	void         Clear            (void);
	void         Update           (void) {}
	void         Load             (const String& loadPath);
	void         Save             (const String& savePath);
	void         Setting          (Device& device, const Color* ovrCol);
	short&       GetID            (void)              { return m_ID; }
	void         SetBGCol         (Color& col)        { m_BGCol = col; }
	Color&       GetBGCol         (void)              { return m_BGCol; }
	LinearLight& GetLinearLight   (const UShort idx)  { return m_linearLights[idx]; }
	void         AddLinearLight   (void);
	void         SubLinearLight   (const UShort& idx);
	UShort&      GetLinearLightNum(void)              { return m_linearLightNum; }
	void         SetIsFog         (const bool& isFog) { m_isFog = isFog; }
	bool&        GetIsFog         (void)              { return m_isFog; }
	FogInfo&     GetFogInfo       (void)              { return m_fogInfo; }
	Rot3D&       GetRot           (void)              { return m_rot; }
	void         SetRot           (const Rot3D& rot)  { m_rot = rot; }

private:
	//========== [[[ 変数宣言 ]]]
	static short  ms_IDCount;
	static UShort ms_linearLightNumOld;
	short         m_ID;
	Color         m_BGCol;
	LinearLight*  m_linearLights;
	UShort        m_linearLightNum;
	bool          m_isFog;
	FogInfo       m_fogInfo;
	Rot3D         m_rot;
	Color         m_col;
};
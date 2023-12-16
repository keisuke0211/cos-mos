//========================================
// 
// セットアップ3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../Mechanical/regist.h"
#include "../../../RNmain.h"
#include "../../Draw/color.h"

//****************************************
// クラス定義
//****************************************
// セットアップ3Dクラス
class _RNC_SetUp3D : public CRegist {
public:
	//========== [[[ 構造体定義 ]]]
	// ボーンデータ構造体
	struct Swaying {
		UShort timeMin = 0;
		UShort timeAdd = 0;
		float  distMin = 0.0f;
		float  distAdd = 0.0f;
	};
	struct Follow {
		short    followIdx = NONEDATA;
		Vector3D posRate   = INITVECTOR3D;
		Vector3D rotRate   = INITVECTOR3D;
		Vector3D scaleRate = INITVECTOR3D;
	};
	struct BoneData {
		short    idx         = NONEDATA;
		short    modelIdx    = NONEDATA;
		short    parentIdx   = NONEDATA;
		Pos3D    relativePos = INITPOS3D;
		Pos3D    relativeRot = INITPOS3D;
		Swaying* swaying     = NULL;
		Follow*  follow      = NULL;
	};
	// フェイスデータ構造体
	struct FaceVtxData {
		UShort boneIdx = NONEDATA;
		UInt   vtxIdx  = NONEDATA;
		Pos2D  texPos  = INITPOS2D;
	};
	struct FaceData {
		FaceVtxData vtxs[4] = {};
		Color       col     = INITCOLOR;
		short       texIdx  = NONEDATA;
	};

	// データ構造体
	class CData {
	public:
		// [[[ 関数宣言 ]]]
		CData();
		~CData();
		void Release(void);

		// [[[ 変数宣言 ]]]
		BoneData* m_boneDatas;
		short     m_boneDataNum;
		FaceData* m_faceDatas;
		short     m_faceDataNum;
	};

	//========== [[[ 関数宣言 ]]]
	_RNC_SetUp3D       ();
	~_RNC_SetUp3D      ();
	void   Init        (void);
	void   Uninit      (void);
	void   Update      (void);
	short  Load        (const char* loadPath, short idx = NONEDATA);
	bool   LoadEditData(const char* loadPath);
	void   SaveEditData(const char* savePath);
	CData& GetData     (const short& idx) { return idx == EDITDATA ? *m_editData : (idx == NONEDATA ? *(CData*)(nullptr) : *m_datas[idx]); }
	void   InitMemory  (const UShort& num);

private:
	//========== [[[ 関数宣言 ]]]
	bool ExecutionLoad(const char* loadPath, CData& data);

	//========== [[[ 変数宣言 ]]]
	CData** m_datas;
	CData*  m_editData;
};
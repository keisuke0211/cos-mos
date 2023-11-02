//========================================
// 
// セットアップ3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../Mechanical/regist.h"
#include "../../../RNmain.h"

//****************************************
// クラス定義
//****************************************
// セットアップ3Dクラス
class CSetUp3D : public CRegist {
public:
	//========== [[[ 構造体定義 ]]]
	// ボーンデータ構造体
	struct BoneData {
		short idx         = NONEDATA;
		short modelIdx    = NONEDATA;
		short parentIdx   = NONEDATA;
		Pos3D relativePos = INITPOS3D;
		Pos3D relativeRot = INITPOS3D;
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
	};

	//========== [[[ 関数宣言 ]]]
	CSetUp3D();
	~CSetUp3D();
	short  Load      (const char* loadPath, short idx = NONEDATA);
	void   Save      (const char* savePath, short idx);
	CData& GetData   (const short& idx) { return m_datas[idx]; }
	void   InitMemory(const unsigned short& num);

private:
	//========== [[[ 変数宣言 ]]]
	CData* m_datas;
};
//========================================
// 
// モーション3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "setup3D.h"
#include "../../Mechanical/regist.h"

//****************************************
// クラス定義
//****************************************
class CMotion3D : public CRegist {
public:
	//========== [[[ 列挙型定義 ]]]
	// コマンド
	enum class COMMAND {
		NONE = -1,
		MOVE,		// 移動
		SPIN,		// 回転
		SCALING,	// 拡縮
		STEP,		// 足踏
		MAX,
	};

	//========== [[[ 構造体定義 ]]]
	// コマンド構造体
	struct CommandData {
		short   time    = 0;
		COMMAND command = (COMMAND)0;
		float*  datas   = NULL;
	};

	// ボーンモーションデータ構造体
	struct BoneMotionData {
		bool         isMove         = false;
		bool         isSpin         = false;
		bool         isScale        = false;
		UShort       commandDataNum = 0;
		CommandData* commandDatas   = NULL;
	};

	// データ構造体
	class CData {
	public:
		// [[[ 関数宣言 ]]]
		CData();
		~CData();
		void Release(void);

		// [[[ 変数宣言 ]]]
		short           loopTime        = 0;
		bool            isLoop          = false;
		UShort          boneNum         = 0;
		BoneMotionData* boneMotionDatas = NULL;
	};

	//========== [[[ 関数宣言 ]]]
	CMotion3D();
	~CMotion3D();
	short  Load      (const char* loadPath, short idx = NONEDATA);
	void   Save      (const char* savePath, const short& idx);
	CData& GetData   (const short& idx) { return m_datas[idx]; }
	void   InitMemory(const UShort& num);

	//========== [[[ 変数宣言 ]]]
	CData* m_datas;

private:
	//========== [[[ 定数宣言 ]]]
	static const UShort COMMAND_DATA_NUM[(int)COMMAND::MAX];
};
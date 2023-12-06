//========================================
// 
// モーション3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "setup3D.h"
#include "../../Mechanical/regist.h"
#include "../../Calculation/ease.h"

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
		MAX,
	};

	//========== [[[ 構造体定義 ]]]
	// コマンド構造体
	struct CommandData_Move {
		UShort      time;
		Pos3D       targetPos;
		CEase::TYPE easeType;
	};
	struct CommandData_Spin {
		UShort      time;
		Rot3D       targetRot;
		CEase::TYPE easeType;
	};
	struct CommandData_Scaling {
		UShort      time;
		Scale3D     targetScale;
		CEase::TYPE easeType;
	};
	struct CommandData {
		short   time    = 0;
		COMMAND command = COMMAND::NONE;
		void*   data    = NULL;
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
		CData       ();
		~CData      ();
		void Release(void);

		// [[[ 変数宣言 ]]]
		short           loopTime        = 0;
		bool            isLoop          = false;
		UShort          boneNum         = 0;
		BoneMotionData* boneMotionDatas = NULL;
	};

	//========== [[[ 関数宣言 ]]]
	CMotion3D          ();
	~CMotion3D         ();
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
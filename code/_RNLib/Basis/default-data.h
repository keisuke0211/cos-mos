//========================================
// 
// デフォルトデータの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// デフォルトデータクラス
class CDefaultData {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class MODEL { PRUFEN_HEAD, MAX, };

	//========== [[[ 関数宣言 ]]]
	CDefaultData();
	~CDefaultData();
	void Init(void);
	UShort& GetModelIdx(const MODEL& model) { return m_modelIdxes[(int)model]; }

private:
	//========== [[[ 定数宣言 ]]]
	static const char* MODEL_PATH[(int)MODEL::MAX];

	//========== [[[ 変数宣言 ]]]
	UShort m_modelIdxes[(int)MODEL::MAX];
};

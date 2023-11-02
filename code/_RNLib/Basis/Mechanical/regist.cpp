//========================================
// 
// 登録処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNLib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]登録クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CRegist::CRegist() {

	m_num       = 0;
	m_loadPaths = NULL;
}

//========================================
// デストラクタ
//========================================
CRegist::~CRegist() {

	// 読み込みパスの解放
	for (int nCnt = 0; nCnt < m_num; nCnt++)
		RNLib::Memory().Release(&m_loadPaths[nCnt]);
	RNLib::Memory().Release(&m_loadPaths);
}

//========================================
// 読み込み処理
//========================================
bool CRegist::Load(const char* loadPath, short& idx) {

	// パス無し or DATANONE指定の時、
	// 番号をDATANONE指定して読み中止
	if ((!strcmp(loadPath, "")) || (!strcmp(loadPath, "DATANONE"))) {
		idx = NONEDATA;
		return false;
	}

	// 番号指定がある時、
	if (idx != NONEDATA) {

		// 番号が範囲内であれば、
		// パスを保存して読み込み開始
		if (idx >= 0 && idx < m_num) {
			sprintf(m_loadPaths[idx], "%s", loadPath);
			return true;
		}
		 
		// 番号が範囲外であれば、
		// 番号をDATANONE指定して読み込み中止
		assert(false);
		idx = NONEDATA;

		return false;
	}

	// 既に読み込まれているか確認
	for (int cnt = 0; cnt < m_num; cnt++) {

		// 既に同じパスを読み込んでいた時、
		// 番号をカウントで指定して読み込み中止
		if (!strcmp(loadPath, m_loadPaths[cnt])) {
			idx = cnt;
			return false;
		}
	}

	// 過去の数 兼 番号
	int numOld = m_num++;

	{// 読み込み済みパスのメモリ再確保
		char** newLoadPaths = new char*[m_num];

		// 過去数が0を越えている時、メモリをコピー
		if (numOld > 0) {
			memcpy(newLoadPaths, m_loadPaths, sizeof(char*) * numOld);
			delete m_loadPaths;
		}

		// 読み込みパスのポインタを入れ替える
		m_loadPaths = newLoadPaths;
		m_loadPaths[numOld] = new char[TXT_MAX];
	}

	// パスを保存
	sprintf(m_loadPaths[numOld], "%s", loadPath);

	// 番号を指定
	idx = numOld;

	// 読み込み開始
	return true;
}

//========================================
// メモリ初期化処理
//========================================
void CRegist::InitMemory(const unsigned short& num) {

	// 数を代入
	m_num = num;

	{// 読み込み済パスのメモリ確保
		char** newLoadPaths = new char*[m_num];

		// 読み込みパスの解放
		for (int nCnt = 0; nCnt < m_num; nCnt++)
			RNLib::Memory().Release<char>(&m_loadPaths[nCnt]);
		RNLib::Memory().Release<char*>(&m_loadPaths);

		// 読み込みパスのポインタを入れ替える
		m_loadPaths = newLoadPaths;

		// 文字列メモリ確保
		for (int nCnt = 0; nCnt < m_num; nCnt++) {
			m_loadPaths[nCnt] = new char[TXT_MAX];
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]登録クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 読み込み済パスのメモリリセット処理
//========================================
void CRegist::ReAllocLoadPath(const short& oldNum) {

	for (int cnt = oldNum; cnt < m_num; cnt++) {
		RNLib::Memory().Release(&m_loadPaths[cnt]);
	}
	RNLib::Memory().ReAlloc(&m_loadPaths, m_num, oldNum);
}
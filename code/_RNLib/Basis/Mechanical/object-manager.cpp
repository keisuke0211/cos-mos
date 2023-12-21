//========================================
// 
// オブジェクトマネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]オブジェクトマネージャークラス
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的メンバ変数定義
//****************************************
CObjectMgr** CObjectMgr::ms_objectMgrs = NULL;
UShort CObjectMgr::ms_objectMgrNum = NULL;

//========================================
// [静的] 全マネージャー更新処理
//========================================
void CObjectMgr::UpdateAllMgrs(void) {

	// 全マネージャーの全更新処理と更新処理を行う
	for (int cntObjectMgr = 0; cntObjectMgr < ms_objectMgrNum; cntObjectMgr++) {
		ms_objectMgrs[cntObjectMgr]->UpdateAll();
		ms_objectMgrs[cntObjectMgr]->Update();
	}
}

//========================================
// [静的] 全マネージャー解放処理
//========================================
void CObjectMgr::ReleaseAllMgrs(void) {

	// 全マネージャーの全更新処理と更新処理を行う
	for (int cntObjectMgr = 0; cntObjectMgr < ms_objectMgrNum; cntObjectMgr++) {
		ms_objectMgrs[cntObjectMgr]->DeleteAll();
	}
}

//========================================
// [静的] マネージャーを排除
//========================================
void CObjectMgr::SubMgr(CObjectMgr* mgr) {

	bool isSub = false;
	for (int cntObjectMgr = 0; cntObjectMgr < ms_objectMgrNum; cntObjectMgr++) {
		if (isSub) 
		{// 排除し終えた
			// 1つ前にずらす
			ms_objectMgrs[cntObjectMgr - 1] = ms_objectMgrs[cntObjectMgr];
			ms_objectMgrs[cntObjectMgr] = NULL;
		}
		else 
		{// 排除してない
			if (mgr == ms_objectMgrs[cntObjectMgr]) 
			{// カウントと対象アドレスが一致した時、
				// 排除フラグを真にする
				isSub = true;
			}
		}
	}

	// 排除出来た時、総数を減らしメモリを再確保
	if (isSub) {
		ms_objectMgrNum--;
		RNLib::Memory().ReAlloc(&ms_objectMgrs, ms_objectMgrNum + 1, ms_objectMgrNum);
	}
}

//========================================
// コンストラクタ
//========================================
CObjectMgr::CObjectMgr(const char* name) {

	{// オブジェクトマネージャー列に追加する
		int numOld = ms_objectMgrNum++;
		RNLib::Memory().ReAlloc<CObjectMgr*>(&ms_objectMgrs, numOld, ms_objectMgrNum, false);
		ms_objectMgrs[numOld] = this;
	}

	m_name = NULL;
	RNLib::Memory().AllocString(&m_name, name);
	m_top  = NULL;
	m_cur  = NULL;
	m_num  = 0;
}

//========================================
// デストラクタ
//========================================
CObjectMgr::~CObjectMgr(void) {

	// 全開放処理
	CObjectMgr::DeleteAll();

	// 自身をマネージャー列から排除
	SubMgr(this);

	// 名前の解放
	RNLib::Memory().Release(&m_name);
}

//========================================
// 全削除処理
//========================================
void CObjectMgr::DeleteAll(void) {

	// 先頭から順番に最後尾まで削除フラグを立たせる
	// (※継承先の要素は解放されるが、
	//    本体は解放せず、死亡フラグを立たせる)
	CObject* obj = NULL;
	while (ListLoop(&obj)) {

		obj->Delete();
	}

	// 削除フラグが立っているオブジェクトを解放する
	ReleaseDeleteObj();
}

//========================================
// 一括更新処理
//========================================
void CObjectMgr::UpdateAll(void) {

	// 先頭から順番に最後尾まで更新していく
	CObject* pList = NULL;
	while (ListLoop(&pList)) {

		pList->Update();
	}

	// 削除フラグが立っているオブジェクトを解放する
	ReleaseDeleteObj();
}

//========================================
// リストに追加
//========================================
void CObjectMgr::AddList(CObject* obj) {

	// 既にリストに追加している場合終了
	if (obj->GetIsAdd()) {
		return;
	}
	else {
		obj->SetIsAdd(true);
	}

	if (m_cur == NULL)
	{// 最後尾が存在しない(※まだ1つもない)時、
		// 先頭と最後尾を新規に設定する
		m_top = obj;
		m_cur = obj;

		obj->SetPrev(NULL);	// 前 無し
		obj->SetNext(NULL);	// 次 無し
	}
	else
	{// 最後尾が存在する時、
		// 元最後尾の次を新規に設定する
		m_cur->SetNext(obj);

		obj->SetPrev(m_cur);	// 前 元最後尾
		obj->SetNext(NULL);		// 次 無し

		// 最後尾を新規に設定する
		m_cur = obj;
	}

	m_num++;
}

//========================================
// リストから排除
//========================================
void CObjectMgr::SubList(CObject* obj) {

	// 既にリストから削除している場合終了
	if (obj->GetIsSub()) {
		return;
	}
	else {
		obj->SetIsSub(true);
	}

	if (m_top == obj)
	{// 自分が先頭の時、
		// 先頭を次に設定する
		m_top = obj->GetNext();

		// 先頭の前を無しにする
		if (m_top != NULL) {
			m_top->SetPrev(NULL);
		}
	}

	if (m_cur == obj)
	{// 自分が最後尾の時、
		// 最後尾を前に設定する
		m_cur = obj->GetPrev();

		// 最後尾の次を無しにする
		if (m_cur != NULL) {
			m_cur->SetNext(NULL);
		}
	}

	if (obj->GetPrev() != NULL && obj->GetNext() != NULL)
	{// 挟まれている時、
		// 前と次を繋ぐ
		CObject* prev = obj->GetPrev();
		CObject* next = obj->GetNext();
		prev->SetNext(next);
		next->SetPrev(prev);
	}

	m_num--;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]オブジェクトマネージャークラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// オブジェクト解放処理
//========================================
void CObjectMgr::Release(CObject* obj) {

	// 既にNULLであれば終了
	if (obj == NULL)
		return;

	// リストから削除
	SubList(obj);

	// 解放
	RNLib::Memory().Release(&obj);
}

//========================================
// 削除フラグが立っているオブジェクトを解放する
//========================================
void CObjectMgr::ReleaseDeleteObj(void) {

	// 先頭から順番に最後尾まで解放していく
	// (※削除フラグが真の時、解放処理)
	CObject* obj = m_top;
	while (obj != NULL) {
		CObject* nextObj = obj->GetNext();

		// 削除フラグが真の時、解放処理
		if (obj->GetIsDelete())
			Release(obj);

		obj = nextObj;
	}
}
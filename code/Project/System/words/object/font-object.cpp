//========================================
// 
// オブジェクト処理
// 
//========================================
// *** object.cpp ***
//========================================
#include "../../../manager.h"
#include "font-object.h"

// 静的変数
CFontObject *CFontObject::m_apTop[] = {};
CFontObject* CFontObject::m_apCir[] = {};

//========================================
// コンストラクタ
//========================================
CFontObject::CFontObject(int nPriority)
{
	m_pNext = NULL;		// 次のオブジェクト
	m_pPrev = NULL;		// 前のオブジェクト
	m_bDeath = false;	// 死亡フラグ
	m_type = TYPE_NONE;	// 種類

	// 先頭オブジェクトがNULLか
	if (m_apTop[nPriority] == NULL)
	{
		m_apTop[nPriority] = this;	// 自分自身を代入(先頭)
		m_apCir[nPriority] = this;	// 自分自身を代入(最後尾)
	}
	else
	{
		this->m_pPrev = m_apCir[nPriority];	// 最後尾のオブジェクトを代入
		m_apCir[nPriority]->m_pNext = this;	// 自分自身を代入
		m_apCir[nPriority] = this;			// 自分自身を代入
	}
	m_nPriority = nPriority;		// 自分自身の優先順位を代入
}

//========================================
// デストラクタ
//========================================
CFontObject::~CFontObject()
{
	
}

//========================================
// 全体の破棄
//========================================
void CFontObject::ReleaseAll(void)
{
	for (int nPrio = 0; nPrio < PRIO_MAX; nPrio++)
	{
		// 先頭オブジェクト
		CFontObject *pObj = m_apTop[nPrio];

		// オブジェクト分の回す
		while (pObj != NULL)
		{
			// 次のオブジェクト
			CFontObject *pObjNext = pObj->m_pNext;

			// 次のオブジェクトがあるか
			if (pObj->m_pNext == NULL)
			{
				pObj->Uninit();		// 終了処理
				pObj->Release();	// 死亡フラグを真にする
				pObj = NULL;
				break;
			}
			else
			{
				pObj->Uninit();		// 終了処理
				pObj->Release();	// 死亡フラグを真にする
				pObj = NULL;
			}
			pObj = pObjNext;	// 次のオブジェクトポインタを代入
		}

		// ポインタを先頭まで戻す
		pObj = m_apTop[nPrio];

		while (pObj != NULL)
		{
			// 次のオブジェクト
			CFontObject *pObjNext = pObj->m_pNext;

			// 死亡フラグが真か
			if (pObj->m_bDeath)
			{
				// 破棄
				pObj->ReleaseList();
			}

			pObj = pObjNext;	// 次のオブジェクトポインタを代入
		}
	}
}

//========================================
// 全て破棄(種類毎)
//========================================
void CFontObject::ReleaseAll(TYPE type)
{
	for (int nPrio = 0; nPrio < PRIO_MAX; nPrio++)
	{
		// 先頭オブジェクト
		CFontObject *pObj = m_apTop[nPrio];

		// オブジェクト分の回す
		while (pObj != NULL)
		{
			// 次のオブジェクト
			CFontObject *pObjNext = pObj->m_pNext;

			// 指定した種類かどうか
			if (pObj->GetType() == type)
			{
				// 次のオブジェクトがあるか
				if (pObj->m_pNext == NULL)
				{
					pObj->Uninit();		// 終了処理
					pObj->Release();	// 死亡フラグを真にする
					pObj = NULL;
					break;
				}
				else
				{
					pObj->Uninit();		// 終了処理
					pObj->Release();	// 死亡フラグを真にする
					pObj = NULL;
				}
			}
			pObj = pObjNext;	// 次のオブジェクトポインタを代入
		}

		// ポインタを先頭まで戻す
		pObj = m_apTop[nPrio];

		while (pObj != NULL)
		{
			// 次のオブジェクト
			CFontObject *pObjNext = pObj->m_pNext;

			// 死亡フラグが真か
			if (pObj->m_bDeath)
			{
				// 破棄
				pObj->ReleaseList();
			}

			pObj = pObjNext;	// 次のオブジェクトポインタを代入
		}
	}
}

//========================================
// 破棄リスト
//========================================
void CFontObject::Release(void)
{
	// 死亡フラグを真にする
	m_bDeath = true;
}
//========================================
// 破棄
//========================================
void CFontObject::ReleaseList(void)
{
	// 自分自身
	CFontObject *pObj = this;

	if (pObj == NULL)
	{// 消す奴が使われているかどうか、
		return;
	}
	else if (m_pPrev == NULL && m_pNext == NULL)
	{// オブジェクトが１つしかない時、
		m_apTop[m_nPriority] = NULL;
		m_apCir[m_nPriority] = NULL;
	}
	else if (m_pPrev == NULL)
	{// Topが消えた時、
		m_apTop[m_nPriority] = pObj->m_pNext;	// 先頭のポインタを代入
		m_apTop[m_nPriority]->m_pPrev = NULL;	// 前のポインタを NULL
	}
	else if (m_pNext == NULL)
	{// Cirが消えた時、
		m_apCir[m_nPriority] = pObj->m_pPrev;	// 最後尾のポインタを代入
		m_apCir[m_nPriority]->m_pNext = NULL;	// 次のポインタを NULL
	}
	else
	{// 間のオブジェクトが消えた時、
		pObj->m_pPrev->m_pNext = pObj->m_pNext;	// 次のポインタを代入
		pObj->m_pNext->m_pPrev = pObj->m_pPrev;	// 前のポインタを代入
	}

	delete pObj;
	pObj = NULL;
}

//========================================
// 全体の更新
//========================================
void CFontObject::UpdateAll(void)
{
	for (int nPrio = 0; nPrio < PRIO_MAX; nPrio++)
	{
		// 先頭オブジェクト
		CFontObject *pObj = m_apTop[nPrio];

		// オブジェクト分の回す
		while (pObj != NULL)
		{
			// 次のオブジェクト
			CFontObject *pObjNext = pObj->m_pNext;

			if (!pObj->m_bDeath)
			{
				// 更新処理
				pObj->Update();
			}

			// 次のオブジェクトを代入
			pObj = pObjNext;
		}

		// ポインタを先頭まで戻す
		pObj = m_apTop[nPrio];

		// オブジェクト分の回す
		while (pObj != NULL)
		{
			// 次のオブジェクト
			CFontObject *pObjNext = pObj->m_pNext;

			if (pObj->m_bDeath)
			{
				// 破棄
				pObj->ReleaseList();
			}

			// 次のオブジェクトを代入
			pObj = pObjNext;
		}
	}
}

//========================================
// 全体の描画
//========================================

void CFontObject::DrawAll(void)
{
	// デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

	for (int nPrio = 0; nPrio < PRIO_MAX; nPrio++)
	{
		// 先頭オブジェクト
		CFontObject *pObj = m_apTop[nPrio];


		// オブジェクト分の回す
		while (pObj != NULL)
		{
			// 死亡フラグが偽か
			if (!pObj->m_bDeath)
			{
				// 描画処理
				pObj->Draw();
			}
			// 次のオブジェクトがあるかどうか、
			if (pObj->m_pNext == NULL)
			{
				break;
			}
			else
			{
				// 次のオブジェクトを代入
				pObj = pObj->m_pNext;
			}
		}
	}
}

//========================================
// 種類の設定
//========================================
void CFontObject::SetType(TYPE type)
{
	m_type = type;
}
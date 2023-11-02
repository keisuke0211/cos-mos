//========================================
// 
// フォント：オブジェクト
// 
//========================================
// *** font-objext.h ***
//========================================
#pragma once

#include "../../../../_RNLib/RNlib.h"

//****************************************
// クラス
//****************************************
class CFontObject
{
public:

	// ***** 列挙型 *****

	// 優先順位種類
	enum PRIO
	{
		PRIO_NONE = 0,	// 無し
		PRIO_OBJECT,	// オブジェクト
		PRIO_TEXT,		// テキスト
		PRIO_MAX
	};

	// オブジェクト種類
	enum TYPE
	{
		TYPE_NONE = 0,	// 無し
		TYPE_FONT,		// フォント
		TYPE_TEXT,		// テキスト
		TYPE_MAX
	};

	// ***** 静的関数 *****
	/* 全ての破棄			*/static void ReleaseAll(void);
	/* 全ての破棄(種類毎)	*/static void ReleaseAll(TYPE type);	// [オーバーロード]
	/* 全ての更新			*/static void UpdateAll(void);
	/* 全ての描画			*/static void DrawAll(void);

	// ***** 関数 *****
	CFontObject(int nPriority = 0);
	virtual~CFontObject();

	/* 初期化	*/virtual HRESULT Init(void) = 0;
	/* 終了		*/virtual void Uninit(void) = 0;
	/* 更新		*/virtual void Update(void) = 0;
	/* 描画		*/virtual void Draw(void) = 0;

	// -- 設定 ------------------------------------------
	/* 種類設定*/void SetType(TYPE type);

	// -- 取得 ------------------------------------------
	/* 先頭の情報				*/static CFontObject *GetTop(int nPriority) { return m_apTop[nPriority]; }
	/* 最後尾の情報				*/static CFontObject *GetCur(int nPriority) { return m_apCir[nPriority]; }
	/* 前のオブジェクト情報		*/CFontObject *GetPrev(void) { return m_pPrev; }
	/* 前のオブジェクト情報		*/CFontObject *GetNext(void) { return m_pNext; }
	/* 幅						*/virtual float GetWidth(void) { return 0.0f; }
	/* 高さ						*/virtual float GetHeight(void) { return 0.0f; }
	/* 奥行き					*/virtual float GetDepth(void) { return 0.0f; }
	/* サイズ					*/virtual D3DXVECTOR3 GetSize(void) { return INITD3DXVECTOR3; }
	/* 位置						*/virtual D3DXVECTOR3 GetPos(void) { return INITD3DXVECTOR3; }
	/* 位置(過去)				*/virtual D3DXVECTOR3 GetPosOld(void) { return INITD3DXVECTOR3; }
	/* 向き						*/virtual D3DXVECTOR3 GetRot(void) { return INITD3DXVECTOR3; }
	/* 向き(過去)				*/virtual D3DXVECTOR3 GetRotOld(void) { return INITD3DXVECTOR3; }
	/* 種類						*/TYPE GetType(void) { return m_type; }

protected:
	
	// ***** 関数 ****
	/* 解放処理 */void Release(void);

	// ***** 変数 *****
	CFontObject *m_pPrev;					// 前のオブジェクトへのポインタ
	CFontObject *m_pNext;					// 次のオブジェクトへのポインタ

private:
	// ***** 関数 *****
	/* 解放処理	*/void ReleaseList(void);

	// ***** 変数 *****
	static CFontObject *m_apTop[PRIO_MAX];	// 先頭のオブジェクトへのポインタ
	static CFontObject *m_apCir[PRIO_MAX];	// 最後尾のオブジェクトへのポインタ
	int m_nPriority;					// 優先順位の位置
	TYPE m_type;						// 自分自身の種類
	bool m_bDeath;						// 死亡フラグ

};
//========================================
// 
// パーティクル処理
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once
#include "../../_RNLib/RNlib.h"

//****************************************
// クラス
//****************************************
class CParticle : public CObject {
public:

	// 共通情報
	struct Info
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 move;		// 移動量
		D3DXVECTOR3 rot;		// 移動量
		D3DXVECTOR3 scale;		// 拡縮
		Color col;				// 頂点カラー
		int nCount;
		int nCountMax;
		int nTex;
		int rdmMagni;
		CDrawState::ALPHA_BLEND_MODE alphamode;
	};

	// 共通情報
	enum class TYPE
	{
		TYPE_NORMAL = 0,
		TYPE_SPIN,
		MAX
	};

	// *** 関数 ***
	CParticle();
	~CParticle();

	/* 初期化	*/HRESULT Init(int nTex,int nCount);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	// -- 設定 ---------------------------------------------
	/* 色		*/void SetColor(const Color col) { m_Info.col = col; }
	/* 位置		*/void SetPos(const D3DXVECTOR3 pos) { m_Info.pos = pos; }
	/* 位置		*/void SetRot(const D3DXVECTOR3 rot) { m_Info.rot = rot; }
	/* 拡縮		*/void SetScale(const D3DXVECTOR3 scale) { m_Info.scale = scale; }
	/* 拡縮		*/void SetType(const TYPE type) { m_type = type; }
	/* 拡縮		*/void SetRdmMagni(const int magni) { m_Info.rdmMagni = magni; }
	/* 拡縮		*/void SetAlphaMode(const CDrawState::ALPHA_BLEND_MODE  alphamode) { m_Info.alphamode = alphamode; }

	// -- 所得 ---------------------------------------------
	/* 色		*/Color GetColor(void) { return m_Info.col; }
	/* 位置		*/D3DXVECTOR3 GetPos(void) { return m_Info.pos; }
	/* 拡縮		*/D3DXVECTOR3 GetScale(void) { return m_Info.scale; }

private:

	// *** 静的変数 ***
	static int m_nNumAll;		// 総数
	static D3DXVECTOR3 m_rot;	// 移動量

	// *** 変数 ***
	Info m_Info;		// 共通情報
	TYPE m_type;
};

//========================================
// 
// 文字設定
// 
//========================================
// *** words.cpp ***
//========================================
#include "words.h"
#include "font.h"
#include "../../main.h"
#include "../../manager.h"

//========================================
// コンストラクタ
//========================================
CWords::CWords(int nPriority) : CFontObject(nPriority)
{
	m_Info.pos = INITD3DXVECTOR3;
	m_Info.OldPos = INITD3DXVECTOR3;
	m_Info.TargetPos = INITD3DXVECTOR3;
	m_Info.DifferencePos = INITD3DXVECTOR3;
	m_Info.rot = INITD3DXVECTOR3;
	m_Info.size = INITD3DXVECTOR3;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.moveRot = INITD3DXVECTOR3;
	m_Info.col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	m_Info.nCntMove = 0;
	m_Info.nMoveTime = 0;
	m_Info.nLetterPop = 0;
	m_Info.bMove = false;
	m_Info.bMoveEnd = true;

	m_pTex = NULL;
	m_pVtxBuff = NULL;
}

//========================================
// デストラクタ
//========================================
CWords::~CWords()
{

}

//========================================
// 初期化
//========================================
HRESULT CWords::Init(void)
{
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetType(TYPE_FONT);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//-------------------
	// 頂点情報の設定
	//-------------------
	SetVtxPos();

	// 頂点カラーの設定
	SetColor(m_Info.col);

	//頂点バッファをロックし頂点情報へのポインタを取得
	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// rhwの設定
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
	
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//========================================
// 終了
//========================================
void CWords::Uninit(void)
{
	// 破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//========================================
// 更新
//========================================
void CWords::Update(void)
{
	// 位置・向き更新
	m_Info.pos += m_Info.move;
	m_Info.rot.z += m_Info.moveRot.z;

	TargetMove();

	//頂点座標設定
	SetVtxPos();
}

//========================================
// 描画
//========================================
void CWords::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTex);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
	pDevice->SetTexture(0, NULL);
}

//========================================
// 目標位置まで移動
//========================================
void CWords::TargetMove(void)
{
	if (m_Info.bMove)
	{
		// カウンター加算
		if (m_Info.nCntMove < m_Info.nMoveTime)
			m_Info.nCntMove++;

		CFloat rate = RNLib::Ease().Easing(EASE_TYPE::LINEAR, m_Info.nCntMove, m_Info.nMoveTime);
		CFloat MoveX = rate * m_Info.DifferencePos.x;
		CFloat MoveY = rate * m_Info.DifferencePos.y;
		CFloat MoveZ = rate * m_Info.DifferencePos.z;

		// 移動
		m_Info.pos.x = m_Info.OldPos.x + MoveX;
		m_Info.pos.y = m_Info.OldPos.y + MoveY;
		m_Info.pos.z = m_Info.OldPos.z + MoveZ;

		if (m_Info.nCntMove == m_Info.nMoveTime)
		{
			m_Info.bMove = false;
			m_Info.bMoveEnd = true;
		}
	}
}

//========================================
// 生成
//========================================
CWords *CWords::Create(const char*text, D3DXVECTOR3 pos, D3DXVECTOR3 size, CFont::FONT type, D3DXCOLOR col)
{
	CWords * pWords = NULL;
	pWords = new CWords;

	if (pWords != NULL)
	{
		pWords->SetPos(pos);
		pWords->SetSize(size);
		pWords->SetWords(text, type);
		pWords->SetColor(col);
		pWords->Init();
	}
	return pWords;
}

//================================================================================
//--------------------------------------------------------------------------------
//*********************************** 設定処理 ***********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// 文字
//========================================
void CWords::SetWords(const char*text, CFont::FONT type)
{
	string Txt = text;
	if (Txt != "")
	{
		m_pTex = Manager::Font()->GetFont(text, type);

		// デバイスの所得
		LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

		// テクスチャの変更
		if (Txt == "Ч") {// 空白（外部ファイルからの読み取り専用）
			m_pTex = Manager::Font()->GetFont("", type);
		}
		else if (Txt == "Й")
		{
			// テクスチャの読み込み
			if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\TextBox\\button_a.png", &m_pTex)))
				m_pTex = Manager::Font()->GetFont(text, type);
		}
	}
	else
	{
		m_pTex = NULL;
		SetColor(D3DXCOLOR{0,0,0,0});
	}
}

//========================================
// 目標位置の設定
//========================================
void CWords::SetTargetPos(D3DXVECTOR3 pos, int Time)
{
	// 解像度の倍率
	D3DXVECTOR3 Target = pos * 2;

	D3DXVECTOR3 TargetPos = m_Info.TargetPos;

	if (!m_Info.bMove && TargetPos != Target)
	{
		m_Info.bMove = true;
		m_Info.bMoveEnd = false;
		TargetPos = Target;
		TargetPos.x = TargetPos.x + ((m_Info.size.x * 2) * m_Info.nLetterPop);

		m_Info.nCntMove = 0;
		m_Info.nMoveTime = 20;

		m_Info.OldPos = m_Info.pos;
		m_Info.DifferencePos = TargetPos - m_Info.pos;
	}
}

//========================================
// 位置
//========================================
void CWords::SetPos(const D3DXVECTOR3 &pos)
{
	m_Info.pos = pos;
	m_Info.TargetPos = pos;

	//頂点座標設定
	SetVtxPos();
}

//========================================
// 移動量
//========================================
void CWords::SetMove(const D3DXVECTOR3 &move)
{
	m_Info.move = move;
}

//========================================
// 色
//========================================
void CWords::SetColor(D3DXCOLOR col)
{
	m_Info.col = col;

	if (m_pVtxBuff == NULL) return;

	//頂点バッファをロックし頂点情報へのポインタを取得
	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);
	pVtx[1].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);
	pVtx[2].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);
	pVtx[3].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
// 頂点座標
//========================================
void CWords::SetVtxPos(void)
{
	if (m_pVtxBuff == NULL) return;

	//頂点バッファをロックし頂点情報へのポインタを取得
	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 計算用マトリックス
	D3DXMATRIX mtx;

	// マトリックス作成
	D3DXMatrixIdentity(&mtx);
	D3DXMatrixRotationYawPitchRoll(&mtx, 0.0f, 0.0f, m_Info.rot.z);

	// 頂点座標
	for (int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 addPos = INITPOS3D;
		D3DXVec3TransformCoord(&addPos, &m_Vtx[i], &mtx);

		// サイズ変更
		pVtx[i].pos.x = m_Info.pos.x + (addPos.x * m_Info.size.x);
		pVtx[i].pos.y = m_Info.pos.y + (addPos.y * m_Info.size.y);
		pVtx[i].pos.z = 0.0f;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
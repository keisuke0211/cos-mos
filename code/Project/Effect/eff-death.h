#pragma once
//========================================================
// 
// 死亡パーティクル処理
// Author:HIRASAWA SHION
// 
//========================================================
#include "../../_RNLib/RNlib.h"
#include "../collision.h"

// 死亡パーティクルクラス
class CEffect_Death : public CObject {
public:
	//種類
	enum class TYPE {
		NONE = 0,
		BALL, //ボール
		INK,  //インク
	};

	//自己情報
	struct Info
	{
		Vector3D pos    = INITVECTOR3D; // 位置
		Vector3D posOld = INITVECTOR3D; // 前回位置
		Vector3D move   = INITVECTOR3D; // 移動量
		Vector3D rot    = INITVECTOR3D; // 向き
		Vector3D spin   = INITVECTOR3D; // 回転量
		Vector2D size   = INITVECTOR2D; // サイズ（幅・高さ）
		Color    color  = INITCOLOR;    // 色
		int      *pLife = NULL;         // 寿命（不死身ならNULL
		int      nTex   = NONEDATA;     // テクスチャ番号
		TYPE     type   = TYPE::NONE;   // 種類
	};

	CEffect_Death();
	~CEffect_Death();

	void Update(void);

	//******************************
	//設定処理（自己情報を一括設定）
	//******************************
	void SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const Vector2D size, const Color color, const int *pLife, const int nTex, const TYPE type);

	//******************************
	//設定処理（自己情報を個別設定）
	//******************************
	void SetPos(const Vector3D pos)       { m_Info.pos = pos; }
	void SetPosOld(const Vector3D posOld) { m_Info.posOld = posOld; }
	void SetMove(const Vector3D move)     { m_Info.move = move; }
	void SetRot(const Vector3D rot)       { m_Info.rot = rot; }
	void SetSpin(const Vector3D spin)     { m_Info.spin = spin; }
	void SetSize(const Vector2D size)     { m_Info.size = size; }
	void SetColor(const Color color)      { m_Info.color = color; }
	void SetLife(const int *pLife)        { *m_Info.pLife = *pLife; }
	void SetTex(const int nTex)           { m_Info.nTex = nTex; }
	void SetType(const TYPE type)         { m_Info.type = type; }

private:
	//種別更新処理
	void UpdateType_Ball(void);
	void UpdateType_Ink(void);

	//情報更新処理
	CCollision::ROT PlayerCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo);
	CCollision::ROT StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo);
	void Move(void);
	void Spin(void);
	void Life(void);

	Info m_Info;
};
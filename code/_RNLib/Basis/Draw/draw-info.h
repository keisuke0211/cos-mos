//========================================
// 
// 基底用描画情報クラスの定義
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//----------------------------------------
// クラス定義
//----------------------------------------
// 基底用描画情報クラス
class CDrawInfoBase {
public:
	//===== [[[ 列挙型定義 ]]
	// 種類
	enum class TYPE {
		NONE,
		POLYGON2D,
		POLYGON3D,
		MODEL,
	};

	//===== [[[ 関数宣言 ]]]
	CDrawInfoBase();
	virtual ~CDrawInfoBase();
	virtual void Draw(LPDIRECT3DDEVICE9& device, const D3DXMATRIX& viewMtx) = 0;

	//===== [[[ 変数定義 ]]]
	TYPE  m_type;
	short m_priority;
};

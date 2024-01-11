//========================================
// 
// ポリゴンの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../camera.h"
#include "../../../RNmain.h"

//****************************************
// 名前空間
//****************************************
// ポリゴン
namespace Polygon2DAnd3D {

	//========== [[[ 列挙型定義 ]]]
	enum class TEX_TYPE      { NONE, IDX, CAMERA, };
	enum class TEX_INFO_TYPE { NONE, NORMAL, UV, MAX, };

	//========== [[[ 構造体定義 ]]]
	struct SetTexInfo {
		UShort ptn;
		UShort ptnX;
		UShort ptnY;
		Pos2D  ptnPos;
	};
	struct SetTexUVInfo {
		Pos2D poses[4];
	};

	//========== [[[ クラスの定義 ]]]
	// テクスチャ設定情報総括
	class CSetTexInfoSum {
	public:
		// [[[ 関数宣言 ]]]
		CSetTexInfoSum     ();
		~CSetTexInfoSum    ();
		void ClearParameter(void);
		void AssignTexInfo (void*& tex, TEX_TYPE& texType, Pos2D& pos0, Pos2D& pos1, Pos2D& pos2, Pos2D& pos3);
		void SetTex        (const short& texIdx, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos);
		void SetTex        (CCamera*& camera, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos);
		void SetTexUV      (const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
		void SetTexUV      (CCamera*& camera, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
		void SetTexMirrorX (const bool& isMirrorX);
		void SetTexMirrorY (const bool& isMirrorY);
		
		// [[[ 変数宣言 ]]]
		void*         m_setTex         = NULL;
		TEX_TYPE      m_setTexType     = TEX_TYPE::NONE;
		void*         m_setTexInfo     = NULL;
		TEX_INFO_TYPE m_setTexInfoType = TEX_INFO_TYPE::NONE;
		bool          m_isTexMirrorX   = false;
		bool          m_isTexMirrorY   = false;
	private:
		// [[[ 関数宣言 ]]]
		void SetTex_Ptn  (const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos);
		void SetTexUV_Pos(const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
		void AllocTex    (const short& texIdx);
		void AllocTex    (CCamera*& camera);
	};

	//========== [[[ 関数の定義 ]]]
	void SetTexture(Device& device, void* tex, const TEX_TYPE& texType);
}
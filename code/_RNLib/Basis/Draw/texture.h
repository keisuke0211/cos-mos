//========================================
// 
// �e�N�X�`���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/regist.h"
#include "../../RNmain.h"

//****************************************
// �N���X��`
//****************************************
// �e�N�X�`���N���X
class _RNC_Texture : public CRegist {
public:
	//========== [[[ �֐��錾 ]]]
	static void CopyTextureData(LPDIRECT3DTEXTURE9& srcTexture, LPDIRECT3DTEXTURE9& destTexture);
	_RNC_Texture ();
	~_RNC_Texture();
	void  Init   (void);
	void  Uninit (void);
	void  Update (void);
	short Load   (const char *loadPath);
	void  Set    (Device pDevice, int nTex);

	// �ݒ�擾
	Texture& GetTexture(int nTex) { return m_texs[nTex]; }
	float GetWidth(int nTex) {
		if (nTex < 0)
			return 0.0f;
		D3DSURFACE_DESC desc;
		m_texs[nTex]->GetLevelDesc(0, &desc);
		return (float)desc.Width;
	}
	float GetHeight(int nTex) {
		if (nTex < 0)
			return 0.0f;
		D3DSURFACE_DESC desc;
		m_texs[nTex]->GetLevelDesc(0, &desc);
		return (float)desc.Height;
	}
	int GetCrntPtn(int nPtnX, int nPtnY, int nSpan);

private:
	//========== [[[ �ϐ��錾 ]]]
	Texture* m_texs;
};
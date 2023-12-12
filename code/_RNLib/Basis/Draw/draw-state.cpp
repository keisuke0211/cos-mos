//========================================
// 
// �`���Ԃ̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ���p�`����N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CDrawState::CDrawState() {

}

//========================================
// �f�X�g���N�^
//========================================
CDrawState::~CDrawState() {

}

//========================================
// ����������
//========================================
void CDrawState::Init(Device& device) {

	// ~~~ �Œ�ݒ� ~~~

	//----------------------------------------
	// �e�N�X�`���X�e�[�W�X�e�[�g�̏����ݒ�
	//----------------------------------------
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//----------------------------------------
	// �T���v���[�X�e�[�g�̏����ݒ�
	//----------------------------------------
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ~~~ �ϐݒ� ~~~

	ResetVariableSetting(device);
}

//========================================
// �I������
//========================================
void CDrawState::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CDrawState::Update(void) {

}

//========================================
// �ϐݒ�̃��Z�b�g����
//========================================
void CDrawState::ResetVariableSetting(Device& device) {

	//----------------------------------------
	// �e�N�X�`�����߃��[�h�̐ݒ�
	//----------------------------------------
	SetIsTextureAlpha(device, true);

	//----------------------------------------
	// Z�e�X�g�̏����ݒ�
	//----------------------------------------
	SetIsZTest(device, true);

	//----------------------------------------
	// �J�����O�̏����ݒ�
	//----------------------------------------
	SetCullingMode(device, CULLING_MODE::FRONT_SIDE);

	//----------------------------------------
	// �A���t�@�u�����h�̏����ݒ�
	//----------------------------------------
	SetAlphaBlendMode(device, ALPHA_BLEND_MODE::NORMAL);

	//----------------------------------------
	// ���C�e�B���O�̏����ݒ�
	//----------------------------------------
	SetIsLighting(device, true);

	//----------------------------------------
	// �t�H�O�̏����ݒ�
	//----------------------------------------
	SetIsFog(device, false);
	SetFogParameter(device, INITCOLOR, 0.0f, 0.0f);
}

//========================================
// Z�e�X�g���[�h�̐ݒ�
//========================================
void CDrawState::SetIsZTest(Device& device, const bool& isZTest) {

	if (isZTest) {
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	else {
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
}

//========================================
// �J�����O���[�h�̐ݒ�
//========================================
void CDrawState::SetCullingMode(Device& device, const CULLING_MODE& mode) {

	switch (mode) {
	case CULLING_MODE::FRONT_SIDE:device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW ); break;
	case CULLING_MODE::BACK_SIDE :device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW  ); break;
	case CULLING_MODE::BOTH_SIDES:device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); break;
	}
}

//========================================
// �A���t�@�u�����h���[�h�̐ݒ�
//========================================
void CDrawState::SetAlphaBlendMode(Device& device, const ALPHA_BLEND_MODE& mode) {

	switch (mode) {
		// [[[ �ʏ� ]]]
	case ALPHA_BLEND_MODE::NORMAL: {
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_ALPHAREF, 255);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}break;
		// [[[ ���Z ]]]
	case ALPHA_BLEND_MODE::ADD: {
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		device->SetRenderState(D3DRS_ALPHAREF, 0);
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}break;
	}
}

//========================================
// �e�N�X�`�����߃��[�h�̐ݒ�
//========================================
void CDrawState::SetIsTextureAlpha(Device& device, const bool& isTextureAlpha) {

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, isTextureAlpha ? D3DTOP_MODULATE : D3DTOP_DISABLE);
}

//========================================
// ���C�e�B���O���[�h�̐ݒ�(�t���O�w��)
//========================================
void CDrawState::SetIsLighting(Device& device, const bool& isLighting) {

	device->SetRenderState(D3DRS_LIGHTING, isLighting);
}

//========================================
// �t�H�O���[�h�̐ݒ�
//========================================
void CDrawState::SetIsFog(Device& device, const bool& isFog) {

	device->SetRenderState(D3DRS_FOGENABLE, isFog);
}

//========================================
// �t�H�O���[�h�̐ݒ�(�t���O�w��)
//========================================
void CDrawState::SetFogParameter(Device& device, const Color& col, const float& startDist, const float& endDist) {

	device->SetRenderState(D3DRS_FOGCOLOR, col.ConvD3DCOLOR());
	device->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&startDist));
	device->SetRenderState(D3DRS_FOGEND  , *(DWORD*)(&endDist));
}
//========================================
// 
// �e�N�X�`���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �e�N�X�`���N���X
//----------|---------------------------------------------------------------------
//================================================================================

// ���z�֐����g�p����LPDIRECT3DTEXTURE9�𕡐�����֐�
LPDIRECT3DTEXTURE9 CloneTexture(LPDIRECT3DTEXTURE9 sourceTexture, LPDIRECT3DDEVICE9 device)
{
	LPDIRECT3DTEXTURE9 clonedTexture = nullptr;

	// �\�[�X�e�N�X�`���̏����擾
	D3DSURFACE_DESC desc;
	sourceTexture->GetLevelDesc(0, &desc);

	// �N���[���p�̃e�N�X�`�����쐬
	HRESULT result = device->CreateTexture(desc.Width, desc.Height, 1, 0, desc.Format, D3DPOOL_MANAGED, &clonedTexture, nullptr);

	if (SUCCEEDED(result))
	{
		// �\�[�X�e�N�X�`���̃T�[�t�F�X���擾
		LPDIRECT3DSURFACE9 sourceSurface;
		sourceTexture->GetSurfaceLevel(0, &sourceSurface);

		// �N���[���e�N�X�`���̃T�[�t�F�X���擾
		LPDIRECT3DSURFACE9 destSurface;
		clonedTexture->GetSurfaceLevel(0, &destSurface);

		// �\�[�X�T�[�t�F�X����f�[�^���擾
		RECT rect = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };
		result = D3DXLoadSurfaceFromSurface(destSurface, nullptr, nullptr, sourceSurface, nullptr, &rect, D3DX_FILTER_NONE, 0);

		// ���\�[�X�̉��
		sourceSurface->Release();
		destSurface->Release();
	}

	return clonedTexture;
}

//========================================
// [�ÓI]�e�N�X�`���̃f�[�^���R�s�[
//========================================
void _RNC_Texture::CopyTextureData(LPDIRECT3DTEXTURE9& srcTexture, LPDIRECT3DTEXTURE9& destTexture) {

	// �R�s�[��e�N�X�`�������
	if (srcTexture != NULL) {
		srcTexture->Release();
		srcTexture = NULL;
	}

	// �R�s�[���e�N�X�`����������ΏI��
	if (destTexture == NULL)
		return;

	srcTexture = CloneTexture(destTexture, RNLib::Window().GetD3DDevice());
}

//========================================
// �R���X�g���N�^
//========================================
_RNC_Texture::_RNC_Texture() {

	m_texs = NULL;	// �e�N�X�`���ւ̃|�C���^
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Texture::~_RNC_Texture() {

}

//========================================
// ����������
//========================================
void _RNC_Texture::Init(void) {
	
}

//========================================
// �I������
//========================================
void _RNC_Texture::Uninit(void) {

	// �e�N�X�`�����
	if (m_texs != NULL) {
		for (int cntTex = 0; cntTex < m_num; cntTex++) {
			if (m_texs[cntTex] != NULL) {
				m_texs[cntTex]->Release();
				m_texs[cntTex] = NULL;
			}
		}
		delete m_texs;
		m_texs = NULL;
	}
}

//========================================
// �X�V����
//========================================
void _RNC_Texture::Update(void) {

}

//========================================
// �ǂݍ��ݏ���
//========================================
short _RNC_Texture::Load(const char* loadPath) {

	UShort numOld = m_num;
	short idx = NONEDATA;

	if (CRegist::Load(loadPath, idx))
	{// �ǂݍ��ݐ���
		// �e�N�X�`���̃������Ċm��
		RNLib::Memory().ReAlloc(&m_texs, numOld, m_num);
		m_texs[idx] = NULL;

		// �e�N�X�`���̓ǂݍ���
		Device device = RNLib::Window().GetD3DDevice();
		if (FAILED(D3DXCreateTextureFromFile(device, loadPath, &m_texs[idx])))
		{// �ǂݍ��ݎ��s
			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B\n%s", loadPath));

			// �e�N�X�`���̃��������Z�b�g
			RNLib::Memory().ReAlloc(&m_texs, m_num, numOld);

			// �ǂݍ��ݍσp�X�̃��������Z�b�g
			ReAllocLoadPath(numOld);

			// �������Z�b�g
			m_num = numOld;

			return NONEDATA;
		}
	}

	return idx;
}

//========================================
// �ݒ菈��
// Author:RIKU NISHIMURA
//========================================
void _RNC_Texture::Set(Device pDevice, int nTex) {
	if (nTex < 0 || nTex >= m_num)
	{// �e�N�X�`���ԍ����͈͊O�̎��A
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);
		return;	// �������I������
	}

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_texs[nTex]);
}

//========================================
// ���̃p�^�[���ԍ����擾
// Author:RIKU NISHIMURA
//========================================
int _RNC_Texture::GetCrntPtn(int nPtnX, int nPtnY, int nSpan) {
	return (RNLib::Count().GetCount() % ((nPtnX * nPtnY) * nSpan)) / nSpan;
}
//========================================
// 
// �e�N�X�`���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �e�N�X�`���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CTexture::CTexture() {
	m_texs = NULL;	// �e�N�X�`���ւ̃|�C���^
}

//========================================
// �f�X�g���N�^
//========================================
CTexture::~CTexture() {

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
// ����������
//========================================
void CTexture::Init(void) {
	
}

//========================================
// �ǂݍ��ݏ���
//========================================
int CTexture::Load(const char* loadPath) {

	unsigned short numOld = m_num;
	short idx = NONEDATA;

	if (CRegist::Load(loadPath, idx))
	{// �ǂݍ��ݐ���
		// �e�N�X�`���̃������Ċm��
		RNLib::Memory().ReAlloc(&m_texs, numOld, m_num);
		m_texs[idx] = NULL;

		// �e�N�X�`���̓ǂݍ���
		LPDIRECT3DDEVICE9 device = RNLib::Window().GetD3DDevice();
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
void CTexture::Set(LPDIRECT3DDEVICE9 pDevice, int nTex) {
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
int CTexture::GetCrntPtn(int nPtnX, int nPtnY, int nSpan) {
	return (RNLib::GetCount() % ((nPtnX * nPtnY) * nSpan)) / nSpan;
}
//========================================
// 
// �����ݒ�
// 
//========================================
// *** words.cpp ***
//========================================
#include "words.h"
#include "font.h"
#include "../../main.h"
#include "../../manager.h"

//========================================
// �R���X�g���N�^
//========================================
CWords::CWords(int nPriority) : CFontObject(nPriority)
{
	m_Info.pos = INITD3DXVECTOR3;
	m_Info.rot = INITD3DXVECTOR3;
	m_Info.size = INITD3DXVECTOR3;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.moveRot = INITD3DXVECTOR3;
	m_Info.col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	m_pTex = NULL;
	m_pVtxBuff = NULL;
}

//========================================
// �f�X�g���N�^
//========================================
CWords::~CWords()
{

}

//========================================
// ������
//========================================
HRESULT CWords::Init(void)
{
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetType(TYPE_FONT);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//-------------------
	// ���_���̐ݒ�
	//-------------------
	SetVtxPos();

	// ���_�J���[�̐ݒ�
	SetColar(m_Info.col);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// rhw�̐ݒ�
	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
	
	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//========================================
// �I��
//========================================
void CWords::Uninit(void)
{
	// �j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//========================================
// �X�V
//========================================
void CWords::Update(void)
{
	// �ʒu�E�����X�V
	m_Info.pos += m_Info.move;
	m_Info.rot.z += m_Info.moveRot.z;

	//���_���W�ݒ�
	SetVtxPos();
}

//========================================
// �`��
//========================================
void CWords::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTex);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
	pDevice->SetTexture(0, NULL);
}

//========================================
// ����
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
		pWords->SetColar(col);
		pWords->Init();
	}
	return pWords;
}

//================================================================================
//--------------------------------------------------------------------------------
//*********************************** �ݒ菈�� ***********************************
//--------------------------------------------------------------------------------
//================================================================================

//========================================
// ����
//========================================
void CWords::SetWords(const char*text, CFont::FONT type)
{
	string Txt = text;
	if (Txt != "")
	{
		m_pTex = Manager::Font()->GetFont(text, type);

		// �f�o�C�X�̏���
		LPDIRECT3DDEVICE9 pDevice = RNLib::Window().GetD3DDevice();

		// �e�N�X�`���̕ύX
		if (Txt == "�X") {// �󔒁i�O���t�@�C������̓ǂݎ���p�j
			m_pTex = Manager::Font()->GetFont("", type);
		}
		else if (Txt == "�J")
		{
			// �e�N�X�`���̓ǂݍ���
			if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\TextBox\\button_a.png", &m_pTex)))
				m_pTex = Manager::Font()->GetFont(text, type);
		}
	}
	else
	{
		m_pTex = NULL;
		SetColar(D3DXCOLOR{0,0,0,0});
	}
}

//========================================
// �ʒu
//========================================
void CWords::SetPos(const D3DXVECTOR3 &pos)
{
	m_Info.pos = pos;

	//���_���W�ݒ�
	SetVtxPos();
}

//========================================
// �ړ���
//========================================
void CWords::SetMove(const D3DXVECTOR3 &move)
{
	m_Info.move = move;
}

//========================================
// �F
//========================================
void CWords::SetColar(D3DXCOLOR col)
{
	m_Info.col = col;

	if (m_pVtxBuff == NULL) return;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);
	pVtx[1].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);
	pVtx[2].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);
	pVtx[3].col = D3DXCOLOR(m_Info.col.r, m_Info.col.g, m_Info.col.b, m_Info.col.a);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
// ���_���W
//========================================
void CWords::SetVtxPos(void)
{
	if (m_pVtxBuff == NULL) return;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtx;

	// �}�g���b�N�X�쐬
	D3DXMatrixIdentity(&mtx);
	D3DXMatrixRotationYawPitchRoll(&mtx, 0.0f, 0.0f, m_Info.rot.z);

	// ���_���W
	for (int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 addPos = INITPOS3D;
		D3DXVec3TransformCoord(&addPos, &m_Vtx[i], &mtx);

		// �T�C�Y�ύX
		pVtx[i].pos.x = m_Info.pos.x + (addPos.x * m_Info.size.x);
		pVtx[i].pos.y = m_Info.pos.y + (addPos.y * m_Info.size.y);
		pVtx[i].pos.z = 0.0f;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
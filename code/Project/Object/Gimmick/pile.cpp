//================================================================================================
// 
// �Y����[pile.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "pile.h"
#include "../../main.h"

//�Y���߂荞�܂���Œፂ��
const float CPile::CAVEIN_DIFF_HEIGHT = 35.0f;

const int NUMTEX = 2;
const int RAND_TEX = rand() % NUMTEX;
//=======================================
// �R���X�g���N�^
//=======================================
CPile::CPile()
{
	Manager::StageObjectMgr()->AddList(this);

	//�������
	m_type = TYPE::PILE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;

	m_TrunkModelID = NONEDATA;
	m_PilePos = INITVECTOR3D;	//�{�̍��W
	m_NumTrunk = 0;			//���̐��i�Œ�R�j
	m_fEvenTrunkCorrHeight = 0.0f;//���̐��������������ꍇ�̕␳�l�i��Ȃ�0.0f
	m_TrunkHeight = 0.0f;	//�����W
	m_TrunkHeightOld = 0.0f;//�����W
	m_StartTrunkHeight = 0.0f;
}

//=======================================
// �f�X�g���N�^
//=======================================
CPile::~CPile()
{

}

//=======================================
// ����������
//=======================================
void CPile::Init(void)
{
	m_TrunkModelID = RNLib::Model().Load("data\\MODEL\\Trunk.x");

	m_nTex[0] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_000.png");
	m_nTex[1] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_001.png");

	//�O�����ۑ�
	SetOld(0.0f);
}

//=======================================
// �I������
//=======================================
void CPile::Uninit(void)
{

}

//=======================================
// �X�V����
//=======================================
void CPile::Update(void)
{
	//�߂荞�݃��Z�b�g
	if (RNLib::Input().GetKeyTrigger(DIK_L)) m_TrunkHeight = m_StartTrunkHeight;

	//�O�����ۑ�
	SetOld(m_TrunkHeight);

	//���f���z�u
	PutModel();
}

//===============================
//���f���z�u
//===============================
void CPile::PutModel(void)
{
	//�����W
	Pos3D PilePos = m_PilePos;

	//��ԏ�̊�����z�u���邽�߁A�z�u���鍂�����v�Z
	{
		//�����̔����i�؂�̂āj
		CInt nNumHalf = m_NumTrunk / EVENPARITY;
		CInt Under = (m_NumTrunk + 1) % EVENPARITY;

		//�{�̃T�C�Y  �{  �T�C�Y �~ ��̊���  �{  ���̂߂荞�ݗ�
		PilePos.y = SIZE_OF_1_SQUARE * 0.5f * Under + SIZE_OF_1_SQUARE * nNumHalf + m_TrunkHeight;
	}

	for (int nCntPile = 0; nCntPile < m_NumTrunk; nCntPile++)
	{
		//�����f��
		RNLib::Model().Put(PRIORITY_OBJECT, m_TrunkModelID, PilePos, INITD3DXVECTOR3);

		//�z�u���W��������
		PilePos.y -= SIZE_OF_1_SQUARE;
	}
}

//===============================
//�ݒ菈��
//�����ɕK�v�����i�Œ�3�j��z�u���W���w�肷��
//-------------------------------
//�����P  pos�F�z�u���W
//�����Q  NumTrunk�F���̐��i�Œ�R�j
//�����R  TrunkHeight�F���̂߂荞�݌W���i�߂荞�݂Ȃ� = 0.0f�j
//===============================
void CPile::Set(Pos3D pos, int NumTrunk, float TrunkHeight)
{
	//�������Œᐔ��������Ă�����C��
	if (NumTrunk < MIN_TRUNK) NumTrunk = MIN_TRUNK;

	//���ݒ�
	m_PilePos = pos;
	m_NumTrunk = NumTrunk;
	m_TrunkHeight = SIZE_OF_1_SQUARE * TrunkHeight;
	m_fEvenTrunkCorrHeight = NumTrunk % EVENPARITY == 0 ? SIZE_OF_1_SQUARE * 0.5f : 0.0f;

	//�����蔻��̍������Đݒ�
	m_height = SIZE_OF_1_SQUARE * NumTrunk;

	//�߂荞�ݗʒ���
	CaveInTrunkHeight(0.0f);

	//�����߂荞�ݗʐݒ�
	m_StartTrunkHeight = m_TrunkHeight;

	//���f���z�u
	PutModel();
}

//===============================
//�߂荞�ݗʔ��f����
//===============================
void CPile::CaveInTrunkHeight(float fCaveInHeight)
{
	//�����ۑ�
	CFloat fCaveTemp = fCaveInHeight;

	//�ւ��ݗʔ��f
	fCaveInHeight += m_TrunkHeight;

	//�Y�̍ő�E�Œፂ�����v�Z
	CInt nNumHalf = m_NumTrunk / EVENPARITY;
	CFloat HalfSize = SIZE_OF_1_SQUARE * nNumHalf;
	CFloat CorrHeight = m_fEvenTrunkCorrHeight * 2.0f;

	//�Y�������Ȃ��悤�ɒ���
	RNLib::Number().Clamp(&fCaveInHeight, -HalfSize - CorrHeight, HalfSize - CorrHeight);

	//�O�����ۑ�
	SetOld(fCaveInHeight);

	//���f���z�u
	PutModel();

	//�p�[�e�B�N��
	float world = 0.0f;
	float side = 0.785f;
	float PopPosY = SIZE_OF_1_SQUARE * 0.5f;

	if (fCaveTemp < 0.0f)
	{
		world = D3DX_PI;
		side *= -1.0f;
		PopPosY *= -1.0f;
	}

	CInt NumEffect = 12;
	for (int Cnt = 0; Cnt < NumEffect; Cnt++){

		const Pos3D TexPos = Pos3D(m_pos.x + (float)(rand() % (int)m_width - m_width * 0.5), PopPosY, m_pos.z);

		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, world + side);
		if (rand() % 2 != 0) rot = D3DXVECTOR3(0.0f, 0.0f, world - side);
		
		CFloat ScaleTex = (float)(rand() % (int)(INIT_EFFECT_SCALE.x * 0.6f) + INIT_EFFECT_SCALE.x * 0.6f);
		Manager::EffectMgr()->ParticleCreate(m_nTex[RAND_TEX], TexPos, D3DXVECTOR3(ScaleTex, ScaleTex, 0.0f), Color{ 255,255,155,30 }, CParticle::TYPE::TYPE_FLOATUP, 300,rot, D3DXVECTOR3(80.0f, 80.0f, 0.0f),false,false,_RNC_DrawState::ALPHA_BLEND_MODE::NORMAL);
		Manager::EffectMgr()->ModelEffectCreate(0, D3DXVECTOR3(TexPos.x, TexPos.y + 1.0f * cosf(world), TexPos.z), rot, INITSCALE3D * 0.1f,COLOR_WHITE);
	}
}

//===============================
//�O�����ۑ�
//===============================
void CPile::SetOld(float fCaveInHeight)
{
	//�߂荞�ݗʂ𔽉f������
	m_TrunkHeightOld = m_TrunkHeight;
	m_TrunkHeight = fCaveInHeight;

	m_pos =    Pos3D(m_PilePos.x, m_PilePos.y + m_TrunkHeight + m_fEvenTrunkCorrHeight, 0.0f);
	m_posOld = Pos3D(m_PilePos.x, m_PilePos.y + m_TrunkHeightOld + m_fEvenTrunkCorrHeight, 0.0f);
}

//===============================
//�S�̂̂߂荞�ݗʂ�������
//===============================
void CPile::ResetTrunkHeightAll(void)
{
	// �I�u�W�F�N�g1��1�����Ă���
	CObject* obj = NULL;
	while (Manager::StageObjectMgr()->ListLoop(&obj)) {

		//�X�e�[�W�I�u�W�F�N�g�ɕϊ����Ď�ނ����v
		CStageObject *stg = (CStageObject *)obj;
		if (stg->GetType() != CStageObject::TYPE::PILE) continue;

		//�����߂荞�ݗʂɐݒ�
		CPile *pPile = (CPile *)stg;
		pPile->m_TrunkHeight = pPile->m_TrunkHeightOld = pPile->m_StartTrunkHeight;
	}
}

//===============================
//�����蔻����擾
//�����P�@���S�ʒu
//�����Q�@���S�ʒu�i�O��
//�����R�@����
//===============================
void CPile::GetCollisionInfo(Pos3D& CenterPos, Pos3D& CenterPosOld, float& height)
{
	height = m_height * 0.5f;

	const Pos2D Upper = Pos2D(m_pos.x, m_pos.y + height + m_fEvenTrunkCorrHeight);
	const Pos2D Lower = Pos2D(m_pos.x, m_pos.y - height + m_fEvenTrunkCorrHeight);

	CFloat center = m_pos.y + m_fEvenTrunkCorrHeight;

	CenterPos    = Pos3D(m_pos.x, m_pos.y + m_fEvenTrunkCorrHeight, 0.0f);
	CenterPosOld = Pos3D(m_posOld.x, m_posOld.y + m_fEvenTrunkCorrHeight, 0.0f);
}
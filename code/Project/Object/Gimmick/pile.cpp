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

//=======================================
// �R���X�g���N�^
//=======================================
CPile::CPile()
{
	Manager::BlockMgr()->AddList(this);

	//�������
	m_type = TYPE::PILE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;

	m_BodyModelID = NONEDATA;
	m_TrunkModelID = NONEDATA;
	m_PilePos = INITVECTOR3D;	//�{�̍��W
	m_NumTrunk = 0;			//���̐��i�Œ�R�j
	m_TrunkHeight = 0.0f;	//�����W
	m_TrunkHeightOld = 0.0f;//�����W
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
	m_BodyModelID = RNLib::Model().Load("data\\MODEL\\Block.x");
	m_TrunkModelID = RNLib::Model().Load("data\\MODEL\\Trunk.x");

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
	if (RNLib::Input().GetKeyTrigger(DIK_L)) m_TrunkHeight = 0.0f;

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
	//�{�̃��f��
	RNLib::Model().Put(m_PilePos, INITD3DXVECTOR3, m_BodyModelID, false);

	//�����W
	Pos3D PilePos = m_PilePos;

	//��ԏ�̊�����z�u���邽�߁A�z�u���鍂�����v�Z
	{
		//�����̔����i�؂�̂āj
		const int nNumHalf = m_NumTrunk / EVENPARITY;

		//�{�̃T�C�Y  �{  �T�C�Y �~ ��̊���  �{  ���̂߂荞�ݗ�
		PilePos.y = SIZE_OF_1_SQUARE * -0.5f + SIZE_OF_1_SQUARE * nNumHalf + m_TrunkHeight;

		RNLib::Text2D().PutDebugLog(CreateText("�Y�ʒu  Y:%f  �߂荞��:%f", PilePos.y, m_TrunkHeight));
	}

	for (int nCntPile = 0; nCntPile < m_NumTrunk; nCntPile++)
	{
		//�����f��
		RNLib::Model().Put(PilePos, INITD3DXVECTOR3, m_TrunkModelID);

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
//�����R  TrunkHeight�F���̂߂荞�ݍ��W�i�߂荞�݂Ȃ� = 0.0f�j
//===============================
void CPile::Set(Pos3D pos, int NumTrunk, float TrunkHeight)
{
	//�������Œᐔ��������Ă�����C��
	if (NumTrunk < MIN_TRUNK) NumTrunk = MIN_TRUNK;

	//���ݒ�
	m_PilePos = pos;
	m_NumTrunk = NumTrunk;
	m_TrunkHeight = TrunkHeight;

	//�����蔻��̍������Đݒ�
	m_height = SIZE_OF_1_SQUARE * NumTrunk;

	//���f���z�u
	PutModel();
}

//===============================
//�߂荞�ݗʔ��f����
//===============================
void CPile::CaveInTrunkHeight(float fCaveInHeight)
{
	//�ւ��ݗʔ��f
	fCaveInHeight += m_TrunkHeight;

	//�Y�̍ő�E�Œፂ�����v�Z
	const int nNumHalf = m_NumTrunk / EVENPARITY;
	const float fMaxHeight = SIZE_OF_1_SQUARE * nNumHalf;
	const float fMinHeight = -SIZE_OF_1_SQUARE * nNumHalf;

	//�Y�������Ȃ��悤�ɒ���
	if (fCaveInHeight < fMinHeight)		fCaveInHeight = fMinHeight;
	else if (fCaveInHeight > fMaxHeight)fCaveInHeight = fMaxHeight;

	//�O�����ۑ�
	SetOld(fCaveInHeight);

	//���f���z�u
	PutModel();
}

//===============================
//�O�����ۑ�
//===============================
void CPile::SetOld(float fCaveInHeight)
{
	//�߂荞�ݗʂ𔽉f������
	m_TrunkHeightOld = m_TrunkHeight;
	m_TrunkHeight = fCaveInHeight;

	m_pos = D3DXVECTOR3(m_PilePos.x, m_PilePos.y + m_TrunkHeight, 0.0f);
	m_posOld = D3DXVECTOR3(m_PilePos.x, m_PilePos.y + m_TrunkHeightOld, 0.0f);
}

//===============================
//�ʒu�擾�i�߂荞�ݗʂ��܂߂�j
//===============================
D3DXVECTOR3 CPile::GetPosCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_PilePos;//���ݍ��W���i�[
	ReturnPos.y += m_TrunkHeight; //�߂荞�ݗʂ𔽉f
	return ReturnPos;			  //���W��Ԃ�
}

//===============================
//�O��ʒu�擾�i�߂荞�ݗʂ��܂߂�j
//===============================
D3DXVECTOR3 CPile::GetPosOldCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_PilePos;	//���ݍ��W���i�[
	ReturnPos.y += m_TrunkHeightOld;//�O��̂߂荞�ݗʂ𔽉f
	return ReturnPos;				//���W��Ԃ�
}
//========================================
// 
// ���l�n����
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"
#include <iostream>
#include <initializer_list>
#include <random>

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�i���o�[�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ����������
//========================================
void CNumber::Init(void) {

}

//========================================
// �I������
//========================================
void CNumber::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CNumber::Update(void) {

}

//========================================
// FloatControl�֐� - float�^�̒l�̐��䏈�� -
// Author:RIKU NISHIMURA
//========================================
void FloatControl(float *pData, float fMax, float fMin)
{
	if (*pData > fMax)
	{// �ő�l�������Ă������A
		// �ő�l����
		*pData = fMax;
	}
	else if (*pData < fMin)
	{// �ŏ��l��������Ă������A
		// �ŏ��l����
		*pData = fMin;
	}
}

//========================================
// FloatLoopControl�֐� - float�^�̒l�̃��[�v���䏈�� -
// Author:RIKU NISHIMURA
//========================================
void FloatLoopControl(float *pData, float fMax, float fMin) {
	float fRange = fMax - fMin;
	while (*pData < fMin) {
		*pData += fRange;
	}
	while (*pData >= fMax) {
		*pData -= fRange;
	}
}

//========================================
// IntControl�֐� - int�^�̒l�̐��䏈�� -
// Author:RIKU NISHIMURA
//========================================
void IntControl(int *pData, int nMax, int nMin)
{
	if (*pData > nMax)
	{// �ő�l�������Ă������A
		// �ő�l����
		*pData = nMax;
	}
	else if (*pData < nMin)
	{// �ŏ��l��������Ă������A
		// �ŏ��l����
		*pData = nMin;
	}
}

//========================================
// IntLoopControl�֐� - int�^�̒l�̃��[�v���䏈�� -
// Author:RIKU NISHIMURA
//========================================
void IntLoopControl(int *pData, int nMax, int nMin) {
	int nRange = nMax - nMin;
	while (*pData < nMin) {
		*pData += nRange;
	}
	while (*pData >= nMax) {
		*pData -= nRange;
	}
}

//========================================
// IntControlReturn�֐� - int�^�̒l�̐��䏈��(���ʂ�Ԃ�) -
// Author:RIKU NISHIMURA
//========================================
int IntControlReturn(int nData, int nMax, int nMin)
{
	if (nData > nMax)
	{// �ő�l�������Ă������A
		// �ő�l����
		nData = nMax;
	}
	else if (nData < nMin)
	{// �ŏ��l��������Ă������A
		// �ŏ��l����
		nData = nMin;
	}

	return nData;
}

//========================================
// IntLoopControlReturn�֐� - int�^�̒l�̃��[�v���䏈��(���ʂ�Ԃ�) -
// Author:RIKU NISHIMURA
//========================================
int IntLoopControlReturn(int nData, int nMax, int nMin)
{
	if (nData >= nMax)
	{// �ő�l�ȏ�̎��A
		// �ŏ��l����
		nData = nMin;
	}
	else if (nData < nMin)
	{// �ŏ��l��������Ă������A
		// �ő�l����
		nData = nMax - 1;
	}

	return nData;
}

//========================================
// Lottery�֐� - ���I���� -
// Author:RIKU NISHIMURA
//========================================
bool Lottery(float fProb)
{
	if (fProb > (0.1f * (rand() % 1000)))
	{// ���I�������A
		// �^��Ԃ�
		return true;
	}
	else
	{// ���I���Ȃ��������A
		// �U��Ԃ�
		return false;
	}
}

//========================================
// fRand�֐� - �����̗�����Ԃ� -
// Author:RIKU NISHIMURA
//========================================
float fRand(void) {
	return (float)rand() / RAND_MAX;
}

//========================================
// Rand�֐� - ������Ԃ� -
// Author:RIKU NISHIMURA
//========================================
int Rand(int nMin, int nMax) 
{
	return nMin + (rand() % (nMax - nMin));
}

//========================================
// Tick�֐� - �l���Ԋu���݂ŕԂ� -
//========================================
float Tick(float fData, float fSpace)
{
	return (int)(fData / fSpace) * fSpace;
}

//========================================
// Sign�֐� - �^����ꂽ�l�ɉ�����1�𕄍��t���ŕԂ� -
// Author:RIKU NISHIMURA
//========================================
int Sign(float fData)
{
	if (fData == 0)
	{// ������0���������A
		// 0��Ԃ�
		return 0;
	}
	
	return fData > 0.0f ? 1 : -1;
}

//========================================
// MaxOfZero�֐� - �^����ꂽ�l��0�ȉ��̎���0���A0�ȏ�̎��͒l�����̂܂ܕԂ� -
// Author:RIKU NISHIMURA
//========================================
float MaxOfZero(float fData) 
{
	if (fData <= 0.0f)
	{
		return 0.0f;
	}
	return fData;
}

//========================================
// �^����ꂽ�����l�������_���ɕԂ�
// Author:RIKU NISHIMURA
//========================================
int OR(std::initializer_list<int> args) {
	int randomIndex = std::rand() % args.size();

	auto it = args.begin();
	std::advance(it, randomIndex);

	return *it;
}

//========================================
// �����_�����X�g���쐬
// Author:RIKU NISHIMURA
//========================================
int* CreateRandList(int num) {
	int* pList = NULL;
	CMemory::Alloc<int>(&pList, num);
	for (int cnt = 0; cnt < num; cnt++) {
		pList[cnt] = cnt;
	}
	for (int cnt = 0; cnt < num; cnt++) {
		int nSwap = cnt + rand() % (num - cnt);
		int nTemp = pList[cnt];
		pList[cnt]  = pList[nSwap];
		pList[nSwap] = nTemp;
	}

	return pList;
}

//========================================
// float������r����
//========================================
bool EqualFloat(const float& numA, const float& numB, const float& allowableError) {
	return (fabs(numA - numB) <= allowableError * fmax(1, fmax(fabs(numA), fabs(numB))));
}

//========================================
// ������1��Ԃ�
//========================================
float GetPlusMinus(const float& num) {
	if (num == 0.0f)
		return 1.0f;
	return num / fabsf(num);
}

//========================================
// �܂�Ԃ��������擾
//========================================
int GetTurnNum(const int& num, const int& max) {

	if ((num / max) % 2 == 0)
	{// �l������ŏ��Z�������ʂ��������������A
		// �l������ŏ�]�Z�������ʂ�Ԃ�
		return num % max;
	}
	else
	{//�p�^�[��������ŏ��Z�������ʂ�����������A
		//�������p�^�[��������ŏ�]�Z�������ʂ����Z�������ʂ�Ԃ�
		return max - (num % max);
	}
}
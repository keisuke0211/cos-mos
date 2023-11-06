//========================================
// 
// 数値処理
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ number.cpp ]]]
//========================================
#include "../../RNlib.h"
#include <iostream>
#include <initializer_list>
#include <random>

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// Counter関数 - カウント処理 -
// Author:RIKU NISHIMURA
//========================================
int Counter(int *pData, int nMin, int nMax, int *pCounter, int nCountTime, COUNTTYPE countType)
{
	if (nCountTime != -1)
	{// カウントにかかる時間が-1でない時、
		if (pCounter == NULL) 
		{// カウンターのポインタがNULLの時、
			*pData += 1;	// 値を加算
		}
		else
		{// カウンターのポインタがNULLでない時、
			*pCounter += 1;	// カウンターを加算

			if (*pCounter >= nCountTime)
			{// カウンターがカウントにかかる時間に達した時、
				*pCounter = 0;	// カウンターを初期化する
				*pData += 1;	// 値を加算
			}
		}
	}
	else
	{// カウントにかかる時間が-1の時、
		// 値をそのまま返す
		return *pData;
	}

	// カウントの種類に応じた値を返す
	switch (countType)
	{
	case COUNTTYPE_NORMAL:
		
		// 値を上限で剰余算した結果を返す
		return nMin + (*pData %= (nMax - nMin + 1));
	case COUNTTYPE_TURN:

		if ((*pData / (nMax - nMin)) % 2 == 0)
		{// 値を上限で除算した結果が偶数だった時、
			// 値を上限で剰余算した結果を返す
			return nMin + (*pData %= (nMax - nMin));
		}
		else
		{//パターンを上限で除算した結果が奇数だった時、
			//上限からパターンを上限で剰余算した結果を減算した結果を返す
			return nMax - (*pData % (nMax - nMin));
		}

	default:
		assert(false);
		return 0;
	}
}

//========================================
// IntConsumption関数 - int型の値の消費処理 -
// Author:RIKU NISHIMURA
//========================================
bool IntConsumption(int *pData, int nSubtract) 
{
	if ((*pData -= nSubtract) >= 0)
	{// 値から消費量を減算した結果0以上の時、
		return true;	// 真を返す
	}
	else 
	{// 値から消費量を減算した結果0を下回っている時、
		*pData += nSubtract;	// 減算した分加算
		return false;			// 偽を返す
	}
}

//========================================
// FloatControl関数 - float型の値の制御処理 -
// Author:RIKU NISHIMURA
//========================================
void FloatControl(float *pData, float fMax, float fMin)
{
	if (*pData > fMax)
	{// 最大値を上回っていた時、
		// 最大値を代入
		*pData = fMax;
	}
	else if (*pData < fMin)
	{// 最小値を下回っていた時、
		// 最小値を代入
		*pData = fMin;
	}
}

//========================================
// FloatLoopControl関数 - float型の値のループ制御処理 -
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
// IntControl関数 - int型の値の制御処理 -
// Author:RIKU NISHIMURA
//========================================
void IntControl(int *pData, int nMax, int nMin)
{
	if (*pData > nMax)
	{// 最大値を上回っていた時、
		// 最大値を代入
		*pData = nMax;
	}
	else if (*pData < nMin)
	{// 最小値を下回っていた時、
		// 最小値を代入
		*pData = nMin;
	}
}

//========================================
// IntLoopControl関数 - int型の値のループ制御処理 -
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
// IntControlReturn関数 - int型の値の制御処理(結果を返す) -
// Author:RIKU NISHIMURA
//========================================
int IntControlReturn(int nData, int nMax, int nMin)
{
	if (nData > nMax)
	{// 最大値を上回っていた時、
		// 最大値を代入
		nData = nMax;
	}
	else if (nData < nMin)
	{// 最小値を下回っていた時、
		// 最小値を代入
		nData = nMin;
	}

	return nData;
}

//========================================
// IntLoopControlReturn関数 - int型の値のループ制御処理(結果を返す) -
// Author:RIKU NISHIMURA
//========================================
int IntLoopControlReturn(int nData, int nMax, int nMin)
{
	if (nData >= nMax)
	{// 最大値以上の時、
		// 最小値を代入
		nData = nMin;
	}
	else if (nData < nMin)
	{// 最小値を下回っていた時、
		// 最大値を代入
		nData = nMax - 1;
	}

	return nData;
}

//========================================
// Lottery関数 - 抽選処理 -
// Author:RIKU NISHIMURA
//========================================
bool Lottery(float fProb)
{
	if (fProb > (0.1f * (rand() % 1000)))
	{// 当選した時、
		// 真を返す
		return true;
	}
	else
	{// 当選しなかった時、
		// 偽を返す
		return false;
	}
}

//========================================
// fRand関数 - 小数の乱数を返す -
// Author:RIKU NISHIMURA
//========================================
float fRand(void) {
	return (float)rand() / RAND_MAX;
}

//========================================
// Rand関数 - 乱数を返す -
// Author:RIKU NISHIMURA
//========================================
int Rand(int nMin, int nMax) 
{
	return nMin + (rand() % (nMax - nMin));
}

//========================================
// Tick関数 - 値を間隔刻みで返す -
//========================================
float Tick(float fData, float fSpace)
{
	return (int)(fData / fSpace) * fSpace;
}

//========================================
// Sign関数 - 与えられた値に応じて1を符号付きで返す -
// Author:RIKU NISHIMURA
//========================================
int Sign(float fData)
{
	if (fData == 0)
	{// 引数が0だった時、
		// 0を返す
		return 0;
	}
	
	return fData > 0.0f ? 1 : -1;
}

//========================================
// MaxOfZero関数 - 与えられた値が0以下の時は0を、0以上の時は値をそのまま返す -
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
// 与えられた整数値をランダムに返す
// Author:RIKU NISHIMURA
//========================================
int OR(std::initializer_list<int> args) {
	int randomIndex = std::rand() % args.size();

	auto it = args.begin();
	std::advance(it, randomIndex);

	return *it;
}

//========================================
// ランダムリストを作成
// Author:RIKU NISHIMURA
//========================================
int* CreateRandList(int num) {
	int* pList = NULL;
	RNLib::Memory().Alloc<int>(&pList, num);
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
//========================================
// 
// 数値系処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// 関数定義
//****************************************
// ナンバークラス
class CNumber {
public:
	//========== [[[ 関数宣言 ]]]
	void Init  (void);
	void Uninit(void);
	void Update(void);
	// 範囲内制御
	template<class T>void Clamp(T* num, const T& max, const T& min) {
		if (!FindIsNumber(*num)) {
			assert(false);
			return;
		}

		if (*num < min)
			*num = min;
		else if (*num > max)
			*num = max;
	}
	// 範囲内ループ制御
	template<class T>void LoopClamp(T* num, const T& max, const T& min) {
		if (!FindIsNumber(*num)) {
			assert(false);
			return;
		}
		else if (min >= max) {
			assert(false);
			return;
		}

		T range = max - min + 1;

		while (*num < min)
			*num += range;
		while (*num > max)
			*num -= range;
	}
	// 型が数値か調べる
	template<class T>bool FindIsNumber(const T& data) {
		return (
			typeid(T) == typeid(char)   ||
			typeid(T) == typeid(int)    ||
			typeid(T) == typeid(UInt)   ||
			typeid(T) == typeid(short)  ||
			typeid(T) == typeid(UShort) ||
			typeid(T) == typeid(long)   ||
			typeid(T) == typeid(ULong)  ||
			typeid(T) == typeid(float));
	}
};

//****************************************
// プロトタイプ宣言
//****************************************

bool  Lottery(float fProb);
float fRand(void);
int   Rand(int nMin, int nMax);
float Tick(float fData, float fSpace);
int   Sign(float fData);
float MaxOfZero(float fData);
int   OR(std::initializer_list<int> args);
int*  CreateRandList(int nNum);
bool  EqualFloat(const float& numA, const float& numB, const float& allowableError);
float GetPlusMinus(const float& num);
int   GetTurnNum(const int& num, const int& max);
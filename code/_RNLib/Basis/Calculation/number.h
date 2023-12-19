//========================================
// 
// ���l�n����
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/memory.h"

//****************************************
// �֐���`
//****************************************
// �i���o�[�N���X
class _RNC_Number {
public:
	//========== [[[ �֐��錾 ]]]
	void  Init            (void);
	void  Uninit          (void);
	void  Update          (void);
	bool  Lottery         (const float& prob)                                                                 { return (prob > (0.001f * (rand() % 100000))); }
	float GetRandomFloat  (const float& max)                                                                  { return ((float)rand() / RAND_MAX) * max; }
	//----- float�l������r
	bool  GetIsEq     /* == */(const float& numA, const float& numB, const float& allowableError = 0.000001f) { return (fabs(numA - numB) <= allowableError * fmax(1, fmax(fabs(numA), fabs(numB)))); }
	bool  GetIsNoEq   /* != */(const float& numA, const float& numB, const float& allowableError = 0.000001f) { return (fabs(numA - numB) <= allowableError * fmax(1, fmax(fabs(numA), fabs(numB)))); }
	bool  GetIsGtOrEq /* >= */(const float& numA, const float& numB, const float& allowableError = 0.000001f) { return (numA > numB) ||  GetIsEq(numA, numB, allowableError); }
	bool  GetIsGt     /* >  */(const float& numA, const float& numB, const float& allowableError = 0.000001f) { return (numA > numB) && !GetIsEq(numA, numB, allowableError); }
	bool  GetIsLsOrEq /* <= */(const float& numA, const float& numB, const float& allowableError = 0.000001f) { return (numA < numB) ||  GetIsEq(numA, numB, allowableError); }
	bool  GetIsLs     /* <  */(const float& numA, const float& numB, const float& allowableError = 0.000001f) { return (numA < numB) && !GetIsEq(numA, numB, allowableError); }
	//-----
	float RoundUpFloat        (const float& num, const float& interval)                                       { return (long)((num / interval) + 1) * interval; }
	float RoundDownFloat      (const float& num, const float& interval)                                       { return (long)(num / interval) * interval; }

	//========== [[[ �֐���` ]]]
	// �͈͓�����
	template<class T, class T2, class T3>void Clamp(T* num, const T2& max, const T3& min) {
		if (!FindIsNumber(*num) || !FindIsNumber(max) || !FindIsNumber(min)) { assert(false); return; }

		if (*num < min)
			*num = min;
		else if (*num > max)
			*num = max;
	}
	// �͈͓����[�v����
	template<class T, class T2, class T3>void LoopClamp(T* num, const T2& max, const T3& min) {
		if (!FindIsNumber(*num) || !FindIsNumber(max) || !FindIsNumber(min)) { assert(false); return; }
		else if (min > max) {
			assert(false);
			return;
		}

		T range = (max + FindIsInteger(*num)) - min;

		while (*num < min)
			*num += range;
		while (*num > max)
			*num -= range;
	}
	// �n���ꂽ�l�������_���ɕԂ�
	template<class T>T OR(std::initializer_list<T> args) {
		int randomIndex = std::rand() % args.size();

		auto it = args.begin();
		std::advance(it, randomIndex);

		return *it;
	}
	// ������1��Ԃ�
	template<class T>float GetPlusMinus(const T& num) {
		if (!FindIsNumber(num)) { assert(false); return 1.0f; }
		const float castNum = (float)num;

		if (castNum == 0.0f)
			return 1.0f;

		return castNum / fabsf(castNum);
	}
	// �܂�Ԃ��������擾
	template<class T, class T2>T GetTurnNum(const T& num, const T2& max) {
		if (!FindIsInteger(num) || !FindIsInteger(max)) { assert(false); return 1.0f; }

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
	// �����_���Ȑ����l���擾
	template<class T>T GetRandomInteger(const T& max) {
		if (!FindIsNumber(max)) { assert(false); return 0; }

		return max == 0 ? 0 : (T)(GetRandomFloat(1.0f) * max);
	}
	// �����_�����X�g���쐬
	template<class T>T* CreateRandomList(const T& num) {
		T* list = NULL;
		RNLib::Memory().Alloc(&list, num);
		for (T cnt = 0; cnt < num; cnt++) {
			list[cnt] = cnt;
		}
		for (T cnt = 0; cnt < num; cnt++) {
			T swapIdx = cnt + rand() % (num - cnt);
			T temp = list[cnt];
			list[cnt] = list[swapIdx];
			list[swapIdx] = temp;
		}

		return list;
	}
	// �^�����l�����ׂ�
	template<class T>static bool FindIsNumber(const T& num) {
		return (
			FindIsInteger(num) ||
			typeid(T) == typeid(float));
	}
	// �^�������l�����ׂ�
	template<class T>static bool FindIsInteger(const T& num) {
		return (
			typeid(T) == typeid(char)   ||
			typeid(T) == typeid(UChar)  ||
			typeid(T) == typeid(int)    ||
			typeid(T) == typeid(UInt)   ||
			typeid(T) == typeid(short)  ||
			typeid(T) == typeid(UShort) ||
			typeid(T) == typeid(long)   ||
			typeid(T) == typeid(ULong));
	}
};
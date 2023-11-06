//========================================
// 
// �������֘A����
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"
#include "object.h"

//****************************************
// �N���X��`
//****************************************
class CMemory {
public:
	//========== [[[ �֐��錾 ]]]
	std::mutex& GetMutex(void) { return m_memoryMutex; }

	//========================================
	// �������m�ۏ���
	//========================================
	template <class T>void Alloc(T** alloc, const int& num = 1, const bool& isMutex = false) {
		
		// ������Ă���
		Release(alloc);

		// ����0�ȉ��̎��A�I��
		if (num <= 0)
			return;

		// ��������̈�Mutex�����b�N
		if (isMutex)
			std::lock_guard<std::mutex> lock(m_memoryMutex);

		// �������m�ۂ���
		*alloc = new T[num];
	}

	//========================================
	// �������m�ۏ���(�T�C�Y�w��)
	//========================================
	void Alloc(void** alloc, const size_t& size, const bool& isMutex = true) {
		
		// ������Ă���
		Release(alloc);

		// ��������̈�Mutex�����b�N
		if (isMutex)
			std::lock_guard<std::mutex> lock(m_memoryMutex);

		// �������m�ۂ���
		*alloc = malloc(size);
	}

	//========================================
	// �������Ċm�ۏ���
	//========================================
	template <class T>void ReAlloc(T** alloc, const int& beforeNum, const int& afterNum, const bool& isMutex = false) {

		// �V��������0�ȉ��̎��A������ďI��
		if (afterNum <= 0) {
			Release(alloc);
			return;
		}

		// �R�s�[�����V���������z���Ȃ��悤����
		int copyNum = beforeNum;
		if (copyNum > afterNum)
			copyNum = afterNum;

		// �V�������������m�ۂ���
		T* newAlloc = NULL;
		Alloc<T>(&newAlloc, afterNum, isMutex);

		// �ߋ��̐���0���z���Ă��鎞�A���������R�s�[����
		if (beforeNum > 0) {
			if (*alloc != NULL)
				memcpy(newAlloc, *alloc, sizeof(T) * copyNum);
		}

		// �Â��̈��������A�V�����|�C���^�ɓ���ւ���
		Release(alloc);
		*alloc = newAlloc;
	}

	//========================================
	// �������Ċm�ۏ���(Double)
	//========================================
	template <class T>void ReAllocDouble(T*** alloc, const int& beforeNum, const int& afterNum, const bool& isMutex = false) {

		// [[[ �����Ă��鎞 ]]]
		if (beforeNum > afterNum) {

			// ���������̃�������������Ă���
			for (int cnt = beforeNum - 1; cnt >= afterNum; cnt--) {
				Release(&*alloc[cnt]);
			}
		}

		// �|�C���^���������Ċm��
		ReAlloc(alloc, beforeNum, afterNum, isMutex);

		// [[[ �����Ă��鎞 ]]]
		if (beforeNum < afterNum) {

			// ���������̃��������m�ۂ��Ă���
			T** allocTemp = *alloc;
			for (int cnt = beforeNum; cnt < afterNum; cnt++) {
				allocTemp[cnt] = NULL;
				Alloc(&allocTemp[cnt], 1, isMutex);
			}
		}
	}

	//========================================
	// �������������
	//========================================
	template <class T>void Release(T** release) {

		// NULL�Ȃ������Ȃ�
		if (*release == NULL)
			return;

		// ���
		delete[] (*release);
		*release = NULL;
	}

	//========================================
	// �������������(Double)
	//========================================
	template <class T>void ReleaseDouble(T*** release, const int& num) {

		// NULL�Ȃ������Ȃ�
		if (*release == NULL)
			return;

		// �m�ۂ��ꂽ�����������ׂĉ��
		T** releaseTemp = *release;
		for (int cntData = 0; cntData < num; cntData++)
			Release(&releaseTemp[cntData]);
		Release(&releaseTemp);
	}

private:
	//========== [[[ �ϐ��錾 ]]]
	std::mutex m_memoryMutex;
};
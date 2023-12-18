//========================================
// 
// ������N���X
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// �N���X��`
//****************************************
// ������N���X(�ϐ��^)
class String {
public:
	//========== [[[ �֐���` ]]]
	// �R���X�g���N�^
	String() {

		m_string = NULL;
	}
	String(const char* format, ...) {

		if (format == NULL) {
			m_string = NULL;
			return;
		}

		// �ϒ������̂��߂̃��X�g
		va_list args;
		int size;

		// �ϒ������̏������n�߂�
		va_start(args, format);

		// �o�͂��镶����̃T�C�Y���擾����
		size = vsnprintf(NULL, 0, format, args);

		// �ϒ������̏������I������
		va_end(args);

		// ������̃T�C�Y�ɍ��킹���������𓮓I�Ɋm�ۂ���
		m_string = (char*)malloc(size + 1);

		// �ϒ������̏������ēx�n�߂�
		va_start(args, format);

		// ������������o��
		vsprintf(m_string, format, args);

		// �ϒ������̏������I������
		va_end(args);
	}
	// �f�X�g���N�^
	~String() {

		if (m_string != NULL)
			free(m_string);
	}
	// char�^�|�C���^���擾
	char*& GetCharPointer(void) { return m_string; }
	// �ϊ�
	operator char* () const {
		return m_string;
	}
	// ���
	char*& operator=(const String& other) {

		if (m_string != NULL)
			free(m_string);

		// �����o�ϐ��Ȃǂ��R�s�[���鏈�����s��
		if (this != &other) {

			// ������̒����ɉ����ă��������m�ۂ���
			int nStrLen = strlen(other.m_string);
			m_string = (char*)malloc(sizeof(char) * (nStrLen + 1));

			// ��������R�s�[����
			strcpy(m_string, other.m_string);
		}
		return m_string;
	}

private:
	//========== [[[ �ϐ��錾 ]]]
	char* m_string;
};
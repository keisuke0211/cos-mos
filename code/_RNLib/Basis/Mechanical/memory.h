//========================================
// 
// メモリ関連処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"
#include "object.h"

//****************************************
// クラス定義
//****************************************
class CMemory {
public:
	//========== [[[ 関数宣言 ]]]
	CMemory();
	~CMemory();
	void Init(void);
	void Uninit(void);
	void Update(void);
	static std::mutex& GetMutex(void) { return m_memoryMutex; }

	//========================================
	// メモリ確保処理
	//========================================
	template <class T>static void Alloc(T** alloc, const int& num = 1, const bool& isMutex = false) {
		
		// 解放しておく
		Release(alloc);

		// 数が0以下の時、終了
		if (num <= 0)
			return;

		// 競合回避の為Mutexをロック
		if (isMutex)
			std::lock_guard<std::mutex> lock(m_memoryMutex);

		// メモリ確保する
		*alloc = new T[num];
	}

	//========================================
	// メモリ再確保処理(Double)
	//========================================
	template <class T>static void AllocDouble(T*** alloc, const int& num, const bool& isMutex = false) {

		// ポインタメモリを再確保
		Alloc(alloc, num, isMutex);

		// 増えた分のメモリを確保していく
		T** allocTemp = *alloc;
		for (int cnt = 0; cnt < num; cnt++) {
			allocTemp[cnt] = NULL;
			Alloc(&allocTemp[cnt], 1, isMutex);
		}
	}

	//========================================
	// メモリ確保処理(サイズ指定)
	//========================================
	static void Alloc(void** alloc, const size_t& size, const bool& isMutex = true) {
		
		// 解放しておく
		Release(alloc);

		// 競合回避の為Mutexをロック
		if (isMutex)
			std::lock_guard<std::mutex> lock(m_memoryMutex);

		// メモリ確保する
		*alloc = malloc(size);
	}

	//========================================
	// メモリ再確保処理
	//========================================
	template <class T>static void ReAlloc(T** alloc, const int& beforeNum, const int& afterNum, const bool& isMutex = false) {

		// 新しい数が0以下の時、解放して終了
		if (afterNum <= 0) {
			Release(alloc);
			return;
		}

		// コピー数が新しい数を越えないよう制御
		int copyNum = beforeNum;
		if (copyNum > afterNum)
			copyNum = afterNum;

		// 新しくメモリを確保する
		T* newAlloc = NULL;
		Alloc<T>(&newAlloc, afterNum, isMutex);

		// 過去の数が0を越えている時、メモリをコピーする
		if (beforeNum > 0) {
			if (*alloc != NULL)
				memcpy(newAlloc, *alloc, sizeof(T) * copyNum);
		}

		// 古い領域を解放し、新しいポインタに入れ替える
		Release(alloc);
		*alloc = newAlloc;
	}

	//========================================
	// メモリ再確保処理(Double)
	//========================================
	template <class T>static void ReAllocDouble(T*** alloc, const int& beforeNum, const int& afterNum, const bool& isMutex = false) {

		// [[[ 減っている時 ]]]
		if (beforeNum > afterNum) {

			// 減った分のメモリを解放していく
			for (int cnt = beforeNum - 1; cnt >= afterNum; cnt--) {
				Release(&(*alloc)[cnt]);
			}
		}

		// ポインタメモリを再確保
		ReAlloc(alloc, beforeNum, afterNum, isMutex);

		// [[[ 増えている時 ]]]
		if (beforeNum < afterNum) {

			// 増えた分のメモリを確保していく
			T** allocTemp = *alloc;
			for (int cnt = beforeNum; cnt < afterNum; cnt++) {
				allocTemp[cnt] = NULL;
				Alloc(&allocTemp[cnt], 1, isMutex);
			}
		}
	}

	//========================================
	// メモリ解放処理
	//========================================
	template <class T>static void Release(T** release) {

		// NULLなら解放しない
		if (*release == NULL)
			return;

		// 解放
		delete[] (*release);
		*release = NULL;
	}

	//========================================
	// メモリ解放処理(Double)
	//========================================
	template <class T>static void ReleaseDouble(T*** release, const int& num) {

		// NULLなら解放しない
		if (*release == NULL)
			return;

		// 確保されたメモリをすべて解放
		T** releaseTemp = *release;
		for (int cntData = 0; cntData < num; cntData++)
			Release(&releaseTemp[cntData]);
		Release(&releaseTemp);
	}

private:
	//========== [[[ 変数宣言 ]]]
	static std::mutex m_memoryMutex;
};
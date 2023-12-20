//========================================
// 
// �F�N���X
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Calculation/number.h"

//****************************************
// �}�N����`
//****************************************
#define COLOR_RED       Color{255,0  ,0  ,255}
#define COLOR_GREEN     Color{0  ,255,0  ,255}
#define COLOR_BLUE      Color{0  ,0  ,255,255}
#define COLOR_YELLOW    Color{255,255,0  ,255}
#define COLOR_MAGENTA   Color{255,0  ,255,255}
#define COLOR_CYAN      Color{0  ,255,255,255}
#define COLOR_WHITE     Color{255,255,255,255}
#define COLOR_WHITE_TRA Color{255,255,255,127}
#define COLOR_BLACK     Color{0  ,0  ,0  ,255}

//****************************************
// �N���X��`
//****************************************
// �F�N���X
class Color {
public:
	//========== [[[ �֐��錾 ]]]
	Color() {
		r = g = b = a = 0;
	}

	// �ݒ�
	template <class R, class G, class B, class A>Color(const R& setR, const G& setG, const B& setB, const A& setA) {
		
		if (!_RNC_Number::FindIsNumber(setR)) { assert(false); return; }
		if (!_RNC_Number::FindIsNumber(setG)) { assert(false); return; }
		if (!_RNC_Number::FindIsNumber(setB)) { assert(false); return; }
		if (!_RNC_Number::FindIsNumber(setA)) { assert(false); return; }
		ULong convR = (ULong)setR;
		ULong convG = (ULong)setG;
		ULong convB = (ULong)setB;
		ULong convA = (ULong)setA;
		convR = convR > 255 ? 255 : convR;
		convG = convG > 255 ? 255 : convG;
		convB = convB > 255 ? 255 : convB;
		convA = convA > 255 ? 255 : convA;
		r = convR;
		g = convG;
		b = convB;
		a = convA;
	}
	// �ϊ�
	operator D3DCOLOR     () const { return D3DCOLOR_RGBA(r, g, b, a); }
	operator D3DXCOLOR    () const { return D3DXCOLOR((float)r / 255, (float)g / 255, (float)b / 255, (float)a / 255); }
	operator D3DCOLORVALUE() const { return D3DXCOLOR((float)r / 255, (float)g / 255, (float)b / 255, (float)a / 255); }
	// ���Z
	Color operator +(const Color& other) const {
		Color col = *this;
		col.r += other.r;
		col.g += other.g;
		col.b += other.b;
		col.a += other.a;
		return col;
	}
	// ��Z
	Color operator *(const float& f) const {
		Color col = *this;
		col.r *= f;
		col.g *= f;
		col.b *= f;
		col.a *= f;
		return col;
	}
	// ���Z
	Color operator /(const float& f) const {
		Color col = *this;
		col.r /= f;
		col.g /= f;
		col.b /= f;
		col.a /= f;
		return col;
	}
	// ���Z���
	Color operator +=(Color& other) {
		this->r += other.r;
		this->g += other.g;
		this->b += other.b;
		this->a += other.a;
		return *this;
	}
	// ��Z���
	Color operator *=(const float& f) {
		this->r *= f;
		this->g *= f;
		this->b *= f;
		this->a *= f;
		return *this;
	}
	// ���Z���
	Color operator /=(const float& f) {
		this->r /= f;
		this->g /= f;
		this->b /= f;
		this->a /= f;
		return *this;
	}
	// ������r(������)
	bool operator ==(const Color& other) {
		return (
			(this->r == other.r) &&
			(this->g == other.g) &&
			(this->b == other.b) &&
			(this->a == other.a)
			);
	}
	// ������r(�������Ȃ�)
	bool operator !=(const Color& other) {
		return !(*this == other);
	}
	// �ݒ�
	Color Set(const D3DXCOLOR& other) {
		this->r = other.r * 255;
		this->g = other.g * 255;
		this->b = other.b * 255;
		this->a = other.a * 255;
		return *this;
	}
	// �������F���擾
	Color GetMixed(const Color& other) {
		Color selfCol = *this;
		selfCol.r *= (float)other.r / 255;
		selfCol.g *= (float)other.g / 255;
		selfCol.b *= (float)other.b / 255;
		selfCol.a *= (float)other.a / 255;
		return selfCol;
	}
	// ���邭�����F���擾
	Color GetBrighten(const float& brightness) {
		Color col = *this;
		col.r *= brightness;
		col.g *= brightness;
		col.b *= brightness;
		col.r = col.r > 255 ? 255 : col.r;
		col.g = col.g > 255 ? 255 : col.g;
		col.b = col.b > 255 ? 255 : col.b;
		col.a = col.a > 255 ? 255 : col.a;
		return col;
	}
	// �����ɂ����F���擾
	Color GetAlphaApplied(const float& alpha) {
		Color col = *this;
		col.a *= alpha;
		col.a = col.a > 255 ? 255 : col.a;
		return col;
	}
	
	//========== [[[ �ϐ��錾 ]]]
	UShort r;	// ��
	UShort g;	// ��
	UShort b;	// ��
	UShort a;	// �s�����x
};
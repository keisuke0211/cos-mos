//========================================
// 
// �F�̏����̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

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
		if (!(typeid(R) == typeid(int) || typeid(R) == typeid(UInt) || typeid(R) == typeid(short) || typeid(R) == typeid(UShort) || typeid(R) == typeid(long) || typeid(R) == typeid(ULong))) { 
			assert(false); 
			return; 
		}
		if (!(typeid(G) == typeid(int) || typeid(G) == typeid(UInt) || typeid(G) == typeid(short) || typeid(G) == typeid(UShort) || typeid(G) == typeid(long) || typeid(G) == typeid(ULong))) {
			assert(false);
			return;
		}
		if (!(typeid(B) == typeid(int) || typeid(B) == typeid(UInt) || typeid(B) == typeid(short) || typeid(B) == typeid(UShort) || typeid(B) == typeid(long) || typeid(B) == typeid(ULong))) {
			assert(false); 
			return;
		}
		if (!(typeid(A) == typeid(int) || typeid(A) == typeid(UInt) || typeid(A) == typeid(short) || typeid(A) == typeid(UShort) || typeid(A) == typeid(long) || typeid(A) == typeid(ULong))) {
			assert(false);
			return;
		}
		UShort convR = (UShort)setR;
		UShort convG = (UShort)setG;
		UShort convB = (UShort)setB;
		UShort convA = (UShort)setA;
		convR = convR > 255 ? 255 : convR;
		convG = convG > 255 ? 255 : convG;
		convB = convB > 255 ? 255 : convB;
		convA = convA > 255 ? 255 : convA;
		r = convR;
		g = convG;
		b = convB;
		a = convA;
	}
	// ���Z
	Color operator +(const Color& ovr) const {
		Color col = *this;
		col.r += ovr.r;
		col.g += ovr.g;
		col.b += ovr.b;
		col.a += ovr.a;
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
	Color operator +=(Color& col) {
		this->r += col.r;
		this->g += col.g;
		this->b += col.b;
		this->a += col.a;
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
	bool operator ==(const Color& col) {
		return (
			(this->r == col.r) &&
			(this->g == col.g) &&
			(this->b == col.b) &&
			(this->a == col.a)
			);
	}
	// ������r(�������Ȃ�)
	bool operator !=(const Color& col) {
		return !(*this == col);
	}
	// �ݒ�
	Color Set(const D3DXCOLOR& col) {
		this->r = col.r * 255;
		this->g = col.g * 255;
		this->b = col.b * 255;
		this->a = col.a * 255;
		return *this;
	}
	// �������F���擾
	Color GetMixed(const Color& col) {
		Color selfCol = *this;
		selfCol.r *= (float)col.r / 255;
		selfCol.g *= (float)col.g / 255;
		selfCol.b *= (float)col.b / 255;
		selfCol.a *= (float)col.a / 255;
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
	// �ϊ�
	D3DCOLOR ConvD3DCOLOR(void) const {
		return D3DCOLOR_RGBA(r, g, b, a);
	}
	D3DXCOLOR ConvD3DXCOLOR(void) const {
		return D3DXCOLOR((float)r / 255, (float)g / 255, (float)b / 255, (float)a / 255);
	}

	//========== [[[ �ϐ��錾 ]]]
	UShort r;	// ��
	UShort g;	// ��
	UShort b;	// ��
	UShort a;	// �s�����x
};
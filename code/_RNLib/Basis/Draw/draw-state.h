//========================================
// 
// �`���Ԃ̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �N���X��`
//****************************************
// �`���ԃN���X
class _RNC_DrawState {
public:
	//========== [[[ �񋓌^��` ]]]]
	// �J�����O���[�h
	enum class CULLING_MODE {
		FRONT_SIDE,	// �\��
		BACK_SIDE,	// ����
		BOTH_SIDES,	// ����
	};
	// �A���t�@�u�����h���[�h
	enum class ALPHA_BLEND_MODE {
		NORMAL,	// �ʏ�
		ADD,	// ���Z
	};
	// ��ԃ��[�h
	enum class INTERPOLATION_MODE {
		NONE,
		LINEAR,
	};

	//========== [[[ �֐��錾 ]]]
	_RNC_DrawState           ();
	~_RNC_DrawState          ();
	void Init                (Device& device);
	void Uninit              (void);
	void Update              (void);
	void ResetVariableSetting(Device& device);
	void SetIsZTest          (Device& device, const bool& isZTest);
	void SetCullingMode      (Device& device, const CULLING_MODE& mode);
	void SetAlphaBlendMode   (Device& device, const ALPHA_BLEND_MODE& mode);
	void SetInterpolationMode(Device& device, const INTERPOLATION_MODE& mode);
	void SetIsTextureAlpha   (Device& device, const bool& isTextureAlpha);
	void SetIsLighting       (Device& device, const bool& isLighting);
	void SetIsFog            (Device& device, const bool& isFog);
	void SetFogParameter     (Device& device, const Color& col, const UShort& startDist, const UShort& endDist);
};
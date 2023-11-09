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
class CDrawState {
public:
	//========== [[[ �񋓌^��` ]]]]
	// Z�e�X�g���[�h
	enum class ZTEST_MODE {
		ENABLED,	// �L��
		DISABLED,	// ����
	};
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
	// �e�N�X�`�����߃��[�h
	enum class TEXTURE_ALPHA_MODE {
		ENABLED,	// �L��
		DISABLED,	// ����
	};
	// ���C�e�B���O���[�h
	enum class LIGHTING_MODE {
		ENABLED,	// �L��
		DISABLED,	// ����
	};
	// �t�H�O���[�h
	enum class FOG_MODE {
		ENABLED,	// �L��
		DISABLED,	// ����
	};

	//========== [[[ �֐��錾 ]]]
	CDrawState();
	~CDrawState();
	void Init(Device& device);
	void Uninit(void);
	void Update(void);
	void StartTemporarySetMode(void);
	void EndTemporarySetMode  (Device& device);
	void SetZTestMode         (const ZTEST_MODE& mode, Device& device);
	void SetZTestMode         (const bool& isZTest, Device& device);
	void SetCullingMode       (const CULLING_MODE& mode, Device& device);
	void SetAlphaBlendMode    (const ALPHA_BLEND_MODE& mode, Device& device);
	void SetTextureAlphaMode  (const TEXTURE_ALPHA_MODE& mode, Device& device);
	void SetTextureAlphaMode  (const bool& isTextureAlpha, Device& device);
	void SetLightingMode      (const LIGHTING_MODE& mode, Device& device);
	void SetLightingMode      (const bool& isLighting, Device& device);
	void SetFogMode           (const FOG_MODE& mode, Device& device);
	void SetFogMode           (const bool& isFog, Device& device);
	void SetFogParameter      (const Color& col, const float& startDist, const float& endDist, Device& device);

private:
	//========== [[[ �\���̒�` ]]]]
	struct FormerMode {
		ZTEST_MODE         ZTest        = ZTEST_MODE::ENABLED;
		CULLING_MODE       culling      = CULLING_MODE::FRONT_SIDE;
		ALPHA_BLEND_MODE   alphaBlend   = ALPHA_BLEND_MODE::NORMAL;
		TEXTURE_ALPHA_MODE textureAlpha = TEXTURE_ALPHA_MODE::ENABLED;
		LIGHTING_MODE      lighting     = LIGHTING_MODE::ENABLED;
		FOG_MODE           fog          = FOG_MODE::ENABLED;
	};

	//========== [[[ �ϐ��錾 ]]]]
	bool m_isAssignFormerMode;
	FormerMode m_formerMode;
};
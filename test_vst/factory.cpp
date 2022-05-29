//VST_SDK�̃C���N���[�h
#include "public.sdk/source/main/pluginfactory.h"

//����̃w�b�_�t�@�C�����C���N���[�h
#include "vst3fuid.h"
#include "processor.h"
#include "controller.h"

//����҂̖��O
#define VST_MAKER_NAME "qlfa"

//����҂�Web�T�C�g��URL
#define VST_MAKER_URL "NONE"

//����҂̃��[���A�h���X
#define VST_MAKER_EMAIL "NONE"

//VST�̖��O
#define VST_NAME "test_vst"

//VST�̃o�[�W����
#define VST_VERSION "0.0.1-bata"

//VST�̃J�e�S��
#define VST_CATE Vst::PlugType::kFx

//=================================
//VST�v���O�C���̃��^���o�^������
//=================================
BEGIN_FACTORY_DEF(VST_MAKER_NAME, VST_MAKER_URL, VST_MAKER_EMAIL)

//Processor�N���X�̐���
DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Vst::ProcessorUID),
	PClassInfo::kManyInstances,
	kVstAudioEffectClass,
	VST_NAME,
	Vst::kDistributable,
	VST_CATE,
	VST_VERSION,
	kVstVersionString,
	Steinberg::Vst::VSTProcessor::createInstance)

	//VSTController�N���X�̍쐬���s��
	DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Vst::ControllerUID),
		PClassInfo::kManyInstances,
		kVstComponentControllerClass,
		VST_NAME "Controller",
		0,
		"",
		VST_VERSION,
		kVstVersionString,
		Steinberg::Vst::VSTController::createInstance)

	END_FACTORY
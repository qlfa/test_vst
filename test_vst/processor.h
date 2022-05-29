//VST_SDK�̃C���N���[�h
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

//VST3�쐬�ɕK�v�Ȗ��O��Ԃ��g�p
namespace Steinberg {
	namespace Vst {
		//=================================
		//�����M������������Processor�N���X
		//=================================
		class VSTProcessor :public AudioEffect {

		protected:
			ParamValue depth; // �{�����[���A�g�������A�p���̂�����(�[��)
			ParamValue freq;  // �g�������A�p���̎���(�{�����[�����͖���)
			int32      type;  // �G�t�F�N�^�[�̃^�C�v(�{�����[���A�g�������A�p����3��)

			ParamValue theta; // �g�������Ŏg�p����SIN�֐��̊p�x��
		public:
			//�R���X�g���N�^
			VSTProcessor();

			//�N���X������������֐�(�K�{)
			tresult PLUGIN_API initialize(FUnknown* context);

			//�o�X�\����ݒ肷��֐�
			tresult PLUGIN_API setBusArrangments(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);

			//�����M������������֐�(�K�{)
			tresult PLUGIN_API process(ProcessData& data);

			//����VST Processor�N���X�̃C���X�^���X�𐶐�����ׂ̊֐�(�K�{)
			static FUnknown* createInstance(void*) { return (IAudioProcessor*)new VSTProcessor(); }
		};
	}
}
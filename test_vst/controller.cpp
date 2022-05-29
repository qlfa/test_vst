//����VST�p�̃C���N���[�h�t�@�C��
#include "vst3fuid.h"
#include "vst3define.h"
#include "controller.h"

namespace Steinberg {
	namespace Vst {

		//�N���X������������֐�
		tresult	PLUGIN_API VSTController::initialize(FUnknown* context) {

			//�p�����̃N���X�̏�����
			tresult result = EditController::initialize(context);

			if (result == kResultTrue) {
				//�p�����[�^�[��ǉ�

				// �W���̃p�����[�^�[���R���g���[���[�ɒǉ�
				parameters.addParameter(STR16("Depth"), STR16("..."), 0, 1, ParameterInfo::kCanAutomate, PARAM_DEPTH_TAG);

				//�͈̓p�����[�^�[���쐬
				RangeParameter* param1 = new RangeParameter(STR16("Speed"), PARAM_SPEED_TAG, STR16("Hz"), 0.5f, 30.0f, 5.0f);
				//�����扽�ʂ܂ŕ\�����邩
				param1->setPrecision(2);
				//�͈̓p�����[�^�[���R���g���[���[�ɒǉ�
				parameters.addParameter(param1);

				//�������X�g�p�����[�^�[���쐬�ǉ�
				StringListParameter* param2 = new StringListParameter(STR16("Type"), PARAM_TYPE_TAG);
				param2->appendString(STR16("Volume"));
				param2->appendString(STR16("Tremolo"));
				param2->appendString(STR16("Panning"));
				//�����񃊃X�g�p�����[�^�[���R���g���[���[�ɒǉ�
				parameters.addParameter(param2);
			}

			//�����������������kResultTrue��Ԃ�
			result = kResultTrue;
			return result;
		}
	}
}
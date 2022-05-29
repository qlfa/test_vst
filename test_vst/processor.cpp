//����VST�p�̃C���N���[�h�t�@�C��
#include "vst3fuid.h"
#include "processor.h"
#include "vst3define.h"

//VST3�쐬�ɕK�v�Ȗ��O��Ԃ��g�p
namespace Steinberg {
	namespace Vst {

		//==============
		//�R���X�g���N�^
		//==============
		VSTProcessor::VSTProcessor() {

			//�R���g���[���[��FUID��ݒ肷��
			setControllerClass(ControllerUID);
		}

		//======================
		//�N���X������������֐�
		//======================
		tresult PLUGIN_API VSTProcessor::initialize(FUnknown* context) {

			//�p���N���X�̏�����
			tresult result = AudioEffect::initialize(context);
			if (result == kResultTrue) {
				//���͂Əo�͂�ݒ�
				addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
				addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);
			}
			//�������ɐ�����kResultTrue��Ԃ�
			return result;
		}

		tresult PLUGIN_API VSTProcessor::setBusArrangments(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) {
			//input��output�̃o�X��1���ŁA����input��output�̍\�����X�e���I�̏ꍇ
			if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo) {
				return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
			}
			//�Ή����Ă��Ȃ��o�X�\���̏ꍇ�AkResultFalse��Ԃ�
			return kResultFalse;
		}

		//======================
		//�����M������������֐�
		//======================
		tresult PLUGIN_API VSTProcessor::process(ProcessData& data) {

			//�p�����[�^�[�ύX�̏���
			//�^����ꂽ�p�����[�^�[�����鎞�Adata��inputParameterChanges��IParameterChanges�Ƀ|�C���^�̃A�h���X������
			if (data.inputParameterChanges != NULL) {

				//�^����ꂽ�p�����[�^�[�̐����擾
				int32 paramChangeConut = data.inputParameterChanges->getParameterCount();

				//�^����ꂽ�p�����[�^�[���������J��Ԃ�
				for (int32 i = 0; i < paramChangeConut; i++) {

					// �p�����[�^�[�ύX�̃L���[���擾
					// (��������T���v�����ɕ����̃p�����[�^�[�ύX��񂪂���\�������邽�߁A�L���[�Ƃ����`�ɂȂ��Ă���B)
					IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
					if (queue != NULL) {

						//�ǂ̃p�����[�^�[���ύX���ꂽ���m�邽�߁A�p�����[�^�[tag���擾
						int32 tag = queue->getParameterId();

						//�ύX���ꂽ�񐔂��擾
						int32 valueChangeCount = queue->getPointCount();
						ParamValue value;
						int32 sampleOffset;

						//�Ō�ɕύX���ꂽ�l���擾
						if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {

							//tag�ɉ��������������{
							switch (tag) {
							case PARAM_DEPTH_TAG:
								//depth��ύX����
								depth = value;
								break;
							case PARAM_SPEED_TAG:
								//freq��ύX����
								//RangeParameter�ō쐬���ꂽ�p�����[�^�[���v���Z�b�T�N���X�ɓn�����Ƃ��́A0.0�`1.0�ƂȂ��Ă��܂�
								//�����ŊeRangeParameter�ɉ������͈͂�ݒ肷��K�v������
								freq = (29.5f * value) + 0.5f;
								break;
							case PARAM_TYPE_TAG:
								// type��ύX����B
								// StringListParameter�ō쐬���ꂽ�p�����[�^�[���A�v���Z�b�T�N���X�ɓn�����Ƃ���0.0�`1.0�ƂȂ��Ă��܂��B
								// ����̓��X�g����3�Ȃ̂ŁAVolume�c0.0f�ATremolo�c0.5f�APanning�c1.0f�ƂȂ�B
								// ���X�g�̐���4�̏ꍇ�A0.0f�A0.333�c�A0.666�c�A1.0f�ƂȂ�B
								// �u1.0f / (���X�g�̐� - 1)�v�ŋ��߂���B
								type = (int32)(value * 2.0f);
								break;
							}
						}
					}
				}
			}

			//���́E�o�̓o�b�t�@�̃|�C���^���킩��₷���ϐ��Ɋi�[
			//inputs[]�Aoutputs[]��AudioBus�̐���������(addAudioInput()�AaddAudioOutput()�Œǉ�����������)
			//�����AudioBus��1�����Ȃ̂� 0 �݂̂ƂȂ�
			//channelBuffers32��32bit���������_�^�̃o�b�t�@�ŉ����M���̃`�����l����������
			//���m����(kMono)�Ȃ� 0 �݂̂ŁA�X�e���I(kStereo)�Ȃ� 0(Left) �� 1(Right) �ƂȂ�
			Sample32* inL = data.inputs[0].channelBuffers32[0];
			Sample32* inR = data.inputs[0].channelBuffers32[1];
			Sample32* outL = data.outputs[0].channelBuffers32[0];
			Sample32* outR = data.outputs[0].channelBuffers32[1];

			//numSamples�Ŏ������T���v�����A��������������
			for (int32 i = 0; i < data.numSamples; i++) {

				//sin�֐��̌��ʂ�0�`1�̊Ԃɂ���(�g�������A�p���p)
				Sample32 a = (sin(theta) * 0.5f) + 0.5f;

				//depth��a������͐M���Ɋ|�����킹��l���v�Z
				Sample32 b = (1.0f - depth) + (a * depth);
				Sample32 c = (1.0f - depth) + ((1.0f - a) * depth);

				switch (type) {

				case 0://�{�����[���̏ꍇ
					//���͐M����depth���|�����킹��
					outL[i] = depth * inL[i];
					outR[i] = depth * inR[i];
					break;

				case 1://�g�������̏ꍇ
					//���͐M����b���|�����킹��
					outL[i] = b * inL[i];
					outR[i] = b * inR[i];
					break;

				case 2://�p���̏ꍇ
					//���͐M����b�Ac���|�����킹��
					outL[i] = b * inL[i];
					outR[i] = c * inR[i];
					break;
				}

				//�p�x�Ƃɉ����x��������
				theta += (2.0f * 3.14159265f * freq) / 44100.0f;
			}

			//���Ȃ����kResultTrue��Ԃ�
			return kResultTrue;
		}
	}
}
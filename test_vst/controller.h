//VST_SDK�̃C���N���[�h
#include "public.sdk/source/vst/vsteditcontroller.h"

//����VST�p�̃C���N���[�h�t�@�C��
#include "vst3define.h"

namespace Steinberg {
	namespace Vst {

		//===============================================
		//VST�̃p�����[�^�[�𑀍삷��ׂ�Controller�N���X
		//===============================================
		class VSTController :public EditController {
		public:
			//�N���X�̏��������s���֐�
			tresult PLUGIN_API initialize(FUnknown* context);
			//����VST Controller�N���X�̃C���X�^���X���쐬�邽�߂̊֐�(�K�{)
 			static FUnknown* createInstance(void*) { return (IEditController*)new VSTController(); }
		};
	}
}
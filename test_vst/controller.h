//VST_SDKのインクルード
#include "public.sdk/source/vst/vsteditcontroller.h"

//自作VST用のインクルードファイル
#include "vst3define.h"

namespace Steinberg {
	namespace Vst {

		//===============================================
		//VSTのパラメーターを操作する為のControllerクラス
		//===============================================
		class VSTController :public EditController {
		public:
			//クラスの初期化を行う関数
			tresult PLUGIN_API initialize(FUnknown* context);
			//自作VST Controllerクラスのインスタンスを作成るための関数(必須)
 			static FUnknown* createInstance(void*) { return (IEditController*)new VSTController(); }
		};
	}
}
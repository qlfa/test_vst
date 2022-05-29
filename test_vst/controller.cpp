//自作VST用のインクルードファイル
#include "vst3fuid.h"
#include "vst3define.h"
#include "controller.h"

namespace Steinberg {
	namespace Vst {

		//クラスを初期化する関数
		tresult	PLUGIN_API VSTController::initialize(FUnknown* context) {

			//継承元のクラスの初期化
			tresult result = EditController::initialize(context);

			if (result == kResultTrue) {
				//パラメーターを追加

				// 標準のパラメーターをコントローラーに追加
				parameters.addParameter(STR16("Depth"), STR16("..."), 0, 1, ParameterInfo::kCanAutomate, PARAM_DEPTH_TAG);

				//範囲パラメーターを作成
				RangeParameter* param1 = new RangeParameter(STR16("Speed"), PARAM_SPEED_TAG, STR16("Hz"), 0.5f, 30.0f, 5.0f);
				//少数第何位まで表示するか
				param1->setPrecision(2);
				//範囲パラメーターをコントローラーに追加
				parameters.addParameter(param1);

				//文字リストパラメーターを作成追加
				StringListParameter* param2 = new StringListParameter(STR16("Type"), PARAM_TYPE_TAG);
				param2->appendString(STR16("Volume"));
				param2->appendString(STR16("Tremolo"));
				param2->appendString(STR16("Panning"));
				//文字列リストパラメーターをコントローラーに追加
				parameters.addParameter(param2);
			}

			//初期化が成功すればkResultTrueを返す
			result = kResultTrue;
			return result;
		}
	}
}
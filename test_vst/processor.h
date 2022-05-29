//VST_SDKのインクルード
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

//VST3作成に必要な名前空間を使用
namespace Steinberg {
	namespace Vst {
		//=================================
		//音声信号を処理するProcessorクラス
		//=================================
		class VSTProcessor :public AudioEffect {

		protected:
			ParamValue depth; // ボリューム、トレモロ、パンのかかり具合(深さ)
			ParamValue freq;  // トレモロ、パンの周期(ボリューム時は無視)
			int32      type;  // エフェクターのタイプ(ボリューム、トレモロ、パンの3種)

			ParamValue theta; // トレモロで使用するSIN関数の角度θ
		public:
			//コンストラクタ
			VSTProcessor();

			//クラスを初期化する関数(必須)
			tresult PLUGIN_API initialize(FUnknown* context);

			//バス構成を設定する関数
			tresult PLUGIN_API setBusArrangments(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);

			//音声信号を処理する関数(必須)
			tresult PLUGIN_API process(ProcessData& data);

			//自作VST Processorクラスのインスタンスを生成する為の関数(必須)
			static FUnknown* createInstance(void*) { return (IAudioProcessor*)new VSTProcessor(); }
		};
	}
}
//自作VST用のインクルードファイル
#include "vst3fuid.h"
#include "processor.h"
#include "vst3define.h"

//VST3作成に必要な名前空間を使用
namespace Steinberg {
	namespace Vst {

		//==============
		//コンストラクタ
		//==============
		VSTProcessor::VSTProcessor() {

			//コントローラーのFUIDを設定する
			setControllerClass(ControllerUID);
		}

		//======================
		//クラスを初期化する関数
		//======================
		tresult PLUGIN_API VSTProcessor::initialize(FUnknown* context) {

			//継承クラスの初期化
			tresult result = AudioEffect::initialize(context);
			if (result == kResultTrue) {
				//入力と出力を設定
				addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
				addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);
			}
			//初期化に成功時kResultTrueを返す
			return result;
		}

		tresult PLUGIN_API VSTProcessor::setBusArrangments(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) {
			//inputとoutputのバスが1つずつで、かつinputとoutputの構成がステレオの場合
			if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo) {
				return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
			}
			//対応していないバス構成の場合、kResultFalseを返す
			return kResultFalse;
		}

		//======================
		//音声信号を処理する関数
		//======================
		tresult PLUGIN_API VSTProcessor::process(ProcessData& data) {

			//パラメーター変更の処理
			//与えられたパラメーターがある時、dataのinputParameterChangesにIParameterChangesにポインタのアドレスが入る
			if (data.inputParameterChanges != NULL) {

				//与えられたパラメーターの数を取得
				int32 paramChangeConut = data.inputParameterChanges->getParameterCount();

				//与えられたパラメーター分処理を繰り返す
				for (int32 i = 0; i < paramChangeConut; i++) {

					// パラメーター変更のキューを取得
					// (処理するサンプル内に複数のパラメーター変更情報がある可能性があるため、キューという形になっている。)
					IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
					if (queue != NULL) {

						//どのパラメーターが変更されたか知るため、パラメーターtagを取得
						int32 tag = queue->getParameterId();

						//変更された回数を取得
						int32 valueChangeCount = queue->getPointCount();
						ParamValue value;
						int32 sampleOffset;

						//最後に変更された値を取得
						if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue) {

							//tagに応じた処理を実施
							switch (tag) {
							case PARAM_DEPTH_TAG:
								//depthを変更する
								depth = value;
								break;
							case PARAM_SPEED_TAG:
								//freqを変更する
								//RangeParameterで作成されたパラメーターもプロセッサクラスに渡されるときは、0.0～1.0となってしまう
								//自分で各RangeParameterに応じた範囲を設定する必要がある
								freq = (29.5f * value) + 0.5f;
								break;
							case PARAM_TYPE_TAG:
								// typeを変更する。
								// StringListParameterで作成されたパラメーターも、プロセッサクラスに渡されるときは0.0～1.0となってしまう。
								// 今回はリスト数は3つなので、Volume…0.0f、Tremolo…0.5f、Panning…1.0fとなる。
								// リストの数が4つの場合、0.0f、0.333…、0.666…、1.0fとなる。
								// 「1.0f / (リストの数 - 1)」で求められる。
								type = (int32)(value * 2.0f);
								break;
							}
						}
					}
				}
			}

			//入力・出力バッファのポインタをわかりやすい変数に格納
			//inputs[]、outputs[]はAudioBusの数だけある(addAudioInput()、addAudioOutput()で追加した分だけ)
			//今回はAudioBusは1つだけなので 0 のみとなる
			//channelBuffers32は32bit浮動小数点型のバッファで音声信号のチャンネル数分ある
			//モノラル(kMono)なら 0 のみで、ステレオ(kStereo)なら 0(Left) と 1(Right) となる
			Sample32* inL = data.inputs[0].channelBuffers32[0];
			Sample32* inR = data.inputs[0].channelBuffers32[1];
			Sample32* outL = data.outputs[0].channelBuffers32[0];
			Sample32* outR = data.outputs[0].channelBuffers32[1];

			//numSamplesで示されるサンプル分、音声を処理する
			for (int32 i = 0; i < data.numSamples; i++) {

				//sin関数の結果を0～1の間にする(トレモロ、パン用)
				Sample32 a = (sin(theta) * 0.5f) + 0.5f;

				//depthとaから入力信号に掛け合わせる値を計算
				Sample32 b = (1.0f - depth) + (a * depth);
				Sample32 c = (1.0f - depth) + ((1.0f - a) * depth);

				switch (type) {

				case 0://ボリュームの場合
					//入力信号とdepthを掛け合わせる
					outL[i] = depth * inL[i];
					outR[i] = depth * inR[i];
					break;

				case 1://トレモロの場合
					//入力信号とbを掛け合わせる
					outL[i] = b * inL[i];
					outR[i] = b * inR[i];
					break;

				case 2://パンの場合
					//入力信号とb、cを掛け合わせる
					outL[i] = b * inL[i];
					outR[i] = c * inR[i];
					break;
				}

				//角度θに加速度を加える
				theta += (2.0f * 3.14159265f * freq) / 44100.0f;
			}

			//問題なければkResultTrueを返す
			return kResultTrue;
		}
	}
}
//VST_SDKのインクルード
#include "public.sdk/source/main/pluginfactory.h"

//自作のヘッダファイルをインクルード
#include "vst3fuid.h"
#include "processor.h"
#include "controller.h"

//制作者の名前
#define VST_MAKER_NAME "qlfa"

//制作者のWebサイトのURL
#define VST_MAKER_URL "NONE"

//制作者のメールアドレス
#define VST_MAKER_EMAIL "NONE"

//VSTの名前
#define VST_NAME "test_vst"

//VSTのバージョン
#define VST_VERSION "0.0.1-bata"

//VSTのカテゴリ
#define VST_CATE Vst::PlugType::kFx

//=================================
//VSTプラグインのメタ情報登録兼生成
//=================================
BEGIN_FACTORY_DEF(VST_MAKER_NAME, VST_MAKER_URL, VST_MAKER_EMAIL)

//Processorクラスの生成
DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Vst::ProcessorUID),
	PClassInfo::kManyInstances,
	kVstAudioEffectClass,
	VST_NAME,
	Vst::kDistributable,
	VST_CATE,
	VST_VERSION,
	kVstVersionString,
	Steinberg::Vst::VSTProcessor::createInstance)

	//VSTControllerクラスの作成を行う
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
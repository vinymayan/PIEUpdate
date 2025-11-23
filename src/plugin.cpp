#include "logger.h"
#include "events.h"
#include "hooks.h"

#include "PayloadHandlerManager.h"
#include "PayloadInterpreter.h"

#include "CPR.h"


void OnMessage(SKSE::MessagingInterface::Message* message) {
	if (message->type == SKSE::MessagingInterface::kDataLoaded) {
		//logger::info("Data loaded.");
		REL::Relocation<uintptr_t> npcPtr{ RELOCATION_ID(261399,207890) }; //165e3b0
		REL::Relocation<uintptr_t> pcPtr{ RELOCATION_ID(261918,208044) }; //1663f78
		Hooks::install();

		auto payloadHandlerManager = PayloadHandlerManager::GetSingleton();

		payloadHandlerManager->RegisterPayloadHandler("PIE", PayloadInterpreter::GetSingleton());
		payloadHandlerManager->RegisterPayloadHandler("CPR", CPR::PayloadInterpreter::GetSingleton());

		// Load payload handlers from mods registered for messages from Payload Interpreter
		payloadHandlerManager->CollectExternPayloadHandlers();
	}
	if (message->type == SKSE::MessagingInterface::kNewGame || message->type == SKSE::MessagingInterface::kPostLoadGame) {
		// Post-load
	}
}



std::string wstring2string(const std::wstring& wstr, UINT CodePage)

{
	std::string ret;

	int len = WideCharToMultiByte(CodePage, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);

	ret.resize((size_t)len, 0);

	WideCharToMultiByte(CodePage, 0, wstr.c_str(), (int)wstr.size(), &ret[0], len, NULL, NULL);

	return ret;
}


SKSEPluginLoad(const SKSE::LoadInterface* skse) {

	SetupLog();
	logger::info("Plugin loaded");

	SKSE::Init(skse);
	PayloadInterpreter::GetSingleton()->LoadPreDefinedPayloads();
	SKSE::GetMessagingInterface()->RegisterListener(OnMessage);


	return true;
}
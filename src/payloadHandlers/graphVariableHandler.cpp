#pragma once
#include "payloadHandler.h"

void graphVariableHandler::setGraphVariableBool(RE::Actor* actor, std::vector<std::string_view>* in)
{
	bool b;
	if (!Utils::string_view::to_bool(in->at(2), b)) {
		printErrMsg(in, "Invalid argument for setGraphVariableBool.");
		return;
	}
	
	actor->SetGraphVariableBool(std::string(in->at(1)), b);
}

void graphVariableHandler::setGraphVariableFloat(RE::Actor* actor, std::vector<std::string_view>* in)
{
	float f;
	if (!Utils::string_view::to_float(in->at(2), f)) {
		printErrMsg(in, "Invalid argument for setGraphVariableFloat.");
		return;
	}
	actor->SetGraphVariableFloat(std::string(in->at(1)), f);
}

void graphVariableHandler::setGraphVariableInt(RE::Actor* actor, std::vector<std::string_view>* in)
{
	int i;
	if (!Utils::string_view::to_int(in->at(2), i)) {
		printErrMsg(in, "Invalid argument for setGraphVariableInt.");
		return;
	}
	actor->SetGraphVariableInt(std::string(in->at(1)), i);
}

void graphVariableHandler::getGraphVariableBool(RE::Actor* actor, std::vector<std::string_view>* in)
{
	// Argumento 1: Nome da variável (ex: "IsAttacking")
	std::string varName(in->at(1));
	bool result = false;

	// Chama a função do motor do jogo. Retorna true se a variável existir.
	if (actor->GetGraphVariableBool(varName, result)) {
		logger::info("GetGraphVariableBool '{}': {}", varName, result);
	}
	else {
		logger::warn("Falha ao ler GraphVariableBool '{}'. Variável não encontrada.", varName);
	}
}

void graphVariableHandler::getGraphVariableFloat(RE::Actor* actor, std::vector<std::string_view>* in)
{
	std::string varName(in->at(1));
	float result = 0.0f;

	if (actor->GetGraphVariableFloat(varName, result)) {
		logger::info("GetGraphVariableFloat '{}': {}", varName, result);
	}
	else {
		logger::warn("Falha ao ler GraphVariableFloat '{}'.", varName);
	}
}

void graphVariableHandler::getGraphVariableInt(RE::Actor* actor, std::vector<std::string_view>* in)
{
	std::string varName(in->at(1));
	int result = 0;

	if (actor->GetGraphVariableInt(varName, result)) {
		logger::info("GetGraphVariableInt '{}': {}", varName, result);
	}
	else {
		logger::warn("Falha ao ler GraphVariableInt '{}'.", varName);
	}
}

void graphVariableHandler::copyGraphVariableBool(RE::Actor* actor, std::vector<std::string_view>* in)
{
	// Formato: @CGVB|TargetVariable|SourceVariable
	std::string targetVar(in->at(1));
	std::string sourceVar(in->at(2));
	bool val;

	// Tenta pegar o valor da Source
	if (actor->GetGraphVariableBool(sourceVar, val)) {
		// Se conseguiu, aplica na Target
		actor->SetGraphVariableBool(targetVar, val);
		// Opcional: logger::info("Copied Bool {} ({}) to {}", sourceVar, val, targetVar);
	}
	else {
		// Opcional: logger::warn("Failed to copy Bool: Source {} not found.", sourceVar);
	}
}

void graphVariableHandler::copyGraphVariableFloat(RE::Actor* actor, std::vector<std::string_view>* in)
{
	std::string targetVar(in->at(1));
	std::string sourceVar(in->at(2));
	float val;

	if (actor->GetGraphVariableFloat(sourceVar, val)) {
		actor->SetGraphVariableFloat(targetVar, val);
	}
}

void graphVariableHandler::copyGraphVariableInt(RE::Actor* actor, std::vector<std::string_view>* in)
{
	std::string targetVar(in->at(1));
	std::string sourceVar(in->at(2));
	int val;

	if (actor->GetGraphVariableInt(sourceVar, val)) {
		// Se conseguiu, aplica na Target e loga o sucesso
		actor->SetGraphVariableInt(targetVar, val);
		logger::info("Copied Int {} ({}) to {}", sourceVar, val, targetVar);
	}
	else {
		// Se falhou, loga um aviso informando que a variável source não foi encontrada
		logger::warn("Failed to copy Int: Source variable '{}' not found on actor.", sourceVar);
	}
	
}

void graphVariableHandler::process(RE::Actor* actor, std::vector<std::string_view>* param, GRAPHVARIABLETYPE graphVariableType)
{
	if (!checkParamCt(param, 2)) {
		return;
	}
	switch (graphVariableType) {
	case GRAPHVARIABLETYPE::Bool: setGraphVariableBool(actor, param); break;
	case GRAPHVARIABLETYPE::Float: setGraphVariableFloat(actor, param); break;
	case GRAPHVARIABLETYPE::Int: setGraphVariableInt(actor, param); break;
	case GRAPHVARIABLETYPE::GetBool: getGraphVariableBool(actor, param); break;
	case GRAPHVARIABLETYPE::GetFloat: getGraphVariableFloat(actor, param); break;
	case GRAPHVARIABLETYPE::GetInt: getGraphVariableInt(actor, param); break;
	case GRAPHVARIABLETYPE::CopyBool: copyGraphVariableBool(actor, param); break;
	case GRAPHVARIABLETYPE::CopyFloat: copyGraphVariableFloat(actor, param); break;
	case GRAPHVARIABLETYPE::CopyInt: copyGraphVariableInt(actor, param); break;
	}
}


/*
@SGVB|"string of graph variable"|0(False)/1(True)| - set graph variable bool
@SGVF|"string of graph variable"|any float value| - set graph variable float
@SGVI|"string of graph variable"|any int value| - set graph variable int
*/

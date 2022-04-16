//
// Created by Yinhao on 4/13/2022.
//

#ifndef CODE_CORE_H
#define CODE_CORE_H

#include "Include.h"
#include "System.h"

class Core {
	//[Argument Numbers][Binded Function]
	using ArgumentTuple_t = std::tuple<uint32_t, uint32_t, uint32_t>;
	using SystemFunction_t = std::pair<bool (System::*)(ArgumentTuple_t *), size_t>;
/*	const std::unordered_map<std::string, size_t> instr_param_map{
			{"con", 2},//	[CACHE_COUNT]	[BLOCK_SIZE]	[POLICY_NUM]	-Set Configurations ✓
			{"inc", 1},// 	[CACHE_NUMBER]									-Initialize Cache ✓
			{"scd", 3},//	[CACHE_NUMBER]	[TOTAL_SIZE]	[SET_ASSOC]		-Set Cache Size and Set Assoc ✓
			{"scl", 2},//	[CACHE_NUMBER]	[LATENCY]						-Set Cache Latency ✓
			{"sml", 2},//	[LATENCY]										-Set Memory Latency ✓
			{"tre", 2},//  	[ADDRESS]		[ARR_TIME]						-Task Read Address at Time ✓
			{"twr", 2},//  	[ADDRESS]		[ARR_TIME]						-Task Write Address at Time ✓
			{"ins", 0},//													-Initialize System ✓
			{"pcr", 1},//	[CACHE_NUMBER]									-Print Cache Hit/Miss Rate ✓
			{"pci", 1},//	[CACHE_NUMBER]									-Print Cache Image ✓
			{"hal", 0}
	};*/


	System system;


	std::tuple<uint32_t, uint32_t, uint32_t> current_arguments;
	std::pair<std::ifstream, std::ofstream> file_manipulator;

	uint64_t clock{0};

	std::unordered_map<std::string, SystemFunction_t> instruction_map;

/**
 * Convert an String Argument to Integer 32-bit type
 * @param _char_to_check Character to be checked
 * @return True if is a Numeric Digit, false otherwise
 */
	uint32_t argumentToInt(const std::string &_argument) {
		bool dollarSignDetected = _argument.at(0) == '$';
		bool allOtherIsDigit = std::all_of(_argument.begin() + 1, _argument.end(), ::isdigit);
		if (!dollarSignDetected || !allOtherIsDigit)
			throw std::runtime_error("ERR Instruction Argument Format Error");
		return std::stoi(std::string(_argument.begin() + 1, _argument.end()), nullptr, 10);
	}

/**
 * Get the Next Instruction
 * Warning: This Function does NOT handle EOF
 * @return [INSTRUCTION_STRING][[ARGUMENT1][ARGUMENT1][ARGUMENT1]]
 */
	std::pair<std::string, ArgumentTuple_t> getNextInstruction() {
		if (!file_manipulator.first.is_open()) throw std::runtime_error("ERR Input File NOT Initialized.");
		else if (!file_manipulator.second.is_open()) throw std::runtime_error("ERR Output File NOT Initialized.");
		std::string this_read_buffer;
		std::pair<std::string, ArgumentTuple_t> to_return{"", {0, 0, 0}};
		do {
			file_manipulator.first >> this_read_buffer;
		} while (instruction_map.find(this_read_buffer) == instruction_map.end());
		to_return.first = this_read_buffer;
		for (size_t i = 0; i < instruction_map.at(this_read_buffer).second; i++) {
			file_manipulator.first >> this_read_buffer;
			if (i == 0) std::get<0>(to_return.second) = argumentToInt(this_read_buffer);
			else if (i == 1) std::get<1>(to_return.second) = argumentToInt(this_read_buffer);
			else if (i == 2) std::get<2>(to_return.second) = argumentToInt(this_read_buffer);
		}
		return to_return;
	}


public:

/**
 * Bind the File Reader and File Writer to Certain Files
 * Perform Check if Files are Detected
 * @param _global_reader_filename Filename of Input Files (containing instructions)
 * @param _global_writer_filename Filename of Output Files (containing all running-logs/hit-misses/images)
 */
	explicit Core(const std::pair<std::string, std::string> &_filename) {
		file_manipulator.first.open(_filename.first);
		file_manipulator.second.open(_filename.second);
		if (!file_manipulator.first.is_open()) throw std::runtime_error("ERR Input File NOT Found.");
		else if (!file_manipulator.second.is_open()) throw std::runtime_error("ERR Output File NOT Found.");
		instruction_map.at("con") = {&System::setConfig, 2};
		instruction_map.at("inc") = {&System::initCache, 1};
		instruction_map.at("scd") = {&System::setCacheDimension, 3};
		instruction_map.at("scl") = {&System::setCacheLatency, 2};
		instruction_map.at("sml") = {&System::setMemoryLatency, 2};
		instruction_map.at("tre") = {&System::taskReadAddress, 2};
		instruction_map.at("twr") = {&System::taskWriteAddress, 2};
		instruction_map.at("ins") = {&System::initSystem, 0};
		instruction_map.at("pcr") = {&System::printCacheRate, 1};
		instruction_map.at("pci") = {&System::printCacheImage, 1};
		instruction_map.at("hal") = {&System::haltProgram, 0};
	}

	~Core() {
		file_manipulator.first.close();
		file_manipulator.second.close();
	}

	void loadInstructions() {
		while (file_manipulator.first.good()) {
			std::pair<std::string, std::vector<uint32_t>> this_instruction = this->getNextInstruction();

		}

	};


#endif //CODE_CORE_H

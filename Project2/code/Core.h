//
// Created by Yinhao on 4/13/2022.
//

#ifndef CODE_CORE_H
#define CODE_CORE_H

#include "Include.h"
#include "System.h"

class Core {
	using ArgumentTuple_t = std::tuple<uint32_t, uint32_t, uint32_t>;
	using SystemFunction_t = std::pair<bool (System::*)(ArgumentTuple_t *), size_t>;
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

	/**w
	 * Get the Next Instruction
	 * Warning: This Function does NOT handle EOF
	 * @return [INSTRUCTION_STRING][[ARGUMENT1][ARGUMENT2][ARGUMENT3]]
	 */
	std::pair<std::string, ArgumentTuple_t> getNextInstruction() {
		if (!file_manipulator.first.is_open()) throw std::runtime_error("ERR Input File NOT Initialized.");
		else if (!file_manipulator.second.is_open()) throw std::runtime_error("ERR Output File NOT Initialized.");
		std::string this_instruction_string, this_argument_string;
		std::pair<std::string, ArgumentTuple_t> to_return{"", {0, 0, 0}};
		do {
			file_manipulator.first >> this_instruction_string;
		} while (instruction_map.find(this_instruction_string) == instruction_map.end() &&
				 file_manipulator.first.good());
		to_return.first = this_instruction_string;
		for (size_t i = 0; i < instruction_map.at(this_instruction_string).second; i = i + 1) {
			file_manipulator.first >> this_argument_string;
			if (i == 0) std::get<0>(to_return.second) = argumentToInt(this_argument_string);
			else if (i == 1) std::get<1>(to_return.second) = argumentToInt(this_argument_string);
			else if (i == 2) std::get<2>(to_return.second) = argumentToInt(this_argument_string);
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
		system.setWriterPtr(&this->file_manipulator.second);
		instruction_map["con"] = {&System::setConfig, 3};
		instruction_map["scd"] = {&System::setCacheDimension, 3};
		instruction_map["scl"] = {&System::setCacheLatency, 2};
		instruction_map["sml"] = {&System::setMemoryLatency, 1};
		instruction_map["inc"] = {&System::initCache, 1};
		instruction_map["tre"] = {&System::taskReadAddress, 2};
		instruction_map["twr"] = {&System::taskWriteAddress, 2};
		instruction_map["ins"] = {&System::initSystem, 0};
		instruction_map["pcr"] = {&System::printCacheRate, 2};
		instruction_map["pci"] = {&System::printCacheImage, 2};
		instruction_map["hat"] = {&System::haltProgram, 0};
	}

	/**
	 * Destructor waits for I/O finishes before thread terminates
	 */
	~Core() {
		file_manipulator.first.close();
		file_manipulator.second.close();
	}


	/**
	 * Load instructions from file, then invoke mapped function from System
	 * Schedule all task-type instructions into task queue (unsorted)
	 * Execute all configuration-type instructions immediately
	 */
	void loadInstructions() {
		while (file_manipulator.first.good()) {
			std::pair<std::string, ArgumentTuple_t> this_instruction = this->getNextInstruction();
			SystemFunction_t this_system_function = instruction_map.at(this_instruction.first);
			std::invoke(this_system_function.first, system, &this_instruction.second);
		}
	};

	/**
	 * Try to execute all tasks from Task Queue
	 */
	void runInstructions() {
		this->system.runTaskQueue();
	}

};

#endif //CODE_CORE_H

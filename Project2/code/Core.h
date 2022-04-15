//
// Created by Yinhao on 4/13/2022.
//

#ifndef CODE_CORE_H
#define CODE_CORE_H

#include "Include.h"
#include "System.h"

class Core {
	const std::unordered_map<std::string, uint8_t> instr_param_map{
			{"con", 2},//	[CACHE_COUNT]	[BLOCK_SIZE]	[POLICY_NUM]	-Set Configurations
			{"inc", 1},// [CACHE_NUMBER]									-Initialize Cache
			{"scd", 3},//	[CACHE_NUMBER]	[TOTAL_SIZE]	[SET_ASSOC]		-Set Cache Size and Set Assoc
			{"scl", 2},//	[CACHE_NUMBER]	[LATENCY]						-Set Cache Latency
			{"pch", 1},//	[CACHE_NUMBER]									-Print Cache Hit Rate
			{"pcm", 1},//	[CACHE_NUMBER]									-Print Cache Miss Rate
			{"ins", 3},//													-Initialize System
			{"rat", 3},//  [ADDRESS]		[ARR_TIME]						-Read Address at Time
			{"wat", 3},//  [ADDRESS]		[ARR_TIME]						-Write Address at Time
			{"hap", 3},//													-Halt Process

	};
	std::ofstream ofs;
	std::ifstream ifs;
	uint64_t clock{0};
	System system;

	void readParam(std::pair<uint32_t, uint32_t> &param, const size_t num) {
		auto ifNumeric = [](const char &c) {
			return c < '0' || c > '9';
		};
		try {
			switch (num) {
				case 1:
					assert(ifNumeric(ifs.peek()));
					ifs >> param.first;
					param.second = 0;
					break;
				case 2:
					assert(ifNumeric(ifs.peek()));
					ifs >> param.first;
					assert(ifNumeric(ifs.peek()));
					ifs >> param.second;
					break;
				default:
					throw std::runtime_error("ERR Wrong Param Size");
			}
		} catch (std::exception &exception) {
			throw exception;
		}
	}

public:
	void init(const std::string &input, const std::string &output) {
		ifs.open(input);
		ofs.open(output);
		if (!ifs.is_open() || !ofs.is_open()) throw std::runtime_error("ERR Files not found.");
		std::string instruction;
		std::pair<uint32_t, uint32_t> param;
		for (ifs >> instruction; ifs.peek() != EOF; ifs >> instruction) {
			if (instruction == "hal") {
				break;
			} else if (instruction == "blo") {
				this->readParam(param, 1);
				this->system.
			} else if (instruction == "new") {
				this->readParam(param, 2);
			} else if (instruction == "lat") {
				this->readParam(param, 2);
			} else if (instruction == "con") {
				this->readParam(param, 2);
			} else if (instruction == "rea") {
				this->readParam(param, 2);
			} else if (instruction == "wri") {
				this->readParam(param, 2);
			} else {
				throw std::runtime_error("ERR Unidentified Instruction");
			}
		}
	}
};


#endif //CODE_CORE_H

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
			{"inc", 1},// 	[CACHE_NUMBER]									-Initialize Cache
			{"scd", 3},//	[CACHE_NUMBER]	[TOTAL_SIZE]	[SET_ASSOC]		-Set Cache Size and Set Assoc
			{"scl", 2},//	[CACHE_NUMBER]	[LATENCY]						-Set Cache Latency
			{"sml", 2},//	[LATENCY]										-Set Memory Latency
			{"pcr", 1},//	[CACHE_NUMBER]									-Print Cache Hit/Miss Rate
			{"pcc", 1},//	[CACHE_NUMBER]									-Print Cache Image
			{"ins", 0},//													-Initialize System
			{"rat", 2},//  	[ADDRESS]		[ARR_TIME]						-Read Address at Time
			{"wat", 2},//  	[ADDRESS]		[ARR_TIME]						-Write Address at Time
			{"hap", 0},//													-Halt Process
	};

	std::ofstream global_writer;
	std::ifstream global_reader;
	uint64_t clock{0};
	System system;

	/**
	 * Check if a character is Numeric Digit
	 * @param _char_to_check Character to be checked
	 * @return True if is a Numeric Digit, false otherwise
	 */
	static inline bool ifNumeric(const char &_char_to_check) {
		return _char_to_check < '0' || _char_to_check > '9';
	}

public:
	/**
	 * Bind the File Reader and File Writer to Certain Files
	 * Perform Check if Files are Detected
	 * @param _global_reader_filename Filename of Input Files (containing instructions)
	 * @param _global_writer_filename Filename of Output Files (containing all running-logs/hit-misses/images)
	 */
	void setFileNames(const std::string &_global_reader_filename, const std::string &_global_writer_filename) {
		global_reader.open(_global_reader_filename);
		global_writer.open(_global_writer_filename);
		if (!global_reader.is_open()) throw std::runtime_error("ERR Input File NOT Found.");
		else if (!global_writer.is_open()) throw std::runtime_error("ERR Output File NOT Found.");
		std::string instruction;
		std::pair<uint32_t, uint32_t> param;
		for (global_reader >> instruction; global_reader.peek() != EOF; global_reader >> instruction) {
		}
	}

	void run() {

	}

	void finish() {
		global_reader.close();
		global_writer.close();
	}
};


#endif //CODE_CORE_H

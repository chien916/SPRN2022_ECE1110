
#ifndef CODE_TASK_H
#define CODE_TASK_H

#include "Include.h"

class Task {
private:
	bool task_type;
	uint32_t address;
	uint64_t arrive_time;
	bool ready;
public:

	Task(const bool &_read, const uint32_t &_address, const uint64_t &_arrive_time) {
		this->task_type = _read;
		this->address = _address;
		this->arrive_time = _arrive_time;
		this->ready = true;
	}


	bool operator<(const Task &_task) const {
		if (!ready)
			throw std::runtime_error("ERR Cannot Sort Task - Not Ready");
		return this->arrive_time < _task.arrive_time;
	}

	bool getTaskType() const {
		return this->task_type;
	}

	uint32_t getAddress() const {
		return this->address;
	}

	uint64_t getArriveTime() const {
		return this->arrive_time;
	}

};


#endif //CODE_TASK_H

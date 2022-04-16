
#ifndef CODE_TASK_H
#define CODE_TASK_H

#include "Include.h"

class Task {
public:
	bool task_type{TASKTYPE_READ};
	uint32_t address{0};
	uint64_t arrive_time{0};
	bool ready{false};
private:
	Task() = default;

	Task(const bool &_read, const uint32_t &_address, const uint64_t &_arrive_time) {
		this->task_type = _read;
		this->address = _address;
		this->arrive_time = _arrive_time;
		this->ready = true;
	}

	bool operator<(const Task &_task) const {
		return this->arrive_time < _task.arrive_time;
	}

	bool getTaskType() const{
		return this->task_type;
	}

	uint32_t getAddress() const{
		return this->address;
	}

	uint64_t getArriveTime() const{
		return this->arrive_time;
	}

};


#endif //CODE_TASK_H

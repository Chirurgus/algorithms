
#include "greedy.h"

std::vector<Task> task_scedueling(const std::vector<Task>& tasks) {
	std::vector<Task> ret;
	for (unsigned i = 0, t = 0; i < tasks.size(); ++i) {
		if (tasks[i].s >= t) {
			ret.push_back(tasks[i]);
			t = tasks[i].f;
		}
	}
	return ret;
}

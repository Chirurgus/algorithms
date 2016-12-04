#ifndef GUARD_GREEDY_H
#define GUARD_GREEDY_H

#include <vector>

struct Task {
	unsigned s;
	unsigned f;
};
std::vector<Task> task_scedueling(const std::vector<Task>& tasks);
#endif

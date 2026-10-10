#include <stdexcept>
#include "task.hpp"

task task::parse(std::string_view serialized) {
	// TODO #16
	// If serialized begins with an x, expect a completed date to
	// follow. After that, or at the beginning of serialized if there is
	// no leading x, there should be the created date, and everything
	// following the created date is the task name (for now)
	throw std::invalid_argument{"serialized"};
}

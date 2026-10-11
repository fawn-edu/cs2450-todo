// Clang doesn't have <spanstream> yet >:(
// https://github.com/llvm/llvm-project/issues/105145
// As a result, we have to create an owning std::string copy of a
// std::string_view just to parse the date inside it. This is stupid
// and evil. Clang pls
#include <sstream>
#include <stdexcept>
#include <string>
#include "task.hpp"

using std::string_view, std::operator""sv;

task task::parse(string_view serialized) {
	task t{};
	if (serialized.contains("\r\n")) throw std::invalid_argument{"contains illegal line breaks"};

	if (serialized.starts_with("x "sv)) {
		serialized.remove_prefix("x "sv.size());
		const auto end = serialized.find_first_of(' ');
		if (end == string_view::npos) throw std::invalid_argument{"malformed completed date"};
		const auto raw = serialized.substr(0, end);
		std::chrono::sys_seconds tm;
		// XXX: EVIL EVIL EVIL!!!!
		// std::ispanstream{raw} >> std::chrono::parse("%FT%TZ%Z", tm);
		std::string raw_and_evil{raw};
		std::istringstream{raw_and_evil} >> std::chrono::parse("%FT%TZ%Z", tm);
		t.completed.emplace(tm);
		serialized.remove_prefix(end + 1); // +1 for trailing space immediately after end
	}

	const auto end = serialized.find_first_of(' ');
	if (end == string_view::npos) throw std::invalid_argument{"malformed created date"};
	const auto raw = serialized.substr(0, end);
	// XXX: agony
	// std::ispanstream{raw} >> std::chrono::parse("%FT%TZ%Z", t.created);
	std::string raw_and_evil{raw};
	std::istringstream{raw_and_evil} >> std::chrono::parse("%FT%TZ%Z", t.created);
	serialized.remove_prefix(end + 1); // +1 for trailing space immediately after end

	if (serialized.empty()) throw std::invalid_argument{"empty name"};
	t.name = std::string{serialized};
	return t;
}

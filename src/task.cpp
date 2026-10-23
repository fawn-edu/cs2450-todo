#include <cctype>
#include <compare>
#include <sstream>
#include <stdexcept>
#include <string>
#include "task.hpp"

using std::string_view, std::operator""sv;

// Lol how is something like this not already in the standard library?
static constexpr string_view strip_whitespace(string_view s) {
	constexpr auto f = [](char c){ return !std::isspace(c); };
	s = { s.begin(), std::ranges::find_if(s.rbegin(), s.rend(), f).base() };
	return { std::ranges::find_if(s, f), s.end() };
}

static constexpr auto parse_priority(string_view &serialized) {
	if (!serialized.starts_with('(')) throw std::invalid_argument{"priority must begin with '('"};
	const auto end = std::min(serialized.find_first_of(' '), serialized.size() - 1);
	const auto raw = serialized.substr(0, end);
	if (!raw.ends_with(')')) throw std::invalid_argument{"priority must end with ')'"};
	if (raw.size() != "(*)"sv.size()) throw std::invalid_argument{"priority is incorrect length"};
	if ('z' < raw[1] || raw[1] < 'A') throw std::invalid_argument{"priority must be alphabetic"};
	serialized.remove_prefix(std::min(end + 1, serialized.size())); // +1 for trailing space immediately after end
	return std::toupper(raw[1]);
}

static constexpr auto parse_date(string_view &serialized) {
	const auto end = std::min(serialized.find_first_of(' '), serialized.size() - 1);
	auto raw = serialized.substr(0, end);
	std::chrono::sys_seconds tm;
	// XXX: EVIL EVIL EVIL!!!! AGONY!!
	// Clang doesn't have <spanstream> yet >:(
	// https://github.com/llvm/llvm-project/issues/105145
	// As a result, we have to create an owning std::string copy of a
	// std::string_view just to parse the date inside it. This is stupid
	// and evil. Clang pls
	// std::ispanstream{raw} >> std::chrono::parse("%FT%TZ%Z", tm);
	std::string raw_and_evil{raw};
	std::istringstream{raw_and_evil} >> std::chrono::parse("%FT%T%z", tm);
	serialized.remove_prefix(std::min(end + 1, serialized.size())); // +1 for trailing space immediately after end
	return tm;
}

task &task::edit(string_view serialized) {
	if (serialized.contains("\r\n")) throw std::invalid_argument{"edit contains illegal line breaks"};
	if (serialized.starts_with('(')) priority.emplace(parse_priority(serialized));
	serialized = strip_whitespace(serialized);
	if (!serialized.empty()) name = std::string{serialized};
	return *this;
}

task task::parse(string_view serialized) {
	task t{};
	if (serialized.contains("\r\n")) throw std::invalid_argument{"task contains illegal line breaks"};
	bool complete = serialized.starts_with("x "sv);
	if (complete) serialized.remove_prefix("x "sv.size());
	if (serialized.starts_with('(')) t.priority.emplace(parse_priority(serialized));
	if (complete) t.completed.emplace(parse_date(serialized));
	t.created = parse_date(serialized);
	serialized = strip_whitespace(serialized);
	if (serialized.empty()) throw std::invalid_argument{"empty task name"};
	t.name = std::string{serialized};
	return t;
}

std::partial_ordering operator<=>(const task &l, const task &r) {
	using std::is_neq;
	if (auto o = l.completed <=> r.completed; is_neq(o)) return o; // incomplete < complete
	if (auto o = l.priority <=> r.priority; is_neq(o)) return l.priority.has_value() != r.priority.has_value() ? 0 <=> o : o; // priority < no priority
	if (auto o = l.created <=> r.created; is_neq(o)) return o;
	return std::partial_ordering::unordered;
}

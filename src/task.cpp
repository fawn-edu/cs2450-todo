#include <cctype>
#include <chrono>
#include <compare>
#include <ctime>
#include <stdexcept>
#include <string>
#include "task.hpp"

using std::string_view, std::operator""sv;

// Lol how is something like this not already in the standard library?
static string_view strip_whitespace(string_view s) {
	constexpr auto w = " \f\n\r\t\v";
	auto first = s.find_first_not_of(w);
	if (first == string_view::npos) return string_view{};
	auto last = s.find_last_not_of(w);
	return s.substr(first, 1 + last - first);
}

static char parse_priority(string_view &serialized) {
	if (!serialized.starts_with('(')) throw std::invalid_argument{"priority must begin with '('"};
	const auto end = std::min(serialized.find_first_of(' '), serialized.size());
	const auto raw = serialized.substr(0, end);
	if (!raw.ends_with(')')) throw std::invalid_argument{"priority must end with ')'"};
	if (raw.size() != "(*)"sv.size()) throw std::invalid_argument{"priority is incorrect length"};
	if ('z' < raw[1] || raw[1] < 'A') throw std::invalid_argument{"priority must be alphabetic"};
	serialized.remove_prefix(std::min(end + 1, serialized.size())); // +1 for trailing space immediately after end
	return static_cast<char>(std::toupper(raw[1]));
}

static auto parse_date(string_view &serialized) {
	const auto end = std::min(serialized.find_first_of(' '), serialized.size() - 1);
	auto raw = serialized.substr(0, end);
	auto sraw = std::string{raw}; // XXX: sscanf needs null-terminated string
	tm traw{};
	const auto ok = sscanf(sraw.c_str(), "%d-%d-%dT%d:%d:%d",
		&traw.tm_year,
		&traw.tm_mon,
		&traw.tm_mday,
		&traw.tm_hour,
		&traw.tm_min,
		&traw.tm_sec
	);
	if (ok != 3 && ok < 6) throw std::invalid_argument{"invalid date format (must be %F or %FT%T)"};
	serialized.remove_prefix(std::min(end + 1, serialized.size())); // +1 for trailing space immediately after end
	traw.tm_year -= 1900;
	traw.tm_mon -= 1;
	std::time_t t = std::mktime(&traw);
	if (t == -1) throw std::invalid_argument{"unrepresentable date"};
	// Correct for time zone offset
	// TODO: Does this respect DST?
	tm epoch{.tm_mday = 1, .tm_year = 70};
	t -= std::mktime(&epoch);
	return std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::from_time_t(t));
}

task &task::complete() {
	if (completed) return *this;
	this->completed = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
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
	if (serialized.starts_with("due:")) {
		serialized.remove_prefix("due:"sv.size());
		t.due.emplace(parse_date(serialized));
	}
	serialized = strip_whitespace(serialized);
	if (serialized.empty()) throw std::invalid_argument{"empty task name"};
	t.name = std::string{serialized};
	return t;
}

std::partial_ordering operator<=>(const task &l, const task &r) {
	using std::is_neq;
	if (auto o = l.completed <=> r.completed; is_neq(o)) return o; // incomplete < complete
	if (auto o = l.due <=> r.due; is_neq(o)) return l.due.has_value() != r.due.has_value() ? 0 <=> o : o; // due date < no due date
	if (auto o = l.priority <=> r.priority; is_neq(o)) return l.priority.has_value() != r.priority.has_value() ? 0 <=> o : o; // priority < no priority
	if (auto o = l.created <=> r.created; is_neq(o)) return o;
	return std::partial_ordering::unordered;
}

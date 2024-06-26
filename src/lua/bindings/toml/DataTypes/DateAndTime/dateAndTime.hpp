#ifndef DATE_AND_TIME_H
#define DATE_AND_TIME_H

#include <lua/bindings/toml/Options.hpp>
#include <cstddef>
#include <optional>
#include <toml.hpp>

/// Wrapper for `toml::date`.
struct TOMLDate {
	toml::date date;

	TOMLDate(uint16_t year, uint8_t month, uint8_t day) { date = toml::date { year, month, day }; }
	TOMLDate(toml::date d) { date = d; }

	uint16_t getYear() const { return date.year; };
	uint8_t getMonth() const { return date.month; };
	uint8_t getDay() const { return date.day; };

	void setYear(uint16_t year) { date.year = year; };
	void setMonth(uint8_t month) { date.month = month; };
	void setDay(uint8_t day) { date.day = day; };

	void toTable(sol::table & table) {
		table["day"] = date.day;
		table["month"] = date.month;
		table["year"] = date.year;
	}

	bool operator==(const TOMLDate & right) const { return date == right.date; }

	bool operator<(const TOMLDate & right) const { return date < right.date; }

	bool operator<=(const TOMLDate & right) const { return date <= right.date; }
};

/// Wrapper for `toml::time`.
struct TOMLTime {
	toml::time time;

	TOMLTime(uint8_t hour, uint8_t minute, uint8_t second, uint32_t nanoSecond) {
		time = toml::time { hour, minute, second, nanoSecond };
	}

	TOMLTime(toml::time t) { time = t; }

	uint8_t getHour() const { return time.hour; };
	uint8_t getMinute() const { return time.minute; };
	uint8_t getSecond() const { return time.second; };
	uint32_t getNanoSecond() const { return time.nanosecond; };

	void setHour(uint8_t hour) { time.hour = hour; };
	void setMinute(uint8_t minute) { time.minute = minute; };
	void setSecond(uint8_t second) { time.second = second; };
	void setNanoSecond(uint32_t nanoSecond) { time.nanosecond = nanoSecond; };

	void toTable(sol::table & table) {
		table["hour"] = time.hour;
		table["minute"] = time.minute;
		table["second"] = time.second;
		table["nanoSecond"] = time.nanosecond;
	}

	bool operator==(const TOMLTime & right) const { return time == right.time; }

	bool operator<(const TOMLTime & right) const { return time < right.time; }

	bool operator<=(const TOMLTime & right) const { return time <= right.time; }
};

/// Wrapper for `toml::time_offset`.
struct TOMLTimeOffset {
	toml::time_offset timeOffset;

	TOMLTimeOffset(toml::time_offset tO) { timeOffset = tO; }

	TOMLTimeOffset(int8_t hours, int8_t minutes) {
		timeOffset = toml::time_offset(hours, minutes);
	};

	int16_t minutes() const { return timeOffset.minutes; };

	void toTable(sol::table & table) { table["minutes"] = timeOffset.minutes; }

	bool operator==(const TOMLTimeOffset & right) const { return timeOffset == right.timeOffset; }

	bool operator<(const TOMLTimeOffset & right) const { return timeOffset < right.timeOffset; }

	bool operator<=(const TOMLTimeOffset & right) const { return timeOffset <= right.timeOffset; }
};

/// Wrapper for `toml::date_time`.
struct TOMLDateTime {

	TOMLDate date;
	TOMLTime time;
	std::optional<TOMLTimeOffset> timeOffset = std::optional<TOMLTimeOffset>();

	TOMLDateTime(TOMLDate d, TOMLTime t) : date(d), time(t) {};

	TOMLDateTime(TOMLDate d, TOMLTime t, TOMLTimeOffset tO) : date(d), time(t), timeOffset(tO) {};

	TOMLDate getDate() { return date; };
	TOMLTime getTime() { return time; };
	TOMLTimeOffset getTimeOffset() { return timeOffset.value(); };

	void setDate(TOMLDate d) { date = d; };
	void setTime(TOMLTime t) { time = t; };
	void setTimeOffset(TOMLTimeOffset tO) { timeOffset = tO; };

	toml::date_time asDateTime() const {
		auto dt = toml::date_time(date.date, time.time);

		if (timeOffset) { dt.offset = timeOffset.value().timeOffset; }

		return dt;
	};

	void toTable(sol::table & table) {
		auto dateTable = table.create();
		date.toTable(dateTable);

		auto timeTable = table.create();
		time.toTable(timeTable);

		table["date"] = dateTable;
		table["time"] = timeTable;

		if (timeOffset) {
			auto timeOffsetTable = table.create();
			timeOffset->toTable(timeOffsetTable);
			table["timeOffset"] = timeOffsetTable;
		}
	}

	bool operator==(const TOMLDateTime & right) const { return asDateTime() == right.asDateTime(); }

	bool operator<(const TOMLDateTime & right) const { return asDateTime() < right.asDateTime(); }

	bool operator<=(const TOMLDateTime & right) const { return asDateTime() <= right.asDateTime(); }
};

std::ostream & operator<<(std::ostream & os, const TOMLDate & date);
std::ostream & operator<<(std::ostream & os, const TOMLTime & time);
std::ostream & operator<<(std::ostream & os, const TOMLTimeOffset & timeOffset);
std::ostream & operator<<(std::ostream & os, const TOMLDateTime & dateTime);

#endif /* DATE_AND_TIME_H */

#pragma once

#include <cstddef>
#include <string>

struct IGetKeyValue {
	virtual size_t Count() = 0;
	virtual std::string GetKey(size_t pos) = 0;
	virtual std::string GetValue(std::string const& key) = 0;
	virtual ~IGetKeyValue() {}
};

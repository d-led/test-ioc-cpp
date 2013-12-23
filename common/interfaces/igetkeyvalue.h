#pragma once

struct IGetKeyValue {
	virtual size_t Count() const = 0;
	virtual std::string GetKey(size_t pos) const = 0;
	virtual std::string GetValue(std::string const& key) const = 0;
	virtual ~IGetKeyValue() {}
};

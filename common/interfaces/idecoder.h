#pragma once

struct IGetKeyValue {
	virtual size_t Count() const = 0;
	virtual std::string GetKey() = 
};

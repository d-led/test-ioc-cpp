#pragma once

#include <string>

struct IModel {
	virtual std::string Get() const = 0;
	virtual ~IModel() {}
};

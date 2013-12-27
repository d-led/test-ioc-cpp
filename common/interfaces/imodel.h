#pragma once

#include <string>

struct IModel {
	virtual std::string Get() = 0;
	virtual ~IModel() {}
};

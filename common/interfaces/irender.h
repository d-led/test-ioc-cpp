#pragma once

#include <string>

struct IRender {
	virtual std::string Render() = 0;
	virtual ~IRender() {}
};

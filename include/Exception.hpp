#pragma once

#include <string>
class Exception {
	public:
		Exception(const std::string& i) :info(i) {}
		std::string getInfo() const { return info; }
	private:
		std::string info;
	};

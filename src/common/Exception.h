#pragma once

#include <stdexcept>
#include <string>

class Exception : public std::runtime_error {
  public:
	using runtime_error::runtime_error;
	virtual std::string prettyPrint() const { return std::string(what()); }
};

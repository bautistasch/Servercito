#pragma once

#include <iostream>

class AbstractProtocol {
public:
	virtual std::string getAnswer(std::string dataRead) = 0;
};
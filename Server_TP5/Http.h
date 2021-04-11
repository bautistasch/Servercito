#pragma once

#include <iostream>
#include "AbstractProtocol.h"

class Http : public AbstractProtocol{
public:

	std::string getAnswer(std::string dataRead);

private:

	void HtppIntrepratateInput();
	std::string inputTCP;
	std::string HtppResponse;
	std::string HtppBodyResponse;
	std::string HtppHeaderResponse;

	std::string HttpPath;

	std::string getWord(int * index);

};

#include <SimpleStringSplitter.h>

SimpleStringSplitter::SimpleStringSplitter()
{
}
SimpleStringSplitter::~SimpleStringSplitter()
{
}

std::vector<std::string> SimpleStringSplitter::splitString(
	char		splitter,
	std::string	splitting)
{
	split_.clear();
	ss_ = std::stringstream(splitting);

	while(std::getline(ss_, intermediate_, splitter))
	{
		if(!intermediate_.empty())
			if(intermediate_.front() != splitter)
				split_.push_back(intermediate_); //avoid duplicate splitter-chars
	}

	return split_;
}
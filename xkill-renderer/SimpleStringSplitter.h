#ifndef XKILL_RENDERER_SIMPLESTRINGSPLITTER_H
#define XKILL_RENDERER_SIMPLESTRINGSPLITTER_H

#include <string>
#include <sstream>
#include <vector>

//! Minor class offering basic functionality to split strings based on a given string and splitting char.
/*!
\ingroup xkill-renderer
*/
class SimpleStringSplitter
{
public:
	//! Does nothing.
	SimpleStringSplitter();
	//! Does nothing.
	~SimpleStringSplitter();

	//! Splits given string based on splitter char.
	/*!
		\param splitter Character to split given string.
		\param splitting String to split
		\return Vector containing split strings.
	*/
	std::vector<std::string> splitString(
		char		splitter,
		std::string	splitting);
protected:
private:
	std::stringstream	ss_;			//!< Used to split strings with std::getline-function.
	std::string			intermediate_;	//!< Intermediate string storing plit strings.

	std::vector<std::string> split_;	//!< Result returned to user.
};

#endif //XKILL_RENDERER_SIMPLESTRINGSPLITTER_H
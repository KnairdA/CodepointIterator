#ifndef CODEPOINT_ITERATOR_H_
#define CODEPOINT_ITERATOR_H_

#include <iterator>
#include <string>
#include <exception>

namespace UTF8 {

class CodepointIterator : public std::iterator<std::bidirectional_iterator_tag,
                                               char32_t,
                                               std::string::difference_type> {
	static_assert(
		sizeof(std::string::value_type) == 1,
		"CodepointIterator only supports single-byte UTF-8 encoded input"
	);

	public:
		CodepointIterator(std::string::const_iterator);
		CodepointIterator(const CodepointIterator&);

		CodepointIterator& operator=(const CodepointIterator&);

		bool operator==(const CodepointIterator&) const;
		bool operator==(const std::string::const_iterator&) const;

		bool operator!=(const CodepointIterator&) const;
		bool operator!=(const std::string::const_iterator&) const;

		std::ptrdiff_t operator-(const std::string::const_iterator&) const;

		char32_t operator*();

		CodepointIterator& operator++();
		CodepointIterator& operator--();

		CodepointIterator operator++(int);
		CodepointIterator operator--(int);

	private:
		std::string::const_iterator iterator_;

};

class codepoint_invalid: public std::exception {
	virtual const char* what() const throw() {
		return "codepoint_invalid";
	}
};

}

#endif  // CODEPOINT_ITERATOR_H_

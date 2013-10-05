#include "codepoint_iterator.h"

#include <cstdint>

namespace {

enum class CodeUnitType : uint8_t {
	CONTINUATION = 128, // 10000000
	LEADING      = 64,  // 01000000
	THREE        = 32,  // 00100000
	FOUR         = 16,  // 00010000
};

enum class CodePoint : uint8_t {
	CONTINUATION = 63,  // 00111111
	TWO          = 31,  // 00011111
	THREE        = 15,  // 00001111
	FOUR         = 7,   // 00000111
};

inline bool match(const uint8_t& codeUnit, CodeUnitType&& type) {
	return codeUnit & static_cast<uint8_t>(type);
}

inline void write(char32_t& codePoint,
                  const uint8_t& codeUnit,
                  CodePoint&& mask,
                  const uint8_t& offset) {
	codePoint += (codeUnit & static_cast<uint8_t>(mask)) << offset;
}

}

namespace UTF8 {

CodepointIterator::CodepointIterator(std::string::const_iterator iter):
	iterator_(iter),
	dereferenced_(false),
	codepoint_(0) { }

CodepointIterator::CodepointIterator(const CodepointIterator& src):
	iterator_(src.iterator_),
	dereferenced_(src.dereferenced_),
	codepoint_(src.codepoint_) { }

CodepointIterator& CodepointIterator::operator=(const CodepointIterator& src) {
	this->iterator_     = src.iterator_;
	this->dereferenced_ = src.dereferenced_;
	this->codepoint_    = src.codepoint_;
	
	return *this;
}

bool CodepointIterator::operator==(const CodepointIterator& src) const {
	return this->iterator_ == src.iterator_;
}

bool CodepointIterator::operator!=(const CodepointIterator& src) const {
	return this->iterator_ != src.iterator_;
}

bool CodepointIterator::operator==(
	const std::string::const_iterator& src) const {
	return this->iterator_ == src;
}

bool CodepointIterator::operator!=(
	const std::string::const_iterator& src) const {
	return this->iterator_ != src;
}

char32_t CodepointIterator::operator*() {
	if ( !this->dereferenced_ ) {
		uint8_t currByte    = *(this->iterator_);
		this->dereferenced_ = true;
		this->codepoint_    = 0;

		if ( match(currByte, CodeUnitType::CONTINUATION) ) {
			if ( match(currByte, CodeUnitType::THREE) ) {
				if ( match(currByte, CodeUnitType::FOUR) ) {
					write(this->codepoint_,
					      currByte,
					      CodePoint::FOUR,
					      18);
					write(this->codepoint_,
					      *(this->iterator_ + 1),
					      CodePoint::CONTINUATION,
					      12);
					write(this->codepoint_,
					      *(this->iterator_ + 2),
					      CodePoint::CONTINUATION,
					      6);
					write(this->codepoint_,
					      *(this->iterator_ + 3),
					      CodePoint::CONTINUATION,
					      0);
				} else {
					write(this->codepoint_,
					      currByte,
					      CodePoint::THREE,
					      12);
					write(this->codepoint_,
					      *(this->iterator_ + 1),
					      CodePoint::CONTINUATION,
					      6);
					write(this->codepoint_,
					      *(this->iterator_ + 2),
					      CodePoint::CONTINUATION,
					      0);
				}
			} else {
				write(this->codepoint_,
				      currByte,
				      CodePoint::TWO,
				      6);
				write(this->codepoint_,
				      *(this->iterator_ + 1),
				      CodePoint::CONTINUATION,
				      0);
			}
		} else {
			this->codepoint_ = currByte;
		}
	}

	return this->codepoint_;
}

CodepointIterator& CodepointIterator::operator++() {
	this->dereferenced_                 = false;
	uint8_t currByte                    = *(this->iterator_);
	std::string::difference_type offset = 1;

	if ( match(currByte, CodeUnitType::CONTINUATION) ) {
		if ( match(currByte, CodeUnitType::THREE) ) {
			if ( match(currByte, CodeUnitType::FOUR) ) {
				offset = 4;
			} else {
				offset = 3;
			}
		} else {
			offset = 2;
		}
	}

	this->iterator_ += offset;

	return *this;
}

CodepointIterator& CodepointIterator::operator--() {
	this->dereferenced_ = false;
	--this->iterator_;

	if ( match(*(this->iterator_), CodeUnitType::CONTINUATION) ) {
		--this->iterator_;

		if ( !match(*(this->iterator_), CodeUnitType::LEADING) ) {
			--this->iterator_;

			if ( !match(*(this->iterator_), CodeUnitType::LEADING) ) {
				--this->iterator_;

				if ( !match(*(this->iterator_), CodeUnitType::LEADING) ) {
					throw codepoint_invalid();
				}
			}
		}
	}

	return *this;
}

CodepointIterator CodepointIterator::operator++(int) {
	CodepointIterator oldIter(*this);

	++(*this);

	return oldIter;
}

CodepointIterator CodepointIterator::operator--(int) {
	CodepointIterator oldIter(*this);

	--(*this);

	return oldIter;
}

}

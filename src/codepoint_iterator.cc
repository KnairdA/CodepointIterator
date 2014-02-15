#include "codepoint_iterator.h"

#include "utility.h"

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

		if ( match(currByte, dtl::CodeUnitType::CONTINUATION) ) {
			if ( match(currByte, dtl::CodeUnitType::THREE) ) {
				if ( match(currByte, dtl::CodeUnitType::FOUR) ) {
					dtl::write(this->codepoint_,
					           currByte,
					           dtl::CodePoint::FOUR,
					           18);
					dtl::write(this->codepoint_,
					           *(this->iterator_ + 1),
					           dtl::CodePoint::CONTINUATION,
					           12);
					dtl::write(this->codepoint_,
					           *(this->iterator_ + 2),
					           dtl::CodePoint::CONTINUATION,
					           6);
					dtl::write(this->codepoint_,
					           *(this->iterator_ + 3),
					           dtl::CodePoint::CONTINUATION,
					           0);
				} else {
					dtl::write(this->codepoint_,
					           currByte,
					           dtl::CodePoint::THREE,
					           12);
					dtl::write(this->codepoint_,
					           *(this->iterator_ + 1),
					           dtl::CodePoint::CONTINUATION,
					           6);
					dtl::write(this->codepoint_,
					           *(this->iterator_ + 2),
					           dtl::CodePoint::CONTINUATION,
					           0);
				}
			} else {
				dtl::write(this->codepoint_,
				           currByte,
				           dtl::CodePoint::TWO,
				           6);
				dtl::write(this->codepoint_,
				           *(this->iterator_ + 1),
				           dtl::CodePoint::CONTINUATION,
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

	if ( match(currByte, dtl::CodeUnitType::CONTINUATION) ) {
		if ( match(currByte, dtl::CodeUnitType::THREE) ) {
			if ( match(currByte, dtl::CodeUnitType::FOUR) ) {
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
	this->iterator_.operator--();

	if ( match(*(this->iterator_), dtl::CodeUnitType::CONTINUATION) ) {
		this->iterator_.operator--();

		if ( !match(*(this->iterator_), dtl::CodeUnitType::LEADING) ) {
			this->iterator_.operator--();

			if ( !match(*(this->iterator_), dtl::CodeUnitType::LEADING) ) {
				this->iterator_.operator--();

				if ( !match(*(this->iterator_), dtl::CodeUnitType::LEADING) ) {
					throw codepoint_invalid();
				}
			}
		}
	}

	return *this;
}

CodepointIterator CodepointIterator::operator++(int) {
	CodepointIterator oldIter(*this);

	this->operator++();

	return oldIter;
}

CodepointIterator CodepointIterator::operator--(int) {
	CodepointIterator oldIter(*this);

	this->operator--();

	return oldIter;
}

}

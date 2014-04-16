#include "codepoint_iterator.h"

#include "utility.h"

namespace UTF8 {

CodepointIterator::CodepointIterator(std::string::const_iterator iter):
	iterator_(iter) { }

CodepointIterator::CodepointIterator(const CodepointIterator& src):
	iterator_(src.iterator_) { }

CodepointIterator& CodepointIterator::operator=(const CodepointIterator& src) {
	this->iterator_ = src.iterator_;

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

std::ptrdiff_t CodepointIterator::operator-(
	const std::string::const_iterator& src) const {
	return this->iterator_ - src;
}

char32_t CodepointIterator::operator*() {
	const std::uint8_t currByte(
		static_cast<std::uint8_t>(*(this->iterator_))
	);
	char32_t codePoint{};

	if ( match(currByte, dtl::CodeUnitType::CONTINUATION) ) {
		if ( match(currByte, dtl::CodeUnitType::THREE) ) {
			if ( match(currByte, dtl::CodeUnitType::FOUR) ) {
				dtl::write(codePoint,
				           currByte,
				           dtl::CodePoint::FOUR,
				           18);
				dtl::write(codePoint,
				           *(this->iterator_ + 1),
				           dtl::CodePoint::CONTINUATION,
				           12);
				dtl::write(codePoint,
				           *(this->iterator_ + 2),
				           dtl::CodePoint::CONTINUATION,
				           6);
				dtl::write(codePoint,
				           *(this->iterator_ + 3),
				           dtl::CodePoint::CONTINUATION,
				           0);
			} else {
				dtl::write(codePoint,
				           currByte,
				           dtl::CodePoint::THREE,
				           12);
				dtl::write(codePoint,
				           *(this->iterator_ + 1),
				           dtl::CodePoint::CONTINUATION,
				           6);
				dtl::write(codePoint,
				           *(this->iterator_ + 2),
				           dtl::CodePoint::CONTINUATION,
				           0);
			}
		} else {
			dtl::write(codePoint,
			           currByte,
			           dtl::CodePoint::TWO,
			           6);
			dtl::write(codePoint,
			           *(this->iterator_ + 1),
			           dtl::CodePoint::CONTINUATION,
			           0);
		}
	} else {
		codePoint = currByte;
	}

	return codePoint;
}

CodepointIterator& CodepointIterator::operator++() {
	const std::uint8_t currByte(
		static_cast<std::uint8_t>(*(this->iterator_))
	);
	std::string::difference_type offset(1);

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

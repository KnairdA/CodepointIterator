#ifndef CODEPOINT_ITERATOR_UTILITY_H_
#define CODEPOINT_ITERATOR_UTILITY_H_

#include <cstdint>

namespace UTF8 {
namespace dtl {

enum class CodeUnitType : std::uint8_t {
	CONTINUATION = 0b10000000,
	LEADING      = 0b01000000,
	THREE        = 0b00100000,
	FOUR         = 0b00010000
};

enum class CodePoint : std::uint8_t {
	CONTINUATION = 0b00111111,
	TWO          = 0b00011111,
	THREE        = 0b00001111,
	FOUR         = 0b00000111
};

inline bool match(const std::uint8_t unit, const CodeUnitType type) {
	return unit & static_cast<std::uint8_t>(type);
}

inline void write(char32_t& point,
                  const std::uint8_t unit,
                  const CodePoint mask,
                  const std::uint8_t offset) {
	point += (unit & static_cast<std::uint8_t>(mask)) << offset;
}

}
}

#endif  // CODEPOINT_ITERATOR_UTILITY_H_

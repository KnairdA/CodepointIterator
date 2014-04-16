#ifndef CODEPOINT_ITERATOR_UTILITY_H_
#define CODEPOINT_ITERATOR_UTILITY_H_

#include <cstdint>

namespace UTF8 {
namespace dtl {

enum class CodeUnitType : std::uint8_t {
	CONTINUATION = (128 >> 0),       // 10000000
	LEADING      = (128 >> 1),       // 01000000
	THREE        = (128 >> 2),       // 00100000
	FOUR         = (128 >> 3),       // 00010000
};

enum class CodePoint : std::uint8_t {
	CONTINUATION = (UINT8_MAX >> 2), // 00111111
	TWO          = (UINT8_MAX >> 3), // 00011111
	THREE        = (UINT8_MAX >> 4), // 00001111
	FOUR         = (UINT8_MAX >> 5), // 00000111
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

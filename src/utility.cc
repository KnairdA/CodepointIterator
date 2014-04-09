#include "utility.h"

namespace UTF8 {
namespace dtl {

bool match(const std::uint8_t& codeUnit, CodeUnitType&& type) {
	return codeUnit & static_cast<std::uint8_t>(type);
}

void write(char32_t& codePoint,
           const std::uint8_t& codeUnit,
           CodePoint&& mask,
           const std::uint8_t& offset) {
	codePoint += (codeUnit & static_cast<std::uint8_t>(mask)) << offset;
}

}
}

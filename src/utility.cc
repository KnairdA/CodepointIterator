#include "utility.h"

namespace UTF8 {
namespace dtl {

bool match(const uint8_t& codeUnit, CodeUnitType&& type) {
	return codeUnit & static_cast<uint8_t>(type);
}

void write(char32_t& codePoint,
           const uint8_t& codeUnit,
           CodePoint&& mask,
           const uint8_t& offset) {
	codePoint += (codeUnit & static_cast<uint8_t>(mask)) << offset;
}

}
}

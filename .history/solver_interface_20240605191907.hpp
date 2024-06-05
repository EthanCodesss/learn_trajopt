#pragma once

#include <cassert>
#include <iosfwd>
#include <limits>
#include <memory>
#include <vector>

namespace Json {
class Value;
}

namespace sco {
enum ConstraintType { EQ, INEQ };

using ConstraintTypeVector = std::vector<ConstraintType>;

} // namespace sco
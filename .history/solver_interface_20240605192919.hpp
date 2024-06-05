#pragma once

#include <cassert>
#include <iosfwd>
#include <limits>
#include <memory>
#include <trajopt_sco/sco_common.hpp>
#include <vector>
namespace Json {
class Value;
}

namespace sco {
enum ConstraintType { EQ, INEQ };

using ConstraintTypeVector = std::vector<ConstraintType>;

enum CvxOptStatus { CVX_SOLVED, CVX_INFEASIBLE, CVX_FAILED };

class Model {
public:
  using Ptr = std::shared_ptr<Model>;
  using ConstPtr = std::shared_ptr<const Model>;
  Model() = default;
  virtual ~Model() = default;
  Model(const Model &) = default;
  Model(Model &&) = default;
  Model &operator=(Model &&) = default;

  virtual Var addVar(const std::string &name) = 0;
}

} // namespace sco
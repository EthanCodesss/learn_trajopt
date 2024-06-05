#include <trajopt_sco/macros.h>
TRAJOPT_IGNORE_WARNINGS_PUSH
#include <boost/format.hpp>
#include <cstdio>
#include <iostream>
#include <sstream>
TRAJOPT_IGNORE_WARNINGS_POP

#include <trajopt_sco/expr_ops.hpp>
#include <trajopt_sco/logging.hpp>
#include <trajopt_sco/modeling.hpp>
#include <trajopt_sco/sco_common.hpp>

namespace sco {
ConvexObjective::ConvexObjective(Model *model) : model_(model) {}
void ConvexObjective::addAffExpr(const AffExpr &affexpr) {
  exprInc(quad_, affexpr);
}
void ConvexObjective::addQuadExpr(const QuadExpr &quadexpr) {
  exprInc(quad_, quadexpr);
}

} // namespace sco

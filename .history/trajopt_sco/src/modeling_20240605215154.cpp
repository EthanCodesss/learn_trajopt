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

// 添加铰链损失函数
void ConvexObjective::addHinge(const AffExpr &affexpr, double coeff) {
  Var hinge = model_->addVar("hinge", 0, static_cast<double>(INFINITY));
  vars_.push_back(hinge);
  ineqs_.push_back(affexpr);
  exprDec(ineqs_.back(), hinge);
  AffExpr hinge_cost = exprMult(AffExpr(hinge), coeff);
  exprInc(quad_, hinge_cost);
}

} // namespace sco

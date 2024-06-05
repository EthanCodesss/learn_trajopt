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

// 将非凸的绝对值目标函数转化为凸函数， 通过添加变量以及等式约束
void ConvexObjective::addAbs(const AffExpr &affexpr, double coeff) {
  // Add variables that will enforce ABS
  Var neg = model_->addVar("neg", 0, static_cast<double>(INFINITY));
  Var pos = model_->addVar("pos", 0, static_cast<double>(INFINITY));
  vars_.push_back(neg);
  vars_.push_back(pos);
  // Coeff will be applied whenever neg/pos are not 0
  AffExpr neg_plus_pos;
  neg_plus_pos.coeffs = DblVec(2, coeff);
  neg_plus_pos.vars.reserve(neg_plus_pos.vars.size() + 2);
  neg_plus_pos.vars.push_back(neg);
  neg_plus_pos.vars.push_back(pos);
  exprInc(quad_, neg_plus_pos);
  // Add neg/pos to problem. They will be nonzero when ABS is not satisfied
  AffExpr affeq = affexpr;
  affeq.vars.reserve(affeq.vars.size() + 2);
  affeq.vars.push_back(neg);
  affeq.vars.push_back(pos);
  affeq.coeffs.reserve(affeq.coeffs.size() + 2);
  affeq.coeffs.push_back(1);
  affeq.coeffs.push_back(-1);
  eqs_.push_back(affeq);
}

} // namespace sco

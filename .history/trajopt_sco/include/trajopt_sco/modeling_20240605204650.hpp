#pragma once

/*
 * Model a non-convex optimization problem by defining Cost and Constraint
 * objects
 * which know how to generate a convex approximation
 *
 *
 */
#include <trajopt_sco/macros.h>
TRAJOPT_IGNORE_WARNINGS_PUSH
// vector 提供容器类
#include <vector>

// memory提供智能指针模版类
#include <memory>
TRAJOPT_IGNORE_WARNINGS_POP

#include <trajopt_sco/solver_interface.hpp>

namespace sco {
/**
Stores convex terms in a objective
For non-quadratic terms like hinge(x) and abs(x), it needs to add auxilliary
variables and linear constraints to the model
Note: When this object is deleted, the constraints and variables it added to the
model are removed
 */
class ConvexObjective {
public:
  using Ptr = std::shared_ptr<ConvexObjective>;

  ConvexObjective(Model *model);
  // 当派生类对象被删除时，如果基类的析构函数是虚函数，那么整个继承链上的析构函数都会被正确调用
  virtual ~ConvexObjective();
  ConvexObjective(const ConvexObjective &) = delete;
  ConvexObjective &operator=(const ConvexObjective &) = delete;
  ConvexObjective(ConvexObjective &&) = default;
  ConvexObjective &operator=(ConvexObjective &&) = default;

  void addAffExpr(const AffExpr &);
  void addQuadExpr(const QuadExpr &);
  void addHinge(const AffExpr &, double coeff);
  void addAbs(const AffExpr &, double coeff);
  void addHinges(const AffExprVector &);
  void addL1Norm(const AffExprVector &);
  void addL2Norm(const AffExprVector &);
  void addMax(const AffExprVector &);

  bool inModel() const { return model_ != nullptr; }
  void addConstraintsToModel();
  void removeFromModel();
  double value(const DblVec &x) const;

  Model *model_;
  /// The objective Function
  QuadExpr quad_;
  VarVector vars_;
  /// EQ Constraints
  AffExprVector eqs_;
  // INEQ Constraints
  AffExprVector ineqs_;
  CntVector cnts_;
};

class ConvexConstraints {
public:
  using Ptr = std::shared_ptr<ConvexConstraints>;

  ConvexConstraints(Model *model) : model_(model);

private:
  Model *model_{nullptr};
}

} // namespace sco

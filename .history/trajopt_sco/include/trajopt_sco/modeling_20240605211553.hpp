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

  ConvexConstraints(Model *model) : model_(model){};
  virtual ~ConvexConstraints();

  /** @todo Add a vector version to allow using reserve to avoid multiple
   * allocations */
  /** Expression that should == 0 */
  void addEqCnt(const AffExpr &);
  /** @todo Add a vector version to allow using reserve to avoid multiple
   * allocations */
  /** Expression that should <= 0 */
  void addIneqCnt(const AffExpr &);
  void setModel(Model *model) {
    assert(!inModel());
    model_ = model;
  }
  bool inModel() { return model_ != nullptr; }
  void addConstraintsToModel();
  void removeFromModel();

  DblVec violations(const DblVec &x);
  double violation(const DblVec &x);

  AffExprVector eqs_;
  AffExprVector ineqs_;

private:
  Model *model_{nullptr};
  CntVector cnts_;
  ConvexConstraints() = default;
  ConvexConstraints(const ConvexConstraints &) = default;
  ConvexConstraints &operator=(const ConvexConstraints &) = default;
  ConvexConstraints(ConvexConstraints &&) = default;
  ConvexConstraints &operator=(ConvexConstraints &&) = default;
};

/**
Non-convex cost function, which knows how to calculate its convex approximation
(convexify() method)
*/
class Cost {
public:
  using Ptr = std::shared_ptr<Cost>;

  /** Evaluate at solution vector x*/
  virtual double value(const DblVec &) = 0;
  /** Convexify at solution vector x*/
  virtual ConvexObjective::Ptr convex(const DblVec &x, Model *model) = 0;
  /** Get problem variables associated with this cost */
  virtual VarVector getVars() = 0;
  std::string name() { return name_; }
  void setName(const std::string &name) { name_ = name; }

  Cost() = default;
  Cost(std::string name) : name_(std::move(name)) {}
  virtual ~Cost() = default;
  Cost(const Cost &) = default;
  Cost &operator=(const Cost &) = default;
  Cost(Cost &&) = default;
  Cost &operator=(Cost &&) = default;

protected:
  std::string name_{"unnamed"};
};

/**
Non-convex vector-valued constraint function, which knows how to calculate its
convex approximation
*/
class Constraint {
public:
  using Ptr = std::shared_ptr<Constraint>;

  /** inequality vs equality */
  virtual ConstraintType type() = 0;
  /** Evaluate at solution vector x*/
  virtual DblVec value(const DblVec &x) = 0;
  /** Convexify at solution vector x*/
  virtual ConvexConstraints::Ptr convex(const DblVec &x, Model *model) = 0;
  /** Calculate constraint violations (positive part for inequality constraint,
   * absolute value for inequality constraint)*/
  DblVec violations(const DblVec &x);
  /** Sum of violations */
  double violation(const DblVec &x);
  /** Get problem variables associated with this constraint */
  virtual VarVector getVars() = 0;
  std::string name() { return name_; }
  void setName(const std::string &name) { name_ = name; }

  Constraint() = default;
  Constraint(std::string name) : name_(std::move(name)) {}
  virtual ~Constraint() = default;
  Constraint(const Constraint &) = default;
  Constraint &operator=(const Constraint &) = default;
  Constraint(Constraint &&) = default;
  Constraint &operator=(Constraint &&) = default;

protected:
  std::string name_{"unnamed"};
};

class EqConstraint : public Constraint {
public:
  using Ptr = std::shared_ptr<EqConstraint>;

  ConstraintType type() override { return EQ; }
  EqConstraint() = default;
  EqConstraint(std::string name) : Constraint(std::move(name)) {}
};

class IneqConstraint : public Constraint {
public:
  using Ptr = std::shared_ptr<IneqConstraint>;

  ConstraintType type() override { return INEQ; }
  IneqConstraint() = default;
  IneqConstraint(std::string name) : Constraint(std::move(name)) {}
};

/**
Non-convex optimization problem
*/
// 用于表示优化问题的类，封装了创建优化问题，添加变量，设置变量界限，添加约束和成本函数。
class OptProb {
public:
  using Ptr = std::shared_ptr<OptProb>;

protected:
  Model::Ptr model_;
  VarVector vars_;
  DblVec lower_bounds_;
  DblVec upper_bounds_;
  std::vector<Cost::Ptr> costs_;
  std::vector<Constraint::Ptr> eqcnts_;
  std::vector<Constraint::Ptr> ineqcnts_;
};

} // namespace sco

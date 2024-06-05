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

  /**
   * @brief Add a var to the model
   * @details These must be threadsafe
   */
  virtual Var addVar(const std::string &name) = 0;
  virtual Var addVar(const std::string &name, double lb, double ub);

  /**
   * @brief Add a equation to the model
   * @details These must be threadsafe
   */
  virtual Cnt addEqCnt(const AffExpr &,
                       const std::string &name) = 0; // expr == 0
  virtual Cnt addIneqCnt(const AffExpr &,
                         const std::string &name) = 0; // expr <= 0
  virtual Cnt addIneqCnt(const QuadExpr &,
                         const std::string &name) = 0; // expr <= 0

  /**
   * @brief Remove items from model
   * @details These must be threadsafe
   */
  virtual void removeVar(const Var &var);
  virtual void removeCnt(const Cnt &cnt);
  virtual void removeVars(const VarVector &vars) = 0;
  virtual void removeCnts(const CntVector &cnts) = 0;

  /**  @details It is not neccessary to make the following methods threadsafe */
  virtual void update() = 0; // call after adding/deleting stuff
  virtual void setVarBounds(const Var &var, double lower, double upper);
  virtual void setVarBounds(const VarVector &vars, const DblVec &lower,
                            const DblVec &upper) = 0;
  virtual double getVarValue(const Var &var) const;
  virtual DblVec getVarValues(const VarVector &vars) const = 0;
  virtual CvxOptStatus optimize() = 0;

  virtual void setObjective(const AffExpr &) = 0;
  virtual void setObjective(const QuadExpr &) = 0;
  virtual void writeToFile(const std::string &fname) const = 0;

  virtual VarVector getVars() const = 0;
}

} // namespace sco
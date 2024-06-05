#include <trajopt_sco/macros.h>
TRAJOPT_IGNORE_WARNINGS_PUSH
#include <boost/format.hpp>
#include <chrono>
#include <cmath>
#include <cstdio>
TRAJOPT_IGNORE_WARNINGS_POP

#include <trajopt_sco/expr_ops.hpp>
#include <trajopt_sco/logging.hpp>
#include <trajopt_sco/modeling.hpp>
#include <trajopt_sco/optimizers.hpp>
#include <trajopt_sco/sco_common.hpp>
#include <trajopt_sco/solver_interface.hpp>
#include <trajopt_sco/stl_to_string.hpp>

namespace sco {
const bool SUPER_DEBUG_MODE = false;

std::ostream &operator<<(std::ostream &o, const OptResults &r) {
  o << "Optimization results:" << std::endl
    << "status: " << statusToString(r.status) << std::endl
    << "cost values: " << trajopt_common::Str(r.cost_vals) << std::endl
    << "constraint violations: " << trajopt_common::Str(r.cnt_viols)
    << std::endl
    << "n func evals: " << r.n_func_evals << std::endl
    << "n qp solves: " << r.n_qp_solves << std::endl;
  return o;
}
// todo: use different coeffs for each constraint
std::vector<ConvexObjective::Ptr>
cntsToCosts(const std::vector<ConvexConstraints::Ptr> &cnts,
            const std::vector<double> &err_coeffs, Model *model) {
  assert(cnts.size() == err_coeffs.size());
  std::vector<ConvexObjective::Ptr> out;
  for (std::size_t c = 0; c < cnts.size(); ++c) {
    // std::make_shared
    // 用于创建一个ConvexObjective类型的对象，model作为参数传入到其构造函数中
    auto obj = std::make_shared<ConvexObjective>(model);
    for (std::size_t idx = 0; idx < cnts[c]->eqs_.size(); ++idx) {
      const AffExpr &aff = cnts[c]->eqs_[idx];
      obj->addAbs(aff, err_coeffs[c]);
    }
    for (std::size_t idx = 0; idx < cnts[c]->ineqs_.size(); ++idx) {
      const AffExpr &aff = cnts[c]->ineqs_[idx];
      obj->addHinge(aff, err_coeffs[c]);
    }
    out.push_back(obj);
  }
  return out;
}

void Optimizer::addCallback(const Callback &cb) { callbacks_.push_back(cb); }
void Optimizer::callCallbacks() {
  for (auto &callback : callbacks_) {
    callback(prob_.get(), results_);
  }
}

} // namespace sco

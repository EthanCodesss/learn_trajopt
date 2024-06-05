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

} // namespace sco

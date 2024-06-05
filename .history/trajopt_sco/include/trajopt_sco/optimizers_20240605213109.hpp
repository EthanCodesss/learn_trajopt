#pragma once
#include <trajopt_sco/macros.h>
TRAJOPT_IGNORE_WARNINGS_PUSH
#include <array>
#include <functional>
#include <string>
TRAJOPT_IGNORE_WARNINGS_POP

#include <trajopt_sco/modeling.hpp>

/*
 * Algorithms for non-convex, constrained optimization
 */
namespace sco {
enum OptStatus {
  OPT_CONVERGED,
  OPT_SCO_ITERATION_LIMIT, // hit iteration limit before convergence
  OPT_PENALTY_ITERATION_LIMIT,
  OPT_TIME_LIMIT,
  OPT_FAILED,
  INVALID
};
static const std::array<std::string, 6> OptStatus_strings = {
    "OPT_CONVERGED",
    "OPT_SCO_ITERATION_LIMIT",
    "OPT_PENALTY_ITERATION_LIMIT",
    "OPT_TIME_LIMIT",
    "OPT_FAILED",
    "INVALID"};
inline std::string statusToString(OptStatus status) {
  return OptStatus_strings[status];
}

struct OptResults {
  /* data */
};

} // namespace sco
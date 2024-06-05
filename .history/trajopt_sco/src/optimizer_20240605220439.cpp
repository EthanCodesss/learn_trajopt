#include <trajopt_sco/macros.h>
TRAJOPT_IGNORE_WARNINGS_PUSH
#include <boost/format.hpp>
#include <chrono>
#include <cmath>
#include <cstdio>
TRAJOPT_IGNORE_WARNINGS_POP

#include <trajopt_common/logging.hpp>
#include <trajopt_common/stl_to_string.hpp>
#include <trajopt_sco/expr_ops.hpp>
#include <trajopt_sco/modeling.hpp>
#include <trajopt_sco/optimizers.hpp>
#include <trajopt_sco/sco_common.hpp>
#include <trajopt_sco/solver_interface.hpp>
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

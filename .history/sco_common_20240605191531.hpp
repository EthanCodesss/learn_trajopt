#pragma once
#include <algorithm>
#include <cmath>
#include <vector>

namespace sco {
struct Var;
struct AffExpr;
struct QuadExpr;
struct Cnt;

using DblVec = std::vector<double>;
using IntVec = std::vector<int>;
using SizeTVec = std::vector<std::size_t>;
using VarVector = std::vector<Var>;
using AffExprVector = std::vector<AffExpr>;
using QuadExprVector = std::vector<QuadExpr>;
using CntVector = std::vector<Cnt>;

inline double vecSum(const DblVec &v) {
  double out = 0;
  for (double i : v)
    out += i;
  return out;
}
} // namespace sco
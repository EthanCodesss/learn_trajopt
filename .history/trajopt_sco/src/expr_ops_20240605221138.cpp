#include <trajopt_common/macros.h>
TRAJOPT_IGNORE_WARNINGS_PUSH
#include <cmath>
TRAJOPT_IGNORE_WARNINGS_POP

#include <trajopt_sco/expr_ops.hpp>

namespace sco {
QuadExpr exprMult(const AffExpr &affexpr1, const AffExpr &affexpr2) {
  QuadExpr out;
  size_t naff1 = affexpr1.coeffs.size();
  size_t naff2 = affexpr2.coeffs.size();
  size_t nquad = naff1 * naff2;

  // Multiply the constants of the two expr
  out.affexpr.constant = affexpr1.constant * affexpr2.constant;

  // Account for vars in each expr multiplied by the constant in the other expr
  out.affexpr.vars.reserve(naff1 + naff2);
  out.affexpr.vars.insert(out.affexpr.vars.end(), affexpr1.vars.begin(),
                          affexpr1.vars.end());
  out.affexpr.vars.insert(out.affexpr.vars.end(), affexpr2.vars.begin(),
                          affexpr2.vars.end());
  out.affexpr.coeffs.resize(naff1 + naff2);
  for (size_t i = 0; i < naff1; ++i)
    out.affexpr.coeffs[i] = affexpr2.constant * affexpr1.coeffs[i];
  for (size_t i = 0; i < naff2; ++i)
    out.affexpr.coeffs[i + naff1] = affexpr1.constant * affexpr2.coeffs[i];

  // Account for the vars in each expr that are multipled by another var in the
  // other expr
  out.coeffs.reserve(nquad);
  out.vars1.reserve(nquad);
  out.vars2.reserve(nquad);
  for (size_t i = 0; i < naff1; ++i) {
    for (size_t j = 0; j < naff2; ++j) {
      out.vars1.push_back(affexpr1.vars[i]);
      out.vars2.push_back(affexpr2.vars[j]);
      out.coeffs.push_back(affexpr1.coeffs[i] * affexpr2.coeffs[j]);
    }
  }
  return out;
}

// 计算变量a的平方
QuadExpr exprSquare(const Var &a) {
  QuadExpr out;
  out.coeffs.push_back(1);
  out.vars1.push_back(a);
  out.vars2.push_back(a);
  return out;
}

// 这个函数通过将仿射表达式的每一项与自身和其他项相乘来构造一个二次表达式。仿射表达式的常数项平方后成为二次表达式的常数项，仿射表达式的洗漱用于构造二次项的系数，
// 每个变量的系数乘以两倍的常数项成为其线性项的系数。

QuadExpr exprSquare(const AffExpr &affexpr) {
  // 声明一个二次项表达式， 将其填充并返回
  QuadExpr out;
  // 计算仿射表达式中系数的数量，即一次项的个数
  size_t naff = affexpr.coeffs.size();
  // 计算二次项的总数
  size_t nquad = (naff * (naff + 1)) / 2;

  // 计算out仿射部分的常数项， sq计算常数项的平方
  out.affexpr.constant = sq(affexpr.constant);
  // 这段代码处理仿射项乘以常数，一次项
  out.affexpr.vars = affexpr.vars;
  out.affexpr.coeffs.resize(naff);
  for (size_t i = 0; i < naff; ++i)
    out.affexpr.coeffs[i] = 2 * affexpr.constant * affexpr.coeffs[i];
  // 预留空间
  out.coeffs.reserve(nquad);
  out.vars1.reserve(nquad);
  out.vars2.reserve(nquad);
  // 先计算平方项， 然后计算每项对应的乘积项
  for (size_t i = 0; i < naff; ++i) {
    out.vars1.push_back(affexpr.vars[i]);
    out.vars2.push_back(affexpr.vars[i]);
    out.coeffs.push_back(sq(affexpr.coeffs[i]));
    for (size_t j = i + 1; j < naff; ++j) {
      out.vars1.push_back(affexpr.vars[i]);
      out.vars2.push_back(affexpr.vars[j]);
      out.coeffs.push_back(2 * affexpr.coeffs[i] * affexpr.coeffs[j]);
    }
  }
  return out;
}

AffExpr cleanupAff(const AffExpr &a) {
  AffExpr out;
  for (size_t i = 0; i < a.size(); ++i) {
    if (fabs(a.coeffs[i]) > 1e-7) {
      out.coeffs.push_back(a.coeffs[i]);
      out.vars.push_back(a.vars[i]);
    }
  }
  out.constant = a.constant;
  return out;
}

///////////////////////////////////////////////////////////////
} // namespace sco

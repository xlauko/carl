#pragma once

#include "../Formula.h"

#include <vector>

namespace carl {
namespace formula {
namespace symmetry {

template<typename Poly>
Formula<Poly> createComparison(Variable x, Variable y, Relation rel) {
	assert(x.type() == y.type());
	switch (x.type()) {
		case VariableType::VT_BOOL:
			switch (rel) {
				case Relation::EQ:
					return Formula<Poly>(FormulaType::IFF, Formula<Poly>(x), Formula<Poly>(y));
				case Relation::LEQ:
					return Formula<Poly>(FormulaType::IMPLIES, Formula<Poly>(x), Formula<Poly>(y));
				case Relation::GEQ:
					return Formula<Poly>(FormulaType::IMPLIES, Formula<Poly>(y), Formula<Poly>(x));
				default:
					assert(false);
			}
		case VariableType::VT_INT:
		case VariableType::VT_REAL:
			return Formula<Poly>(Constraint<Poly>(Poly(x) - y, rel));
		default:
			CARL_LOG_ERROR("carl.formula.symmetry", "Tried to break symmetry on unsupported variable type " << x.type());
	}
	return Formula<Poly>(FormulaType::TRUE);
}

template<typename Poly>
Formula<Poly> lexLeaderConstraint(const Symmetry& vars) {
	Formulas<Poly> eq;
	Formulas<Poly> res;
	for (const auto& v: vars) {
		Formula<Poly> cur = createComparison<Poly>(v.first, v.second, Relation::LEQ);
		res.emplace_back(Formula<Poly>(FormulaType::IMPLIES, Formula<Poly>(FormulaType::AND, eq), cur));
		eq.emplace_back(createComparison<Poly>(v.first, v.second, Relation::EQ));
	}
	return Formula<Poly>(FormulaType::AND, std::move(res));
}

}
}
}

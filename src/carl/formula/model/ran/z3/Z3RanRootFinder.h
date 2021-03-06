#pragma once

#include "Z3Ran.h"

#ifdef RAN_USE_Z3

#include "../RealAlgebraicNumber.h"

namespace carl {
    template<typename Coeff, typename Number>
    std::vector<RealAlgebraicNumber<Number>> realRootsZ3(
		const UnivariatePolynomial<Coeff>& polynomial,
		const Interval<Number>& interval = Interval<Number>::unboundedInterval()
    );

    template<typename Coeff, typename Number = typename UnderlyingNumberType<Coeff>::type>
    std::vector<RealAlgebraicNumber<Number>> realRootsZ3(
        const UnivariatePolynomial<Coeff>& p,
        const std::map<Variable, RealAlgebraicNumber<Number>>& m,
        const Interval<Number>& interval = Interval<Number>::unboundedInterval()
    );
}

#endif
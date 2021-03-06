#include "Z3Ran.h"

#ifdef RAN_USE_Z3

#include "../../../../converter/Z3Converter.h"

#include "Z3RanContent.h"

namespace carl {

    template<typename Number>
    Z3Ran<Number>::Z3Ran() {
        mContent = std::make_shared<Z3RanContent>();
    }

    template<typename Number>
    Z3Ran<Number>::Z3Ran(const Z3RanContent& content) {
        mContent = std::make_shared<Z3RanContent>(content);
    }

    template<typename Number>
    Z3Ran<Number>::Z3Ran(Z3RanContent&& content) {
        mContent = std::make_shared<Z3RanContent>(std::move(content));
    }

    template<typename Number>
    Z3Ran<Number>::Z3Ran(const Number& r) : Z3Ran() {
        mpq val = z3().toZ3MPQ(r);
        z3().anumMan().set(content(), val);
        z3().free(val);
    }

	template<typename Number>
	bool Z3Ran<Number>::is_number() const {
		return z3().anumMan().is_rational(content());
	}
	template<typename Number>
	Number Z3Ran<Number>::get_number() const {
		assert(isNumeric());
        mpq res;
        z3().anumMan().to_rational(content(), res);
        Number num = z3().toNumber<Number>(res);
        z3().free(res);
        return num;
	}

    template<typename Number>
    bool Z3Ran<Number>::is_zero() const {
        return z3().anumMan().is_zero(content());
    }

    template<typename Number>
    bool Z3Ran<Number>::is_integral() const {
        return z3().anumMan().is_int(content());
    }

    template<typename Number>
    bool Z3Ran<Number>::isNumeric() const {
        return z3().anumMan().is_rational(content());
    }

    template<typename Number>
    Number Z3Ran<Number>::getNumber() const {
        assert(isNumeric());
        mpq res;
        z3().anumMan().to_rational(content(), res);
        Number num = z3().toNumber<Number>(res);
        z3().free(res);
        return num;
    }

    template<typename Number>
    const Number& Z3Ran<Number>::lower() const {
        assert(!isNumeric());
        mpq res;
        z3().anumMan().get_lower(content(), res);
        mLower = z3().toNumber<Number>(res);
        z3().free(res);
        return mLower;
    } 

    template<typename Number>
    const Number& Z3Ran<Number>::upper() const {
        assert(!isNumeric());
        mpq res;
        z3().anumMan().get_upper(content(), res);
        mUpper = z3().toNumber<Number>(res);
        z3().free(res);
        return mUpper;
    } 

    template<typename Number>
    const Interval<Number>& Z3Ran<Number>::getInterval() const {
        assert(!isNumeric());
        const Number& lo = lower();
        const Number& up = upper();
        mInterval = Interval<Number>(lo, BoundType::STRICT, up, BoundType::STRICT);
        return mInterval;
    }

    template<typename Number>
    Number Z3Ran<Number>::branchingPoint() const
    {
        const Number& low = lower();
        const Number& up = upper();
        const Number& mid = (up-low)/2;
        
        const Number& midf = carl::floor(mid);
        if (low <= midf)
            return midf;
        const Number& midc = carl::ceil(mid);
        if (up >= midc)
            return midc;
        return mid;
    }

    template<typename Number>
    const UnivariatePolynomial<Number>& Z3Ran<Number>::getPolynomial() const {
        svector<mpz> res;
        z3().anumMan().get_polynomial(content(), res);
        mPolynomial = z3().toUnivPoly<Number>(res);
        for (unsigned i = 0; i < res.size(); i++) {
            z3().free(res[i]);
        }
        return *mPolynomial;
    }

    template<typename Number>
    Sign Z3Ran<Number>::sgn() const {
        if (z3().anumMan().is_zero(content()))
            return Sign::ZERO;
        else if (z3().anumMan().is_pos(content()))
            return Sign::POSITIVE;
        else if (z3().anumMan().is_neg(content()))
            return Sign::NEGATIVE;
        else
            assert(false);
    }

    template<typename Number>
    Sign Z3Ran<Number>::sgn(const UnivariatePolynomial<Number>& p) const {
        polynomial::polynomial_ref poly = z3().toZ3IntCoeff(p);
        nlsat::assignment map(z3().anumMan());  // map frees its elements automatically
        polynomial::var var = z3().toZ3(p.mainVar());
        map.set(var, content());
        int rs = z3().anumMan().eval_sign_at(poly, map);
        if (rs < 0) return Sign::NEGATIVE;
        else if (rs == 0) return Sign::ZERO;
        else /* rs > 0 */ return Sign::POSITIVE;
    }

    template<typename Number>
    bool Z3Ran<Number>::containedIn(const Interval<Number>& i) const {
        if(i.lowerBoundType() != BoundType::INFTY) {
            if(i.lowerBoundType() == BoundType::STRICT &&
                z3().anumMan().le(content(), z3().toZ3MPQ(i.lower()))) return false;
            if(i.lowerBoundType() == BoundType::WEAK &&
                z3().anumMan().lt(content(), z3().toZ3MPQ(i.lower()))) return false;
        }
        if(i.upperBoundType() != BoundType::INFTY) {
            if(i.upperBoundType() == BoundType::STRICT &&
                z3().anumMan().ge(content(), z3().toZ3MPQ(i.upper()))) return false;
            if(i.upperBoundType() == BoundType::WEAK &&
                z3().anumMan().gt(content(), z3().toZ3MPQ(i.upper()))) return false;
        }
        return true;
    }

    template<typename Number>
    Z3Ran<Number> Z3Ran<Number>::abs() const {
        if (z3().anumMan().is_pos(content())) {
            return *this;
        }
        else if (z3().anumMan().is_zero(content())) {
            return *this;
        }
        else if (z3().anumMan().is_neg(content())) {
            algebraic_numbers::anum res;
            z3().anumMan().set(res, content());
            z3().anumMan().neg(res);
            return Z3Ran<Number>(std::move(res));
        }
		assert(false);
		return *this;
    }

    template<typename Number>
    bool Z3Ran<Number>::equal(const Z3Ran<Number>& n) const {
        return z3().anumMan().eq(content(),n.content());
    }

    template<typename Number>
    bool Z3Ran<Number>::less(const Z3Ran<Number>& n) const {
        return z3().anumMan().lt(content(),n.content());
    }

    template<typename Number>
    bool Z3Ran<Number>::equal(const Number& n) const {
        mpq zn = z3().toZ3MPQ(n);
        bool res = z3().anumMan().eq(content(), zn);
        z3().free(zn);
        return res;
    }

    template<typename Number>
    bool Z3Ran<Number>::less(const Number& n) const {
        mpq zn = z3().toZ3MPQ(n);
        bool res = z3().anumMan().lt(content(), zn);
        z3().free(zn);
        return res;
    }

    template<typename Number>
    bool Z3Ran<Number>::greater(const Number& n) const {
        mpq zn = z3().toZ3MPQ(n);
        bool res = z3().anumMan().gt(content(), zn);
        z3().free(zn);
        return res;
    }

    template<typename Number>
    std::ostream& operator<<(std::ostream& os, const Z3Ran<Number>& zr) {
		os << "Z3 ";
        z3().anumMan().display_root(os, zr.content());
        return os;
    }

    template class Z3Ran<mpq_class>;
    template std::ostream& operator<<(std::ostream& os, const Z3Ran<mpq_class>& zr);

}

#endif
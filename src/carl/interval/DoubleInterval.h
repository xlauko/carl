/**
 * @author Florian Corzilius
 * @author Sebastian Junges
 */
#pragma once

#include <map>
#include <cassert>
#include <cmath>
#include <cfloat>

#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/interval.hpp>

#include <cln/cln.h>

#include "../core/Variable.h"
#include "../core/Sign.h"
#include "../core/numbers.h"

using namespace boost::numeric::interval_lib;

namespace carl
{

class DoubleInterval
{
public:
	///////////////
	//  Typedef  //
	///////////////

	/// Determines whether the bound is strict or weak.

	enum BoundType
	{
		/// the given bound is compared by a strict ordering relation
		STRICT_BOUND,
		/// the given bound is compared by a weak ordering relation
		WEAK_BOUND,
		/// the given bound is interpreted as minus or plus infinity depending on whether it is the left or the right bound
		INFINITY_BOUND
	};

	typedef boost::numeric::interval_lib::save_state<boost::numeric::interval_lib::rounded_arith_opp<double> > Rounding;

	typedef boost::numeric::interval_lib::checking_no_nan<double, boost::numeric::interval_lib::checking_no_nan<double> > Checking;

	typedef boost::numeric::interval< double, boost::numeric::interval_lib::policies< Rounding, Checking > > BoostDoubleInterval;

	typedef std::map<Variable, DoubleInterval> evaldoubleintervalmap;

	/// Standard assertion for checking the input to constructors and setters: the interval bounds might define an empty interval but can never cross (left > right).
#define DOUBLE_BOUNDS_OK( left, leftType, right, rightType )\
            (leftType == INFINITY_BOUND || rightType == INFINITY_BOUND || left < right)

protected:

	///////////////
	//  Members  //
	///////////////

	BoostDoubleInterval mInterval;
	BoundType mLeftType;
	BoundType mRightType;

public:

	//////////////////////////////////
	//  Constructors & Destructors  //
	//////////////////////////////////

	/** Empty Constructor creates a unbounded interval
	 */
	DoubleInterval();

	/** Creates point interval at n
	 * @param n
	 * @param overapproximate compute the approximations of the given bounds so that the interval is an over-approximation of the corresponding OpenInterval; otherwise under-approximate (default: false)
	 */
	DoubleInterval(const double& n);
	
	DoubleInterval(int n) : DoubleInterval((double)n)  {}

	/** Creates (preferably point) interval at n
	 * @param n
	 * @param bool overapproximate
	 */
	template<typename Rational>
	DoubleInterval(const Rational& n, bool overapproximate = false);

	template<typename Rational>
	DoubleInterval(const Rational& lower, BoundType lowerType, const Rational& upper, BoundType upperType, bool overapproxleft = false, bool overapproxright = false);

	/** Creates closed DoubleInterval
	 * @param _content
	 */
	DoubleInterval(const BoostDoubleInterval& _content);

	/** Creates closed DoubleInterval
	 * @param left
	 * @param right
	 */
	DoubleInterval(const double& left, const double& right);
	DoubleInterval(int left, int right) : DoubleInterval((double)left, (double)right) {}

	/** Creates DoubleInterval with BoundTypes
	 * @param _content
	 * @param leftType
	 * @param rightType
	 */
	DoubleInterval(const BoostDoubleInterval& _content, BoundType leftType, BoundType rightType);

	/** Creates DoubleInterval with BoundTypes
	 * @param left
	 * @param leftType
	 * @param right
	 * @param rightType
	 */
	DoubleInterval(double left, BoundType leftType, double right, BoundType rightType);
	DoubleInterval(int left, BoundType leftType, double right, BoundType rightType) :
		DoubleInterval((double)left,leftType, right, rightType) {}
	DoubleInterval(double left, BoundType leftType, int right, BoundType rightType): 
		DoubleInterval(left, leftType, (double)right, rightType) {}
	DoubleInterval(int left, BoundType leftType, int right, BoundType rightType) : 
		DoubleInterval((double)left, leftType, (double)right, rightType) {}

	/** Destructor.
	 */
	~DoubleInterval();

	///////////////////////
	//  Getter & Setter  //
	///////////////////////

	/**
	 * Get the whole interval.
	 * @return mInterval
	 */
	const BoostDoubleInterval& content() const
	{
		return mInterval;
	}

	/**
	 * Get left bound
	 * @return The left bound.
	 */
	const double& left() const
	{
		return mInterval.lower();
	}

	/**
	 * Get right bound
	 * @return The right bound.
	 */
	const double& right() const
	{
		return mInterval.upper();
	}

	/**
	 * Set new left bound for the interval.
	 * @param l new left bound
	 */
	void setLeft(const double l)
	{
		mInterval.set(l, mInterval.upper());
	}

	/**
	 * Set new left bound for the interval by rounding down.
	 * @param l new left bound
	 */
	void setLeft(const cln::cl_RA& l)
	{
		mInterval.set(roundDown(l), mInterval.upper());
	}

	/**
	 * Set new left bound type for the interval.
	 * @param lType new left bound type
	 */
	void setLeftType(BoundType lType)
	{
		mLeftType = lType;
	}

	/**
	 * Set new right bound for the interval.
	 * @param r new right bound
	 */
	void setRight(const double& r)
	{
		mInterval.set(left(), r);
	}

	/**
	 * Set new right bound for the interval by rounding up.
	 * @param r new right bound
	 */
	void setRight(const cln::cl_RA& r)
	{
		mInterval.set(left(), roundUp(r));
	}

	/**
	 * Set new right bound type for the interval.
	 * @param rType new right bound type
	 */
	void setRightType(const BoundType& rType)
	{
		mRightType = rType;
	}

	/** Get left BoundType
	 * @return mLeftType
	 */
	BoundType leftType() const
	{
		return mLeftType;
	}

	/** Get right BoundType
	 * @return mRightType
	 */
	BoundType rightType() const
	{
		return mRightType;
	}

	/** Set left bound
	 * @param left
	 */
	void cutUntil(const double& left);

	/** Set right bound
	 * @param right
	 */
	void cutFrom(const double& right);

	//////////////////
	//  Arithmetic  //
	//////////////////

	/** Adds two intervals and returns their sum.
	 * @param o
	 * @return sum
	 */
	DoubleInterval add(const DoubleInterval& o) const;

	/** Returns the negative value.
	 * @return negative value
	 */
	DoubleInterval minus() const;

	/** Returns the negative value.
	 * @return negative value
	 */
	DoubleInterval sqrt() const;

	/** Multiplies two intervals and returns their product.
	 * @param o
	 * @return product
	 */
	DoubleInterval mul(const DoubleInterval& o) const;

	/** Divides two intervals.
	 * @param o
	 * @return this interval divided by the argument
	 * @throws invalid_argument in case the argument interval contains zero
	 */
	DoubleInterval div(const DoubleInterval& o) const throw ( std::invalid_argument);

	/** Extended Intervaldivision with intervals containing 0
	 * @param a
	 * @param b
	 * @param o
	 * @return true if interval splitting (results in a and b), false else (results only in a)
	 */
	bool div_ext(DoubleInterval& a, DoubleInterval& b, const DoubleInterval& o);

	/** Computes the power to <code>e</code> of this interval.
	 * @param e exponent
	 * @return power to <code>e</code> of this interval
	 */
	DoubleInterval power(int e) const;

	/** Computes the inverse to the interval with respect to division by zero and infinity
	 * @param a first result reference
	 * @param b second result reference
	 * @return true if the result contains two intervals, else false
	 */
	bool inverse(DoubleInterval& a, DoubleInterval& b) const;


	/**
	 * Calculates the diameter of the interval
	 * @return the diameter of the interval
	 */
	double diameter() const;

	//////////////////
	//  Operations  //
	//////////////////

	/**
	 * @return true if the bounds define an empty interval, false otherwise
	 */
	bool empty() const;

	/**
	 * @return true if the bounds define the whole real line, false otherwise
	 */
	bool unbounded() const
	{
		return mLeftType == INFINITY_BOUND && mRightType == INFINITY_BOUND;
	}

	/**
	 * Determine whether the interval lays entirely left of 0 (NEGATIVE_SIGN), right of 0 (POSITIVE_SIGN) or contains 0 (ZERO_SIGN).
	 * @return NEGATIVE_SIGN, if the interval lays entirely left of 0; POSITIVE_SIGN, if right of 0; or ZERO_SIGN, if contains 0.
	 */
	Sign sgn();

	/**
	 * @param n
	 * @return true in case n is contained in this Interval
	 */
	bool contains(const double& n) const;

	/**
	 * @param o
	 * @return true in case o is a subset of this Interval
	 */
	bool contains(const DoubleInterval& o) const;

	/**
	 * Checks whether n is contained in the <b>closed</b> interval defined by the bounds.
	 * @param n
	 * @return true in case n meets the interval bounds or a point inbetween
	 */
	bool meets(double n) const;

	/**
	 * @param o
	 * @return intersection with the given Interval and this Interval, or (0, 0) in case the intersection is empty
	 */
	DoubleInterval intersect(const DoubleInterval& o) const;

	/**
	 * @return the midpoint of this interval
	 */
	double midpoint() const;

	/**
	 * Computes the absolute value of this interval, i.e. the maximum of the absolute values of its bounds.
	 * @return absolute value of the interval
	 * @see Marc Daumas, Guillaume Melquiond, and Cesar Munoz - "Guaranteed Proofs Using Interval Arithmetic".
	 */
	DoubleInterval abs() const;

	///////////////////////////
	// Relational Operations //
	///////////////////////////

	/**
	 * @param o
	 * @return true in case the other interval equals this
	 */
	bool isEqual(const DoubleInterval& o) const;

	/** Checks whether the left bound of this interval is less or equal the left bound of the other interval.
	 * @param o
	 * @return true if the left bound of this interval is less or equal the left bound of the other interval
	 */
	bool isLessOrEqual(const DoubleInterval& o) const;

	/** Checks whether the right bound of this interval is greater or equal the right bound of the other interval
	 * @param o
	 * @return true if the right bound of this interval is greater or equal the right bound of the other interval
	 */
	bool isGreaterOrEqual(const DoubleInterval& o) const;

	/**
	 * Prints out the Interval
	 */
	void dbgprint() const;


	////////////////////
	// Static Methods //
	////////////////////

	/**
	 * Creates the empty interval denoted by ]0,0[
	 * @return empty interval
	 */
	static DoubleInterval emptyInterval()
	{
		return DoubleInterval(BoostDoubleInterval(0), STRICT_BOUND, STRICT_BOUND);
	}

	/**
	 * Creates the unbounded interval denoted by ]-infty,infty[
	 * @return empty intervaleval
	 */
	static DoubleInterval unboundedInterval()
	{
		return DoubleInterval();
	}


	/** Returns a down-rounded representation of the given numeric
	 * @param numeric o
	 * @param bool overapproximate
	 * @return double representation of o (underapprox) Note, that it can return the double INFINITY.
	 */
	template<typename Rational>
	static double roundDown(const Rational& o, bool overapproximate = false);

	/** Returns a up-rounded representation of the given numeric
	 * @param numeric o
	 * @param bool overapproximate
	 * @return double representation of o (overapprox) Note, that it can return the double INFINITY.
	 */
	template<typename Rational>
	static double roundUp(const Rational& o, bool overapproximate = false);

	void operator +=(const DoubleInterval&);
	void operator -=(const DoubleInterval&);
	void operator *=(const DoubleInterval&);

	// unary arithmetic operators of DoubleInterval
	const DoubleInterval operator -(const DoubleInterval& lh) const;
	friend std::ostream& operator <<(std::ostream& str, const DoubleInterval&);


private:
	////////////////////////////
	//  Auxiliary Functions:  //
	////////////////////////////

	/** Return the bound type which corresponds to the weakest-possible type when combining all elements in two intervals.
	 * @param type1
	 * @param type2
	 * @return INFINITY_BOUND if one of the given types is INIFNITY_BOUND, STRICT_BOUND if one of the given types is STRICT_BOUND
	 */
	inline static BoundType getWeakestBoundType(BoundType type1, BoundType type2)
	{
		return (type1 == INFINITY_BOUND || type2 == INFINITY_BOUND)
				? INFINITY_BOUND : (type1 == STRICT_BOUND || type2 == STRICT_BOUND) ? STRICT_BOUND : WEAK_BOUND;
	}

}; // class DoubleInterval

template<typename Rational>
DoubleInterval::DoubleInterval(const Rational& lower, BoundType lowerType, const Rational& upper, BoundType upperType, bool overapproxleft, bool overapproxright)
{
	double dLeft = roundDown(lower, overapproxleft);
	double dRight = roundUp(upper, overapproxright);
	if(dLeft == -INFINITY) mLeftType = INFINITY_BOUND;
	if(dRight == INFINITY) mRightType = INFINITY_BOUND;
	if(mLeftType == INFINITY_BOUND && mRightType == INFINITY_BOUND)
	{
		mInterval = BoostDoubleInterval(0);
	}
	else if(mLeftType == INFINITY_BOUND)
	{
		mInterval = BoostDoubleInterval(dRight);
	}
	else if(mRightType == INFINITY_BOUND)
	{
		mInterval = BoostDoubleInterval(dLeft);
	}
	else if((lower == upper && lowerType != upperType) || lower > upper)
	{
		mLeftType = STRICT_BOUND;
		mRightType = STRICT_BOUND;
		mInterval = BoostDoubleInterval(0);
	}
	else
	{
		mLeftType = lowerType;
		mRightType = upperType;
		mInterval = BoostDoubleInterval(dLeft, dRight);
	}
}

template<typename Rational>
double DoubleInterval::roundDown(const Rational& o, bool overapproximate)
{
	double result = getDouble(o);
	if(result == -INFINITY) return result;
	if(result == INFINITY) return DBL_MAX;
	// If the cln::cl_RA cannot be represented exactly by a double, round.
	Rational r = rationalize<Rational>(result);
	if(overapproximate || r != o)
	{
		if(result == -DBL_MAX) return -INFINITY;
		return std::nextafter(result, -INFINITY);
	}
	else
	{
		return result;
	}
}

template<typename Rational>
double DoubleInterval::roundUp(const Rational& o, bool overapproximate)
{
	double result = getDouble(o);
	if(result == INFINITY) return result;
	if(result == -INFINITY) return -DBL_MAX;
	// If the cln::cl_RA cannot be represented exactly by a double, round.
	Rational r = rationalize<Rational>(result);
	if(overapproximate || r != o)
	{
		if(result == DBL_MAX) return INFINITY;
		return std::nextafter(result, INFINITY);
	}
	else
	{
		return result;
	}
}
// namespace carl

inline const DoubleInterval operator +(const DoubleInterval& lh, const DoubleInterval& rh)
{
	return lh.add(rh);
}

inline const DoubleInterval operator +(const DoubleInterval& lh, const double& rh)
{
	// TODO optimization potential
	return lh.add(DoubleInterval(rh));
}

inline const DoubleInterval operator +(const double& lh, const DoubleInterval& rh)
{
	// TODO optimization potential
	return rh.add(DoubleInterval(lh));
}

inline const DoubleInterval operator -(const DoubleInterval& lh, const DoubleInterval& rh)
{
	return lh.add(rh.minus());
}

inline const DoubleInterval operator -(const DoubleInterval& lh, const double& rh)
{
	return lh + (-rh);
}

inline const DoubleInterval operator -(const double& lh, const DoubleInterval& rh)
{
	return (-lh) +rh;
}

inline const DoubleInterval operator *(const DoubleInterval& lh, const DoubleInterval& rh)
{
	return lh.mul(rh);
}

inline const DoubleInterval operator *(const DoubleInterval& lh, const double& rh)
{
	return DoubleInterval(lh.mul(DoubleInterval(rh)));
}

inline const DoubleInterval operator *(const double& lh, const DoubleInterval& rh)
{
	return rh * lh;
}

inline const DoubleInterval operator /(const DoubleInterval& lh, const double& rh) throw ( std::overflow_error)
{
	return lh.div(DoubleInterval(rh));
}

inline const DoubleInterval operator /(const double& lh, const DoubleInterval& rh) throw ( std::overflow_error)
{
	DoubleInterval result = DoubleInterval(lh);
	result.div(rh);
	return result;
}

// relational operators

inline bool operator ==(const DoubleInterval& lh, const DoubleInterval& rh)
{
	return lh.isEqual(rh);
}

inline bool operator !=(const DoubleInterval& lh, const DoubleInterval& rh)
{
	return !lh.isEqual(rh);
}

inline bool operator <=(const DoubleInterval& lh, const DoubleInterval& rh)
{
	return lh.isLessOrEqual(rh);
}

inline bool operator >=(const DoubleInterval& lh, const DoubleInterval& rh)
{
	return lh.isGreaterOrEqual(rh);
}

}


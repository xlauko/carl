/* 
 * File:   RealAlgebraicNumberIR.h
 * Author: Gereon Kremer <gereon.kremer@cs.rwth-aachen.de>
 */

#pragma once

#include "RealAlgebraicNumber.h"

#include "UnivariatePolynomial.h"
#include "../interval/Interval.h"
#include "../util/SFINAE.h"

namespace carl {

/// @todo Add `EnableIf<is_fraction<Number>>` such that gcc does not crash.
template<typename Number>
class RealAlgebraicNumberIR : public RealAlgebraicNumber<Number> {

protected:
	/**
	 * Pointer to the polynomial of this interval representation.
	 */
	UnivariatePolynomial<Number> polynomial;
	
	/**
	 * Isolating interval of this interval representation.
	 * 
	 * The interval is mutable, that means it may be changed by const methods, as the actual data is the represented root.
	 * However, all methods must make sure, that the interval always includes the same root.
	 */
	mutable Interval<Number> interval;
	
	/**
	 * Standard Sturm sequence of the polynomial and its derivative.
	 */
	std::list<UnivariatePolynomial<Number>> sturmSequence;
	
	/**
	 * Number of refinements executed to the isolating interval.
	 */
	mutable unsigned int refinementCount;
	
private:
	std::weak_ptr<RealAlgebraicNumberIR> pThis;
	std::shared_ptr<RealAlgebraicNumberIR> thisPtr() const {
		return std::shared_ptr<RealAlgebraicNumberIR>(this->pThis);
	}
	
	//////////////////////////
	// Con- and destructors //
	//////////////////////////


	RealAlgebraicNumberIR(const Variable& var);
	
	/**
	 * Constructs a real algebraic number in interval and order representation (p, l, r, o) with a normalized interval w.r.t. normalizeInterval.
	 *
	 * <ul>
	 * <li>p can be changed within the constructor, but is later constant.<li>
	 * <li>i can be changed later by refine(), but is only normalized within the constructor in case not normalized before.</li>
	 * </ul>
	 *
	 * @param p polynomial having the real algebraic number as one of its roots
	 * @param i open interval ]l, r[ containing the real algebraic number (should be normalized)
	 * @param s standard Sturm sequence
	 * @param normalize if set to false, the interval will not be normalized in the constructor (default is true)
	 * @param isRoot true marks this real algebraic number to stem from a root computation
	 */
	RealAlgebraicNumberIR(
			const UnivariatePolynomial<Number>& p,
			const Interval<Number>& i,
			const std::list<UnivariatePolynomial<Number>>& s = std::list<UnivariatePolynomial<Number>>(),
			const bool normalize = true,
			const bool isRoot = true );

public:
	static std::shared_ptr<RealAlgebraicNumberIR> create(const Variable& var) {
		auto res = std::shared_ptr<RealAlgebraicNumberIR>(new RealAlgebraicNumberIR(var));
		res->pThis = res;
		return res;
	}

	static std::shared_ptr<RealAlgebraicNumberIR> create(
			const UnivariatePolynomial<Number>& p,
			const Interval<Number>& i,
			const std::list<UnivariatePolynomial<Number>>& s = std::list<UnivariatePolynomial<Number>>(),
			const bool normalize = true,
			const bool isRoot = true) {
		assert(i.isOpenInterval() || i.isPointInterval());
		auto res = std::shared_ptr<RealAlgebraicNumberIR>(new RealAlgebraicNumberIR(p, i, s, normalize, isRoot));
		LOGMSG_TRACE("carl.core", "Creating " << res);
		res->pThis = res;
		return res;
	}

	/**
	 * Destructor.
	 */
	virtual ~RealAlgebraicNumberIR() {
	}
	
	///////////////
	// Selectors //
	///////////////

	bool isZero() const {
		return this->isNumeric() ? this->value() == 0 : (this->interval.lower() == 0 && this->interval.upper() == 0);
	}

	virtual bool isNumericRepresentation() const {
		return false;
	}

	/**
	 * Selects the polynomial having this real algebraic number as one of its roots.
	 * @return polynomial having the number as one of its roots
	 */
	const UnivariatePolynomial<Number>& getPolynomial() const {
		return this->polynomial;
	}

	/**
	 * Selects the open interval ]l, r[ containing the real algebraic number.
	 * @return open interval ]l, r[ containing the real algebraic number
	 */
	const Interval<Number>& getInterval() const
	{
		return this->interval;
	}
	
	const Number& lower() const {
		return this->getInterval().lower();
	}
	const Number& upper() const {
		return this->getInterval().upper();
	}
	
	void setLower(const Number& n) const {
		this->interval.setLower(n);
	}
	void setUpper(const Number& n) const {
		this->interval.setUpper(n);
	}

	/**
	 * Returns a pre-computed standard Sturm sequence of the polynomial and its derivative.
	 * @return standard Sturm sequence of the polynomial and its derivative.
	 */
	const std::list<UnivariatePolynomial<Number>>& getSturmSequence() const
	{
		return this->sturmSequence;
	}

	/** Returns how often one of the refine methods was called before.
	 * @return number of refinement steps executed on this real algebraic number
	 */
	unsigned getRefinementCount() const {
		return this->refinementCount;
	}
	
	///////////////
	// Operators //
	///////////////

	/**
	 * This number gets all values of the other.
	 */
	const RealAlgebraicNumberIR& operator=(const RealAlgebraicNumberIR& obj);
	
	template<typename Num>
	friend std::ostream& operator<<(std::ostream& os, const RealAlgebraicNumberIR<Num>* n);

	std::shared_ptr<RealAlgebraicNumberIR<Number>> add(const std::shared_ptr<RealAlgebraicNumberIR<Number>>& n);

	std::shared_ptr<RealAlgebraicNumberIR<Number>> minus() const;

	virtual bool containedIn(const Interval<Number>& i) const {
		return i.contains(this->getInterval());
	}

	bool equal(std::shared_ptr<RealAlgebraicNumberIR>& n);
	
private:
	// Helper functions for lessWhileUnequal
	
	/**
	 * Checks if *this < *n.
	 * If the method could not decide the order, it returns (false, false)
	 * Otherwise, it returns (true, X), X being the value of *this < *n
     * @param n Another RealAlgebraicNumberIR
     * @return 
     */
	std::pair<bool,bool> checkOrder(std::shared_ptr<RealAlgebraicNumberIR> n);
	std::pair<bool,bool> intervalContained(std::shared_ptr<RealAlgebraicNumberIR> n, bool twisted);
	bool checkIntersection(std::shared_ptr<RealAlgebraicNumberIR> n, const Interval<Number> i);
	

public:	
	bool lessWhileUnequal(std::shared_ptr<RealAlgebraicNumberIR> n);
	
	////////////////
	// Operations //
	////////////////

	/** Normalizes the interval of an real algebraic number to not contain zero, in case the number is non-zero.
	 */
	void normalizeInterval();

	/**
	 * Coarsen the given isolating interval as much as possible while staying in the given interval.
	 * If interval is no over-approximation of the current isolating interval, do nothing.
	 * @param interval
	 */
	void coarsen(const Interval<Number>& interval) const;
	
	
	/** Refines the interval i of this real algebraic number yielding the interval j such that <code>2*(j.Right()-j.Left()) &lt;= i.Right()-i.Left()</code>. This is cutting the interval in the middle and choosing the half where the root lays in.
	 * @param strategy strategy selection according to RealAlgebraicNumberFactory::searchRealRootsStrategy
	 * @rcomplexity constant
	 * @scomplexity constant
	 */
	void refine(RealAlgebraicNumberSettings::RefinementStrategy strategy = RealAlgebraicNumberSettings::RefinementStrategy::DEFAULT);

	/** Refines the interval i of this real algebraic number yielding the interval j such that <code>(j.Right()-j.Left()) &lt;= eps</code>.
	 * @param eps
	 */
	inline void refine(const Number& eps) {
		while (this->getInterval().diameter() > eps)
			this->refine();
	}

	/** Refines the interval i of this real algebraic number yielding the interval j such that !j.meets(n). If true is returned, n is the exact numeric representation of this root. Otherwise not.
	 * @param n
	 * @rcomplexity constant
	 * @scomplexity constant
	 * @return true, if n is the exact numeric representation of this root, otherwise false
	 */
	bool refineAvoiding(const Number& n);

	Sign sgn() const;

	Sign sgn(const UnivariatePolynomial<Number>& p) const;

};

template<typename Number>
using RealAlgebraicNumberIRPtr = std::shared_ptr<RealAlgebraicNumberIR<Number>>;

template<typename Number>
std::ostream& operator<<(std::ostream& os, const carl::RealAlgebraicNumberIR<Number>* n) {
	if (n->isNumeric()) {
		return os << "(IR " << n->getInterval() << ", " << n->getPolynomial() << " = " << n->value() << ")";
	}
	return os << "(IR " << n->getInterval() << ", " << n->getPolynomial() << ")";
}

}

#include "RealAlgebraicNumberIR.tpp"

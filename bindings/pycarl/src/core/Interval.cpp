/*
 * term.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "Interval.h"

#include "common.h"


void define_boundtype(py::module& m) {
    py::enum_<carl::BoundType>(m, "BoundType")
        .value("STRICT", carl::BoundType::STRICT)
        .value("WEAK", carl::BoundType::WEAK)
        .value("INFTY", carl::BoundType::INFTY);
}

void define_interval(py::module& m) {
	py::class_<Interval>(m, "Interval")
		.def(py::init<const Rational&>())
		.def(py::init<const Rational&, const Rational&>())
		.def(py::init<const Rational&, carl::BoundType, const Rational&, carl::BoundType>())
		
		// TODO: does not work :-(
		//.def_property("lower", &Interval::lower, &Interval::setLower)
		.def("lower", &Interval::lower, py::return_value_policy::reference_internal)
		.def("setLower", &Interval::setLower)
		.def("upper", &Interval::upper, py::return_value_policy::reference_internal)
		.def("setUpper", &Interval::setUpper)
		
		.def("isInfinite", &Interval::isInfinite)
		.def("isUnbounded", &Interval::isUnbounded)
		.def("isHalfBounded", &Interval::isHalfBounded)
		.def("isEmpty", &Interval::isEmpty)
		.def("isPointInterval", &Interval::isPointInterval)
		.def("isOpenInterval", &Interval::isOpenInterval)
		.def("isClosedInterval", &Interval::isClosedInterval)
		.def("isZero", &Interval::isZero)
		.def("isOne", &Interval::isOne)
		.def("isPositive", &Interval::isPositive)
		.def("isNegative", &Interval::isNegative)
		.def("isSemiPositive", &Interval::isSemiPositive)
		.def("isSemiNegative", &Interval::isSemiNegative)
		
		.def("integralPart", &Interval::integralPart)
		.def("diameter", &Interval::diameter)
		.def("center", [](const Interval& i){ return i.center(); })
		.def("sample", &Interval::sample)
		.def("contains", [](const Interval& i, const Rational& r){ return i.contains(r); })
		.def("contains", [](const Interval& i, const Interval& i2){ return i.contains(i2); })
		.def("meets", &Interval::meets)
		.def("isSubset", &Interval::isSubset)
		.def("isProperSubset", &Interval::isProperSubset)
		
		.def("div", &Interval::div)
		
		.def("inverse", &Interval::inverse)
		.def("abs", &Interval::abs)
		.def("__pow__", [](const Interval& i, carl::uint exp) { return i.pow(exp); })
		.def("intersectsWith", &Interval::intersectsWith)
		.def("intersect", &Interval::intersect)
		.def("unite", &Interval::unite)
		.def("difference", &Interval::difference)
		.def("complement", &Interval::complement)
		.def("symmetricDifference", &Interval::symmetricDifference)
		
		.def(py::self + py::self)
		.def(Rational() + py::self)
		.def(py::self + Rational())
		.def(py::self += py::self)
		.def(py::self += Rational())
		
		.def(-py::self)
		
		.def(py::self - py::self)
		.def(Rational() - py::self)
		.def(py::self - Rational())
		.def(py::self -= py::self)
		.def(py::self -= Rational())
		
		.def(py::self * py::self)
		.def(Rational() * py::self)
		.def(py::self * Rational())
		.def(py::self *= py::self)
		.def(py::self *= Rational())
		
		.def(py::self / Rational())
		.def(py::self /= Rational())
		
		.def(py::self == py::self)
		.def(py::self != py::self)
		.def(py::self <= py::self)
		.def(py::self <= Rational())
		.def(Rational() <= py::self)
		.def(py::self >= py::self)
		.def(py::self >= Rational())
		.def(Rational() >= py::self)
		.def(py::self < py::self)
		.def(py::self < Rational())
		.def(Rational() < py::self)
		.def(py::self > py::self)
		.def(py::self > Rational())
		.def(Rational() > py::self)
		
		.def("__str__", &streamToString<Interval>)
		.def("__repr__", &streamToString<Interval>)
	;
	
	m.def("isInteger", [](const Interval& i){ return carl::isInteger(i); });
	m.def("div", [](const Interval& l, const Interval& r){ return carl::div(l, r); });
	m.def("quotient", [](const Interval& l, const Interval& r){ return carl::quotient(l, r); });
	m.def("abs", [](const Interval& i){ return carl::abs(i); });
	m.def("pow", [](const Interval& i, carl::uint exp){ return carl::pow(i, exp); });
	m.def("floor", [](const Interval& i){ return carl::floor(i); });
	m.def("ceil", [](const Interval& i){ return carl::ceil(i); });
}

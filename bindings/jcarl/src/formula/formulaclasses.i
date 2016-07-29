
%module FormulaClasses
%{
#include "Formula.h"
#include "Constraint.h"
#include "../core/SimpleConstraint.h"
#include "gmpxx.h"
#include "../core/RationalFunction.h"
#include "../core/Relation.h"


typedef mpq_class Rational;

typedef carl::Monomial::Arg Monomial;
typedef carl::Term<Rational> Term;
typedef carl::MultivariatePolynomial<Rational> Polynomial;
typedef carl::FactorizedPolynomial<Polynomial> FactorizedPolynomial;
typedef carl::RationalFunction<Polynomial> RationalFunction;
typedef carl::RationalFunction<FactorizedPolynomial> FactorizedRationalFunction;
typedef carl::PolynomialFactorizationPair<Polynomial> FactorizationPair;



%}

%include "std_string.i"
%import "../core/variable.i"
%import "../core/polynomial.i"
%import "../core/rationalfunction.i"
%import "../core/factorizedpolynomial.i"

typedef mpq_class Rational;

typedef carl::Monomial::Arg Monomial;
typedef carl::Term<Rational> Term;
typedef carl::MultivariatePolynomial<Rational> Polynomial;
typedef carl::FactorizedPolynomial<Polynomial> FactorizedPolynomial;
typedef carl::RationalFunction<Polynomial> RationalFunction;
typedef carl::RationalFunction<FactorizedPolynomial> FactorizedRationalFunction;
typedef carl::PolynomialFactorizationPair<Polynomial> FactorizationPair;


namespace carl {


    enum FormulaType {
        // Generic
        ITE, EXISTS, FORALL,
        
        // Core Theory
        TRUE, FALSE,
        BOOL,
        NOT, IMPLIES, AND, OR, XOR,
        IFF, 

		// Arithmetic Theory
		CONSTRAINT,
		
		// Bitvector Theory
		BITVECTOR,
		
		// Uninterpreted Theory
		UEQ
    };

enum Relation { EQ = 0, NEQ = 1, LESS = 2, LEQ = 4, GREATER = 3, GEQ = 5 };
/* TODO: wrap this toString
inline std::ostream& operator<<(std::ostream& os, const Relation& r) {
	switch (r) {
		case Relation::EQ:	os << "="; break;
		case Relation::NEQ:	os << "<>"; break;
		case Relation::LESS:	os << "<"; break;
		case Relation::LEQ:	os << "<="; break;
		case Relation::GREATER:	os << ">"; break;
		case Relation::GEQ:	os << ">="; break;
	}
	return os;
} */

template<typename Pol> class Constraint {
	    public:
	        Constraint( bool _valid = true );
  
            explicit Constraint( carl::Variable::Arg _var, carl::Relation _rel,  const typename Pol::NumberType& _bound = constant_zero<typename Pol::NumberType>::get() );
            
            explicit Constraint( const Pol& _lhs, carl::Relation _rel );
		
	    unsigned satisfiedBy( const carl::EvaluationMap<typename Pol::NumberType>& _assignment ) const;

            std::string toString( unsigned _unequalSwitch = 0, bool _infix = true, bool _friendlyVarNames = true ) const;

	    const Pol& lhs() const;

            carl::Relation relation() const;

	//TODO: boolean connectives
	

	};

template<typename LhsType> class SimpleConstraint {
         public:
		SimpleConstraint(bool v) : mLhs(v ? 0 : 1), mRelation(carl::Relation::EQ) {}
		SimpleConstraint(const LhsType& lhs, carl::Relation rel) : mLhs(lhs), mRelation(rel)
		{}
	
		const LhsType& lhs() const {return mLhs;}
		const carl::Relation& rel() const {return mRelation;}
		
		%extend {
			std::string toString() {
			     std::stringstream ss;
    			     ss << *$self;
    			     return ss.str();	
			}
		}

};



//TODO: wrap EvaluationMap, Formulas, and the other types that are missing!

template<typename Pol> class Formula {
    public:
      explicit Formula( carl::Variable::Arg _booleanVar ):
        Formula( carl::FormulaPool<Pol>::getInstance().create( _booleanVar ) )
      {}

     explicit Formula( const carl::Constraint<Pol>& _constraint ):
        Formula( carl::FormulaPool<Pol>::getInstance().create( _constraint ) )
    {}

    explicit Formula( carl::FormulaType _type, const Formula& _subformula ):
        Formula(carl::FormulaPool<Pol>::getInstance().create(_type, std::move(Formula(_subformula))))
    {}

    explicit Formula( carl::FormulaType _type, const carl::Formulas<Pol>& _subasts ):
        Formula( carl::FormulaPool<Pol>::getInstance().create( _type, _subasts ) )
    {}


    unsigned satisfiedBy( const carl::EvaluationMap<typename Pol::NumberType>& _assignment ) const;

    std::string toString( bool _withActivity = false, unsigned _resolveUnequal = 0, const std::string _init = "", bool _oneline = true, bool _infix = false, bool _friendlyNames = true, bool _withVariableDefinition = false ) const;

    size_t size() const;



    carl::FormulaType getType() const
    {
        return mpContent->mType;
    }
//TODO: operators and iterator!

};
}

%include "std_map.i"




%template(FormulaPoly) carl::Formula<Polynomial>;
%template(ConstraintPoly) carl::Constraint<Polynomial>;
%template(SimpleConstraintPoly) carl::SimpleConstraint<Polynomial>;
%template(SimpleConstraintFunc) carl::SimpleConstraint<FactorizedRationalFunction>;

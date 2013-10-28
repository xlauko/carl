/**
 * @file Term.h 
 * @author Sebastian Junges
 * @author Florian Corzilius
 */

#pragma once

#include <memory>
#include "Monomial.h"
#include "VariablesInformation.h"

namespace carl
{
    enum class Definiteness : int { NEGATIVE = -2, NEGATIVE_SEMI = -1, NON = 0, POSITIVE_SEMI = 1, POSITIVE = 2 };

template<typename Coefficient>
class Term
{
    private:
        Coefficient                     mCoeff = 0;
        std::shared_ptr<const Monomial> mMonomial;
    
    public:
        Term();
        explicit Term(const Coefficient& c);
        explicit Term(Variable::Arg v);
        explicit Term(const Monomial& m);
        explicit Term(const std::shared_ptr<const Monomial>& m);
        Term(const Coefficient& c, const Monomial& m);
        Term(const Coefficient& c, const Monomial* m);
        Term(const Coefficient& c, const std::shared_ptr<const Monomial>& m);
        Term(const Coefficient& c, Variable::Arg v, unsigned exponent);
        
		/**
         * Get the coefficient.
         * @return 
         */
        const Coefficient& coeff() const
        {
            return mCoeff;
        }
        
        /**
         * 
         * @return 
         */
        std::shared_ptr<const Monomial> monomial() const
        {
            return mMonomial;
        }
        /**
         * Gives the total degree, i.e. the sum of all exponents.
         * @return 
         */
        exponent tdeg() const
        {
            if(!mMonomial) return 0;
            return mMonomial->tdeg();
        }
		
		bool isZero() const
		{
			return mCoeff == 0;
		}
        /**
         * Checks whether the monomial is a constant.
         * @return 
         */
        bool isConstant() const
        {
            return !mMonomial;
        }
        /**
         * Checks whether the monomial has exactly the degree one.
         * @return 
         */
        bool isLinear() const
        {
            if(!mMonomial) return true;
            return mMonomial->isLinear();
        }
		/**
		 * 
         * @return 
         */
        size_t getNrVariables() const
        {
            if(!mMonomial) return 0;
            return mMonomial->nrVariables();
        }
		
		/**
		 * Checks if the monomial is either a constant or the only variable occuring is the variable v.
         * @param v The variable which may occur.
         * @return true if no variable occurs, or just v occurs. 
         */
		bool hasNoOtherVariable(Variable::Arg v) const
		{
			if(!mMonomial) return true;
			return mMonomial->hasNoOtherVariable(v);
		}
		
		/**
		 * For terms with exactly one variable, get this variable.
         * @return The only variable occuring in the term.
         */
		Variable::Arg getSingleVariable() const
		{
			assert(getNrVariables() == 1);
			return mMonomial->getSingleVariable();
		}
		
		/**
		 * Is square.
         * @return 
         */
		bool isSquare() const
		{
			return (mCoeff >= (Coefficient)0) && ((!mMonomial) || mMonomial->isSquare());
		}
        
        /**
         * Set the term to zero with the canonical representation.
         */
        void clear()
        {
            mCoeff = 0;
            mMonomial.reset();
        }
		
		void negate()
		{
			mCoeff = -mCoeff;
		}
        
        /**
         * 
         * @param a non-zero coefficient.
         * @return 
         */
        Term* dividedBy(const Coefficient&) const;
        /**
         * 
         * @param 
         * @return 
         */
        Term* dividedBy(Variable::Arg) const;
        /**
         * 
         * @param 
         * @return 
         */
        Term* dividedBy(const Monomial&) const;
        
        Term* dividedBy(const Term&) const;
		
		Term calcLcmAndDivideBy( const Monomial&) const;
        
        Term* derivative(Variable::Arg) const;
        
        Definiteness definiteness() const;
		
		Term* substitute(const std::map<Variable, Coefficient>& substitutions) const;
		Term* substitute(const std::map<Variable, Term<Coefficient>>& substitutions) const;
		
		template<bool gatherCoeff, typename CoeffType>
		void gatherVarInfo(VariablesInformation<gatherCoeff, CoeffType>& varinfo) const;
		
		void gatherVariables(std::set<Variable>& variables) const
		{
			if(mMonomial)
			{
				mMonomial->gatherVariables(variables);
			}
		}
        
        std::string toString(bool infix=true, bool friendlyVarNames=true) const;
        
        template<typename Coeff>
        friend bool operator==(const Term<Coeff>& lhs, const Term<Coeff>& rhs);
        template<typename Coeff>
        friend bool operator==(const Term<Coeff>& lhs, const Coeff& rhs);
        template<typename Coeff>
        friend bool operator==(const Coeff& lhs, const Term<Coeff>& rhs);
        template<typename Coeff>
        friend bool operator==(const Term<Coeff>& lhs, Variable::Arg rhs);
        template<typename Coeff>
        friend bool operator==(Variable::Arg lhs, const Term<Coeff>& rhs);
        template<typename Coeff>
        friend bool operator==(const Term<Coeff>& lhs, const Monomial& rhs);
        template<typename Coeff>
        friend bool operator==(const Monomial& lhs, const Term<Coeff>& rhs);
        
        template<typename Coeff>
        friend bool operator!=(const Term<Coeff>& lhs, const Term<Coeff>& rhs);
        template<typename Coeff>
        friend bool operator!=(const Term<Coeff>& lhs, const Coeff& rhs);
        template<typename Coeff>
        friend bool operator!=(const Coeff& lhs, const Term<Coeff>& rhs);
        template<typename Coeff>
        friend bool operator!=(const Term<Coeff>& lhs, Variable::Arg rhs);
        template<typename Coeff>
        friend bool operator!=(Variable::Arg lhs, const Term<Coeff>& rhs);
        template<typename Coeff>
        friend bool operator!=(const Term<Coeff>& lhs, const Monomial& rhs);
        template<typename Coeff>
        friend bool operator!=(const Monomial& lhs, const Term<Coeff>& rhs);
        
        const Term<Coefficient> operator-() const;
        
        Term& operator *=(const Coefficient& rhs);
        Term& operator *=(Variable::Arg rhs);
        Term& operator *=(const Monomial& rhs);
        Term& operator *=(const Term& rhs);
        
        template<typename Coeff>
        friend const Term<Coeff> operator*(const Term<Coeff>& lhs, const Term<Coeff>& rhs);
        template<typename Coeff>
        friend const Term<Coeff> operator*(const Term<Coeff>& lhs, const Coeff& rhs);
		template<typename Coeff>
        friend const Term<Coeff> operator*(const Coeff& lhs, const Term<Coeff>& rhs);
        template<typename Coeff>
        friend const Term<Coeff> operator*(const Term<Coeff>& lhs, Variable::Arg rhs);
        template<typename Coeff>
        friend const Term<Coeff> operator*(Variable::Arg, const Term<Coeff>& rhs);
        template<typename Coeff>
        friend const Term<Coeff> operator*(const Coeff& lhs, Variable::Arg rhs);
        template<typename Coeff>
        friend const Term<Coeff> operator*(Variable::Arg lhs, const Coeff& rhs);
        template<typename Coeff>
        friend const Term<Coeff> operator*(const Coeff& lhs, const Monomial& rhs);
        template<typename Coeff>
        friend const Term<Coeff> operator*(const Monomial& lhs, const Coeff& rhs);
        
        template<typename Coeff>
        friend std::ostream& operator<<(std::ostream& lhs, const Term<Coeff>& rhs);
        
        static bool EqualMonomial(const Term& lhs, const Term& rhs)
		{
			return	(!lhs.mMonomial && !rhs.mMonomial) || // both no monomial 
					(lhs.mMonomial && rhs.mMonomial && *lhs.mMonomial == *rhs.mMonomial);
		}
};

} // namespace carl

namespace std
{
    template<typename Coefficient>
    class hash<carl::Term<Coefficient>>
    {
    public:
        size_t operator()(const carl::Term<Coefficient>& term) const 
        {
            if(term.isConstant())
                return hash<Coefficient>()(term.coeff());
            else
                return hash<Coefficient>()(term.coeff()) ^ hash<carl::Monomial>()(*term.monomial());
        }
    };
} // namespace std

#include "Term.tpp"

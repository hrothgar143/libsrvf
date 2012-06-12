/*
 * LibSRVF - a shape analysis library using the square root velocity framework.
 *
 * Copyright (C) 2012  Daniel Robinson
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include <cmath>
#include <vector>
#include <stdexcept>

#include "plf.h"
#include "pointset.h"
#include "util.h"
#include "interp.h"

namespace srvf
{

/**
 * Evaluate the PLF at the given parameter value.
 *
 * The result is stored in the first column of \a result.
 *
 * The represented function is taken to be right-continuous at any 
 * discontinuities.
 *
 * \param t the parameter value
 * \param result a \c Pointset to receive the result
 */
void Plf::evaluate(double t, Pointset &result) const
{
  std::vector<double> tv(1,t);
  evaluate(tv,result);
}

/**
 * Evaluate the PLF at several parameter values.
 *
 * The result will be stored in \a result, with one point per column.
 * The caller is responsible for making sure that \a result is large enough 
 * to hold the result.
 *
 * The represented function is taken to be right-continuous at any 
 * discontinuities.
 *
 * \param tv the parameter values at which the function will be evaluated
 * \param result a \c Pointset to receive the result
 */
void Plf::evaluate(const std::vector<double> &tv, Pointset &result) const
{
  srvf::interp::interp_linear(samps(),params(),tv,result);
}

/**
 * Computes the preimages of the numbers in \a tv under this \c Plf.
 *
 * The \c Plf must represent a non-decreasing 1-D function (i.e. 
 * \c dim()==1 and samps()[i]<=samps()[i+1] for \c i=0,...,ncp()-2 ).
 *
 * If the function is not strictly increasing, then a number in \a tv may 
 * not have a unique preimage.  In this case, the rightmost preimage is used.
 *
 * The numbers in \a tv must be sorted in non-decreasing order, and 
 * must lie between the first and last elements of \c samps(), provided 
 * that this \c Plf is non-empty.  If this \c Plf is empty (i.e. 
 * ncp()==0), then this routine will return immediately and \a result will 
 * be left unchanged.
 *
 * \param tv the numbers whose preimages will be found
 * \param result a \c Sequence to receive the result
 */
void Plf::preimages(const std::vector<double> &tv, 
                    std::vector<double> &result) const
{
  if (dim()>1)
    throw std::logic_error("preimages() only supported for 1-D functions");
  
  // Do nothing if this PLF is the empty map, or if tv is empty
  if (ncp()==0) return;
  if (tv.size()==0) return;

  srvf::interp::interp_linear(params(), samps(), tv, result);
}

/**
 * Computes the arc length of the \c Plf.
 *
 * Since the function is piecewise-linear, the arc length is just the sum 
 * of the lengths of the linear segments.
 */
double Plf::arc_length() const
{
  if (samps_.npts()==0) return 0.0;

  double res=0.0;
  for (size_t i=0; i<samps_.npts()-1; ++i)
  {
    res += samps_.distance(i,i+1);
  }
  return res;
}

/**
 * Computes the centroid of this \c Plf.
 */
Point Plf::centroid() const
{
  return samps_.centroid();
}

/**
 * Scale this \c Plf to unit arc length.
 */
void Plf::scale_to_unit_arc_length()
{
  double L = arc_length();
  if (L > 1e-9)
  {
    scale(1.0 / L);
  }
}

/**
 * Subtract the centroid from this \c Plf.
 */
void Plf::translate_to_origin()
{
  Point ctr = centroid();
  ctr *= -1.0;
  translate(ctr);
}

/**
 * Apply a translation to this \c Plf.
 *
 * Adds \a v to each of this function's sample points.  \a v must have 
 * number of rows equal to \c this->dim().
 *
 * \param v the vector by which to translate
 */
void Plf::translate(const Point &P)
{
  if (P.dim() != dim())
    throw std::invalid_argument("P has incorrect dimension");

  samps_.translate(P);
}

/**
 * Apply a rotation to the curve.
 *
 * \param R a \c DxD rotation matrix, where \c D=this->dim()
 */
void Plf::rotate(const Matrix &R)
{
  samps_.rotate(R);
}

/**
 * Apply a uniform scaling to the curve.
 *
 * \param s the scale factor.
 */
void Plf::scale(double s)
{
  samps_.scale(s);
}


////////////////////////////////////////////////////////////////////////////
////////////////////////// friend functions  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////


/**
 * Computes a linear combination of \a F1 and \a F2, using the given weights.
 *
 * \param F1
 * \param F2
 * \param w1
 * \param w2
 */
Plf linear_combination(const Plf &F1, const Plf &F2, 
                       double w1, double w2)
{
  if (F1.dim()!=F2.dim())
    throw std::invalid_argument("F1 and F2 must have the same dimension");
  size_t dim=F1.dim();

  std::vector<double> new_params=srvf::util::unique(F1.params(), F2.params());
  Pointset F1vals(dim,new_params.size());
  Pointset F2vals(dim,new_params.size());
  Pointset new_samps(dim,new_params.size());

  F1.evaluate(new_params,F1vals);
  F2.evaluate(new_params,F2vals);

  for (size_t i=0; i<new_samps.npts(); ++i)
  {
    weighted_sum(F1vals,F2vals,i,i,w1,w2,new_samps,i);
  }

  return Plf(new_samps, new_params);
}

/**
 * Creates a new \c Plf representing \c F1(F2).
 *
 * \param F1 the outer function
 * \param F2 the inner function.  Must be a 1-D function (i.e. \c F2.dim() 
 *   must equal 1).  Also, the range of \a F2 must be contained in the domain 
 *   of \a F1, so that the functions can be composed.
 * \return \c F1(F2), the composition of \a F1 on \a F2
 */
Plf composition(const Plf &F1, const Plf &F2)
{
  if (F2.dim()!=1)
    throw std::invalid_argument("F2 must be 1-dimensional");
  
  std::vector<double> T1pi(F1.ncp());
  F2.preimages(F1.params(),T1pi);
  std::vector<double> T=srvf::util::unique(F2.params(),T1pi);

  Pointset F2T(1,T.size());
  F2.evaluate(T,F2T);
  std::vector<double> F2Tv=F2T.to_vector();

  Pointset F12T(F1.dim(),T.size());
  F1.evaluate(F2Tv,F12T);

  return Plf(F12T,T);
}

/**
 * Returns a new \c Plf representing the inverse of \a F.
 *
 * \c F must represent a 1-dimensional function that is either non-decreasing 
 * or non-increasing.  If the function is horizontal on an interval 
 * \f$[a,b]\f$, so that $\f F(a)=F(b) \f$, then the result will be the 
 * one-sided inverse function which is right-continuous at \f$ F(a) \f$.
 *
 * \param F a \c Plf representing a 1-dimensional monotone function
 * \return a new \c Plf representing the inverese of \a F
 */
Plf inverse(const Plf &F)
{
  return Plf(Pointset(1,F.ncp(),F.params()), F.samps().to_vector());
}

} // namespace srvf

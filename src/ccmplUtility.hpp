/*   This file is part of ccmpl
 *
 *   Copyright (C) 2015,  CentraleSupelec
 *
 *   Author : Herve Frezza-Buet
 *
 *   Contributor :
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public
 *   License (GPL) as published by the Free Software Foundation; either
 *   version 3 of the License, or any later version.
 *   
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *   General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *   Contact : Herve.Frezza-Buet@centralesupelec.fr
 *
 */

#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <iterator>

namespace ccmpl {


  inline std::string filename(const std::string& prefix, unsigned int i, const std::string& suffix) {
    std::ostringstream ostr;
    ostr << prefix << '-' << std::setw(6) << std::setfill('0') << i << '.' << suffix;
    return ostr.str();
  }

  inline std::string nofile() {
    return "";
  }


  class range {
  private:
    
    double min;
    double max;
    unsigned int nb;
    
  public:

    range(double min, double max, unsigned int nb) : min(min), max(max), nb(nb) {}
    range(unsigned int nb)                         : range(0,1,nb)              {}
    range()                        = delete;
    range(const range&)            = default;
    range& operator=(const range&) = default;
    
    class iterator : public std::iterator<std::random_access_iterator_tag,double> {
    private:
      unsigned int current;
      double min;
      double max;
      double coef;
      iterator(unsigned int current, double min, double max, double coef) : current(current), min(min), max(max), coef(coef) {}
    public:
      iterator()                           = default;
      iterator(const iterator&)            = default;
      iterator& operator=(const iterator&) = default;
      iterator(unsigned int current, double min, double max, unsigned int nb) : current(current), min(min), max(max), coef((max-min)/(nb-1)) {}
      iterator(double min, double max, unsigned int nb) : iterator(0,min,max,nb) {}
      iterator& operator++() {++current; return *this;}
      iterator& operator--() {--current; return *this;}
      iterator& operator+=(int diff) {current+=diff; return *this;}
      iterator& operator-=(int diff) {current-=diff; return *this;}
      iterator operator++(int) {iterator res = *this; ++*this; return res;}
      iterator operator--(int) {iterator res = *this; --*this; return res;}
      int operator-(const iterator& i) const {return current - i.current;}
      iterator operator+(int i) const {return iterator(current+i,min,max,coef);}
      iterator operator-(int i) const {return iterator(current-i,min,max,coef);}
      double operator*() const {return min + current*coef;}
      bool operator==(const iterator& i) const {return current == i.current && min == i.min && max == i.max;}
      bool operator!=(const iterator& i) const {return current != i.current || min != i.min || max != i.max;}
    };

    iterator begin() const {return iterator( 0,min,max,nb);}
    iterator end()   const {return iterator(nb,min,max,nb);}
  };

  
  
}

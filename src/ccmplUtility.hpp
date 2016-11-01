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

namespace ccmpl {


  std::string filename(const std::string& prefix, unsigned int i, const std::string& suffix) {
    std::ostringstream ostr;
    ostr << prefix << '-' << std::setw(6) << std::setfill('0') << i << '.' << suffix;
    return ostr.str();
  }

  std::string nofile() {
    return "";
  }
}

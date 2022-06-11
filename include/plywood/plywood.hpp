// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PLYWOOD_HPP
#define PLYWOOD_HPP

#include "plywood_exceptions.hpp"
#include "plywood_header_parser.hpp"
#include "plywood_header_scanner.hpp"
#include "plywood_std.hpp"

#include <istream>
#include <vector>

namespace plywood
{
  class PlyFile
  {
  public:
    PlyFile(std::istream &is) : parser_{is} {}

    const std::vector<PlyElement> &elements() const { return parser_.elements(); }

  private:
    void parseHeader();

    HeaderParser parser_;
  };
}

#endif

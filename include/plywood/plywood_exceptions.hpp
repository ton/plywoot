// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PLYWOOD_EXCEPTIONS_HPP
#define PLYWOOD_EXCEPTIONS_HPP

#include <fmt/format.h>

#include <stdexcept>
#include <string>

namespace plywood
{
  class Exception : public std::runtime_error
  {
  public:
    Exception(const std::string &message) : std::runtime_error(message) {}
  };
}

#endif

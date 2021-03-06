//------------------------------------------------------------------------------
//
//   Copyright 2018-2019 Fetch.AI Limited
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//------------------------------------------------------------------------------

#include "oef-core/karma/KarmaPolicyNone.hpp"

#include <unistd.h>

KarmaPolicyNone::KarmaPolicyNone()
  : IKarmaPolicy()
{
  FETCH_LOG_INFO(LOGGING_NAME, "KarmaPolicyNone created");
  /// std::cout << "KarmaPolicyNone created" << std::endl;
  sleep(2);  // TODO: Is this supposed to be there?
}

KarmaAccount KarmaPolicyNone::GetAccount(const std::string & /*pubkey*/, const std::string & /*ip*/)
{
  return mkAccount(0, "NO_KARMA_POLICY");
}

bool KarmaPolicyNone::perform(const KarmaAccount & /*identifier*/, const std::string & /*action*/,
                              bool /*force*/)
{
  return true;
}

bool KarmaPolicyNone::CouldPerform(const KarmaAccount & /*identifier*/,
                                   const std::string & /*action*/)
{
  return true;
}

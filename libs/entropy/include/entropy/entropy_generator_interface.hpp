#pragma once
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

#include "beacon/block_entropy.hpp"
#include "core/byte_array/const_byte_array.hpp"

namespace fetch {
namespace ledger {

class EntropyGeneratorInterface
{
public:
  using Digest = byte_array::ConstByteArray;

  // Construction / Destruction
  EntropyGeneratorInterface()          = default;
  virtual ~EntropyGeneratorInterface() = default;

  enum class Status
  {
    OK,
    NOT_READY,
    FAILED
  };

  /// @name Entropy Generator
  /// @{
  virtual Status GenerateEntropy(uint64_t block_number, beacon::BlockEntropy &entropy) = 0;
  /// @}
};

}  // namespace ledger
}  // namespace fetch

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

#include "dmlf/update_interface.hpp"
#include "muddle/muddle_interface.hpp"
#include "muddle/rpc/client.hpp"
#include "oef-base/threading/Task.hpp"

namespace fetch {
namespace dmlf {
namespace colearn {

class MuddleOutboundAnnounceTask : public oef::base::Task
{
public:
  using MuddlePtr                           = muddle::MuddlePtr;
  using RpcClient                           = fetch::muddle::rpc::Client;
  using RpcClientPtr                        = std::shared_ptr<RpcClient>;
  using Bytes                               = byte_array::ByteArray;
  using ExitState                           = oef::base::ExitState;
  using Address                             = fetch::muddle::Address;
  static constexpr char const *LOGGING_NAME = "MuddleOutboundAnnounceTask";

  MuddlePtr mud_;

  unsigned int rate_;

  MuddleOutboundAnnounceTask(MuddlePtr mud, unsigned int rate)
    : mud_(std::move(mud))
    , rate_(rate)
  {}
  ~MuddleOutboundAnnounceTask() override = default;

  ExitState run() override;
  bool      IsRunnable() const override;

  MuddleOutboundAnnounceTask(MuddleOutboundAnnounceTask const &other) = delete;
  MuddleOutboundAnnounceTask &operator=(MuddleOutboundAnnounceTask const &other)  = delete;
  bool                        operator==(MuddleOutboundAnnounceTask const &other) = delete;
  bool                        operator<(MuddleOutboundAnnounceTask const &other)  = delete;

protected:
private:
};

}  // namespace colearn
}  // namespace dmlf
}  // namespace fetch

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

#include "core/byte_array/decoders.hpp"
#include "core/service_ids.hpp"
#include "dmlf/colearn/colearn_protocol.hpp"
#include "dmlf/colearn/muddle_outbound_announce_task.hpp"
#include "muddle/rpc/client.hpp"
#include "oef-base/threading/Taskpool.hpp"
#include "dmlf/colearn/muddle_learner_networker_impl.hpp"

namespace fetch {
namespace dmlf {
namespace colearn {

MuddleOutboundAnnounceTask::ExitState MuddleOutboundAnnounceTask::run()
{
  serializers::MsgPackSerializer buf;

  std::string update_name = "HELO";

  {
    MuddleLearnerNetworkerImpl::Lock lock(impl_ . mutex_);
    buf << update_name << impl_ . my_waypoints_;
  }
  mud_->GetEndpoint().Broadcast(SERVICE_DMLF, CHANNEL_COLEARN_BROADCAST, buf.data());

  FETCH_LOG_WARN(LOGGING_NAME, "ANNOUNCING:", fetch::byte_array::ToBase64(mud_->GetAddress()));

  submit(pool_to_restart_in_, std::chrono::milliseconds(rate_));
  return ExitState::COMPLETE;
}

bool MuddleOutboundAnnounceTask::IsRunnable() const
{
  return true;
}
}  // namespace colearn
}  // namespace dmlf
}  // namespace fetch

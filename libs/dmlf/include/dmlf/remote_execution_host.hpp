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

#include "dmlf/execution/execution_interface.hpp"
#include "dmlf/execution/execution_params.hpp"
#include "dmlf/execution/execution_result.hpp"
#include "dmlf/execution_workload.hpp"
#include "muddle/muddle_interface.hpp"
#include "muddle/rpc/client.hpp"
#include "muddle/rpc/server.hpp"
#include "network/service/call_context.hpp"

namespace fetch {
namespace dmlf {

class RemoteExecutionHost
{
public:
  using Name                  = ExecutionInterface::Name;
  using SourceFiles           = ExecutionInterface::SourceFiles;
  using ExecutionInterfacePtr = ExecutionWorkload::ExecutionInterfacePtr;

  using MuddlePtr = muddle::MuddlePtr;
  using Uri       = network::Uri;
  using RpcClient = fetch::muddle::rpc::Client;
  using OpIdent   = ExecutionWorkload::OpIdent;
  using Params    = ExecutionParams;

  using PendingWorkloads = std::list<ExecutionWorkload>;

  RemoteExecutionHost(MuddlePtr mud, ExecutionInterfacePtr executor);

  virtual ~RemoteExecutionHost() = default;

  RemoteExecutionHost(RemoteExecutionHost const &other) = delete;
  RemoteExecutionHost &operator=(RemoteExecutionHost const &other)  = delete;
  bool                 operator==(RemoteExecutionHost const &other) = delete;
  bool                 operator<(RemoteExecutionHost const &other)  = delete;

  virtual bool CreateExecutable(service::CallContext const &context, OpIdent const &op_id,
                                Name const &execName, SourceFiles const &sources);
  virtual bool DeleteExecutable(service::CallContext const &context, OpIdent const &op_id,
                                Name const &execName);

  virtual bool CreateState(service::CallContext const &context, OpIdent const &op_id,
                           Name const &stateName);
  virtual bool CopyState(service::CallContext const &context, OpIdent const &op_id,
                         Name const &srcName, Name const &newName);
  virtual bool DeleteState(service::CallContext const &context, OpIdent const &op_id,
                           Name const &stateName);

  virtual bool Run(service::CallContext const &context, OpIdent const &op_id, Name const &execName,
                   Name const &stateName, std::string const &entrypoint, const Params &params);

  bool ExecuteOneWorkload();

protected:
private:
  MuddlePtr                  mud_;
  std::shared_ptr<RpcClient> client_;

  PendingWorkloads      pending_workloads_;
  ExecutionInterfacePtr executor_;
};

}  // namespace dmlf
}  // namespace fetch

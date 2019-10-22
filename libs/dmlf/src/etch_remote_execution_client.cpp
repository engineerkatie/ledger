#include "dmlf/etch_remote_execution_client.hpp"
#include "vm/string.hpp"
#include "vm/module.hpp"

using namespace fetch::vm;

namespace fetch {
namespace vm_modules {

fetch::vm::Ptr<fetch::vm::String> EtchRemoteExecutionClient::Wabble()
{
  return fetch::vm::Ptr<fetch::vm::String>{
    new fetch::vm::String{vm_, "wabble"}
  };
}

void fetch::vm_modules::EtchRemoteExecutionClient::Bind(fetch::vm::Module &module)
{
  module.CreateClassType<EtchRemoteExecutionClient>("EtchRemoteExecutionClient")
    .CreateMemberFunction("Wabble", &EtchRemoteExecutionClient::Wabble);
}

EtchRemoteExecutionClient::EtchRemoteExecutionClient(VM *vm, TypeId type_id)
  : Object(vm, type_id)
  , vm_(vm)
{}

Ptr<EtchRemoteExecutionClient> EtchRemoteExecutionClient::Constructor(fetch::vm::VM *vm, TypeId type_id)
{
  return Ptr<EtchRemoteExecutionClient>{ new EtchRemoteExecutionClient(vm, type_id) };
}

}  // namespace vm_modules
}  // namespace fetch


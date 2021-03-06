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

#include "core/mutex.hpp"

#include <cstdint>

namespace fetch {
namespace http {

class HTTPRequest;

class AbstractHTTPServer
{
public:
  using HandleType = uint64_t;

  virtual void PushRequest(HandleType client, HTTPRequest req) = 0;

  static HandleType next_handle()
  {
    FETCH_LOCK(global_handle_mutex_);
    HandleType ret = global_handle_counter_;
    ++global_handle_counter_;
    return ret;
  }

private:
  static HandleType global_handle_counter_;
  static Mutex      global_handle_mutex_;
};

}  // namespace http
}  // namespace fetch

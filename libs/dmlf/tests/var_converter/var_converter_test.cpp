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

#include "core/byte_array/byte_array.hpp"
#include "core/serializers/array_interface.hpp"
#include "core/serializers/container_constructor_interface.hpp"
#include "core/serializers/main_serializer.hpp"
#include "core/service_ids.hpp"
#include "dmlf/execution/basic_vm_engine.hpp"
#include "dmlf/execution/execution_engine_interface.hpp"
#include "dmlf/execution/execution_error_message.hpp"
#include "dmlf/execution/execution_params.hpp"
#include "dmlf/execution/execution_result.hpp"
#include "dmlf/remote_execution_client.hpp"
#include "dmlf/remote_execution_host.hpp"
#include "dmlf/remote_execution_protocol.hpp"
#include "dmlf/var_converter.hpp"
#include "gtest/gtest.h"

#include <iomanip>

namespace fetch {
namespace dmlf {

ExecutionResult testEtchExec(const char *src, const std::string &name, variant::Variant const &x)
{
  BasicVmEngine engine;

  ExecutionResult createdProgram = engine.CreateExecutable("helloWorld", {{"etch", src}});

  if (!createdProgram.succeeded())
  {
    std::cout << "COMPILE FAIL:" << createdProgram.error().message() << std::endl;
    return createdProgram;
  }

  ExecutionResult createdState = engine.CreateState("state");
  if (!createdState.succeeded())
  {
    std::cout << "STATE FAIL:" << createdState.error().message() << std::endl;
    return createdState;
  }

  ExecutionResult result = engine.RunDirectConversion("helloWorld", "state", name, {x});
  std::cout << "OUTPUT TYPE:" << VarConverter::describe(result.output()) << std::endl;
  std::cout << "OUTPUT TEXT:" << result.console() << std::endl;
  std::cout << "OUTPUT ERR :" << result.error().message() << std::endl;

  return result;
}

TEST(VarConverterTest, convert_INT32)
{
  static char const *TEXT4 = R"(
    function main(x : Int32) : Int32
      return
      x + 17
         ;
    endfunction
  )";

  fetch::variant::Variant input(4);

  auto result = testEtchExec(TEXT4, "main", input);
  EXPECT_TRUE(result.output().IsInteger());
  EXPECT_EQ(result.output().As<int32_t>(), 21);
}


TEST(VarConverterTest, convert_4xINT32_array)
{
  static char const *TEXT4 = R"(
    function main(x : Array<Int32>) : Int32
      return
         x[0]+x[1]+x[2]+x[3]
         ;
    endfunction
  )";

  auto input = fetch::variant::Variant::Array(4);
  for (std::size_t i = 0; i < 4; i++)
  {
    input[i] = i + 1;
  }

  auto result = testEtchExec(TEXT4, "main", input);
  EXPECT_TRUE(result.output().IsInteger());
  EXPECT_EQ(result.output().As<int32_t>(), 10);
}

TEST(DISABLED_VarConverterTest, convert_4xFLOAT32_array) // FAILS, returns random number.
{
  static char const *TEXT4 = R"(
    function main(x : Array<Float32>) : Float32
      return
         x[0]+x[1]+x[2]+x[3]
         ;
    endfunction
  )";

  auto input = fetch::variant::Variant::Array(4);
  for (std::size_t i = 0; i < 4; i++)
  {
    input[i] = float(i + 1);
  }

  auto result = testEtchExec(TEXT4, "main", input);
  EXPECT_TRUE(result.output().IsFloatingPoint());
  EXPECT_EQ(result.output().As<double>(), 10);
}

TEST(DISABLED_VarConverterTest, convert_4xFLOAT64_array)
{
  static char const *TEXT4 = R"(
    function main(x : Array<Float64>) : Float64
      return
         x[0]+x[1]+x[2]+x[3]
         ;
    endfunction
  )";

  auto input = fetch::variant::Variant::Array(4);
  for (std::size_t i = 0; i < 4; i++)
  {
    input[i] = double(i + 1);
  }

  auto result = testEtchExec(TEXT4, "main", input);
  EXPECT_TRUE(result.output().IsFloatingPoint());
  EXPECT_EQ(result.output().As<float>(), 10);
}

TEST(DISABLED_VarConverterTest, convert_4xFIXED64_array) // Fails, can't get result out.
{
  static char const *TEXT4 = R"(
    function main(x : Array<Fixed64>) : Fixed64
      var r : Fixed64 =
         x[0]+x[1]+x[2]+x[3]
         ;
      printLn(r);
      return r;
    endfunction
  )";

  auto input = fetch::variant::Variant::Array(4);
  for (std::size_t i = 0; i < 4; i++)
  {
    input[i] = fixed_point::fp64_t(i + 1);
  }

  auto result = testEtchExec(TEXT4, "main", input);
  EXPECT_TRUE(result.output().IsFixedPoint());
  auto r_fp = result.output().As<fixed_point::fp64_t>();
  EXPECT_EQ(r_fp, 10);
}

TEST(DISABLED_VarConverterTest, convert_4xFIXED32_array) // Fails, don't know.
{
  static char const *TEXT4 = R"(
    function main(x : Array<Fixed32>) : Fixed32
      return
         x[0]+x[1]+x[2]+x[3]
         ;
    endfunction
  )";
  std::cerr << "*A" << std::endl;

  auto input = fetch::variant::Variant::Array(4);
  std::cerr << "*B" << std::endl;
  for (std::size_t i = 0; i < 4; i++)
  {
  std::cerr << "*C" << std::endl;
    input[i] = fixed_point::fp32_t(i + 1);
  }

  std::cerr << "*D" << std::endl;
  auto result = testEtchExec(TEXT4, "main", input);
  EXPECT_TRUE(result.output().IsFixedPoint());
  auto r_fp = result.output().As<fixed_point::fp32_t>();
  EXPECT_EQ(r_fp, 10);
}

TEST(DISABLED_VarConverterTest, convert_i64)
{
  static char const *TEXT4 = R"(
    function main(x : Int64) : Int64
      return x + 17i64;
    endfunction
  )";

  variant::Variant input = variant::Variant(7);
  auto result = testEtchExec(TEXT4, "main", input);
  std::cout << "OUTPUT TYPE:" << VarConverter::describe(result.output()) << std::endl;
  EXPECT_TRUE(result.output().IsInteger());
  EXPECT_EQ(result.output().As<long long int>(), 24);
}

TEST(DISABLED_VarConverterTest, convert_fp64)
{
  static char const *TEXT4 = R"(
    function main(x : Fixed64) : Fixed64
      return x + 17fp64;
    endfunction
  )";

  variant::Variant input = variant::Variant(7);
  auto result = testEtchExec(TEXT4, "main", input);
  std::cout << "OUTPUT TYPE:" << VarConverter::describe(result.output()) << std::endl;
  EXPECT_TRUE(result.output().IsFixedPoint());
  EXPECT_EQ(result.output().As<fixed_point::fp64_t>(), 24);
}

TEST(DISABLED_VarConverterTest, convert_4xSTR_array) // FAILS, string output doesn't work.
{
  static char const *TEXT4 = R"(
    function main(x : Array<String>) : String
      var r : String;
      r = x[0]+x[1]+x[2]+x[3];
      printLn("Result");
      printLn(r);
      return r;
    endfunction
  )";

  auto input = fetch::variant::Variant::Array(4);
  for (std::size_t i = 0; i < 4; i++)
  {
    input[i] = std::to_string(i + 1);
  }

  auto result = testEtchExec(TEXT4, "main", input);
  EXPECT_TRUE(result.output().IsString());
  EXPECT_EQ(result.output().As<std::string>(), "1234");
}

TEST(DISABLED_VarConverterTest, convert_4x4xINT32_array) // Fails, needs branch with new param handling code.
{
  static char const *TEXT44 = R"(
    function main(x : Array<Array<Int32>>) : Int32
      return
         x[0][0]+x[1][0]+x[2][0]+x[3][0]+
         x[0][1]+x[1][1]+x[2][1]+x[3][1]+
         x[0][2]+x[1][2]+x[2][2]+x[3][2]+
         x[0][3]+x[1][3]+x[2][3]+x[3][3]
      ;
    endfunction
  )";

  auto input = fetch::variant::Variant::Array(4);
  input[0]   = fetch::variant::Variant::Array(4);
  input[1]   = fetch::variant::Variant::Array(4);
  input[2]   = fetch::variant::Variant::Array(4);
  input[3]   = fetch::variant::Variant::Array(4);

  for (std::size_t i = 0; i < 4; i++)
  {
    for (std::size_t j = 0; j < 4; j++)
    {
      input[i][j] = i * j;
    }
  }

  auto result = testEtchExec(TEXT44, "main", input);
  EXPECT_TRUE(result.output().IsInteger());
  EXPECT_EQ(result.output().As<uint32_t>(), 36);
}

}  // namespace dmlf
}  // namespace fetch

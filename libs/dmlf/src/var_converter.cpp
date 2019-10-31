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
#include "dmlf/var_converter.hpp"
#include "vm/vm.hpp"
#include "vm/array.hpp"

namespace fetch {
namespace dmlf {

using Var = variant::Variant;
using Serializer = fetch::serializers::MsgPackSerializer;

using ConverterFunction = std::function<void (Serializer &, const Var &)>;
using ConverterFunctionInputs = std::pair<std::string, std::string>;
using ConverterFunctions =std::map<ConverterFunctionInputs, ConverterFunction>;

using AssignerFunction = std::function<VarConverter::VMVar (const VarConverter::LedVar &)>;
using AssignerFunctionInputs = std::pair<std::string, std::string>;
using AssignerFunctions =std::map<AssignerFunctionInputs, AssignerFunction>;

class BadConv : public std::range_error
{
public:
  BadConv(std::string const &format, variant::Variant const &source)
    : std::range_error("Bad Conversion: " + VarConverter::describe(source) + " =/=> " + format)
  {
  }
};

std::string VarConverter::describe(const variant::Variant &x)
{
  if (x.IsUndefined())
  {
    return "undef";
  }
  if (x.IsInteger())
  {
    return "integer";
  }
  if (x.IsFloatingPoint())
  {
    return "float";
  }
  if (x.IsFixedPoint())
  {
    return "fp";
  }
  if (x.IsBoolean())
  {
    return "bool";
  }
  if (x.IsString())
  {
    return "string";
  }
  if (x.IsNull())
  {
    return "null";
  }
  if (x.IsArray())
  {
    return "array";
  }
  if (x.IsObject())
  {
    return "object";
  }
  return "unknown";
}



bool VarConverter::Convert(byte_array::ByteArray &target, const variant::Variant &source,
                           const std::string &format)
{
  fetch::serializers::MsgPackSerializer ss;
  std::cerr << "*CONVERT" << std::endl;
  try
  {
    if (Convert(ss, source, format))
    {
      auto data = ss.data().pointer();
      auto size = ss.size();
      target.Reserve(size);
      target.Resize(size);
      target.WriteBytes(data, size);
      Dump(target);
      return true;
    }
  }
  catch(const BadConv &x)
  {
    std::cerr << "***************" << x.what() << std::endl;
  }
  return false;
}


ConverterFunctions CONVERTER_FUNCTIONS = {
  {{"Int64", "integer"}, [](Serializer &os, const Var &v){ os << v.As<int64_t>(); } },
  {{"UInt64", "integer"}, [](Serializer &os, const Var &v){ os << v.As<uint64_t>(); } },
  {{"Int32", "integer"}, [](Serializer &os, const Var &v){ os << v.As<int32_t>(); } },
  {{"UInt32", "integer"}, [](Serializer &os, const Var &v){ os << v.As<uint32_t>(); } },
  {{"Int16", "integer"}, [](Serializer &os, const Var &v){ os << v.As<int16_t>(); } },
  {{"UInt16", "integer"}, [](Serializer &os, const Var &v){ os << v.As<uint16_t>(); } },
  {{"Int8", "integer"}, [](Serializer &os, const Var &v){ os << v.As<int8_t>(); } },
  {{"UInt8", "integer"}, [](Serializer &os, const Var &v){ os << v.As<uint8_t>(); } },

  {{"Float32", "float"}, [](Serializer &os, const Var &v){ os << v.As<float>(); } },
  {{"Float64", "float"}, [](Serializer &os, const Var &v){ os << v.As<double>(); } },

  {{"Fixed32", "fp"}, [](Serializer &os, const Var &v){
      auto val = v.As<fixed_point::fp32_t>();
      std::cout << "val=" << val << std::endl;
      os << val;
    }
  },
  {{"Fixed64", "fp"}, [](Serializer &os, const Var &v){
      auto val = v.As<fixed_point::fp64_t>();
      std::cout << "val=" << val << std::endl;
      os << val;
    }
  },
  {{"String", "string"}, [](Serializer &os, const Var &v){ os << v.As<std::string>(); } },

  {{"Bool", "bool"}, [](Serializer &os, const Var &v){ os << v.As<bool>(); } },
};

AssignerFunctions ASSIGNER_FUNCTIONS = {
  {{"Int64", "integer"}, [](const Var &v) { return VarConverter::VMVar(v.As<int64_t>(), vm::TypeIds::Int64);  } },
  {{"Int32", "integer"}, [](const Var &v) { return VarConverter::VMVar(v.As<int32_t>(), vm::TypeIds::Int32);  } },
 // {{"Int32", "integer"}, [](const Var &v, TypeId tid) { return VarConverter::VMVar(targ.Assign(v.As<int64_t>(), tid);  } },
 // {{"UInt64", "integer"}, [](VarConverter::VMVar &targ, const Var &v, TypeId tid)){ targ.Assign(v.As<uint64_t>(), tid); } },
 // {{"Int32", "integer"}, [](VarConverter::VMVar &targ, const Var &v, TypeId tid)) { targ.Assign(v.As<int32_t>() , tid);  } },
 // {{"UInt32", "integer"}, [](VarConverter::VMVar &targ, const Var &v, TypeId tid)){ targ.Assign(v.As<uint32_t>(), tid); } },
  /*
  {{"Int16", "integer"}, [](VarConverter::VMVar &targ, const Var &v){ os << v.As<int16_t>(); } },
  {{"UInt16", "integer"}, [](VarConverter::VMVar &targ, const Var &v){ os << v.As<uint16_t>(); } },
  {{"Int8", "integer"}, [](VarConverter::VMVar &targ, const Var &v){ os << v.As<int8_t>(); } },
  {{"UInt8", "integer"}, [](VarConverter::VMVar &targ, const Var &v){ os << v.As<uint8_t>(); } },

  {{"Float32", "float"}, [](VarConverter::VMVar &targ, const Var &v){ os << v.As<float>(); } },
  {{"Float64", "float"}, [](VarConverter::VMVar &targ, const Var &v){ os << v.As<double>(); } },

  {{"Fixed32", "fp"}, [](VarConverter::VMVar &targ, const Var &v){
      auto val = v.As<fixed_point::fp32_t>();
      std::cout << "val=" << val << std::endl;
      os << val;
    }
  },
  {{"Fixed64", "fp"}, [](VarConverter::VMVar &targ, const Var &v){
      auto val = v.As<fixed_point::fp64_t>();
      std::cout << "val=" << val << std::endl;
      os << val;
    }
  },
  {{"String", "string"}, [](VarConverter::VMVar &targ, const Var &v){ os << v.As<std::string>(); } },

  {{"Bool", "bool"}, [](VarConverter::VMVar &targ, const Var &v){ os << v.As<bool>(); } },*/
};

VarConverter::VMVar VarConverter::Convert(const LedVar &source, const std::string &format, VM &vm)
{
  AssignerFunctionInputs input{format, VarConverter::describe(source)};
  AssignerFunctions::iterator iter = ASSIGNER_FUNCTIONS.find(input);
  std::cout << format << " <= " << VarConverter::describe(source) << "?" << std::endl;
  if (iter != ASSIGNER_FUNCTIONS.end())
  {
    VMVar r;
    r = iter -> second(source);
    return r;
  }

  if (format.substr(0, 6) == "Array<")
  {
    if (!source.IsArray())
    {
      throw BadConv(format, source);
    }
    vm::Ptr<vm::Object> r = vm.CreateNewObjectByType(format);
    auto subformat = format.substr(6, format.size() - 7);
    vm::Object *objp = &(*r);
    auto arr = dynamic_cast<vm::IArray*>(objp);

    for (std::size_t i = 0; i < source.size(); i++)
    {
      //auto element = Convert(source[i], subformat, vm);
      std::cout << arr -> elements.size() << std::endl;
      //Append(element.As);
    }

    return VMVar(r, r->GetTypeId());
  }

  throw BadConv(format, source);
}

//bool VarConverter::Convert(fetch::serializers::MsgPackSerializer &os,
//                           const Var &source, const std::string &format)
//{
//}

bool VarConverter::Convert(fetch::serializers::MsgPackSerializer &os,
                           const Var &source, const std::string &format)
{

  ConverterFunctionInputs input{format, VarConverter::describe(source)};
  ConverterFunctions::iterator iter = CONVERTER_FUNCTIONS.find(input);
  std::cout << format << " <= " << VarConverter::describe(source) << "?" << std::endl;
  if (iter != CONVERTER_FUNCTIONS.end())
  {
    iter->second(os, source);
    return true;
  }

  if (format.substr(0, 6) == "Array<")
  {
    if (!source.IsArray())
    {
      throw BadConv(format, source);
    }
    //os << format;

    unsigned char foo[1];
    foo[0] = 0xdd;
    os.WriteBytes(foo, 1);
    os << uint32_t(source.size());

    auto subformat = format.substr(6, format.size() - 7);
    // std::cout << "array of.... " << subformat << std::endl;
    bool r = true;
    for (std::size_t i = 0; i < source.size(); i++)
    {
      r &= Convert(os, source[i], subformat);
    }
    return r;
  }

  throw BadConv(format, source);
}

void VarConverter::Dump(byte_array::ByteArray &ba)
{
  std::size_t width = 40;

  for (std::size_t orig = 0; orig < ba.size(); orig += width)
  {
    for (std::size_t offs = 0; offs < width && orig + offs < ba.size(); offs++)
    {
      auto byte = ba[orig + offs];
      std::cout << "  " << std::hex << std::setw(2) << int(byte);
    }
    std::cout << std::endl;
    for (std::size_t offs = 0; offs < width && orig + offs < ba.size(); offs++)
    {
      auto byte = ba[orig + offs];
      std::cout << " " << std::dec << std::setw(3) << int(byte);
    }
    std::cout << std::endl;
    for (std::size_t offs = 0; offs < width && orig + offs < ba.size(); offs++)
    {
      auto byte = ba[orig + offs];
      auto c    = static_cast<char>(byte);
      if (byte < 32 || byte >= 127)
      {
        c = '.';
      }
      std::cout << "   " << std::dec << c;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }
}

}  // namespace dmlf
}  // namespace fetch

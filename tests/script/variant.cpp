#include<iostream>
#include<script/variant.hpp>

#include<cassert>
#include<sstream>
using namespace fetch::script;

#include"unittest.hpp"

int main() {
  
  SCENARIO("Basic manipulation"){
    SECTION("Variant") {
      Variant x;
      x = 1;
      EXPECT( x.type() == fetch::script::VariantType::INTEGER );
      x = "Hello world";
      EXPECT( x.type() == fetch::script::VariantType::STRING );
      x = nullptr;
      EXPECT( x.type() == fetch::script::VariantType::NULL_VALUE );
      x = 4.21;
      EXPECT( x.type() == fetch::script::VariantType::FLOATING_POINT );
      x.MakeUndefined();
      EXPECT( x.type() == fetch::script::VariantType::UNDEFINED );      
    };
    
    SECTION("Variant list") {
      VariantList x(6);
      EXPECT( x.size() == 6 );
      
      x[0] = 1.2;
      x[1] = "Hello world";
      x[2] = 2;
      x[3] = true;
      x[5] = nullptr;    
      
      EXPECT( x[0].type() == fetch::script::VariantType::FLOATING_POINT );
      EXPECT( x[1].type() == fetch::script::VariantType::STRING );
      EXPECT( x[2].type() == fetch::script::VariantType::INTEGER );
      EXPECT( x[3].type() == fetch::script::VariantType::BOOLEAN);
      EXPECT( x[4].type() == fetch::script::VariantType::UNDEFINED );
      EXPECT( x[5].type() == fetch::script::VariantType::NULL_VALUE );    

      VariantList y(x, 2, 3);
      EXPECT( y[0].type() == fetch::script::VariantType::INTEGER );      
      EXPECT( y[1].type() == fetch::script::VariantType::BOOLEAN);
      EXPECT( y[2].type() == fetch::script::VariantType::UNDEFINED );
      
    };


    SECTION("Variant object") {
      Variant obj  = Variant::Object();
      obj["numberOfTransactions"] = uint32_t(9);
      EXPECT( obj["numberOfTransactions"].type() == fetch::script::VariantType::INTEGER );
      EXPECT( obj["numberOfTransactions"].as_int() == 9 );

      
      obj["numberOfTransactions"] = "Hello world";
      std::cout << obj["numberOfTransactions"].type() << std::endl;      
      obj["blah"] = 9;
      obj["Hello"] = false;
      obj["XX"] = nullptr ;
      
      std::cout << obj["numberOfTransactions"].type() << std::endl;
      
      EXPECT( obj["numberOfTransactions"].type() == fetch::script::VariantType::STRING );
      EXPECT( obj["numberOfTransactions"].as_byte_array() == "Hello world" );
      
      EXPECT( obj["blah"].type() == fetch::script::VariantType::INTEGER );
      EXPECT( obj["blah"].as_int() == 9 );
      
      EXPECT( obj["Hello"].type() == fetch::script::VariantType::BOOLEAN );
      EXPECT( obj["Hello"].as_bool() == false );

      EXPECT( obj["XX"].type() == fetch::script::VariantType::NULL_VALUE );

      Variant obj2  = Variant::Object();
      obj["numberOfTransactions"] = uint32_t(9);
      EXPECT( obj2["numberOfTransactions"].type() == fetch::script::VariantType::INTEGER );
      EXPECT( obj2["numberOfTransactions"].as_int() == 9 );
    };
    
    
    
    SECTION("Nested variants") {
      Variant x;
      x.MakeArray(2);
      x[0].MakeArray(3);
      x[0][0] = 1;
      x[0][1] = 3;
      x[0][2] = 7;
      x[1] = 1.23e-6;

      EXPECT( x.type() == fetch::script::VariantType::ARRAY );
      EXPECT( x[0].type() == fetch::script::VariantType::ARRAY );
      EXPECT( x[0][0].type() == fetch::script::VariantType::INTEGER );
      EXPECT( x[0][1].type() == fetch::script::VariantType::INTEGER );
      EXPECT( x[0][2].type() == fetch::script::VariantType::INTEGER );
      EXPECT( x[1].type() == fetch::script::VariantType::FLOATING_POINT );      

      std::cout << x << std::endl;
      
    };    
  };

  SCENARIO("Streaming"){
    SECTION("Variant list") {
      VariantList x(6);
      EXPECT( x.size() == 6 );
      
      x[0] = 1.2;
      x[1] = "Hello world";
      x[2] = 2;
      x[3] = true;
      x[5] = nullptr;    

      std::stringstream ss;      
      ss.str("");
      ss << x;
      EXPECT(ss.str() == "[1.2, \"Hello world\", 2, true, (undefined), null]");
    };
    
    SECTION("Nested variants") {
      Variant x;
      
      x.MakeArray(2);
      x[0].MakeArray(3);
      x[0][0] = 1;
      x[0][1] = 3;
      x[0][2] = 7;
      x[1] = 1.23;
      
      std::stringstream ss;      
      ss.str("");
      ss << x;

      EXPECT( ss.str() == "[[1, 3, 7], 1.23]" ); 
      
    };        
  };

  return 0;
}


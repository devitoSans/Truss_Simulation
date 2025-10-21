#include "component.hpp"
#include <catch_amalgamated.hpp>

TEST_CASE("Component: Converting from MemberInterface")
{
    MemberInterface Imember1 = MemberInterface();
    
    cmp::component comp = cmp::convert(Imember1);
    REQUIRE(cmp::member(comp)->get_properties().get_length() == 20.0);
    
    {
        MemberInterface Imember2 = MemberInterface();
        Imember2.modify_angle(20);
        
        comp = cmp::convert(Imember2);
    }
    
    REQUIRE(cmp::member(comp)->get_angle() == 20.0);
}
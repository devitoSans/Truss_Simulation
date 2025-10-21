#ifndef __COMPONENT__
#define __COMPONENT__

#include <stdexcept>
#include <variant>

#include "member/memberInterface.hpp"
#include "supports/supportInterface.hpp"
#include "../mathHelpers/mathHelpers.hpp"

namespace cmp
{
    enum componentType
    {
        MEMBER,
        SUPPORT
    };

    enum componentStatus
    {
        NONE,
        MEMBER_MOVE,
        MEMBER_ROTATE_START,
        MEMBER_ROTATE_END,
        MEMBER_CHANGE_START,
        MEMBER_CHANGE_END
    };
    
    struct component
    {
        std::variant<MemberInterface, 
                     SupportInterface> d;
        componentStatus status;
        componentType ct;
        unsigned int id;
    };
    
    inline component convert(const MemberInterface& member)
    {
        return {
            member,
            NONE,
            cmp::MEMBER,
            (unsigned)mh_random(0, 2147483630)
        }; 
    }

    inline MemberInterface* member(component& comp)
    {
        void* ptr = std::get_if<MemberInterface>(&comp.d);
        if(ptr == nullptr)
        {
            throw std::runtime_error("Component currently does not hold member value.");
        }
        return (MemberInterface*)ptr;
    }
}

#endif
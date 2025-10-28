#include <catch_amalgamated.hpp>
#include "action.hpp"

TEST_CASE("Action: Click Action")
{
    // Sanity check
    bool input_1 = false;
    bool input_2 = false;
    auto type_1 = ActionType::MEMBER_CREATE;
    auto type_2 = ActionType::MEMBER_CREATE;
    REQUIRE(requestAction.click(input_1, type_1, 1) == false);
    REQUIRE(requestAction.click(input_2, type_2, 2) == false);
    
    // Check if one is on, and two is off
    input_1 = true;
    input_2 = false;
    type_1 = ActionType::MEMBER_CREATE;
    type_2 = ActionType::MEMBER_CREATE;
    REQUIRE(requestAction.click(input_1, type_1, 1) == true);
    REQUIRE(requestAction.click(input_1, type_1, 1) == false);
    REQUIRE(requestAction.click(input_2, type_2, 2) == false);
    
    // Check if one is off, and two is on
    input_1 = false;
    input_2 = true;
    type_1 = ActionType::MEMBER_CREATE;
    type_2 = ActionType::MEMBER_CREATE;
    REQUIRE(requestAction.click(input_1, type_1, 1) == false);
    REQUIRE(requestAction.click(input_2, type_2, 2) == true);
    REQUIRE(requestAction.click(input_2, type_2, 2) == false);
    
    // Check if one is on, while two has already been on
    input_1 = true;
    type_1 = ActionType::MEMBER_DELETE;
    REQUIRE(requestAction.click(input_1, type_1, 1) == false);

    // Check if two with a different state get turned on
    type_2 = ActionType::MEMBER_DELETE;
    REQUIRE(requestAction.click(input_2, type_2, 2) == false);

    // reset
    requestAction.hold(false, ActionType::MEMBER_CREATE, 1);
    requestAction.hold(false, ActionType::MEMBER_DELETE, 1);
    requestAction.hold(false, ActionType::MEMBER_CREATE, 2);
    requestAction.hold(false, ActionType::MEMBER_DELETE, 2);
}

TEST_CASE("Action: Hold Action")
{
    // Sanity Check
    bool input_1 = false;
    bool input_2 = false;
    auto type_1 = ActionType::MEMBER_MOVE;
    auto type_2 = ActionType::MEMBER_MOVE;
    REQUIRE(requestAction.hold(input_1, type_1, 1) == false);
    REQUIRE(requestAction.hold(input_2, type_2, 2) == false);

    // Check if one is on, and two is off
    input_1 = true;
    input_2 = false;
    type_1 = ActionType::MEMBER_MOVE;
    type_2 = ActionType::MEMBER_MOVE;
    REQUIRE(requestAction.hold(input_1, type_1, 1) == true);
    REQUIRE(requestAction.hold(input_1, type_1, 1) == true);
    REQUIRE(requestAction.hold(input_2, type_2, 2) == false);
    REQUIRE(requestAction.hold(input_2, type_2, 2) == false);

    // Check if one is on, and two is off
    input_1 = false;
    input_2 = true;
    type_1 = ActionType::MEMBER_MOVE;
    type_2 = ActionType::MEMBER_MOVE;
    REQUIRE(requestAction.hold(input_1, type_1, 1) == false);
    REQUIRE(requestAction.hold(input_1, type_1, 1) == false);
    REQUIRE(requestAction.hold(input_2, type_2, 2) == true);
    REQUIRE(requestAction.hold(input_2, type_2, 2) == true);
    
    // Check if one is on, while two has already been on
    input_1 = true;
    REQUIRE(requestAction.hold(input_1, type_1, 1) == false);
    
    // Check if two with a different state get turned on
    type_2 = ActionType::MEMBER_MOVE_ON_START;
    REQUIRE(requestAction.hold(input_2, type_2, 2) == false);

    // reset
    requestAction.hold(false, ActionType::MEMBER_MOVE, 1);
    requestAction.hold(false, ActionType::MEMBER_MOVE_ON_START, 1);
    requestAction.hold(false, ActionType::MEMBER_MOVE, 2);
    requestAction.hold(false, ActionType::MEMBER_MOVE_ON_START, 2);
}

TEST_CASE("Action: Hold Action with condition")
{
    bool input_1 = false;
    bool input_2 = false;
    bool condition_1 = false;
    bool condition_2 = false;
    auto type_1 = ActionType::MEMBER_MOVE;
    auto type_2 = ActionType::MEMBER_MOVE;
    REQUIRE(requestAction.hold(input_1, type_1, 1, condition_1) == false);
    REQUIRE(requestAction.hold(input_2, type_2, 2, condition_2) == false);

    // check one and its condition gets turned on
    input_1 = true;
    input_2 = false;
    condition_1 = true;
    condition_2 = false;
    REQUIRE(requestAction.hold(input_1, type_1, 1, condition_1) == true);
    REQUIRE(requestAction.hold(input_1, type_1, 1, condition_1) == true);
    
    // check only one's condition off
    condition_1 = false;
    REQUIRE(requestAction.hold(input_1, type_1, 1, condition_1) == true);
    REQUIRE(requestAction.hold(input_2, type_2, 2, condition_2) == false);
    REQUIRE(requestAction.hold(input_2, type_2, 2, condition_2) == false);

    // check one is off, and two is on but its condition is false 
    input_1 = false;
    input_2 = true;
    REQUIRE(requestAction.hold(input_1, type_1, 1, condition_1) == false);
    REQUIRE(requestAction.hold(input_1, type_1, 1, condition_1) == false);
    REQUIRE(requestAction.hold(input_2, type_2, 2, condition_2) == false);

    // check only two's condition is on 
    condition_2 = true;
    REQUIRE(requestAction.hold(input_2, type_2, 2, condition_2) == true);
    REQUIRE(requestAction.hold(input_2, type_2, 2, condition_2) == true);

    // check if two's with a different state is on
    type_2 = ActionType::MEMBER_MOVE_ON_START;
    REQUIRE(requestAction.hold(input_2, type_2, 2, condition_2) == false);
    REQUIRE(requestAction.hold(input_2, type_2, 2, condition_2) == false);

    // reset
    requestAction.hold(false, ActionType::MEMBER_MOVE, 1);
    requestAction.hold(false, ActionType::MEMBER_MOVE_ON_START, 1);
    requestAction.hold(false, ActionType::MEMBER_MOVE, 2);
    requestAction.hold(false, ActionType::MEMBER_MOVE_ON_START, 2);
}

TEST_CASE("Action: Hold Action (checking action state)")
{
    bool input_1 = false;
    auto type_1 = ActionType::MEMBER_MOVE;
    REQUIRE(requestAction.hold(true, ActionType::NONE, -1) == true);
    REQUIRE(requestAction.hold(false, ActionType::NONE, -1) == false);
    REQUIRE(requestAction.hold(input_1, type_1, 1) == false);
    
    input_1 = true;
    REQUIRE(requestAction.hold(true, ActionType::NONE, -1) == true);
    REQUIRE(requestAction.hold(false, ActionType::NONE, -1) == false);
    REQUIRE(requestAction.hold(input_1, type_1, 1) == true);

    REQUIRE(requestAction.hold(true, ActionType::NONE, -1) == false);
    REQUIRE(requestAction.hold(false, ActionType::NONE, -1) == false);   
    
    // reset
    requestAction.hold(false, ActionType::NONE, -1);
    requestAction.hold(false, ActionType::MEMBER_MOVE, 1);
}
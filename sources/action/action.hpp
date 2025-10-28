#ifndef __ACTION__
#define __ACTION__

namespace ActionType
{
    enum ActionType
    {
        NONE,

        // Member
        MEMBER_CREATE,
        MEMBER_DELETE,
        MEMBER_MOVE,
        MEMBER_ROTATE_ON_START,
        MEMBER_ROTATE_ON_END,
        MEMBER_MOVE_ON_START,
        MEMBER_MOVE_ON_END,

        // Support

        //Force
    };
}

class RequestAction
{

    private:
        int inActionID; // -1 representing no components (member, support, force, etc..) that are in action
        ActionType::ActionType actionType;

    public:
        RequestAction() : inActionID(-1), actionType(ActionType::NONE) {}

        bool is_in_action(int ID, ActionType::ActionType actionType)
        {
            return ((ID == this->inActionID) && actionType == this->actionType);
        }

        bool click(bool input, ActionType::ActionType actionType, int ID)
        {
            if(input && this->is_in_action(-1, ActionType::NONE))
            {   
                this->inActionID = ID;
                this->actionType = actionType;
                return true;
            }
            if(!input && this->is_in_action(ID, actionType))
            {
                this->inActionID = -1;
                this->actionType = ActionType::NONE;
            }
            return false;
        }

        bool hold(bool input, ActionType::ActionType actionType, int ID, bool condition=true)
        {
            if(!input && this->is_in_action(ID, actionType))
            {
                this->inActionID = -1;
                this->actionType = ActionType::NONE;
                return false;
            }
            // used to block a request if any other components have already been in action,
            // except for those that already in action
            if(!this->is_in_action(-1, ActionType::NONE) && !this->is_in_action(ID, actionType))
            {
                return false;
            }
            if((input && condition) || this->is_in_action(ID, actionType))
            {
                this->inActionID = ID;
                this->actionType = actionType;
                return true;
            }
            return false;
        }
};

inline RequestAction requestAction = RequestAction();

#endif
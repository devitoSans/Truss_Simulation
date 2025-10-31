#ifndef __JOINTS_LIST__
#define __JOINTS_LIST__

#include <map>
#include <utility>
#include <vector>
#include <set>
#include <splashkit.h>
#include "../action/action.hpp"
#include "../mathHelpers/mathHelpers.hpp"
#include "../component/componentControl.hpp"
#include "../component/member/memberController.hpp"
#include "../component/supports/supportController.hpp"
#include "../component/force/forceController.hpp"

class ConnectionManager
{
    private:
        // key: joint's id
        std::map<int, std::set<Connection>> joints;
        std::map<Connection,int> r_joints; // as a lookup table to easy to search which joint a component's part is connected to

        bool isReady;

        int get_focused_component_index(ComponentController** components, int numComponentType, int focusedID)
        {
            ComponentType::ComponentType type = ComponentType::NONE;
            int index = -1;
            for(int i = 0; i < numComponentType; i++)
            {
                if(type == ComponentType::NONE)
                {
                    type = components[i]->get_type(focusedID);
                    index = i;
                }
            }
            if(type == ComponentType::NONE) { index = -1; }
            return index;
        }

        bool is_connected(const Connection& connection) const
        {
            if(this->r_joints.find(connection) == this->r_joints.end())
            {
                return false;
            }
            return true;
        }

        void create_joint(Connection& c1, Connection& c2)
        {
            if(this->is_connected(c1) || this->is_connected(c2))
            {
                return;
            }
            int jointId = mh_random(0,MAX_ID);

            this->joints.emplace(jointId, std::set({c1, c2}));
            this->r_joints.emplace(c1, jointId);
            this->r_joints.emplace(c2, jointId);
        }

        void transfer_connection(Connection& from, Connection& to)
        {
            if(!this->is_connected(to))
            {
                fprintf(stderr, "Warning: Attempting to transfer to non-existing joint.");
                return;
            }
            this->clear_connection(from);

            int toJoint = this->r_joints.at(to);

            this->r_joints[from] = toJoint;
            this->joints.at(toJoint).insert(from);
        }

        void clear_connection(Connection& connection)
        {
            if(!this->is_connected(connection))
            {
                return;
            }
            int atJoint = this->r_joints.at(connection);

            this->r_joints.erase(connection);
            auto& temp = this->joints.at(atJoint);
            temp.erase(connection);

            if(temp.size() <= 1)
            {
                for(auto& i : temp)
                {
                    this->r_joints.erase(i);
                }
                this->joints.erase(atJoint);
            }
        }

        std::vector<Part::PartType> get_part(int id)
        {
            std::vector<Part::PartType> parts;
            for(auto& [connection, _] : this->r_joints)
            {
                if(connection.first == id)
                {
                    parts.push_back(connection.second);
                }
            }
            return parts;
        }

        void change_connection(Connection& connection1, Connection& connection2) 
        {
            if(this->is_connected(connection1))
            {
                this->transfer_connection(connection2, connection1);
            }
            else if(!this->is_connected(connection1) && !this->is_connected(connection2))
            {
                this->create_joint(connection1, connection2);
            }
            else
            {
                this->transfer_connection(connection1, connection2);
            }
        }

    public:
        ConnectionManager() : isReady(true) {}

        std::map<int, std::set<Connection>>& get_joints()
        {
            return this->joints;
        }

        bool update(ComponentController** components, int numComponentType, int focusedID)
        // no action is taken.
        // there must be a component in focused/selected.
        // only done once.
        // otherwise, cannot update components' connection
        {
            if(!(requestAction.is_in_action(-1, ActionType::NONE) && focusedID != -1))  
            { 
                this->isReady = true;
                return false;
            }
            if(!this->isReady) { return false; }
            this->isReady = false;

            int focusedIndex = this->get_focused_component_index(components, numComponentType, focusedID);
            if(focusedIndex == -1)
            {
                for(auto& i : this->get_part(focusedID))
                {
                    Connection temp = {focusedID, i};
                    this->clear_connection(temp);
                }
            }
            else 
            {
                std::vector<Hitbox> focusedComponentHitboxes = components[focusedIndex]->get_hit_box(focusedID);
    
                for(auto& hitbox : focusedComponentHitboxes)
                {
                    Connection focusedConnection = Connection {focusedID, hitbox.type};
                    this->clear_connection(focusedConnection);
                    for(int i = 0; i < numComponentType; i++)
                    {
                        std::vector<Connection> intersection = components[i]->get_intersection(hitbox.shape.center.x, 
                                                            hitbox.shape.center.y, 
                                                            hitbox.shape.radius);
                        
                        for(auto& connection : intersection)
                        {
                            // printf("1. connection id: %d, focused id: %d\n", connection.first, focusedID);
                            if(connection.first == focusedID)
                            {
                                continue;
                            }
                            // printf("2. connection id: %d, focused id: %d\n", connection.first, focusedID);
    
                            this->change_connection(focusedConnection, connection);
                        }
                    }
                }
            }

            // for(auto& j : this->joints)
            // {
            //     printf("joint: %d -> ", j.first);
            //     for(auto& c : j.second)
            //     {
            //         printf("%d, ", c.first);
            //     }
            //     printf("\n");
            // }
            // printf("\n\n");
            return true;
        }
};

#endif
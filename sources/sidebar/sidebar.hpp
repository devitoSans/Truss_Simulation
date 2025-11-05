#ifndef __SIDE_BAR__
#define __SIDE_BAR__

#include <splashkit.h>
#include <map>
#include <functional>
#include "../fileManager/fileManager.hpp"
#include "../action/action.hpp"
#include "../mathHelpers/mathHelpers.hpp"

#define DRAGGABLE_INPUT mouse_position()

const double MIN_WIDTH_SIDEBAR = 50.0;

template<typename T>
class UIManager
{
    protected:
        std::map<std::string, T> cache;
        std::map<std::string, bool> stillUsed;

    public:
        void clear()
        {
            std::vector<std::string> temp;
            for(auto& [label, isUsed] : this->stillUsed)
            {
                if(!isUsed)
                {
                    this->cache.erase(label);
                    temp.emplace_back(label);
                }
            }
            for(auto& label : temp)
            {
                this->stillUsed.erase(label);
            }
        }
};

class CheckBoxManager : public UIManager<bool>
{
    public:
        bool toggle(const std::string& label="", const bool& firstValue=false)
        {
            if(this->cache.find(label) == this->cache.end())
            {
                this->cache[label] = firstValue;
            }
            if(button(label, (this->cache.at(label) ? "Yes" : "No")))
            {
                this->cache[label] = !this->cache[label];
            }
            return this->cache[label];
        }
};
inline CheckBoxManager checkbox_works = CheckBoxManager();

template<typename T>
class UIControlManager : public UIManager<T>
{
    typedef std::function<T(const std::string& label, const T& current)> UpdateFn;

    private:
        std::map<std::string, T> cache;
        std::map<std::string, bool> stillUsed;
        UpdateFn update_fn_;
    
    public:
        UIControlManager(UpdateFn update_fn) : cache(), update_fn_(update_fn) {}

        const T& get(const std::string& label="", const T& value="", bool setValue=false)
        {
            if(this->cache.find(label) == this->cache.end())
            {
                this->cache[label] = value;
            }
            this->stillUsed[label] = true;

            const T& temp = (setValue ? value : this->cache.at(label));
            return this->cache[label] = this->update_fn_(label, temp);
        }
};
inline UIControlManager<std::string> textboxManager = UIControlManager<std::string>(
    [](auto& label, auto& current) -> std::string {
        return text_box(label, current, true);
    });
inline UIControlManager<bool> checkboxManager = UIControlManager<bool>(
    [](auto& label, auto& current) -> bool {
        return checkbox_works.toggle(label, current);
    }
);

class Page
{
    protected:
        std::string resourcesPath;

    public:
        Page() : resourcesPath("") {}

        void set_resources_path(std::string path)
        {
            this->resourcesPath = path;
        }

        // show button component that is correspond to the page
        // if clicked, then the page must be shown
        virtual Page* show_button() = 0;
        virtual void draw() = 0;
};

class HelpPage : public Page
{
    public:
        Page* show_button() override
        {
            if(button("Help"))
            {
                return (Page*)this;
            }
            return nullptr;
        }

        void draw() override
        {
            label_element("User guidelines:");
            label_element("   ");
            std::string info = read_file_lines(this->resourcesPath + "guidelines/help.txt");
            paragraph(info);
        }
};

class FilePage : public Page
{
    private:
        std::string importName;
        std::string exportName;

    public:
        FilePage() : importName(""), exportName("") {}

        Page* show_button() override
        {
            if(button("File"))
            {
                return (Page*)this;
            }
            return nullptr;
        }

        void draw() override
        {
            label_element("");

            std::string temp1 = textboxManager.get("Import");
            if(button("DONE"))
            {
                this->importName = temp1;
                printf("OK! imported");
                return;
            }
        
            std::string temp2 = textboxManager.get("Export");
            if(button("DONE."))
            {
                this->exportName = temp2;
                printf("OK! exported");
            }
        }
};

class StrengthCheckPage : public Page
{
    private:
        bool doCheck;

    public:
        StrengthCheckPage() : doCheck(false) {}

        Page* show_button() override
        {
            if(button("Strength Check"))
            {
                return (Page*)this;
            }
            return nullptr;
        }

        void draw() override
        {
            label_element("");
            if(button((this->doCheck ? "Yes" : "No"), "Check strength?"))
            {
                doCheck = !doCheck;
            }
            if(doCheck) 
            {
                label_element("Done!");
            }
            label_element("");
            label_element("Green: members pass strength check");
            label_element("Red: means fails on yeilding");
            label_element("Blue: means fails on buckling");
            label_element("Purple: means fails on both yeilding & buckling");
        }

        bool get_do_check()
        {
            return this->doCheck;
        }

        void set_do_check(bool doCheck)
        {
            this->doCheck = doCheck;
        }
};
inline StrengthCheckPage* strengthPage = new StrengthCheckPage();

class InfoContent
{
    protected:
        bool draw_properties(const std::string& name, double value, double& newValue, int buttonNo) const
        {
            const std::string& label =  name+"    ";
            bool condition = requestAction.is_in_action(ActionType::SIDEBAR, ActionType::SIDEBAR);

            std::string string_val = textboxManager.get(label, format_double(value, 3), !condition);
            std::string idButton(buttonNo, ' ');
            set_layout_height(0);
            if(button(std::string("OK").append(idButton)))
            {
                if(is_double(string_val))
                {
                    newValue = std::stod(string_val);
                    return true;
                }
            }
            return false;
        }

        void show_properties(const std::string& name, bool value, bool& newValue) const
        {
            if(button((value ? "Yes" : "No"), name))
            {
                newValue = !newValue;
            }
        }

    public:
        virtual void draw() = 0;
};

class InfoPage : public Page
{
    private:
        InfoContent* infoContent;

    public:
        InfoPage() : infoContent(nullptr) {}

        Page* show_button() override
        {
            if(button("Info"))
            {
                return (Page*)this;
            }
            return nullptr;
        }
    
        void draw() override
        {
            if(infoContent == nullptr)
            {
                label_element("");
                label_element("No component is selected...");
            }
            else
            {
                infoContent->draw();
            }
        }

        const InfoContent* get_content() const
        {
            return this->infoContent;
        }

        void set_content(InfoContent* content)
        {
            this->infoContent = content;
        }
};
inline InfoPage* ingfokan = new InfoPage();

inline std::vector<Page*> pages = {
    new HelpPage(),
    new FilePage(),
    strengthPage,
    ingfokan
};

class Sidebar
{
    private:
        // Big three rules applied here. Check MemberData class
        double width;
        int idPanel;

        int screenWidth;
        int screenHeight;

        rectangle draggablePart;

        Page* showedPage;

        std::string resourcesPath;

        void update_draggable_part()
        {
            this->draggablePart = { this->width, 0.0, 10, (double)this->screenHeight };
        }

        void drag()
        {
            if(requestAction.hold(mouse_down(LEFT_BUTTON), ActionType::DRAG_SIDEBAR, ActionType::DRAG_SIDEBAR, point_in_rectangle(DRAGGABLE_INPUT, this->draggablePart)))
            {
                this->set_width(mouse_position().x-this->draggablePart.width/2.0);
            }
        }

        void show_navigator()
        {
            for(auto& i : pages)
            {
                auto temp = i->show_button();
                if(temp != nullptr)
                {
                    this->showedPage = temp;
                }
            }
        }



    public:
        Sidebar(int width, int height, std::string resourcesPath) 
            : screenWidth(width), screenHeight(height), resourcesPath(resourcesPath)
        {
            this->idPanel = 0;
            this->width = this->screenWidth / 4.0;
            this->update_draggable_part();
            this->showedPage = nullptr;

            for(auto& i : pages)
            {
                i->set_resources_path(this->resourcesPath);
            }
        }

        void set_width(double width)
        {
            if(width < MIN_WIDTH_SIDEBAR) return;
            if(width > this->screenWidth - 50) return;
            this->width = width;
            this->idPanel++;
        }

        void update()
        {
            // Sidebar in action
            if(point_in_rectangle(mouse_position(), rectangle_from(0.0, 0.0, this->width, this->screenHeight)))
            { 
                requestAction.click(true, ActionType::SIDEBAR, ActionType::SIDEBAR); 
            }
            else 
            { 
                requestAction.click(false, ActionType::SIDEBAR, ActionType::SIDEBAR); 
            }

            this->drag();
        }

        void draw()
        {
            set_interface_style(BUBBLE);
            if(start_panel(std::to_string(this->idPanel), rectangle_from(0, -20, this->width, this->screenHeight+20)))
            {
                this->show_navigator();
                if(showedPage != nullptr)
                {
                    showedPage->draw();
                }
                end_panel(std::to_string(this->idPanel));
            }
            this->update_draggable_part();
            fill_rectangle(COLOR_GAINSBORO, this->draggablePart);
            draw_rectangle(COLOR_BLACK, this->draggablePart);
        }
};

#endif
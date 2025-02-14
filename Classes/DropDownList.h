// DropDownList.h
#ifndef __DROP_DOWN_LIST_H__
#define __DROP_DOWN_LIST_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;


class DropDownList : public Widget {
public:
    static DropDownList* create(const std::string& defaultText, const std::vector<std::string>& options);

    void setDefaultText(const std::string& text);
    std::string getSelectedOption() const;
    void setSelectedIndex(int index);

    void toggleOptions();

protected:
    bool init(const std::string& defaultText, const std::vector<std::string>& options);

private:
    void onArrowClicked(Ref* sender);
    void onOptionClicked(Ref* sender);

    Label* _label;
    Sprite* _arrow;
    Menu* _menu;
    std::vector<MenuItemLabel*> _optionItems;
    int _selectedIndex;
};

#endif // __DROP_DOWN_LIST_H__
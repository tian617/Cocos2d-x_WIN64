// DropDownList.h
#ifndef __DROP_DOWN_LIST_H__
#define __DROP_DOWN_LIST_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;


class MenuItemLabelWithBg : public MenuItem
{
public:

    /** Creates a MenuItemLabelWithBg with a Label and a callback. */
    static MenuItemLabelWithBg* create(Node* label, Node* sprite, const ccMenuCallback& callback);

    /** Creates a MenuItemLabelWithBg with a Label. Target and selector will be nil. */
    static MenuItemLabelWithBg* create(Node* label);

    /** Sets a new string to the inner label. */
    void setString(const std::string& label);

    /** Get the inner string of the inner label. */
    std::string getString() const;

    /** Gets the color that will be used when the item is disabled. */
    const Color3B& getDisabledColor() const { return _disabledColor; }

    /** Sets the color that will be used when the item is disabled. */
    void setDisabledColor(const Color3B& color) { _disabledColor = color; }

    /** Gets the label that is rendered. */
    Node* getLabel() const { return _label; }

    /** Sets the label that is rendered. */
    void setLabel(Node* node);

    // Overrides
    virtual void activate() override;
    virtual void selected() override;
    virtual void unselected() override;
    virtual void setEnabled(bool enabled) override;

CC_CONSTRUCTOR_ACCESS:
    /**
     * @js ctor
     */
    MenuItemLabelWithBg()
        : _originalScale(0.0)
        , _label(nullptr)
    {}
    /**
     * @js NA
     * @lua NA
     */
    virtual ~MenuItemLabelWithBg();

    /** Initializes a MenuItemLabelWithBg with a Label, target and selector. */
    bool initWithLabel(Node* label, Node* sprite, const ccMenuCallback& callback);

protected:
    Color3B    _colorBackup;
    float      _originalScale;

    /** The color that will be used to disable the item. */
    Color3B _disabledColor;
    /** Label that is rendered. It can be any Node that implements the LabelProtocol. */
    Node* _label;
    Node* _spriteBg;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MenuItemLabelWithBg);
};

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
    std::vector<MenuItemLabelWithBg*> _optionItems;
    int _selectedIndex;
};

#endif // __DROP_DOWN_LIST_H__
// DropDownList.cpp
#include "DropDownList.h"

DropDownList* DropDownList::create(const std::string& defaultText, const std::vector<std::string>& options) {
    DropDownList* pRet = new (std::nothrow) DropDownList();
    if (pRet && pRet->init(defaultText, options)) {
        pRet->autorelease();
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

bool DropDownList::init(const std::string& defaultText, const std::vector<std::string>& options) {
    if (!Widget::init()) {
        return false;
    }

    _selectedIndex = 0;

    float width = 120;
    float height = 36;

    this->setContentSize(Size(width, height));
    LayerColor* _bgColor = LayerColor::create(Color4B(125, 125, 125, 255));
    _bgColor->setPosition(0, 0);
    _bgColor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _bgColor->setContentSize(Size(width, height));
    this->addChild(_bgColor, -1);

    // 创建显示区域
    _label = Label::createWithSystemFont(defaultText, "Arial", 16);
    _label->setPosition(Vec2(0, height / 2));
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _label->setWidth(width*2/3);
    this->addChild(_label);

    // 创建下拉箭头
    _arrow = Sprite::create("arrow.png"); // 需要准备一个箭头的图片资源
    _arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _arrow->setPosition(Vec2(width * 2 / 3, height / 2));
    _arrow->setContentSize(Size(width * 1 / 3, height));
    this->addChild(_arrow);

    // 创建选项菜单
    _menu = Menu::create();
    _menu->setPosition(Vec2(0, height/2));
    _menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    
    this->addChild(_menu);

    for (size_t i = 0; i < options.size(); ++i) {
        auto label = Label::createWithSystemFont(options[i], "Arial", 12);
        auto menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(DropDownList::onOptionClicked, this));
        menuItem->setTag(i);
        _optionItems.push_back(menuItem);


		LayerColor* _bgColor = LayerColor::create(Color4B(200, 200, 200, 66));
		_bgColor->setPosition(0, 0);
		_bgColor->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_bgColor->setContentSize(Size(width, height - 2));
		menuItem->addChild(_bgColor, -1);

        menuItem->setContentSize(Size(width, height));
        menuItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        menuItem->setPosition(Vec2(width/2, height * (i + 1)));

        _menu->addChild(menuItem);

        // 初始隐藏所有选项
        menuItem->setVisible(false);
    }

    // 添加箭头点击事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        Rect bound = _arrow->getBoundingBox();
        Vec2 posLocal = this->convertTouchToNodeSpace(touch);
        Vec2 pos = touch->getLocation();
        if (_arrow->getBoundingBox().containsPoint(posLocal)) {
            this->toggleOptions();
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void DropDownList::setDefaultText(const std::string& text) {
    _label->setString(text);
}

std::string DropDownList::getSelectedOption() const {
    if (_selectedIndex >= 0 && _selectedIndex < _optionItems.size()) {
        return _optionItems[_selectedIndex]->getString();
    }
    return "";
}

void DropDownList::setSelectedIndex(int index) {
    if (index >= 0 && index < _optionItems.size()) {
        _selectedIndex = index;
        _label->setString(_optionItems[index]->getString());
        toggleOptions();
    }
}

void DropDownList::onOptionClicked(Ref* sender) {
    auto menuItem = dynamic_cast<MenuItemLabel*>(sender);
    if (menuItem) {
        _selectedIndex = menuItem->getTag();
        _label->setString(menuItem->getString());
        toggleOptions();
    }
}

void DropDownList::toggleOptions() {
    bool isVisible = false;
    for (auto& item : _optionItems) {
        isVisible = item->isVisible();
        item->setVisible(!isVisible);
    }
}
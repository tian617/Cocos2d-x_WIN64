// DropDownList.cpp
#include "DropDownList.h"
#include <2d/CCMenuItem.cpp>

//
//CCMenuItemLabel
//

void MenuItemLabelWithBg::setLabel(Node* var)
{
	if (var)
	{
		var->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        Size spBgSize = _spriteBg->getContentSize();
        var->setPosition(Vec2(0, spBgSize.height / 2));
		setContentSize(spBgSize);
		addChild(var, 1);
	}

	if (_label)
	{
		removeChild(_label, true);
	}

	_label = var;
}

MenuItemLabelWithBg* MenuItemLabelWithBg::create(Node* label, Node* sprite, const ccMenuCallback& callback)
{
	MenuItemLabelWithBg* ret = new (std::nothrow) MenuItemLabelWithBg();
	ret->initWithLabel(label, sprite, callback);
	ret->autorelease();
	return ret;
}

MenuItemLabelWithBg* MenuItemLabelWithBg::create(Node* label)
{
	MenuItemLabelWithBg* ret = new (std::nothrow) MenuItemLabelWithBg();
	ret->initWithLabel(label, nullptr, (const ccMenuCallback&) nullptr);
	ret->autorelease();
	return ret;
}


bool MenuItemLabelWithBg::initWithLabel(Node* label, Node* sprite, const ccMenuCallback& callback)
{
	MenuItem::initWithCallback(callback);
	_originalScale = 1.0f;
	_colorBackup = Color3B::WHITE;
	setDisabledColor(Color3B(126, 126, 126));
	_spriteBg = sprite;
	this->setLabel(label);

    label->setContentSize(_spriteBg->getContentSize());
    _spriteBg->setPosition(Vec2::ZERO);
    _spriteBg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_spriteBg);  // add bg

	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);

	return true;
}


MenuItemLabelWithBg::~MenuItemLabelWithBg()
{
}

void MenuItemLabelWithBg::setString(const std::string& label)
{
	dynamic_cast<LabelProtocol*>(_label)->setString(label);

    this->setContentSize(_spriteBg->getContentSize());
	//this->setContentSize(_label->getContentSize());
}

std::string MenuItemLabelWithBg::getString() const
{
	auto label = dynamic_cast<LabelProtocol*>(_label);
	return label->getString();
}

void MenuItemLabelWithBg::activate()
{
	if (_enabled)
	{
		this->stopAllActions();
		this->setScale(_originalScale);
		MenuItem::activate();
	}
}

void MenuItemLabelWithBg::selected()
{
	// subclass to change the default action
	if (_enabled)
	{
		MenuItem::selected();

		Action* action = getActionByTag(kZoomActionTag);
		if (action)
		{
			this->stopAction(action);
		}
		else
		{
			_originalScale = this->getScale();
		}

		Action* zoomAction = ScaleTo::create(0.1f, _originalScale * 1.05f);
		zoomAction->setTag(kZoomActionTag);
		this->runAction(zoomAction);
	}
}

void MenuItemLabelWithBg::unselected()
{
	// subclass to change the default action
	if (_enabled)
	{
		MenuItem::unselected();
		this->stopActionByTag(kZoomActionTag);
		Action* zoomAction = ScaleTo::create(0.1f, _originalScale);
		zoomAction->setTag(kZoomActionTag);
		this->runAction(zoomAction);
	}
}

void MenuItemLabelWithBg::setEnabled(bool enabled)
{
	if (_enabled != enabled)
	{
		if (enabled == false)
		{
			_colorBackup = this->getColor();
			this->setColor(_disabledColor);
		}
		else
		{
			this->setColor(_colorBackup);
		}
	}
	MenuItem::setEnabled(enabled);
}


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
        Sprite* bgSpr = Sprite::create();
        bgSpr->initWithFile("btn_normal.png");
        bgSpr->setContentSize(Size(width, height));
        auto menuItem = MenuItemLabelWithBg::create(label, bgSpr, CC_CALLBACK_1(DropDownList::onOptionClicked, this));
        menuItem->setTag(i);
        _optionItems.push_back(menuItem);

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
    auto menuItem = dynamic_cast<MenuItemLabelWithBg*>(sender);
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



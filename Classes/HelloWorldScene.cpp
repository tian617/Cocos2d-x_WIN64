/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "ui/UIScrollView.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}


void HelloWorld::Log(std::string log)
{
	mLog.append(log+"\n");
	mText->setString(mLog);

	Vec2 size = mScrollView->getInnerContainerSize();

	Size textSize = mText->getContentSize();

	mScrollView->setInnerContainerSize(textSize);
	mScrollView->scrollToBottom(0, false);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	logCount = 0;
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	auto layer = Layer::create();
	this->addChild(layer);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto mainLayout = Layout::create();
	mainLayout->setLayoutType(Layout::Type::HORIZONTAL);
	mainLayout->setContentSize(visibleSize);
	mainLayout->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	mainLayout->setAnchorPoint(Vec2(0.5, 0.5));
	mainLayout->setSizeType(SizeType::ABSOLUTE);

	layer->addChild(mainLayout);

	// 左边是一ScrollView

	ScrollView* scrollView = ScrollView::create();
	scrollView->setDirection(ScrollView::Direction::VERTICAL); // 设置滚动方向为垂直
	scrollView->setSizeType(SizeType::PERCENT);
	scrollView->setSizePercent(Vec2(0.5, 1)); // 设置 ScrollView setSizePercent 根据父节点大小调整
	scrollView->setBounceEnabled(true); // 允许回弹效果

	mainLayout->addChild(scrollView);
	mScrollView = scrollView;


	// 创建 Text 并添加到布局中
	mText = Label::createWithSystemFont("", "Arial", 18);
	mText->setHorizontalAlignment(TextHAlignment::LEFT);
	mText->setVerticalAlignment(TextVAlignment::TOP);
	mText->setLineBreakWithoutSpace(true);
	mText->setAnchorPoint(Vec2(0, 0));
	float maxWidth = visibleSize.width / 2 - 100;
	mText->setWidth(maxWidth);
	mScrollView->addChild(mText);



	// 创建一个垂直布局容器，宽度为屏幕宽度，高度为屏幕高度
	auto layout = Layout::create();
	layout->setSizeType(SizeType::PERCENT);
	layout->setLayoutType(Layout::Type::VERTICAL);
	layout->setSizePercent(Vec2(0.5, 1));
	layout->setAnchorPoint(Vec2(0.0, 0.0));

	// 添加功能按钮
	auto layoutParam1 = ui::LinearLayoutParameter::create();
	layoutParam1->setMargin(ui::Margin(6, 6, 6, 6));
	std::vector<std::string> buttonLabels = { "button1", "button2", "button3", "button4" };
	for (const auto& label : buttonLabels)
	{
		auto btn = Button::create("btn_normal.png", "btn_pressed.png", "btn_disabled.png");

		btn->setAnchorPoint(Vec2(0.5f, 0.5f));
		btn->ignoreContentAdaptWithSize(false);
		btn->setContentSize(Size(120, 66));
		btn->setLayoutParameter(layoutParam1);
		btn->setTitleText(label);
		btn->setTitleFontSize(24);

		btn->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED) {
				CCLOG("btn is click");
				this->logCount++;


				std::ostringstream oss;
				oss << "btn is clickaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aaaaassssssssssssssssssssssssaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaaaaaa::" << this->logCount;
				this->Log(oss.str());
				// 处理按钮点击事件
			}
			});
		layout->addChild(btn);
	}

	// Dropdown
	// 创建下拉列表选项
	std::vector<std::string> options = { "Option 1", "Option 2"};
	mDropdown = DropDownList::create("Select One", options);
	mDropdown->ignoreContentAdaptWithSize(false);
	mDropdown->setLayoutParameter(layoutParam1);
	layout->addChild(mDropdown);

	// input
	ui::EditBox* inputBox = createEditBox();
	inputBox->setLayoutParameter(layoutParam1);
	layout->addChild(inputBox);

	layout->forceDoLayout();
	mainLayout->addChild(layout);
	mainLayout->forceDoLayout();


	

    return true;
}


// 创建输入框
ui::EditBox* HelloWorld::createEditBox() {
	// 输入框尺寸
	Size editBoxSize = Size(200, 40);

	// 创建 EditBox
	ui::EditBox* editBox = ui::EditBox::create(editBoxSize, "btn_disabled.png"); // 背景图
	//editBox->setPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y));
	editBox->setFontSize(16);
	editBox->setFontColor(Color4B::WHITE);
	editBox->setPlaceHolder("input password:"); // 占位符提示
	editBox->setPlaceholderFontColor(Color4B::WHITE);
	editBox->setMaxLength(128); // 最大输入长度

	// 设置输入模式（例如：文本、数字等）
	editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	return editBox;
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

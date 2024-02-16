#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/TextInputDelegate.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

const char* geode::getCommonFilterAllowedChars(CommonFilter filter) {
    switch (filter) {
        default:
        case CommonFilter::Uint:         return "0123456789";
        case CommonFilter::Int:          return "-0123456789";
        case CommonFilter::Float:        return "-.0123456789";
        case CommonFilter::ID:           return "abcdefghijklmnopqrstuvwxyz0123456789-_.";
        case CommonFilter::Name:         return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ ";
        case CommonFilter::Any:          return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-+/\\&$%^~*\'\"{}()[]<>=!?@,;.:|• ";
        case CommonFilter::Hex:          return "0123456789abcdefABCDEF";
        case CommonFilter::Base64Normal: return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/=";
        case CommonFilter::Base64URL:    return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_=";
    }
}

bool TextInput::init(float width, std::string const& placeholder, std::string const& font) {
    if (!CCNode::init())
        return false;

    constexpr float HEIGHT = 30.f;

    this->setContentSize({ width, HEIGHT });
    this->setAnchorPoint({ .5f, .5f });

    m_bgSprite = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    m_bgSprite->setScale(.5f);
    m_bgSprite->setColor({ 0, 0, 0 });
    m_bgSprite->setOpacity(90);
    m_bgSprite->setContentSize({ width * 2, HEIGHT * 2 });
    this->addChildAtPosition(m_bgSprite, cocos2d::Anchor::Center);

    m_input = CCTextInputNode::create(width - 10.f, HEIGHT, placeholder.c_str(), "Thonburi", 24, font.c_str());
    m_input->setLabelPlaceholderColor({ 150, 150, 150 });
    m_input->setLabelPlaceholderScale(.6f);
    m_input->setMaxLabelScale(.6f);
    this->addChildAtPosition(m_input, cocos2d::Anchor::Center);

    handleTouchPriority(this);

    return true;
}

TextInput* TextInput::create(float width, std::string const& placeholder, std::string const& font) {
    auto ret = new TextInput();
    if (ret && ret->init(width, placeholder, font)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void TextInput::textChanged(CCTextInputNode* input) {
    if (m_onInput) {
        m_onInput(input->getString());
    }
}

void TextInput::setPlaceholder(std::string const& placeholder) {
    m_input->m_caption = placeholder;
    m_input->refreshLabel();
}
void TextInput::setFilter(std::string const& allowedChars) {
    m_input->m_allowedChars = allowedChars;
}
void TextInput::setCommonFilter(CommonFilter filter) {
    this->setFilter(getCommonFilterAllowedChars(filter));
}
void TextInput::setMaxCharCount(size_t length) {
    m_input->m_maxLabelLength = length == 0 ? 9999999 : length;
}
void TextInput::setPasswordMode(bool enable) {
    m_input->m_usePasswordChar = enable;
    m_input->refreshLabel();
}
void TextInput::setWidth(float width) {
    m_input->m_maxLabelWidth = width - 10;
    m_input->setContentWidth(width * 2);
    m_bgSprite->setContentWidth(width * 2);
}
void TextInput::setDelegate(TextInputDelegate* delegate, std::optional<int> tag) {
    m_input->setDelegate(delegate);
    m_onInput = nullptr;
    if (tag.has_value()) {
        m_input->setTag(tag.value());
    }
}
void TextInput::setCallback(std::function<void(std::string const&)> onInput) {
    this->setDelegate(this);
    m_onInput = onInput;
}

void TextInput::hideBG() {
    m_bgSprite->setVisible(false);
}

void TextInput::setString(std::string const& str, bool triggerCallback) {
    auto oldDelegate = m_input->m_delegate;
    // Avoid triggering the callback
    m_input->m_delegate = nullptr;
    m_input->setString(str);
    m_input->m_delegate = oldDelegate;
    if (triggerCallback && m_input->m_delegate) {
        m_input->m_delegate->textChanged(m_input);
    }
}

std::string TextInput::getString() const {
    return m_input->getString();
}

CCTextInputNode* TextInput::getInputNode() const {
    return m_input;
}
CCScale9Sprite* TextInput::getBGSprite() const {
    return m_bgSprite;
}

#include <Geode/modify/IDManager.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/NodeIDs.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;
using namespace geode::node_ids;

$register_ids(LoadingLayer) {
    setIDs(
        0, this,
        "bg-texture",
        "gd-logo",
        "robtop-logo",
        "cocos2d-logo",
        "fmod-logo",
        "loading-text",
        "text-area",
        "progress-slider"
    );
}
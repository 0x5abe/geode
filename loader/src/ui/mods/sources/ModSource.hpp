#pragma once

#include <Geode/loader/Mod.hpp>
#include <server/Server.hpp>

using namespace geode::prelude;

class ModSource final {
private:
    std::variant<Mod*, server::ServerModMetadata> m_value;
    std::optional<server::ServerModUpdate> m_availableUpdate;

public:
    ModSource() = default;
    ModSource(Mod* mod);
    ModSource(server::ServerModMetadata&& metadata);

    std::string getID() const;
    ModMetadata getMetadata() const;
    std::optional<std::string> getAbout() const;
    std::optional<std::string> getChangelog() const;
    CCNode* createModLogo() const;
    bool wantsRestart() const;
    // note: be sure to call checkUpdates first...
    std::optional<server::ServerModUpdate> hasUpdates() const;

    auto visit(auto&& func) {
        return std::visit(func, m_value);
    }

    // Returns a new ModSource that is either a copy of the current source or 
    // an installed version of a server mod
    ModSource tryConvertToMod() const;

    Mod* asMod() const;
    server::ServerModMetadata const* asServer() const;

    server::ServerPromise<server::ServerModMetadata> fetchServerInfo() const;
    server::ServerPromise<std::unordered_set<std::string>> fetchValidTags() const;
    server::ServerPromise<std::optional<server::ServerModUpdate>> checkUpdates();
};

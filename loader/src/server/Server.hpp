#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/Promise.hpp>
#include <Geode/utils/web2.hpp>

using namespace geode::prelude;

namespace server {
    struct ServerDateTime final {
        using Clock = std::chrono::system_clock;
        using Value = std::chrono::time_point<Clock>;

        Value value;

        std::string toAgoString() const;

        static Result<ServerDateTime> parse(std::string const& str);
    };

    struct ServerDeveloper {
        std::string username;
        std::string displayName;
    };

    struct ServerModVersion {
        ModMetadata metadata;
        std::string downloadURL;
        std::string hash;
        size_t downloadCount;

        static Result<ServerModVersion> parse(matjson::Value const& json);
    };

    struct ServerModMetadata {
        std::string id;
        bool featured;
        size_t downloadCount;
        std::vector<ServerDeveloper> developers;
        std::vector<ServerModVersion> versions;
        std::unordered_set<std::string> tags;
        std::optional<std::string> about;
        std::optional<std::string> changelog;
        std::optional<ServerDateTime> createdAt;
        std::optional<ServerDateTime> updatedAt;

        static Result<ServerModMetadata> parse(matjson::Value const& json);
    };

    struct ServerModsList {
        std::vector<ServerModMetadata> mods;
        size_t totalModCount = 0;

        static Result<ServerModsList> parse(matjson::Value const& json);
    };

    enum class ModsSort {
        Downloads,
        RecentlyUpdated,
        RecentlyPublished,
    };

    static const char* sortToString(ModsSort sorting);

    struct ModsQuery {
        std::optional<std::string> query;
        std::unordered_set<PlatformID> platforms = { GEODE_PLATFORM_TARGET };
        std::unordered_set<std::string> tags;
        std::optional<bool> featured;
        ModsSort sorting = ModsSort::Downloads;
        std::optional<std::string> developer;
        size_t page = 0;
        size_t pageSize = 10;
    };

    struct ServerError {
        int code;
        std::string details;

        ServerError() = default;

        template <class... Args>
        ServerError(
            int code,
            fmt::string_view format,
            Args&&... args
        ) : code(code), details(fmt::vformat(format, fmt::make_format_args(args...))) {}
    };
    template <class T>
    using ServerPromise = Promise<T, ServerError>;

    std::string getServerAPIBaseURL();
    std::string getServerUserAgent();
    ServerPromise<ServerModsList> getMods(ModsQuery const& query);
    ServerPromise<ServerModMetadata> getMod(std::string const& id);
    ServerPromise<ByteVector> getModLogo(std::string const& id);
}

/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#ifdef ENABLE_SCRIPTING

#    include "../Context.h"
#    include "../common.h"
#    include "../object/ObjectManager.h"
#    include "../object/RideObject.h"
#    include "../object/SmallSceneryObject.h"
#    include "Duktape.hpp"
#    include "ScriptEngine.h"

#    include <optional>

namespace OpenRCT2::Scripting
{
    class ScObject
    {
    private:
        uint8_t _type{};
        int32_t _index{};

    public:
        ScObject(uint8_t type, int32_t index)
            : _type(type)
            , _index(index)
        {
        }

        static void Register(duk_context* ctx)
        {
            dukglue_register_property(ctx, &ScObject::type_get, nullptr, "type");
            dukglue_register_property(ctx, &ScObject::index_get, nullptr, "index");
            dukglue_register_property(ctx, &ScObject::identifier_get, nullptr, "identifier");
            dukglue_register_property(ctx, &ScObject::name_get, nullptr, "name");
        }

        static std::optional<uint8_t> StringToObjectType(const std::string_view& type)
        {
            for (uint8_t i = 0; i < OBJECT_TYPE_COUNT; i++)
            {
                auto s = ObjectTypeToString(i);
                if (s == type)
                {
                    return i;
                }
            }
            return std::nullopt;
        }

        static std::string_view ObjectTypeToString(uint8_t type)
        {
            static const char* Types[] = { "ride",     "small_scenery",     "large_scenery", "wall",          "banner",
                                           "footpath", "footpath_addition", "scenery_group", "park_entrance", "water",
                                           "stex",     "terrain_surface",   "terrain_edge",  "station",       "music" };
            if (type >= std::size(Types))
                return "unknown";
            return Types[type];
        }

    private:
        std::string type_get() const
        {
            return std::string(ObjectTypeToString(_type));
        }

        int32_t index_get() const
        {
            return _index;
        }

        std::string identifier_get() const
        {
            auto obj = GetObject();
            if (obj != nullptr)
            {
                return obj->GetIdentifier();
            }
            return {};
        }

        std::string name_get() const
        {
            auto obj = GetObject();
            if (obj != nullptr)
            {
                return obj->GetName();
            }
            return {};
        }

    protected:
        Object* GetObject() const
        {
            auto& objManager = GetContext()->GetObjectManager();
            return objManager.GetLoadedObject(_type, _index);
        }
    };

    class ScRideObject : public ScObject
    {
    public:
        ScRideObject(uint8_t type, int32_t index)
            : ScObject(type, index)
        {
        }

        static void Register(duk_context* ctx)
        {
            dukglue_set_base_class<ScObject, ScRideObject>(ctx);
            dukglue_register_property(ctx, &ScRideObject::description_get, nullptr, "description");
            dukglue_register_property(ctx, &ScRideObject::capacity_get, nullptr, "capacity");
        }

    private:
        std::string description_get() const
        {
            auto obj = GetObject();
            if (obj != nullptr)
            {
                return obj->GetDescription();
            }
            return {};
        }

        std::string capacity_get() const
        {
            auto obj = GetObject();
            if (obj != nullptr)
            {
                return obj->GetCapacity();
            }
            return {};
        }

    protected:
        RideObject* GetObject() const
        {
            return static_cast<RideObject*>(ScObject::GetObject());
        }
    };

    class ScSmallSceneryObject : public ScObject
    {
    public:
        ScSmallSceneryObject(uint8_t type, int32_t index)
            : ScObject(type, index)
        {
        }

        static void Register(duk_context* ctx)
        {
            dukglue_set_base_class<ScObject, ScSmallSceneryObject>(ctx);
            dukglue_register_property(ctx, &ScSmallSceneryObject::flags_get, nullptr, "flags");
            dukglue_register_property(ctx, &ScSmallSceneryObject::height_get, nullptr, "height");
            dukglue_register_property(ctx, &ScSmallSceneryObject::price_get, nullptr, "price");
            dukglue_register_property(ctx, &ScSmallSceneryObject::removalPrice_get, nullptr, "removalPrice");
        }

    private:
        uint32_t flags_get() const
        {
            auto sceneryEntry = GetLegacyData();
            if (sceneryEntry != nullptr)
            {
                return sceneryEntry->small_scenery.flags;
            }
            return 0;
        }

        uint8_t height_get() const
        {
            auto sceneryEntry = GetLegacyData();
            if (sceneryEntry != nullptr)
            {
                return sceneryEntry->small_scenery.height;
            }
            return 0;
        }

        uint8_t price_get() const
        {
            auto sceneryEntry = GetLegacyData();
            if (sceneryEntry != nullptr)
            {
                return sceneryEntry->small_scenery.price;
            }
            return 0;
        }

        uint8_t removalPrice_get() const
        {
            auto sceneryEntry = GetLegacyData();
            if (sceneryEntry != nullptr)
            {
                return sceneryEntry->small_scenery.removal_price;
            }
            return 0;
        }

    protected:
        rct_scenery_entry* GetLegacyData() const
        {
            auto obj = GetObject();
            if (obj != nullptr)
            {
                return static_cast<rct_scenery_entry*>(obj->GetLegacyData());
            }
            return nullptr;
        }

        SmallSceneryObject* GetObject() const
        {
            return static_cast<SmallSceneryObject*>(ScObject::GetObject());
        }
    };
} // namespace OpenRCT2::Scripting

#endif

/*****************************************************************************
 * Copyright (c) 2014-2022 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include <algorithm>
#include <openrct2-ui/interface/Widget.h>
#include <openrct2-ui/windows/Window.h>
#include <openrct2/Context.h>
#include <openrct2/drawing/Drawing.h>
#include <openrct2/localisation/Formatter.h>
#include <openrct2/localisation/Localisation.h>
#include <openrct2/platform/Platform.h>
#include <openrct2/ride/TrackDesignRepository.h>

static constexpr const StringId WINDOW_TITLE = STR_STRING;
static constexpr const int32_t WH = 44;
static constexpr const int32_t WW = 250;
static constexpr const int32_t WH_DELETE_PROMPT = 74;
static constexpr const int32_t WW_DELETE_PROMPT = 250;
static constexpr const int32_t TrackDesignNameMaxLength = 127;

#pragma region Widgets

// clang-format off
enum {
    WIDX_BACKGROUND,
    WIDX_TITLE,
    WIDX_CLOSE,
    WIDX_RENAME,
    WIDX_DELETE,

    WIDX_PROMPT_DELETE = 3,
    WIDX_PROMPT_CANCEL = 4,
};

static rct_widget window_track_manage_widgets[] = {
    WINDOW_SHIM(WINDOW_TITLE, WW, WH),
    MakeWidget({ 10, 24}, {110, 12}, WindowWidgetType::Button, WindowColour::Primary, STR_TRACK_MANAGE_RENAME),
    MakeWidget({130, 24}, {110, 12}, WindowWidgetType::Button, WindowColour::Primary, STR_TRACK_MANAGE_DELETE),
    WIDGETS_END,
};

static rct_widget window_track_delete_prompt_widgets[] = {
    WINDOW_SHIM(STR_DELETE_FILE, WW_DELETE_PROMPT, WH_DELETE_PROMPT),
    MakeWidget({ 10, 54}, {110, 12}, WindowWidgetType::Button, WindowColour::Primary, STR_TRACK_MANAGE_DELETE),
    MakeWidget({130, 54}, {110, 12}, WindowWidgetType::Button, WindowColour::Primary, STR_CANCEL             ),
    WIDGETS_END,
};

#pragma endregion

// clang-format on

class TrackDesignManageWindow final : public Window
{
private:
    TrackDesignFileRef* _trackDesignFileReference = nullptr;

public:
    void SetFileReference(TrackDesignFileRef* tdFileRef)
    {
        _trackDesignFileReference = tdFileRef;
    }

    void OnOpen() override
    {
        widgets = window_track_manage_widgets;
        WindowInitScrollWidgets(*this);
    }

    void OnClose() override
    {
        rct_window* trackDesignListWindow = window_find_by_class(WindowClass::TrackDesignList);
        if (trackDesignListWindow != nullptr)
        {
            trackDesignListWindow->track_list.track_list_being_updated = false;
        }
    }

    void OnMouseUp(WidgetIndex widgetIndex) override
    {
        switch (widgetIndex)
        {
            case WIDX_CLOSE:
                window_close_by_class(WindowClass::TrackDeletePrompt);
                Close();
                break;
            case WIDX_RENAME:
                WindowTextInputRawOpen(
                    this, widgetIndex, STR_TRACK_DESIGN_RENAME_TITLE, STR_TRACK_DESIGN_RENAME_DESC, {},
                    _trackDesignFileReference->name.c_str(), TrackDesignNameMaxLength);
                break;
            case WIDX_DELETE:
                WindowTrackDeletePromptOpen();
                break;
        }
    }

    void OnTextInput(WidgetIndex widgetIndex, std::string_view text) override
    {
        if (widgetIndex != WIDX_RENAME || str_is_null_or_empty(std::string(text).c_str()))
        {
            return;
        }

        if (str_is_null_or_empty(std::string(text).c_str()))
        {
            context_show_error(STR_CANT_RENAME_TRACK_DESIGN, STR_NONE, {});
            return;
        }

        if (!Platform::IsFilenameValid(text))
        {
            context_show_error(STR_CANT_RENAME_TRACK_DESIGN, STR_NEW_NAME_CONTAINS_INVALID_CHARACTERS, {});
            return;
        }

        if (track_repository_rename(_trackDesignFileReference->path, std::string(text).c_str()))
        {
            window_close_by_class(WindowClass::TrackDeletePrompt);
            Close();
            WindowTrackDesignListReloadTracks();
        }
        else
        {
            context_show_error(STR_CANT_RENAME_TRACK_DESIGN, STR_ANOTHER_FILE_EXISTS_WITH_NAME_OR_FILE_IS_WRITE_PROTECTED, {});
        }
    }

    void OnDraw(rct_drawpixelinfo& dpi) override
    {
        Formatter::Common().Add<const utf8*>(_trackDesignFileReference->name.c_str());
        DrawWidgets(dpi);
    }

private:
    class TrackDeletePromptWindow final : public Window
    {
    private:
        TrackDesignFileRef* _trackDesignFileReference = nullptr;

    public:
        void SetFileReference(TrackDesignFileRef* tdFileRef)
        {
            _trackDesignFileReference = tdFileRef;
        }

        void OnOpen() override
        {
            widgets = window_track_delete_prompt_widgets;
            WindowInitScrollWidgets(*this);
        }

        void OnMouseUp(WidgetIndex widgetIndex) override
        {
            switch (widgetIndex)
            {
                case WIDX_CLOSE:
                case WIDX_PROMPT_CANCEL:
                    Close();
                    break;
                case WIDX_PROMPT_DELETE:
                    if (track_repository_delete(_trackDesignFileReference->path))
                    {
                        WindowTrackDesignListReloadTracks();
                        window_close_by_class(WindowClass::ManageTrackDesign);
                    }
                    else
                    {
                        context_show_error(STR_CANT_DELETE_TRACK_DESIGN, STR_FILE_IS_WRITE_PROTECTED_OR_LOCKED, {});
                    }
                    Close();
                    break;
            }
        }

        void OnDraw(rct_drawpixelinfo& dpi) override
        {
            DrawWidgets(dpi);

            auto ft = Formatter();
            ft.Add<const utf8*>(_trackDesignFileReference->name.c_str());
            DrawTextWrapped(
                &dpi, { windowPos.x + (WW_DELETE_PROMPT / 2), windowPos.y + ((WH_DELETE_PROMPT / 2) - 9) },
                (WW_DELETE_PROMPT - 4), STR_ARE_YOU_SURE_YOU_WANT_TO_PERMANENTLY_DELETE_TRACK, ft, { TextAlignment::CENTRE });
        }
    };

    static void WindowTrackDesignListReloadTracks()
    {
        rct_window* trackListWindow = window_find_by_class(WindowClass::TrackDesignList);
        if (trackListWindow != nullptr)
        {
            trackListWindow->track_list.reload_track_designs = true;
        }
    }

    void WindowTrackDeletePromptOpen()
    {
        window_close_by_class(WindowClass::TrackDeletePrompt);

        int32_t screenWidth = context_get_width();
        int32_t screenHeight = context_get_height();
        auto screenCoords = ScreenCoordsXY(
            std::max(TOP_TOOLBAR_HEIGHT + 1, (screenWidth - WW_DELETE_PROMPT) / 2), (screenHeight - WH_DELETE_PROMPT) / 2);

        auto window = WindowCreate<TrackDeletePromptWindow>(
            WindowClass::TrackDeletePrompt, screenCoords, WW_DELETE_PROMPT, WH_DELETE_PROMPT,
            WF_STICK_TO_FRONT | WF_TRANSPARENT);
        window->SetFileReference(_trackDesignFileReference);
    }
};

rct_window* WindowTrackManageOpen(TrackDesignFileRef* tdFileRef)
{
    window_close_by_class(WindowClass::ManageTrackDesign);

    auto window = WindowFocusOrCreate<TrackDesignManageWindow>(
        WindowClass::ManageTrackDesign, WW, WH, WF_STICK_TO_FRONT | WF_TRANSPARENT | WF_CENTRE_SCREEN);
    window->SetFileReference(tdFileRef);

    rct_window* trackDesignListWindow = window_find_by_class(WindowClass::TrackDesignList);
    if (trackDesignListWindow != nullptr)
    {
        trackDesignListWindow->track_list.track_list_being_updated = true;
    }
    return window;
}

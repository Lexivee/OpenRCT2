/*****************************************************************************
* Copyright (c) 2014 Ted John
* OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
*
* This file is part of OpenRCT2.
*
* OpenRCT2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <time.h>
#include "addresses.h"
#include "audio/audio.h"
#include "config.h"
#include "drawing/drawing.h"
#include "editor.h"
#include "game.h"
#include "input.h"
#include "localisation/date.h"
#include "localisation/localisation.h"
#include "interface/screenshot.h"
#include "interface/viewport.h"
#include "intro.h"
#include "management/news_item.h"
#include "management/research.h"
#include "ride/ride.h"
#include "scenario.h"
#include "util/util.h"
#include "world/climate.h"
#include "world/map.h"
#include "world/park.h"
#include "world/scenery.h"
#include "world/sprite.h"

static const int gRandomShowcase = 0;

#pragma region Showcase script

enum {
	TITLE_SCRIPT_WAIT,
	TITLE_SCRIPT_LOADMM,
	TITLE_SCRIPT_LOCATION,
	TITLE_SCRIPT_ROTATE,
	TITLE_SCRIPT_RESTART,
	TITLE_SCRIPT_LOAD
};

#define WAIT(t)				TITLE_SCRIPT_WAIT, t
#define LOADMM()			TITLE_SCRIPT_LOADMM
#define LOCATION(x, y)		TITLE_SCRIPT_LOCATION, x, y
#define ROTATE(n)			TITLE_SCRIPT_ROTATE, n
#define RESTART()			TITLE_SCRIPT_RESTART
#define LOAD(i)				TITLE_SCRIPT_LOAD, i

static const uint8 _magicMountainScript[] = {
	LOADMM(),
	LOCATION(210, 112), WAIT(13),
	ROTATE(1), LOCATION(210, 112), WAIT(14),
	ROTATE(3), LOCATION(167, 180), WAIT(12),
	ROTATE(1), LOCATION(155, 189), WAIT(12),
	LOCATION(106, 39), WAIT(12),
	LOCATION(182, 50), WAIT(12),
	ROTATE(3), LOCATION(209, 47), WAIT(12),
	ROTATE(1), LOCATION(159, 93), WAIT(12),
	RESTART(),
};

static const uint8* _loadedScript;
static const uint8* _currentScript;
static int _scriptNoLoadsSinceRestart;
static int _scriptWaitCounter;

static void title_init_showcase();
static void title_update_showcase();

static uint8 *generate_random_script();

#pragma endregion

static void title_create_windows();

static uint8 *title_script_load();

/**
 *
 *  rct2: 0x0068E8DA
 */
void title_load()
{
	log_verbose("loading title");

	if (RCT2_GLOBAL(RCT2_ADDRESS_GAME_PAUSED, uint8) & 1)
		pause_toggle();

	RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_FLAGS, uint8) = SCREEN_FLAGS_TITLE_DEMO;

	reset_park_entrances();
	user_string_clear_all();
	reset_sprite_list();
	ride_init_all();
	window_guest_list_init_vars_a();
	sub_6BD3A4();
	map_init(150);
	park_init();
	date_reset();
	climate_reset(CLIMATE_COOL_AND_WET);
	scenery_set_default_placement_configuration();
	window_new_ride_init_vars();
	window_guest_list_init_vars_b();
	window_staff_list_init_vars();
	map_update_tile_pointers();
	reset_0x69EBE4();
	stop_ride_music();
	stop_crowd_sound();
	stop_other_sounds();
	viewport_init_all();
	news_item_init_queue();
	title_create_windows();
	title_init_showcase();
	gfx_invalidate_screen();
	RCT2_GLOBAL(0x009DEA66, uint16) = 0;

	log_verbose("loading title finished");
}

/**
 * Creates the windows shown on the title screen; New game, load game,
 * tutorial, toolbox and exit.
 *  rct2: 0x0066B5C0 (part of 0x0066B3E8)
 */
static void title_create_windows()
{
	window_main_open();
	window_title_menu_open();
	window_title_exit_open();
	window_title_options_open();
	window_title_logo_open();
	window_resize_gui(RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_WIDTH, sint16), RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_HEIGHT, sint16));
}

/**
 *
 *  rct2: 0x00678680
 */
static void title_init_showcase()
{
	_scriptNoLoadsSinceRestart = 1;

	SafeFree(_loadedScript);

	_currentScript = _magicMountainScript;
	switch (gConfigGeneral.title_sequence) {
	case TITLE_SEQUENCE_OPENRCT2:
		_loadedScript = title_script_load();
		if (_loadedScript != NULL)
			_currentScript = _loadedScript;
		break;
	case TITLE_SEQUENCE_RANDOM:
		_loadedScript = generate_random_script();
		_currentScript = _loadedScript;
		break;
	}

	_scriptWaitCounter = 0;
	title_update_showcase();
}

static int title_load_park(const char *path)
{
	rct_window* w;
	int successfulLoad;

	successfulLoad = _strcmpi(path_get_extension(path), ".sv6") == 0 ?
		game_load_sv6(path) :
		scenario_load(path);

	if (!successfulLoad)
		return 0;

	w = window_get_main();
	w->viewport_target_sprite = -1;
	w->saved_view_x = RCT2_GLOBAL(RCT2_ADDRESS_SAVED_VIEW_X, sint16);
	w->saved_view_y = RCT2_GLOBAL(RCT2_ADDRESS_SAVED_VIEW_Y, sint16);

	{
		char _cl = (RCT2_GLOBAL(0x0138869E, sint16) & 0xFF) - w->viewport->zoom;
		w->viewport->zoom = RCT2_GLOBAL(0x0138869E, sint16) & 0xFF;
		*((char*)(&RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_ROTATION, sint32))) = RCT2_GLOBAL(0x0138869E, sint16) >> 8;
		if (_cl != 0) {
			if (_cl < 0) {
				_cl = -_cl;
				w->viewport->view_width >>= _cl;
				w->viewport->view_height >>= _cl;
			} else {
				w->viewport->view_width <<= _cl;
				w->viewport->view_height <<= _cl;
			}
		}
		w->saved_view_x -= w->viewport->view_width >> 1;
		w->saved_view_y -= w->viewport->view_height >> 1;
	}

	window_invalidate(w);
	reset_all_sprite_quadrant_placements();
	window_new_ride_init_vars();
	sub_684AC3();
	RCT2_CALLPROC_EBPSAFE(0x006DFEE4);
	news_item_init_queue();
	gfx_invalidate_screen();
	RCT2_GLOBAL(0x009DEA66, sint16) = 0;
	RCT2_GLOBAL(0x009DEA5C, sint16) = 0x0D6D8;
	return 1;
}

static void title_skip_opcode()
{
	uint8 script_opcode;

	script_opcode = *_currentScript++;
	switch (script_opcode) {
	case TITLE_SCRIPT_WAIT:
		_currentScript++;
		break;
	case TITLE_SCRIPT_LOADMM:
		_currentScript++;
		break;
	case TITLE_SCRIPT_LOCATION:
		_currentScript++;
		_currentScript++;
		break;
	case TITLE_SCRIPT_ROTATE:
		_currentScript++;
		break;
	case TITLE_SCRIPT_RESTART:
		break;
	case TITLE_SCRIPT_LOAD:
		do {
			_currentScript++;
		} while (*(_currentScript - 1) != 0);
		break;
	}
}

static void title_do_next_script_opcode()
{
	int i;
	short x, y, z;
	uint8 script_opcode, script_operand;
	rct_window* w;
	
	script_opcode = *_currentScript++;
	switch (script_opcode) {
	case TITLE_SCRIPT_WAIT:
		_scriptWaitCounter = (*_currentScript++) * 32;
		break;
	case TITLE_SCRIPT_LOADMM:
		if (!title_load_park(get_file_path(PATH_ID_SIXFLAGS_MAGICMOUNTAIN))) {
			log_fatal("OpenRCT2 can not currently cope when unable to load title screen scenario.");
			exit(-1);
		}
		break;
	case TITLE_SCRIPT_LOCATION:
		x = (*_currentScript++) * 32 + 16;
		y = (*_currentScript++) * 32 + 16;
		z = map_element_height(x, y);

		// Update viewport
		w = window_get_main();
		if (w != NULL) {
			window_scroll_to_location(w, x, y, z);
			w->flags &= ~WF_SCROLLING_TO_LOCATION;
			viewport_update_position(w);
		}
		break;
	case TITLE_SCRIPT_ROTATE:
		script_operand = (*_currentScript++);
		w = window_get_main();
		if (w != NULL)
			for (i = 0; i < script_operand; i++)
				window_rotate_camera(w);
		break;
	case TITLE_SCRIPT_RESTART:
		_scriptNoLoadsSinceRestart = 1;
		_currentScript = _loadedScript;
		if (gRandomShowcase) {
			if (_currentScript != NULL)
				free((uint8*)_currentScript);
			_currentScript = generate_random_script();
		}
		break;
	case TITLE_SCRIPT_LOAD:
		{
			const uint8 *loadPtr;
			char *ch, filename[32], path[MAX_PATH];
			
			loadPtr = _currentScript - 1;

			// Get filename
			ch = filename;
			do {
				*ch++ = *_currentScript++;
			} while (*(_currentScript - 1) != 0);

			// Construct full relative path
			sprintf(path, "data%ctitle%c%s", platform_get_path_separator(), platform_get_path_separator(), filename);
			if (title_load_park(path)) {
				_scriptNoLoadsSinceRestart = 0;
			} else {
				script_opcode = *_currentScript;
				while (script_opcode != TITLE_SCRIPT_LOADMM && script_opcode != TITLE_SCRIPT_LOAD && script_opcode != TITLE_SCRIPT_RESTART) {
					title_skip_opcode();
					script_opcode = *_currentScript;
				}

				if (script_opcode == TITLE_SCRIPT_RESTART && _scriptNoLoadsSinceRestart) {
					if (_currentScript != _magicMountainScript) {
						_currentScript = _magicMountainScript;
					} else {
						log_fatal("OpenRCT2 can not currently cope when unable to load title screen scenario.");
						exit(-1);
					}
				}
			}
		}
		break;
	}
}

/**
 *
 *  rct2: 0x00678761
 */
static void title_update_showcase()
{
	if (_scriptWaitCounter <= 0) {
		do {
			title_do_next_script_opcode();
		} while (_scriptWaitCounter == 0);
	}

	_scriptWaitCounter--;
}

static void DrawOpenRCT2(int x, int y)
{
	char buffer[256];
	rct_drawpixelinfo *dpi = RCT2_ADDRESS(RCT2_ADDRESS_SCREEN_DPI, rct_drawpixelinfo);
	
	// Draw background
	gfx_fill_rect_inset(dpi, x, y, x + 128, y + 20, 0x80 | 12, 0x8);

	// Format text (name and version)
	sprintf(buffer, "%c%c%c%s, v%s", FORMAT_MEDIUMFONT, FORMAT_OUTLINE, FORMAT_WHITE, OPENRCT2_NAME, OPENRCT2_VERSION);

	// Draw Text
	gfx_draw_string(dpi, buffer, 0, x + 5, y + 5);
}


void game_handle_input();
void title_update()
{
	screenshot_check();
	title_handle_keyboard_input();

	if (RCT2_GLOBAL(RCT2_ADDRESS_GAME_PAUSED, uint8) == 0) {
		title_update_showcase();
		game_logic_update();
		start_title_music();
	}

	RCT2_GLOBAL(RCT2_ADDRESS_INPUT_FLAGS, uint32) &= ~0x80;

	window_map_tooltip_update_visibility();
	window_dispatch_update_all();
	window_update_all();
	DrawOpenRCT2(0, RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_HEIGHT, uint16) - 20);

	RCT2_GLOBAL(0x01388698, uint16)++;

	// Input
	game_handle_input();

	update_palette_effects();
	update_rain_animation();

	if (RCT2_GLOBAL(0x009AAC73, uint8) != 255) {
		RCT2_GLOBAL(0x009AAC73, uint8)++;
		if (RCT2_GLOBAL(0x009AAC73, uint8) == 255)
			config_save_default();
	}
}

static uint8 *generate_random_script()
{
	int i, j;
	const int views = 16;

	srand((unsigned int)time(NULL));

	uint8 *script = malloc(views * 8 + 2);
	i = 0;
	script[i++] = TITLE_SCRIPT_LOAD;
	for (j = 0; j < views; j++) {
		script[i++] = TITLE_SCRIPT_LOCATION;
		script[i++] = 64 + (rand() % 128);
		script[i++] = 64 + (rand() % 128);

		int rotationCount = rand() % 4;
		if (rotationCount > 0) {
			script[i++] = TITLE_SCRIPT_ROTATE;
			script[i++] = rotationCount;
		}

		script[i++] = TITLE_SCRIPT_WAIT;
		script[i++] = 8 + (rand() % 6);
	}
	script[i] = TITLE_SCRIPT_RESTART;

	return script;
}

#pragma region Load script.txt

static void title_script_get_line(FILE *file, char *parts)
{
	int i, c, part, cindex, whitespace, comment;

	for (i = 0; i < 3; i++)
		parts[i * 32] = 0;

	part = 0;
	cindex = 0;
	whitespace = 1;
	comment = 0;
	for (;;) {
		c = fgetc(file);
		if (c == '\n' || c == '\r' || c == EOF) {
			parts[part * 32 + cindex] = 0;
			return;
		} else if (c == '#') {
			parts[part * 32 + cindex] = 0;
			comment = 1;
		} else if (c == ' ' && !comment) {
			if (!whitespace) {
				parts[part * 32 + cindex] = 0;
				part++;
				cindex = 0;
			}
		} else if (!comment) {
			whitespace = 0;
			if (cindex < 31) {
				parts[part * 32 + cindex] = c;
				cindex++;
			}
		}
	}
}

static uint8 *title_script_load()
{
	FILE *file;
	char parts[3 * 32], *token, *part1, *part2, *src;

	char path[] = "data/title/script.txt";

	log_verbose("loading title script, %s", path);
	file = fopen(path, "r");
	if (file == NULL) {
		log_error("unable to load title script");
		return NULL;
	}

	uint8 *binaryScript = (uint8*)malloc(1024 * 8);
	if (binaryScript == NULL) {
		fclose(file);

		log_error("unable to allocate memory for script");
		return NULL;
	}

	uint8 *scriptPtr = binaryScript;

	do {
		title_script_get_line(file, parts);

		token = &parts[0 * 32];
		part1 = &parts[1 * 32];
		part2 = &parts[2 * 32];

		if (token[0] != 0) {
			if (_stricmp(token, "LOAD") == 0) {
				src = part1;
				*scriptPtr++ = TITLE_SCRIPT_LOAD;
				do {
					*scriptPtr++ = *src++;
				} while (*(src - 1) != 0);
			} else if (_stricmp(token, "LOCATION") == 0) {
				*scriptPtr++ = TITLE_SCRIPT_LOCATION;
				*scriptPtr++ = atoi(part1) & 0xFF;
				*scriptPtr++ = atoi(part2) & 0xFF;
			} else if (_stricmp(token, "ROTATE") == 0) {
				*scriptPtr++ = TITLE_SCRIPT_ROTATE;
				*scriptPtr++ = atoi(part1) & 0xFF;
			} else if (_stricmp(token, "WAIT") == 0) {
				*scriptPtr++ = TITLE_SCRIPT_WAIT;
				*scriptPtr++ = atoi(part1) & 0xFF;
			} else {
				log_error("unknown token, %s", token);
				return NULL;
			}
		}
	} while (!feof(file));
	fclose(file);

	*scriptPtr++ = TITLE_SCRIPT_RESTART;

	int scriptLength = (int)(scriptPtr - binaryScript);
	binaryScript = realloc(binaryScript, scriptLength);
	if (binaryScript == NULL) {
		log_error("unable to reallocate memory for script");
		return NULL;
	}

	return binaryScript;
}

#pragma endregion

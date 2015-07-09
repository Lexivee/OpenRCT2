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

#include "addresses.h"
#include "audio/audio.h"
#include "audio/mixer.h"
#include "cmdline.h"
#include "config.h"
#include "editor.h"
#include "game.h"
#include "hook.h"
#include "interface/window.h"
#include "localisation/localisation.h"
#include "network/http.h"
#include "openrct2.h"
#include "platform/platform.h"
#include "util/sawyercoding.h"
#include "world/mapgen.h"
#include "title.h"

int gOpenRCT2StartupAction = STARTUP_ACTION_TITLE;
char gOpenRCT2StartupActionPath[512] = { 0 };
char gExePath[MAX_PATH];

// This should probably be changed later and allow a custom selection of things to initialise like SDL_INIT
bool gOpenRCT2Headless = false;

bool gOpenRCT2ShowChangelog;

// This needs to be set when a park is loaded. It could also be used to reset other important states, it should then be renamed
// to something more general.
bool gOpenRCT2ResetFrameSmoothing = false;

/** If set, will end the OpenRCT2 game loop. Intentially private to this module so that the flag can not be set back to 0. */
int _finished;

static void openrct2_loop();

static void openrct2_copy_files_over(const char *originalDirectory, const char *newDirectory, const char *extension)
{
	char *ch, filter[MAX_PATH], oldPath[MAX_PATH], newPath[MAX_PATH];
	int fileEnumHandle;
	file_info fileInfo;
	
	if (!platform_ensure_directory_exists(newDirectory)) {
		log_error("Could not create directory %s.", newDirectory);
		return;
	}

	// Create filter path
	strcpy(filter, originalDirectory);
	ch = strchr(filter, '*');
	if (ch != NULL)
		*ch = 0;
	strcat(filter, "*");
	strcat(filter, extension);

	fileEnumHandle = platform_enumerate_files_begin(filter);
	while (platform_enumerate_files_next(fileEnumHandle, &fileInfo)) {
		strcpy(newPath, newDirectory);
		strcat(newPath, fileInfo.path);
		
		strcpy(oldPath, originalDirectory);
		ch = strchr(oldPath, '*');
		if (ch != NULL)
			*ch = 0;
		strcat(oldPath, fileInfo.path);

		if (!platform_file_exists(newPath))
			platform_file_copy(oldPath, newPath);
	}
	platform_enumerate_files_end(fileEnumHandle);

	fileEnumHandle = platform_enumerate_directories_begin(originalDirectory);
	while (platform_enumerate_directories_next(fileEnumHandle, filter)) {
		strcpy(newPath, newDirectory);
		strcat(newPath, filter);

		strcpy(oldPath, originalDirectory);
		ch = strchr(oldPath, '*');
		if (ch != NULL)
			*ch = 0;
		strcat(oldPath, filter);

		if (!platform_ensure_directory_exists(newPath)) {
			log_error("Could not create directory %s.", newPath);
			return;
		}
		openrct2_copy_files_over(oldPath, newPath, extension);
	}
	platform_enumerate_directories_end(fileEnumHandle);
}

static void openrct2_set_exe_path()
{
	char exePath[MAX_PATH];
	char tempPath[MAX_PATH];
	char *exeDelimiter;
	int pathEnd;
	int exeDelimiterIndex;

	GetModuleFileName(NULL, exePath, MAX_PATH);
	exeDelimiter = strrchr(exePath, platform_get_path_separator());
	exeDelimiterIndex = (int)(exeDelimiter - exePath);
	pathEnd = strlen(exePath) - (strlen(exePath) - exeDelimiterIndex);
	strncpy(tempPath, exePath, pathEnd);
	tempPath[pathEnd] = '\0';
	_fullpath(gExePath, tempPath, MAX_PATH);
}

/**
 * Copy saved games and landscapes to user directory
 */
static void openrct2_copy_original_user_files_over()
{
	char path[MAX_PATH];

	platform_get_user_directory(path, "save");
	openrct2_copy_files_over((char*)RCT2_ADDRESS_SAVED_GAMES_PATH, path, ".sv6");

	platform_get_user_directory(path, "landscape");
	openrct2_copy_files_over((char*)RCT2_ADDRESS_LANDSCAPES_PATH, path, ".sc6");
}

bool openrct2_initialise()
{
	char userPath[MAX_PATH];

	platform_get_user_directory(userPath, NULL);
	if (!platform_ensure_directory_exists(userPath)) {
		log_fatal("Could not create user directory (do you have write access to your documents folder?)");
		return false;
	}

	openrct2_set_exe_path();

	config_set_defaults();
	if (!config_open_default()) {
		if (!config_find_or_browse_install_directory()) {
			log_fatal("An RCT2 install directory must be specified!");
			return false;
		}
	}

	gOpenRCT2ShowChangelog = true;
	if (gConfigGeneral.last_run_version != NULL && (strcmp(gConfigGeneral.last_run_version, OPENRCT2_VERSION) == 0))
		gOpenRCT2ShowChangelog = false;
	gConfigGeneral.last_run_version = OPENRCT2_VERSION;
	config_save_default();

	// TODO add configuration option to allow multiple instances
	if (!gOpenRCT2Headless && !platform_lock_single_instance()) {
		log_fatal("OpenRCT2 is already running.");
		return false;
	}

	get_system_info();
	if (!gOpenRCT2Headless) {
		audio_init();
		audio_get_devices();
		get_dsound_devices();
	}
	language_open(gConfigGeneral.language);
	http_init();

	themes_set_default();
	themes_load_presets();
	title_sequences_set_default();
	title_sequences_load_presets();

	if (!rct2_init())
		return false;

	openrct2_copy_original_user_files_over();

	addhook(0x006E732D, (int)gfx_set_dirty_blocks, 0, (int[]){EAX, EBX, EDX, EBP, END}, 0); // remove after all drawing is decompiled

	Mixer_Init(NULL);
	return true;
}

/**
 * Launches the game, after command line arguments have been parsed and processed.
 */
void openrct2_launch()
{
	if (openrct2_initialise()) {
		RCT2_GLOBAL(RCT2_ADDRESS_RUN_INTRO_TICK_PART, uint8) = 0;
		switch (gOpenRCT2StartupAction) {
		case STARTUP_ACTION_INTRO:
			RCT2_GLOBAL(RCT2_ADDRESS_RUN_INTRO_TICK_PART, uint8) = 8;
			break;
		case STARTUP_ACTION_TITLE:
			RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_FLAGS, uint8) = SCREEN_FLAGS_TITLE_DEMO;
			break;
		case STARTUP_ACTION_OPEN:
			assert(gOpenRCT2StartupActionPath != NULL);
			rct2_open_file(gOpenRCT2StartupActionPath);

			RCT2_GLOBAL(RCT2_ADDRESS_SCREEN_FLAGS, uint8) = SCREEN_FLAGS_PLAYING;
			break;
		case STARTUP_ACTION_EDIT:
			if (strlen(gOpenRCT2StartupActionPath) == 0) {
				editor_load();
			} else {
				editor_load_landscape(gOpenRCT2StartupActionPath);
			}
			break;
    	}
		openrct2_loop();
	}
	openrct2_dispose();

	// HACK Some threads are still running which causes the game to not terminate. Investigation required!
	exit(gExitCode);
}

void openrct2_dispose()
{
	http_dispose();
	language_close_all();
	platform_free();
}

/**
 * Determines whether its worth tweening a sprite or not when frame smoothing is on.
 */
static bool sprite_should_tween(rct_sprite *sprite)
{
	if (sprite->unknown.linked_list_type_offset == SPRITE_LINKEDLIST_OFFSET_VEHICLE)
		return true;
	if (sprite->unknown.linked_list_type_offset == SPRITE_LINKEDLIST_OFFSET_PEEP)
		return true;
	if (sprite->unknown.linked_list_type_offset == SPRITE_LINKEDLIST_OFFSET_UNKNOWN)
		return true;

	return false;
}

/**
 * Run the main game loop until the finished flag is set at 40fps (25ms interval).
 */
static void openrct2_loop()
{
	uint32 currentTick, ticksElapsed, lastTick = 0;
	static uint32 uncapTick;
	static int fps = 0;
	static uint32 secondTick = 0;
	static bool uncappedinitialized = false;
	static struct { sint16 x, y, z; } spritelocations1[MAX_SPRITES], spritelocations2[MAX_SPRITES];

	log_verbose("begin openrct2 loop");

	_finished = 0;
	do {
		if (gConfigGeneral.uncap_fps && gGameSpeed <= 4) {
			currentTick = SDL_GetTicks();
			if (!uncappedinitialized) {
				// Reset sprite locations
				uncapTick = SDL_GetTicks();
				for (uint16 i = 0; i < MAX_SPRITES; i++) {
					spritelocations1[i].x = spritelocations2[i].x = g_sprite_list[i].unknown.x;
					spritelocations1[i].y = spritelocations2[i].y = g_sprite_list[i].unknown.y;
					spritelocations1[i].z = spritelocations2[i].z = g_sprite_list[i].unknown.z;
				}
				uncappedinitialized = true;
			}

			while (uncapTick <= currentTick && currentTick - uncapTick > 25) {
				// Get the original position of each sprite
				for (uint16 i = 0; i < MAX_SPRITES; i++) {
					spritelocations1[i].x = g_sprite_list[i].unknown.x;
					spritelocations1[i].y = g_sprite_list[i].unknown.y;
					spritelocations1[i].z = g_sprite_list[i].unknown.z;
				}
				
				// Update the game so the sprite positions update
				rct2_update();
				if (gOpenRCT2ResetFrameSmoothing) {
					gOpenRCT2ResetFrameSmoothing = false;
					uncappedinitialized = false;
					continue;
				}

				// Get the next position of each sprite
				for (uint16 i = 0; i < MAX_SPRITES; i++) {
					spritelocations2[i].x = g_sprite_list[i].unknown.x;
					spritelocations2[i].y = g_sprite_list[i].unknown.y;
					spritelocations2[i].z = g_sprite_list[i].unknown.z;
				}

				uncapTick += 25;
			}

			// Tween the position of each sprite from the last position to the new position based on the time between the last
			// tick and the next tick.
			float nudge = 1 - ((float)(currentTick - uncapTick) / 25);
			for (uint16 i = 0; i < MAX_SPRITES; i++) {
				if (!sprite_should_tween(&g_sprite_list[i]))
					continue;

				sprite_move(
					spritelocations2[i].x + (sint16)((spritelocations1[i].x - spritelocations2[i].x) * nudge),
					spritelocations2[i].y + (sint16)((spritelocations1[i].y - spritelocations2[i].y) * nudge),
					spritelocations2[i].z + (sint16)((spritelocations1[i].z - spritelocations2[i].z) * nudge),
					&g_sprite_list[i]
				);
				invalidate_sprite(&g_sprite_list[i]);
			}

			// Viewports need to be updated to reduce chopiness of those which follow sprites
			window_update_all_viewports();

			platform_process_messages();
			rct2_draw();
			platform_draw();
			fps++;
			if (SDL_GetTicks() - secondTick >= 1000) {
				fps = 0;
				secondTick = SDL_GetTicks();
			}

			// Restore the real positions of the sprites so they aren't left at the mid-tween positions
			for (uint16 i = 0; i < MAX_SPRITES; i++) {
				if (!sprite_should_tween(&g_sprite_list[i]))
					continue;

				invalidate_sprite(&g_sprite_list[i]);
				sprite_move(spritelocations2[i].x, spritelocations2[i].y, spritelocations2[i].z, &g_sprite_list[i]);
			}
		} else {
			uncappedinitialized = false;
			currentTick = SDL_GetTicks();
			ticksElapsed = currentTick - lastTick;
			if (ticksElapsed < 25) {
				if (ticksElapsed < 15)
					SDL_Delay(15 - ticksElapsed);
				continue;
			}

			lastTick = currentTick;

			platform_process_messages();

			rct2_update();
			gOpenRCT2ResetFrameSmoothing = false;

			rct2_draw();
			platform_draw();
		}
	} while (!_finished);
}

/**
 * Causes the OpenRCT2 game loop to finish.
 */
void openrct2_finish()
{
	_finished = 1;
}
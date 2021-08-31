/*
	Copyright (c) 2017-2020 ByteBit

	This file is part of BetterSpades.

	BetterSpades is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	BetterSpades is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with BetterSpades.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include <window.hpp>
#include <file.hpp>
#include <ini.hpp>
#include <config.hpp>
#include <sound.hpp>
#include <model.hpp>
#include <camera.hpp>

struct RENDER_OPTIONS settings, settings_tmp;
struct list config_keys;
struct list config_settings;

struct list config_file;

static void config_sets(const char* section, const char* name, const char* value) {
	for(int k = 0; k < list_size(&config_file); k++) {
		auto e = (config_file_entry*) list_get(&config_file, k);
		if(strcmp(e->name, name) == 0) {
			strncpy(e->value, value, sizeof(e->value) - 1);
			return;
		}
	}
	struct config_file_entry e;
	strncpy(e.section, section, sizeof(e.section) - 1);
	strncpy(e.name, name, sizeof(e.name) - 1);
	strncpy(e.value, value, sizeof(e.value) - 1);
	list_add(&config_file, &e);
}

static void config_seti(const char* section, const char* name, int value) {
	char tmp[32];
	sprintf(tmp, "%i", value);
	config_sets(section, name, tmp);
}

static void config_setf(const char* section, const char* name, float value) {
	char tmp[32];
	sprintf(tmp, "%0.6f", value);
	config_sets(section, name, tmp);
}

void config_save() {
	kv6_rebuild_complete();

	config_sets("client", "name", settings.name);
	config_seti("client", "xres", settings.window_width);
	config_seti("client", "yres", settings.window_height);
	config_seti("client", "windowed", !settings.fullscreen);
	config_seti("client", "multisamples", settings.multisamples);
	config_seti("client", "greedy_meshing", settings.greedy_meshing);
	config_seti("client", "vsync", settings.vsync);
	config_setf("client", "mouse_sensitivity", settings.mouse_sensitivity);
	config_seti("client", "show_news", settings.show_news);
	config_seti("client", "vol", settings.volume);
	config_seti("client", "show_fps", settings.show_fps);
	config_seti("client", "voxlap_models", settings.voxlap_models);
	config_seti("client", "force_displaylist", settings.force_displaylist);
	config_seti("client", "inverty", settings.invert_y);
	config_seti("client", "smooth_fog", settings.smooth_fog);
	config_seti("client", "ambient_occlusion", settings.ambient_occlusion);
	config_setf("client", "camera_fov", settings.camera_fov);
	config_seti("client", "hold_down_sights", settings.hold_down_sights);
	config_seti("client", "chat_shadow", settings.chat_shadow);

	for(int k = 0; k < list_size(&config_keys); k++) {
		auto e = (config_key_pair*) list_get(&config_keys, k);
		if(strlen(e->name) > 0)
			config_seti("controls", e->name, e->def);
	}

	void* f = file_open("config.ini", "w");
	if(f) {
		char last_section[32] = {0};
		for(int k = 0; k < list_size(&config_file); k++) {
			auto e = (config_file_entry*) list_get(&config_file, k);
			if(strcmp(e->section, last_section) != 0) {
				file_printf(f, "\r\n[%s]\r\n", e->section);
				strcpy(last_section, e->section);
			}
			file_printf(f, "%s", e->name);
			for(int l = 0; l < 31 - strlen(e->name); l++)
				file_printf(f, " ");
			file_printf(f, "= %s\r\n", e->value);
		}
		file_close(f);
	}
}

static int config_read_key(void* user, const char* section, const char* name, const char* value) {
	struct config_file_entry e;
	strncpy(e.section, section, sizeof(e.section) - 1);
	strncpy(e.name, name, sizeof(e.name) - 1);
	strncpy(e.value, value, sizeof(e.value) - 1);
	list_add(&config_file, &e);

	if(!strcmp(section, "client")) {
		if(!strcmp(name, "name")) {
			strcpy(settings.name, value);
		} else if(!strcmp(name, "xres")) {
			settings.window_width = atoi(value);
		} else if(!strcmp(name, "yres")) {
			settings.window_height = atoi(value);
		} else if(!strcmp(name, "windowed")) {
			settings.fullscreen = !atoi(value);
		} else if(!strcmp(name, "multisamples")) {
			settings.multisamples = atoi(value);
		} else if(!strcmp(name, "greedy_meshing")) {
			settings.greedy_meshing = atoi(value);
		} else if(!strcmp(name, "vsync")) {
			settings.vsync = atoi(value);
		} else if(!strcmp(name, "mouse_sensitivity")) {
			settings.mouse_sensitivity = atof(value);
		} else if(!strcmp(name, "show_news")) {
			settings.show_news = atoi(value);
		} else if(!strcmp(name, "vol")) {
			settings.volume = maxc(minc(atoi(value), 10), 0);
			sound_volume(settings.volume / 10.0F);
		} else if(!strcmp(name, "show_fps")) {
			settings.show_fps = atoi(value);
		} else if(!strcmp(name, "voxlap_models")) {
			settings.voxlap_models = atoi(value);
		} else if(!strcmp(name, "force_displaylist")) {
			settings.force_displaylist = atoi(value);
		} else if(!strcmp(name, "inverty")) {
			settings.invert_y = atoi(value);
		} else if(!strcmp(name, "smooth_fog")) {
			settings.smooth_fog = atoi(value);
		} else if(!strcmp(name, "ambient_occlusion")) {
			settings.ambient_occlusion = atoi(value);
		} else if(!strcmp(name, "camera_fov")) {
			settings.camera_fov = fmax(fmin(atof(value), CAMERA_MAX_FOV), CAMERA_DEFAULT_FOV);
		} else if(!strcmp(name, "hold_down_sights")) {
			settings.hold_down_sights = atoi(value);
		} else if(!strcmp(name, "chat_shadow")) {
			settings.chat_shadow = atoi(value);
		}
	}
	if(!strcmp(section, "controls")) {
		for(int k = 0; k < list_size(&config_keys); k++) {
			auto key = (config_key_pair*) list_get(&config_keys, k);
			if(!strcmp(name, key->name)) {
				log_debug("found override for %s, from %i to %i", key->name, key->def, atoi(value));
				key->def = strtol(value, NULL, 0);
				break;
			}
		}
	}
	return 1;
}

void config_register_key(int internal, int def, const char* name, int toggle, const char* display,
						 const char* category) {
	struct config_key_pair key;
	key.internal = internal;
	key.def = def;
	key.original = def;
	key.toggle = toggle;
	if(display)
		strncpy(key.display, display, sizeof(key.display) - 1);
	else
		*key.display = 0;

	if(name)
		strncpy(key.name, name, sizeof(key.name) - 1);
	else
		*key.name = 0;

	if(category)
		strncpy(key.category, category, sizeof(key.category) - 1);
	else
		*key.category = 0;
	list_add(&config_keys, &key);
}

int config_key_translate(int key, int dir, int* results) {
	int count = 0;

	for(int k = 0; k < list_size(&config_keys); k++) {
		auto a = (config_key_pair*) list_get(&config_keys, k);

		if(dir && a->internal == key) {
			if(results)
				results[count] = a->def;
			count++;
		} else if(!dir && a->def == key) {
			if(results)
				results[count] = a->internal;
			count++;
		}
	}

	return count;
}

struct config_key_pair* config_key(int key) {
	for(int k = 0; k < list_size(&config_keys); k++) {
		auto a = (config_key_pair*) list_get(&config_keys, k);
		if(a->internal == key)
			return a;
	}
	return NULL;
}

void config_key_reset_togglestates() {
	for(int k = 0; k < list_size(&config_keys); k++) {
		auto a = (config_key_pair*) list_get(&config_keys, k);
		if(a->toggle)
			window_pressed_keys[a->internal] = 0;
	}
}

static int config_key_cmp(const void* a, const void* b) {
	auto A = (const struct config_key_pair*) a;
	auto B = (const struct config_key_pair*) b;

	int cmp = strcmp(A->category, B->category);
	return cmp ? cmp : strcmp(A->display, B->display);
}

static void config_label_pixels(char* buffer, size_t length, int value, size_t index) {
	if(value == 800 || value == 600) {
		snprintf(buffer, length, "default: %ipx", value);
	} else {
		snprintf(buffer, length, "%ipx", value);
	}
}

static void config_label_vsync(char* buffer, size_t length, int value, size_t index) {
	if(value == 0) {
		snprintf(buffer, length, "disabled");
	} else if(value == 1) {
		snprintf(buffer, length, "enabled");
	} else {
		snprintf(buffer, length, "max %i fps", value);
	}
}

static void config_label_msaa(char* buffer, size_t length, int value, size_t index) {
	if(index == 0) {
		snprintf(buffer, length, "No MSAA");
	} else {
		snprintf(buffer, length, "%ix MSAA", value);
	}
}

void config_reload() {
	if(!list_created(&config_file))
		list_create(&config_file, sizeof(struct config_file_entry));
	else
		list_clear(&config_file);

	if(!list_created(&config_keys))
		list_create(&config_keys, sizeof(struct config_key_pair));
	else
		list_clear(&config_keys);

#ifdef USE_SDL
	config_register_key(WINDOW_KEY_UP, SDLK_w, "move_forward", 0, "Forward", "Movement");
	config_register_key(WINDOW_KEY_DOWN, SDLK_s, "move_backward", 0, "Backward", "Movement");
	config_register_key(WINDOW_KEY_LEFT, SDLK_a, "move_left", 0, "Left", "Movement");
	config_register_key(WINDOW_KEY_RIGHT, SDLK_d, "move_right", 0, "Right", "Movement");
	config_register_key(WINDOW_KEY_SPACE, SDLK_SPACE, "jump", 0, "Jump", "Movement");
	config_register_key(WINDOW_KEY_SPRINT, SDLK_LSHIFT, "sprint", 0, "Sprint", "Movement");
	config_register_key(WINDOW_KEY_SHIFT, SDLK_LSHIFT, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_CURSOR_UP, SDLK_UP, "cube_color_up", 0, "Color up", "Block");
	config_register_key(WINDOW_KEY_CURSOR_DOWN, SDLK_DOWN, "cube_color_down", 0, "Color down", "Block");
	config_register_key(WINDOW_KEY_CURSOR_LEFT, SDLK_LEFT, "cube_color_left", 0, "Color left", "Block");
	config_register_key(WINDOW_KEY_CURSOR_RIGHT, SDLK_RIGHT, "cube_color_right", 0, "Color right", "Block");
	config_register_key(WINDOW_KEY_BACKSPACE, SDLK_BACKSPACE, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_TOOL1, SDLK_1, "tool_spade", 0, "Select spade", "Tools & Weapons");
	config_register_key(WINDOW_KEY_TOOL2, SDLK_2, "tool_block", 0, "Select block", "Tools & Weapons");
	config_register_key(WINDOW_KEY_TOOL3, SDLK_3, "tool_gun", 0, "Select gun", "Tools & Weapons");
	config_register_key(WINDOW_KEY_TOOL4, SDLK_4, "tool_grenade", 0, "Select grenade", "Tools & Weapons");
	config_register_key(WINDOW_KEY_TAB, SDLK_TAB, "view_score", 0, "Score", "Information");
	config_register_key(WINDOW_KEY_ESCAPE, SDLK_ESCAPE, "quit_game", 0, "Quit", "Game");
	config_register_key(WINDOW_KEY_ESCAPE, SDLK_AC_BACK, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_MAP, SDLK_m, "view_map", 1, "Map", "Information");
	config_register_key(WINDOW_KEY_CROUCH, SDLK_LCTRL, "crouch", 0, "Crouch", "Movement");
	config_register_key(WINDOW_KEY_SNEAK, SDLK_v, "sneak", 0, "Sneak", "Movement");
	config_register_key(WINDOW_KEY_ENTER, SDLK_RETURN, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_F1, SDLK_F1, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_F2, SDLK_F2, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_F3, SDLK_F3, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_F4, SDLK_F4, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_YES, SDLK_y, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_YES, SDLK_z, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_NO, SDLK_n, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_VOLUME_UP, SDLK_KP_PLUS, "volume_up", 0, "Volume up", "Game");
	config_register_key(WINDOW_KEY_VOLUME_DOWN, SDLK_KP_MINUS, "volume_down", 0, "Volume down", "Game");
	config_register_key(WINDOW_KEY_V, SDLK_v, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_RELOAD, SDLK_r, "reload", 0, "Reload", "Tools & Weapons");
	config_register_key(WINDOW_KEY_CHAT, SDLK_t, "chat_global", 0, "Chat", "Game");
	config_register_key(WINDOW_KEY_FULLSCREEN, SDLK_F11, "fullscreen", 0, "Fullscreen", "Game");
	config_register_key(WINDOW_KEY_SCREENSHOT, SDLK_F5, "screenshot", 0, "Screenshot", "Information");
	config_register_key(WINDOW_KEY_CHANGETEAM, SDLK_COMMA, "change_team", 0, "Team select", "Game");
	config_register_key(WINDOW_KEY_CHANGEWEAPON, SDLK_PERIOD, "change_weapon", 0, "Gun select", "Tools & Weapons");
	config_register_key(WINDOW_KEY_PICKCOLOR, SDLK_e, "cube_color_sample", 0, "Pick color", "Block");
	config_register_key(WINDOW_KEY_COMMAND, SDLK_SLASH, "chat_command", 0, "Command", "Game");
	config_register_key(WINDOW_KEY_HIDEHUD, SDLK_F6, "hide_hud", 1, "Hide HUD", "Game");
	config_register_key(WINDOW_KEY_LASTTOOL, SDLK_q, "last_tool", 0, "Last tool", "Tools & Weapons");
	config_register_key(WINDOW_KEY_NETWORKSTATS, SDLK_F12, "network_stats", 1, "Network stats", "Information");
	config_register_key(WINDOW_KEY_SAVE_MAP, SDLK_F9, "save_map", 0, "Save map", "Game");
	config_register_key(WINDOW_KEY_SELECT1, SDLK_1, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_SELECT2, SDLK_2, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_SELECT3, SDLK_3, NULL, 0, NULL, NULL);
#endif

#ifdef USE_GLFW
	config_register_key(WINDOW_KEY_UP, GLFW_KEY_W, "move_forward", 0, "Forward", "Movement");
	config_register_key(WINDOW_KEY_DOWN, GLFW_KEY_S, "move_backward", 0, "Backward", "Movement");
	config_register_key(WINDOW_KEY_LEFT, GLFW_KEY_A, "move_left", 0, "Left", "Movement");
	config_register_key(WINDOW_KEY_RIGHT, GLFW_KEY_D, "move_right", 0, "Right", "Movement");
	config_register_key(WINDOW_KEY_SPACE, GLFW_KEY_SPACE, "jump", 0, "Jump", "Movement");
	config_register_key(WINDOW_KEY_SPRINT, GLFW_KEY_LEFT_SHIFT, "sprint", 0, "Sprint", "Movement");
	config_register_key(WINDOW_KEY_SHIFT, GLFW_KEY_LEFT_SHIFT, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_CURSOR_UP, GLFW_KEY_UP, "cube_color_up", 0, "Color up", "Block");
	config_register_key(WINDOW_KEY_CURSOR_DOWN, GLFW_KEY_DOWN, "cube_color_down", 0, "Color down", "Block");
	config_register_key(WINDOW_KEY_CURSOR_LEFT, GLFW_KEY_LEFT, "cube_color_left", 0, "Color left", "Block");
	config_register_key(WINDOW_KEY_CURSOR_RIGHT, GLFW_KEY_RIGHT, "cube_color_right", 0, "Color right", "Block");
	config_register_key(WINDOW_KEY_BACKSPACE, GLFW_KEY_BACKSPACE, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_TOOL1, GLFW_KEY_1, "tool_spade", 0, "Select spade", "Tools & Weapons");
	config_register_key(WINDOW_KEY_TOOL2, GLFW_KEY_2, "tool_block", 0, "Select block", "Tools & Weapons");
	config_register_key(WINDOW_KEY_TOOL3, GLFW_KEY_3, "tool_gun", 0, "Select gun", "Tools & Weapons");
	config_register_key(WINDOW_KEY_TOOL4, GLFW_KEY_4, "tool_grenade", 0, "Select grenade", "Tools & Weapons");
	config_register_key(WINDOW_KEY_TAB, GLFW_KEY_TAB, "view_score", 0, "Score", "Information");
	config_register_key(WINDOW_KEY_ESCAPE, GLFW_KEY_ESCAPE, "quit_game", 0, "Quit", "Game");
	config_register_key(WINDOW_KEY_MAP, GLFW_KEY_M, "view_map", 1, "Map", "Information");
	config_register_key(WINDOW_KEY_CROUCH, GLFW_KEY_LEFT_CONTROL, "crouch", 0, "Crouch", "Movement");
	config_register_key(WINDOW_KEY_SNEAK, GLFW_KEY_V, "sneak", 0, "Sneak", "Movement");
	config_register_key(WINDOW_KEY_ENTER, GLFW_KEY_ENTER, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_F1, GLFW_KEY_F1, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_F2, GLFW_KEY_F2, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_F3, GLFW_KEY_F3, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_F4, GLFW_KEY_F4, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_YES, GLFW_KEY_Y, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_YES, GLFW_KEY_Z, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_NO, GLFW_KEY_N, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_VOLUME_UP, GLFW_KEY_KP_ADD, "volume_up", 0, "Volume up", "Game");
	config_register_key(WINDOW_KEY_VOLUME_DOWN, GLFW_KEY_KP_SUBTRACT, "volume_down", 0, "Volume down", "Game");
	config_register_key(WINDOW_KEY_V, GLFW_KEY_V, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_RELOAD, GLFW_KEY_R, "reload", 0, "Reload", "Tools & Weapons");
	config_register_key(WINDOW_KEY_CHAT, GLFW_KEY_T, "chat_global", 0, "Chat", "Game");
	config_register_key(WINDOW_KEY_FULLSCREEN, GLFW_KEY_F11, "fullscreen", 0, "Fullscreen", "Game");
	config_register_key(WINDOW_KEY_SCREENSHOT, GLFW_KEY_F5, "screenshot", 0, "Screenshot", "Game");
	config_register_key(WINDOW_KEY_CHANGETEAM, GLFW_KEY_COMMA, "change_team", 0, "Team select", "Game");
	config_register_key(WINDOW_KEY_CHANGEWEAPON, GLFW_KEY_PERIOD, "change_weapon", 0, "Gun select", "Tools & Weapons");
	config_register_key(WINDOW_KEY_PICKCOLOR, GLFW_KEY_E, "cube_color_sample", 0, "Pick color", "Block");
	config_register_key(WINDOW_KEY_COMMAND, GLFW_KEY_SLASH, "chat_command", 0, "Command", "Game");
	config_register_key(WINDOW_KEY_HIDEHUD, GLFW_KEY_F6, "hide_hud", 1, "Hide HUD", "Game");
	config_register_key(WINDOW_KEY_LASTTOOL, GLFW_KEY_Q, "last_tool", 0, "Last tool", "Tools & Weapons");
	config_register_key(WINDOW_KEY_NETWORKSTATS, GLFW_KEY_F12, "network_stats", 1, "Network stats", "Information");
	config_register_key(WINDOW_KEY_SAVE_MAP, GLFW_KEY_F9, "save_map", 0, "Save map", "Game");
	config_register_key(WINDOW_KEY_SELECT1, GLFW_KEY_1, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_SELECT2, GLFW_KEY_2, NULL, 0, NULL, NULL);
	config_register_key(WINDOW_KEY_SELECT3, GLFW_KEY_3, NULL, 0, NULL, NULL);
#endif

	list_sort(&config_keys, config_key_cmp);

	auto s = (const char*) file_load("config.ini");
	if(s) {
		ini_parse_string(s, config_read_key, NULL);
		free((void*) s);
	}

	if(!list_created(&config_settings))
		list_create(&config_settings, sizeof(struct config_setting));
	else
		list_clear(&config_settings);

	config_setting player_name {
		settings_tmp.name,
		CONFIG_TYPE_STRING,
		0, sizeof(settings.name) - 1,
		"Name", "Ingame player name"
	};

	config_setting mouse_sensitivity {
		&settings_tmp.mouse_sensitivity,
		CONFIG_TYPE_FLOAT,
		0, INT_MAX,
		"Mouse sensitivity",
	};

	config_setting camera_fov {
		&settings_tmp.camera_fov,
		CONFIG_TYPE_FLOAT,
		{ .f = CAMERA_DEFAULT_FOV },
		{ .f = CAMERA_MAX_FOV },
		"Camera FOV",
		"Field of View in degrees",
	};

	config_setting volume {
		&settings_tmp.volume,
		CONFIG_TYPE_INT,
		0, 10, "Volume",
	};

	config_setting window_width {
		&settings_tmp.window_width,
		CONFIG_TYPE_INT, 0, INT_MAX,
		"Game width", "Default: 800",
		{ 640, 800, 854, 1024, 1280, 1920, 3840 }, 7,
		config_label_pixels,
	};

	config_setting window_height {
		&settings_tmp.window_height,
		CONFIG_TYPE_INT, 0, INT_MAX,
		"Game height", "Default: 600",
		{ 480, 600, 720, 768, 1024, 1080, 2160 }, 7,
		config_label_pixels,
	};

	config_setting vsync {
		&settings_tmp.vsync,
		CONFIG_TYPE_INT, 0, INT_MAX,
		"V-Sync", "Limits your game's fps",
		{ 0, 1, 60, 120, 144, 240 }, 6,
		config_label_vsync,
	};

	config_setting fullscreen {
		&settings_tmp.fullscreen,
		CONFIG_TYPE_INT, 0, 1, "Fullscreen",
	};

	config_setting multisamples {
		&settings_tmp.multisamples,
		CONFIG_TYPE_INT, 0, 16,
		"Multisamples", "Smooth out block edges",
		{ 0, 2, 4, 8, 16 }, 5, config_label_msaa,
	};

	config_setting chat_shadow {
		&settings_tmp.chat_shadow,
		CONFIG_TYPE_INT, 0, 1,
		"Chat shadow", "Dark chat background",
	};

	config_setting voxlap_models {
		&settings_tmp.voxlap_models,
		CONFIG_TYPE_INT, 0, 1,
		"Voxlap models", "Render models like in voxlap",
	};

	config_setting hold_down_sights {
		&settings_tmp.hold_down_sights,
		CONFIG_TYPE_INT, 0, 1,
		"Hold down sights", "Only aim while pressing RMB",
	};

	config_setting greedy_meshing {
		&settings_tmp.greedy_meshing,
		CONFIG_TYPE_INT, 0, 1,
		"Greedy meshing", "Join similar mesh faces",
	};

	config_setting force_displaylist {
		&settings_tmp.force_displaylist,
		CONFIG_TYPE_INT, 0, 1,
		"Force Displaylist", "Enable this on buggy drivers",
	};

	config_setting smooth_fog {
		&settings_tmp.smooth_fog,
		CONFIG_TYPE_INT, 0, 1,
		"Smooth fog", "Enable this on buggy drivers",
	};

	config_setting ambient_occlusion {
		&settings_tmp.ambient_occlusion,
		CONFIG_TYPE_INT, 0, 1,
		"Ambient occlusion", "(won't work with greedy mesh)",
	};

	config_setting show_fps {
		&settings_tmp.show_fps,
		CONFIG_TYPE_INT, 0, 1,
		"Show fps", "Show fps and ping ingame",
	};

	config_setting invert_y {
		&settings_tmp.invert_y,
		CONFIG_TYPE_INT, 0, 1,
		"Invert y", "Invert vertical mouse movement",
	};

	config_setting show_news {
		&settings_tmp.show_news,
		CONFIG_TYPE_INT, 0, 1,
		"Show news", "Show news on server list",
	};

	list_add(&config_settings, &player_name);
	list_add(&config_settings, &mouse_sensitivity);
	list_add(&config_settings, &camera_fov);
	list_add(&config_settings, &volume);
	list_add(&config_settings, &window_width);
	list_add(&config_settings, &window_height);
	list_add(&config_settings, &vsync);
	list_add(&config_settings, &fullscreen);
	list_add(&config_settings, &multisamples);
	list_add(&config_settings, &chat_shadow);
	list_add(&config_settings, &voxlap_models);
	list_add(&config_settings, &hold_down_sights);
	list_add(&config_settings, &greedy_meshing);
	list_add(&config_settings, &force_displaylist);
	list_add(&config_settings, &smooth_fog);
	list_add(&config_settings, &ambient_occlusion);
	list_add(&config_settings, &show_fps);
	list_add(&config_settings, &invert_y);
	list_add(&config_settings, &show_news);
}

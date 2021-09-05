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
#include <string.h>

#include <common.hpp>
#include <main.hpp>
#include <window.hpp>
#include <config.hpp>
#include <hud.hpp>

#ifdef OS_WINDOWS
#include <sysinfoapi.h>
#include <windows.h>
#endif

#ifdef OS_LINUX
#include <unistd.h>
#endif

void window_textinput(int allow) { }

void window_setmouseloc(double x, double y) { }

static void window_impl_mouseclick(GLFWwindow* window, int button, int action, int mods) {
    int b = 0;
    switch(button) {
        case GLFW_MOUSE_BUTTON_LEFT: b = WINDOW_MOUSE_LMB; break;
        case GLFW_MOUSE_BUTTON_RIGHT: b = WINDOW_MOUSE_RMB; break;
        case GLFW_MOUSE_BUTTON_MIDDLE: b = WINDOW_MOUSE_MMB; break;
    }

    int a = -1;
    switch(action) {
        case GLFW_RELEASE: a = WINDOW_RELEASE; break;
        case GLFW_PRESS: a = WINDOW_PRESS; break;
    }

    if(a >= 0)
        mouse_click(hud_window, b, a, mods & GLFW_MOD_CONTROL);
}
static void window_impl_mouse(GLFWwindow* window, double x, double y) {
    mouse(hud_window, x, y);
}
static void window_impl_mousescroll(GLFWwindow* window, double xoffset, double yoffset) {
    mouse_scroll(hud_window, xoffset, yoffset);
}
static void window_impl_error(int i, const char* s) {
    on_error(i, s);
}
static void window_impl_reshape(GLFWwindow* window, int width, int height) {
    reshape(hud_window, width, height);
}
static void window_impl_textinput(GLFWwindow* window, unsigned int codepoint) {
    text_input(hud_window, codepoint);
}
static void window_impl_keys(GLFWwindow* window, int key, int scancode, int action, int mods) {
    int count = config_key_translate(key, 0, NULL);

    int a = -1;
    switch(action) {
        case GLFW_RELEASE: a = WINDOW_RELEASE; break;
        case GLFW_PRESS: a = WINDOW_PRESS; break;
        case GLFW_REPEAT: a = WINDOW_REPEAT; break;
    }

    if(count > 0) {
        int results[count];
        config_key_translate(key, 0, results);

        for(int k = 0; k < count; k++) {
            keys(hud_window, results[k], scancode, a, mods & GLFW_MOD_CONTROL);

            if(hud_active->input_keyboard)
                hud_active->input_keyboard(results[k], action, mods & GLFW_MOD_CONTROL, key);
        }
    } else {
        if(hud_active->input_keyboard)
            hud_active->input_keyboard(WINDOW_KEY_UNKNOWN, action, mods & GLFW_MOD_CONTROL, key);
    }
}

// https://github.com/glfw/glfw/blob/master/tests/events.c
const char* get_function_key_name(int keycode) {
    switch (keycode) {
        case GLFW_KEY_ESCAPE:        return "ESCAPE";
        case GLFW_KEY_SPACE:         return "SPACE";
        case GLFW_KEY_F1:            return "F1";
        case GLFW_KEY_F2:            return "F2";
        case GLFW_KEY_F3:            return "F3";
        case GLFW_KEY_F4:            return "F4";
        case GLFW_KEY_F5:            return "F5";
        case GLFW_KEY_F6:            return "F6";
        case GLFW_KEY_F7:            return "F7";
        case GLFW_KEY_F8:            return "F8";
        case GLFW_KEY_F9:            return "F9";
        case GLFW_KEY_F10:           return "F10";
        case GLFW_KEY_F11:           return "F11";
        case GLFW_KEY_F12:           return "F12";
        case GLFW_KEY_F13:           return "F13";
        case GLFW_KEY_F14:           return "F14";
        case GLFW_KEY_F15:           return "F15";
        case GLFW_KEY_F16:           return "F16";
        case GLFW_KEY_F17:           return "F17";
        case GLFW_KEY_F18:           return "F18";
        case GLFW_KEY_F19:           return "F19";
        case GLFW_KEY_F20:           return "F20";
        case GLFW_KEY_F21:           return "F21";
        case GLFW_KEY_F22:           return "F22";
        case GLFW_KEY_F23:           return "F23";
        case GLFW_KEY_F24:           return "F24";
        case GLFW_KEY_F25:           return "F25";
        case GLFW_KEY_UP:            return "UP";
        case GLFW_KEY_DOWN:          return "DOWN";
        case GLFW_KEY_LEFT:          return "LEFT";
        case GLFW_KEY_RIGHT:         return "RIGHT";
        case GLFW_KEY_LEFT_SHIFT:    return "LEFT SHIFT";
        case GLFW_KEY_RIGHT_SHIFT:   return "RIGHT SHIFT";
        case GLFW_KEY_LEFT_CONTROL:  return "LEFT CONTROL";
        case GLFW_KEY_RIGHT_CONTROL: return "RIGHT CONTROL";
        case GLFW_KEY_LEFT_ALT:      return "LEFT ALT";
        case GLFW_KEY_RIGHT_ALT:     return "RIGHT ALT";
        case GLFW_KEY_TAB:           return "TAB";
        case GLFW_KEY_ENTER:         return "ENTER";
        case GLFW_KEY_BACKSPACE:     return "BACKSPACE";
        case GLFW_KEY_INSERT:        return "INSERT";
        case GLFW_KEY_DELETE:        return "DELETE";
        case GLFW_KEY_PAGE_UP:       return "PAGE UP";
        case GLFW_KEY_PAGE_DOWN:     return "PAGE DOWN";
        case GLFW_KEY_HOME:          return "HOME";
        case GLFW_KEY_END:           return "END";
        case GLFW_KEY_KP_0:          return "KEYPAD 0";
        case GLFW_KEY_KP_1:          return "KEYPAD 1";
        case GLFW_KEY_KP_2:          return "KEYPAD 2";
        case GLFW_KEY_KP_3:          return "KEYPAD 3";
        case GLFW_KEY_KP_4:          return "KEYPAD 4";
        case GLFW_KEY_KP_5:          return "KEYPAD 5";
        case GLFW_KEY_KP_6:          return "KEYPAD 6";
        case GLFW_KEY_KP_7:          return "KEYPAD 7";
        case GLFW_KEY_KP_8:          return "KEYPAD 8";
        case GLFW_KEY_KP_9:          return "KEYPAD 9";
        case GLFW_KEY_KP_DIVIDE:     return "KEYPAD DIVIDE";
        case GLFW_KEY_KP_MULTIPLY:   return "KEYPAD MULTIPLY";
        case GLFW_KEY_KP_SUBTRACT:   return "KEYPAD SUBTRACT";
        case GLFW_KEY_KP_ADD:        return "KEYPAD ADD";
        case GLFW_KEY_KP_DECIMAL:    return "KEYPAD DECIMAL";
        case GLFW_KEY_KP_EQUAL:      return "KEYPAD EQUAL";
        case GLFW_KEY_KP_ENTER:      return "KEYPAD ENTER";
        case GLFW_KEY_PRINT_SCREEN:  return "PRINT SCREEN";
        case GLFW_KEY_NUM_LOCK:      return "NUM LOCK";
        case GLFW_KEY_CAPS_LOCK:     return "CAPS LOCK";
        case GLFW_KEY_SCROLL_LOCK:   return "SCROLL LOCK";
        case GLFW_KEY_PAUSE:         return "PAUSE";
        case GLFW_KEY_LEFT_SUPER:    return "LEFT SUPER";
        case GLFW_KEY_RIGHT_SUPER:   return "RIGHT SUPER";
        case GLFW_KEY_MENU:          return "MENU";
        default:                     return "?";
    }
}

void window_keyname(int keycode, char* output, size_t length) {
#ifdef OS_WINDOWS
    GetKeyNameTextA(glfwGetKeyScancode(keycode) << 16, output, length);
#else
    const char* name = glfwGetKeyName(keycode, 0);

    if (name) strncpy(output, name, length);
    else strncpy(output, get_function_key_name(keycode), length);

    output[length - 1] = 0;
#endif
}

float window_time() {
    return glfwGetTime();
}

int window_pressed_keys[64] = {0};

const char* window_clipboard() {
    return glfwGetClipboardString((GLFWwindow*) hud_window->impl);
}

int window_key_down(int key) {
    return window_pressed_keys[key];
}

void window_mousemode(int mode) {
    int s = glfwGetInputMode((GLFWwindow*) hud_window->impl, GLFW_CURSOR);
    if((s == GLFW_CURSOR_DISABLED && mode == WINDOW_CURSOR_ENABLED)
       || (s == GLFW_CURSOR_NORMAL && mode == WINDOW_CURSOR_DISABLED))
        glfwSetInputMode((GLFWwindow*) hud_window->impl, GLFW_CURSOR,
                         mode == WINDOW_CURSOR_ENABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void window_mouseloc(double* x, double* y) {
    glfwGetCursorPos((GLFWwindow*) hud_window->impl, x, y);
}

void window_swapping(int value) {
    glfwSwapInterval(value);
}

void window_title(char* suffix) {
    if(suffix) {
        char title[128];
        snprintf(title, sizeof(title) - 1, "BetterSpades %s - %s", BETTERSPADES_VERSION, suffix);
        glfwSetWindowTitle((GLFWwindow*) hud_window->impl, title);
    } else {
        glfwSetWindowTitle((GLFWwindow*) hud_window->impl, "BetterSpades " BETTERSPADES_VERSION);
    }
}

void window_init() {
    static struct window_instance i;
    hud_window = &i;

    glfwWindowHint(GLFW_VISIBLE, 0);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwSetErrorCallback(window_impl_error);

    if(!glfwInit()) {
        log_fatal("GLFW3 init failed");
        exit(1);
    }

    if(settings.multisamples > 0) {
        glfwWindowHint(GLFW_SAMPLES, settings.multisamples);
    }

    hud_window->impl
        = glfwCreateWindow(settings.window_width, settings.window_height, "BetterSpades " BETTERSPADES_VERSION,
                           settings.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    if(!hud_window->impl) {
        log_fatal("Could not open window");
        glfwTerminate();
        exit(1);
    }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos((GLFWwindow*) hud_window->impl, (mode->width - settings.window_width) / 2.0F,
                     (mode->height - settings.window_height) / 2.0F);
    glfwShowWindow((GLFWwindow*) hud_window->impl);

    glfwMakeContextCurrent((GLFWwindow*) hud_window->impl);

    glfwSetFramebufferSizeCallback((GLFWwindow*) hud_window->impl, window_impl_reshape);
    glfwSetCursorPosCallback((GLFWwindow*) hud_window->impl, window_impl_mouse);
    glfwSetKeyCallback((GLFWwindow*) hud_window->impl, window_impl_keys);
    glfwSetMouseButtonCallback((GLFWwindow*) hud_window->impl, window_impl_mouseclick);
    glfwSetScrollCallback((GLFWwindow*) hud_window->impl, window_impl_mousescroll);
    glfwSetCharCallback((GLFWwindow*) hud_window->impl, window_impl_textinput);

    if(glfwRawMouseMotionSupported())
        glfwSetInputMode((GLFWwindow*) hud_window->impl, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void window_fromsettings() {
    glfwWindowHint(GLFW_SAMPLES, settings.multisamples);
    glfwSetWindowSize((GLFWwindow*) hud_window->impl, settings.window_width, settings.window_height);

    if(settings.vsync < 2)
        window_swapping(settings.vsync);
    if(settings.vsync > 1)
        window_swapping(0);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if(settings.fullscreen)
        glfwSetWindowMonitor((GLFWwindow*) hud_window->impl, glfwGetPrimaryMonitor(), 0, 0, settings.window_width,
                             settings.window_height, mode->refreshRate);
    else
        glfwSetWindowMonitor((GLFWwindow*) hud_window->impl, NULL, (mode->width - settings.window_width) / 2,
                             (mode->height - settings.window_height) / 2, settings.window_width, settings.window_height,
                             0);
}

void window_deinit() {
    glfwTerminate();
}

void window_update() {
    glfwSwapBuffers((GLFWwindow*) hud_window->impl);
    glfwPollEvents();
}

int window_closed() {
    return glfwWindowShouldClose((GLFWwindow*) hud_window->impl);
}

int window_cpucores() {
#ifdef OS_LINUX
    return get_nprocs();
#endif

#ifdef OS_WINDOWS
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
#endif
    return 1;
}

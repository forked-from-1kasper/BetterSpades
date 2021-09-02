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

#ifdef USE_GLFW

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
#ifdef OPENGL_ES
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
#endif

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

#endif

#ifdef USE_SDL

void window_textinput(int allow) {
    if(allow && !SDL_IsTextInputActive())
        SDL_StartTextInput();
    if(!allow && SDL_IsTextInputActive())
        SDL_StopTextInput();
}

void window_fromsettings() {
    SDL_SetWindowSize(hud_window->impl, settings.window_width, settings.window_height);

    if(settings.vsync < 2)
        window_swapping(settings.vsync);
    if(settings.vsync > 1)
        window_swapping(0);

    if(settings.fullscreen)
        SDL_SetWindowFullscreen(hud_window->impl, SDL_WINDOW_FULLSCREEN);
    else
        SDL_SetWindowFullscreen(hud_window->impl, 0);
}

void window_keyname(int keycode, char* output, size_t length) {
    strncpy(output, SDL_GetKeyName(keycode), length);
    output[length - 1] = 0;
}

float window_time() {
    return ((double)SDL_GetTicks()) / 1000.0F;
}

int window_pressed_keys[64] = {0};

const char* window_clipboard() {
    return SDL_HasClipboardText() ? SDL_GetClipboardText() : NULL;
}

int window_key_down(int key) {
    return window_pressed_keys[key];
}

void window_mousemode(int mode) {
    int s = SDL_GetRelativeMouseMode();
    if((s && mode == WINDOW_CURSOR_ENABLED) || (!s && mode == WINDOW_CURSOR_DISABLED))
        SDL_SetRelativeMouseMode(mode == WINDOW_CURSOR_ENABLED ? 0 : 1);
}

static double mx = -1, my = -1;

void window_setmouseloc(double x, double y) {
    mx = x;
    my = y;
}

void window_mouseloc(double* x, double* y) {
    if(mx < 0 && my < 0) {
        int xi, yi;
        SDL_GetMouseState(&xi, &yi);
        *x = xi;
        *y = yi;
    } else {
        *x = mx;
        *y = my;
    }
}

void window_swapping(int value) {
    SDL_GL_SetSwapInterval(value);
}

static struct window_finger fingers[8];

void window_init() {
    static struct window_instance i;
    hud_window = &i;

#ifdef USE_TOUCH
    SDL_SetHintWithPriority(SDL_HINT_ANDROID_SEPARATE_MOUSE_AND_TOUCH, "1", SDL_HINT_OVERRIDE);
#endif

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);

    hud_window->impl
        = SDL_CreateWindow("BetterSpades " BETTERSPADES_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           settings.window_width, settings.window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef OPENGL_ES
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif
    SDL_GLContext* ctx = SDL_GL_CreateContext(hud_window->impl);

    memset(fingers, 0, sizeof(fingers));
}

void window_deinit() {
    SDL_DestroyWindow(hud_window->impl);
    SDL_Quit();
}

static int quit = 0;
void window_update() {
    SDL_GL_SwapWindow(hud_window->impl);
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT: quit = 1; break;
            case SDL_KEYDOWN: {
                int count = config_key_translate(event.key.keysym.sym, 0, NULL);

                if(count > 0) {
                    int results[count];
                    config_key_translate(event.key.keysym.sym, 0, results);

                    for(int k = 0; k < count; k++) {
                        keys(hud_window, results[k], event.key.keysym.sym, WINDOW_PRESS,
                             event.key.keysym.mod & KMOD_CTRL);

                        if(hud_active->input_keyboard)
                            hud_active->input_keyboard(results[k], WINDOW_PRESS, event.key.keysym.mod & KMOD_CTRL,
                                                       event.key.keysym.sym);
                    }
                } else {
                    if(hud_active->input_keyboard)
                        hud_active->input_keyboard(WINDOW_KEY_UNKNOWN, WINDOW_PRESS, event.key.keysym.mod & KMOD_CTRL,
                                                   event.key.keysym.sym);
                }
                break;
            }
            case SDL_KEYUP: {
                int count = config_key_translate(event.key.keysym.sym, 0, NULL);

                if(count > 0) {
                    int results[count];
                    config_key_translate(event.key.keysym.sym, 0, results);

                    for(int k = 0; k < count; k++) {
                        keys(hud_window, results[k], event.key.keysym.sym, WINDOW_RELEASE,
                             event.key.keysym.mod & KMOD_CTRL);

                        if(hud_active->input_keyboard)
                            hud_active->input_keyboard(results[k], WINDOW_RELEASE, event.key.keysym.mod & KMOD_CTRL,
                                                       event.key.keysym.sym);
                    }
                } else {
                    if(hud_active->input_keyboard)
                        hud_active->input_keyboard(WINDOW_KEY_UNKNOWN, WINDOW_RELEASE, event.key.keysym.mod & KMOD_CTRL,
                                                   event.key.keysym.sym);
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                int a = 0;
                switch(event.button.button) {
                    case SDL_BUTTON_LEFT: a = WINDOW_MOUSE_LMB; break;
                    case SDL_BUTTON_RIGHT: a = WINDOW_MOUSE_RMB; break;
                    case SDL_BUTTON_MIDDLE: a = WINDOW_MOUSE_MMB; break;
                }
                mouse_click(hud_window, a, WINDOW_PRESS, 0);
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                int a = 0;
                switch(event.button.button) {
                    case SDL_BUTTON_LEFT: a = WINDOW_MOUSE_LMB; break;
                    case SDL_BUTTON_RIGHT: a = WINDOW_MOUSE_RMB; break;
                    case SDL_BUTTON_MIDDLE: a = WINDOW_MOUSE_MMB; break;
                }
                mouse_click(hud_window, a, WINDOW_RELEASE, 0);
                break;
            }
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_RESIZED
                   || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    reshape(hud_window, event.window.data1, event.window.data2);
                }
                break;
            case SDL_MOUSEWHEEL: mouse_scroll(hud_window, event.wheel.x, event.wheel.y); break;
            case SDL_MOUSEMOTION: {
                if(SDL_GetRelativeMouseMode()) {
                    static int x, y;
                    x += event.motion.xrel;
                    y += event.motion.yrel;
                    mouse(hud_window, x, y);
                } else {
                    mouse(hud_window, event.motion.x, event.motion.y);
                }
                break;
            }
            case SDL_TEXTINPUT: text_input(hud_window, event.text.text[0]); break;
            case SDL_FINGERDOWN:
                if(hud_active->input_touch) {
                    struct window_finger* f;
                    for(int k = 0; k < 8; k++) {
                        if(!fingers[k].full) {
                            fingers[k].finger = event.tfinger.fingerId;
                            fingers[k].start.x = event.tfinger.x * settings.window_width;
                            fingers[k].start.y = event.tfinger.y * settings.window_height;
                            fingers[k].down_time = window_time();
                            fingers[k].full = 1;
                            f = fingers + k;
                            break;
                        }
                    }

                    hud_active->input_touch(f, TOUCH_DOWN, event.tfinger.x * settings.window_width,
                                            event.tfinger.y * settings.window_height,
                                            event.tfinger.dx * settings.window_width,
                                            event.tfinger.dy * settings.window_height);
                }
                break;
            case SDL_FINGERUP:
                if(hud_active->input_touch) {
                    struct window_finger* f;
                    for(int k = 0; k < 8; k++) {
                        if(fingers[k].full && fingers[k].finger == event.tfinger.fingerId) {
                            fingers[k].full = 0;
                            f = fingers + k;
                            break;
                        }
                    }
                    hud_active->input_touch(
                        f, TOUCH_UP, event.tfinger.x * settings.window_width, event.tfinger.y * settings.window_height,
                        event.tfinger.dx * settings.window_width, event.tfinger.dy * settings.window_height);
                }
                break;
            case SDL_FINGERMOTION:
                if(hud_active->input_touch) {
                    struct window_finger* f;
                    for(int k = 0; k < 8; k++) {
                        if(fingers[k].full && fingers[k].finger == event.tfinger.fingerId) {
                            f = fingers + k;
                            break;
                        }
                    }
                    hud_active->input_touch(f, TOUCH_MOVE, event.tfinger.x * settings.window_width,
                                            event.tfinger.y * settings.window_height,
                                            event.tfinger.dx * settings.window_width,
                                            event.tfinger.dy * settings.window_height);
                }
                break;
        }
    }
}

int window_closed() {
    return quit;
}

void window_title(char* suffix) {
    if(suffix) {
        char title[128];
        snprintf(title, sizeof(title) - 1, "BetterSpades %s - %s", BETTERSPADES_VERSION, suffix);
        SDL_SetWindowTitle(hud_window->impl, title);
    } else {
        SDL_SetWindowTitle(hud_window->impl, "BetterSpades " BETTERSPADES_VERSION);
    }
}

#endif

int window_cpucores() {
#ifdef OS_LINUX
#ifdef USE_TOUCH
    return sysconf(_SC_NPROCESSORS_CONF);
#else
    return get_nprocs();
#endif
#endif
#ifdef OS_WINDOWS
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
#endif
    return 1;
}

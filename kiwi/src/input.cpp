// input.cpp

#include "input.hpp"
#include <GLFW/glfw3.h>

int mouse_t::x() const
{
    return m_x;
}

int mouse_t::y() const
{
    return m_y;
}

int mouse_t::x_delta() const
{
    return m_delta_x;
}

int mouse_t::y_delta() const
{
    return m_delta_y;
}

int mouse_t::scroll() const
{
    return m_scroll;
}

bool mouse_t::down(const int index) const
{
    return m_buttons[index].down;
}

bool mouse_t::pressed(const int index) const
{
    return m_buttons[index].pressed;
}

bool mouse_t::released(const int index) const
{
    return m_buttons[index].released;
}

bool keyboard_t::down(const int index) const
{
    return m_keys[index].down;
}

bool keyboard_t::pressed(const int index) const
{
    return m_keys[index].pressed;
}

bool keyboard_t::released(const int index) const
{
    return m_keys[index].released;
}

input_handler_t::input_handler_t(mouse_t& mouse, keyboard_t& keyboard)
    : m_mouse(mouse)
    , m_keyboard(keyboard)
{
}

void input_handler_t::on_mouse_move(const int x, const int y)
{
    m_mouse.m_delta_x = m_mouse.m_x - x;
    m_mouse.m_delta_y = m_mouse.m_y - y;

    m_mouse.m_x = x;
    m_mouse.m_y = y;
}

void input_handler_t::on_key_pressed(const int index)
{
    m_keyboard.m_keys[index].down = true;
    m_keyboard.m_keys[index].pressed = true;
    m_keyboard.m_keys[index].released = false;
}

void input_handler_t::on_key_released(const int index)
{
    m_keyboard.m_keys[index].down = false;
    m_keyboard.m_keys[index].pressed = false;
    m_keyboard.m_keys[index].released = true;
}

void input_handler_t::on_button_pressed(const int index)
{
    m_mouse.m_buttons[index].down = true;
    m_mouse.m_buttons[index].pressed = true;
    m_mouse.m_buttons[index].released = false;
}

void input_handler_t::on_button_released(const int index)
{
    m_mouse.m_buttons[index].down = false;
    m_mouse.m_buttons[index].pressed = false;
    m_mouse.m_buttons[index].released = true;
}

void input_handler_t::post_frame_update()
{
    m_mouse.m_delta_x = 0;
    m_mouse.m_delta_y = 0;
    m_mouse.m_scroll = 0;
    for (auto& button : m_mouse.m_buttons) {
        button.pressed = false;
        button.released = false;
    }

    for (auto& key : m_keyboard.m_keys) {
        key.pressed = false;
        key.released = false;
    }
}

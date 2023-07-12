// input.hpp

#pragma once

struct mouse_t {
    mouse_t() = default;

    int x() const;
    int y() const;
    int x_delta() const;
    int y_delta() const;
    int scroll() const;

    bool down(const int index) const;
    bool pressed(const int index) const;
    bool released(const int index) const;

    int m_x = 0;
    int m_y = 0;
    int m_delta_x = 0;
    int m_delta_y = 0;
    int m_scroll = 0;
    struct {
        bool down;
        bool pressed;
        bool released;
    } m_buttons[8] = {};
};

struct keyboard_t {
    keyboard_t() = default;

    bool down(const int index) const;
    bool pressed(const int index) const;
    bool released(const int index) const;

    struct {
        bool down;
        bool pressed;
        bool released;
    } m_keys[512] = {};
};

struct input_handler_t {
    input_handler_t(mouse_t& mouse, keyboard_t& keyboard);

    void on_mouse_move(const int x, const int y);
    void on_key_pressed(const int index);
    void on_key_released(const int index);
    void on_button_pressed(const int index);
    void on_button_released(const int index);
    void post_frame_update();

    mouse_t& m_mouse;
    keyboard_t& m_keyboard;
};

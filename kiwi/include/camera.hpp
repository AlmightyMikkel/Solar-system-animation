#pragma once

#include <glm/glm.hpp>
#include <iostream>


struct mouse_t;
struct keyboard_t;
struct timespan_t;

struct camera_t {
    camera_t();
    camera_t(const glm::mat4& projection);

    void update();

    void strafe(const float amount);
    void elevate(const float amount);
    void forward(const float amount);
    void rotate_x(const float amount);
    void rotate_y(const float amount);
    void rotate_z(const float amount);
    glm::mat4 get_viewmat();

    void set_perspective(const float fovy,
        const float aspect,
        const float znear = 1.0f,
        const float zfar = 100.0f);
    void set_projection(const glm::mat4& projection);

    float     m_pitch = 0.0f;
    float     m_yaw = 0.0f;
    float     m_roll = 0.0f;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_forward;
    glm::vec3 m_position;
    glm::mat4 m_projection;
    glm::mat4 m_view;
};

struct first_person_camera_controller_t {
    static constexpr float default_movement_speed = 10.0f;
    static constexpr float default_mouse_sensitivity = 0.001f;

    struct key_mapping_t {
        int32_t forward;
        int32_t backward;
        int32_t left;
        int32_t right;
        int32_t up;
        int32_t down;
    };

    first_person_camera_controller_t(camera_t& camera);

    void update(const mouse_t& mouse,
        const keyboard_t& keyboard,
        const timespan_t& deltatime);

    void set_camera_speed(const float speed);
    void set_invert_yaw(const bool invert);
    void set_invert_pitch(const bool invert);
    void set_sensitivity(const float sensitivity);
    void set_key_mapping(const key_mapping_t& mapping);

    camera_t& m_camera;
    key_mapping_t m_key_mapping = {};
    bool          m_invert_pitch = true;
    bool          m_invert_yaw = true;
    float         m_movement_speed = default_movement_speed;
    float         m_mouse_sensitivity = default_mouse_sensitivity;
};

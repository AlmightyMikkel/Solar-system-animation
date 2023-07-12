// camera.cpp

#include "camera.hpp"
#include "input.hpp"
#include "system.hpp"

#include <numbers> 
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

camera_t::camera_t()
    : m_right(1.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_forward(0.0f, 0.0f, -1.0f)
    , m_position(0.0f)
    , m_projection(1.0f)
    , m_view(1.0f)
{
}

camera_t::camera_t(const glm::mat4& projection)
    : m_right(1.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_forward(0.0f, 0.0f, -1.0f)
    , m_position(0.0f)
    , m_projection(projection)
    , m_view(1.0f)
{
}

void camera_t::update()
{
    // note: unit axis
    glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
    glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
    glm::vec3 z_axis(0.0f, 0.0f, 1.0f);

    // note: construct rotation quaternions from angles and axis
    glm::quat x_rotation = glm::angleAxis(m_pitch, x_axis);
    glm::quat y_rotation = glm::angleAxis(m_yaw, y_axis);
    glm::quat z_rotation = glm::angleAxis(m_roll, z_axis);

    // note: orientation for the view matrix
    glm::quat orientation = glm::normalize(x_rotation * y_rotation * z_rotation);
    glm::mat4 rotation = glm::mat4_cast(orientation);

    // note: move the camera
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_position);

    // note: final view matrix with orientation and translation
    m_view = rotation * translation;

    // note: calculate helper movement vectors
    m_right = glm::normalize(glm::conjugate(orientation) * x_axis);
    m_up = glm::normalize(glm::conjugate(orientation) * y_axis);
    m_forward = glm::normalize(glm::conjugate(orientation) * z_axis);
}

void camera_t::strafe(const float amount)
{
    m_position += m_right * amount;
}

void camera_t::elevate(const float amount)
{
    m_position += m_up * amount;
}

void camera_t::forward(const float amount)
{
    m_position += m_forward * amount;
}

void camera_t::rotate_x(const float amount)
{
    m_pitch += amount;
}

void camera_t::rotate_y(const float amount)
{
    m_yaw += amount;
}

void camera_t::rotate_z(const float amount)
{
    m_roll += amount;
}

glm::mat4 camera_t::get_viewmat()
{
    return glm::lookAt(m_position, m_position + m_forward, m_up);
}

void camera_t::set_perspective(const float fovy,
    const float aspect,
    const float znear,
    const float zfar)
{
    m_projection = glm::perspective(fovy, aspect, znear, zfar);
}

void camera_t::set_projection(const glm::mat4& projection)
{
    m_projection = projection;
}

first_person_camera_controller_t::first_person_camera_controller_t(camera_t& camera)
    : m_camera(camera)
    , m_key_mapping{ GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_Q }
{
}

void first_person_camera_controller_t::update(const mouse_t& mouse,
    const keyboard_t& keyboard,
    const timespan_t& deltatime)
{
    const float current_movement_speed = m_movement_speed * deltatime.elapsed_seconds();

    if (keyboard.down(m_key_mapping.forward)) {
        m_camera.forward(current_movement_speed);
    }
    if (keyboard.down(m_key_mapping.backward)) {
        m_camera.forward(-current_movement_speed);
    }
    if (keyboard.down(m_key_mapping.left)) {
        m_camera.strafe(current_movement_speed);
    }
    if (keyboard.down(m_key_mapping.right)) {
        m_camera.strafe(-current_movement_speed);
    }
    if (keyboard.down(m_key_mapping.down)) {
        m_camera.elevate(current_movement_speed);
    }
    if (keyboard.down(m_key_mapping.up)) {
        m_camera.elevate(-current_movement_speed);
    }

    const float yaw_factor = m_invert_yaw ? -1.0f : 1.0f;
    const float pitch_factor = m_invert_pitch ? -1.0f : 1.0f;
    const glm::vec2 mouse_delta(float(mouse.x_delta()), float(mouse.y_delta()));

        m_camera.rotate_y(mouse_delta.x * m_mouse_sensitivity * yaw_factor);
        m_camera.rotate_x(mouse_delta.y * m_mouse_sensitivity * -pitch_factor);


    m_camera.update();
}

void first_person_camera_controller_t::set_camera_speed(const float speed)
{
    m_movement_speed = speed;
}

void first_person_camera_controller_t::set_invert_yaw(const bool invert)
{
    m_invert_yaw = invert;
}

void first_person_camera_controller_t::set_invert_pitch(const bool invert)
{
    m_invert_pitch = invert;
}

void first_person_camera_controller_t::set_sensitivity(const float sensitivity)
{
    m_mouse_sensitivity = sensitivity;
}

void first_person_camera_controller_t::set_key_mapping(const key_mapping_t& mapping)
{
    m_key_mapping = mapping;
}

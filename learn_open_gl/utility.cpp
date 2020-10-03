#include "utility.h"

#include <glfw3.h>

int window_width = 2560;
int window_height = 1440;

void Mytime::update_delta_time() {

    double current_frame_start_time = glfwGetTime();
    delta_time = current_frame_start_time - last_frame_start_time;

    last_frame_start_time = current_frame_start_time;
}

Mytime::Mytime()
    : delta_time{0}, last_frame_start_time{glfwGetTime()} {

}
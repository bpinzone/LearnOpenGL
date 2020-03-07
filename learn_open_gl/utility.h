#ifndef UTILITY_H
#define UTILITY_H

#include <glfw3.h>

#include <memory>

class Time {

public:

    // Call at the top of the rendering loop.
    void update_delta_time() {

        double current_frame_start_time = glfwGetTime();
        delta_time = current_frame_start_time - last_frame_start_time;

        last_frame_start_time = current_frame_start_time;
    }

    double get_delta_time() const {
        return delta_time;
    }

    static Time& get_instance(){
        static Time time;
        return time;
    }

	Time(const Time&) = delete;
	Time(Time&&) = delete;
	Time& operator= (const Time&)  = delete;
	Time& operator= (Time&&) = delete;

private:

    Time()
        : delta_time{0}, last_frame_start_time{glfwGetTime()} {
    }

    double delta_time;
    double last_frame_start_time;
};

#endif
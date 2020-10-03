#ifndef UTILITY_H
#define UTILITY_H

#include <memory>

extern int window_width;
extern int window_height;

class Mytime {

public:

    // Call at the top of the rendering loop.
    void update_delta_time();

    // In seconds
    double get_delta_time() const {
        return delta_time;
    }

    static Mytime& get_instance(){
        static Mytime time;
        return time;
    }

	Mytime(const Mytime&) = delete;
	Mytime(Mytime&&) = delete;
	Mytime& operator= (const Mytime&)  = delete;
	Mytime& operator= (Mytime&&) = delete;

private:

    Mytime();

    double delta_time;
    double last_frame_start_time;
};

#endif
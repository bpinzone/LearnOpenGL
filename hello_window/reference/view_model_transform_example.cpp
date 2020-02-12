// MODEL STUFF
glm::mat4 model = glm::mat4(1.0f);
model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

// ==========================
// ==========================
// VIEW STUFF

// positive z axis is the vector from screen to you.
// glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
// glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
// The name direction vector is not the best chosen name, since it is actually pointing in the reverse direction of what it is targeting.
// glm::vec3 camera_dir = glm::normalize(camera_pos - camera_target);
// glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
// glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_dir));
// glm::vec3 camera_up = glm::cross(camera_dir, camera_right); // this seems unnecessary.
// Want to create lookAt, luckily glm will do it for us.
// glm::mat4 view;
// view = glm::lookAt(
//     glm::vec3(0.0f, 0.0f, 3.0f), // camera pos. (pos z is screen to me)
//     glm::vec3(0.0f, 0.0f, 0.0f), // target, the thing you want to look at.
//     glm::vec3(0.0f, 1.0f, 0.0f)  // up vector for the camera.
// );
// const float radius = 10.0f;
// float cam_x = sin(glfwGetTime()) * radius;
// float cam_z = cos(glfwGetTime()) * radius;
// view = glm::lookAt(glm::vec3(cam_x, 0.0f, cam_z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

// glm::mat4 view = glm::mat4(1.0f);
// note that we're translating the scene in the reverse direction of where we want to move
// view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

// ==========================
// ==========================
// GENERATING (ortho and perspective) projection matrices.


// random learning
// To create an orthographic projection matrix we make use of GLM's built-in function glm::ortho
// ** REMEMBER, THE THINGS GOING INTO THIS ARE ALREADY IN VIEW SPACE!!!!! **
// THIS MATRIX TAKES US FROM VIEW SPACE TO CLIP SPACE. Its one of the two forms a projection matrix takes.
// AND we're still working on vertices!
glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
// The first two parameters specify the left and right coordinate of the frustum and the third and fourth parameter specify the bottom and top part of the frustum. With those 4 points we've defined the size of the near and far planes and the 5th and 6th parameter then define the distances between the near and far plane. This specific projection matrix transforms all coordinates between these x, y and z range values to normalized device coordinates.

// (left coord, right coord, bottom coord, top coord of frustum, 5 and 6 define the distances between the near and far plane...)

// Perspective viewing
// higher w -> appear farthur away.
// Once the coordinates are transformed to clip space they are in the range -w to w. They are in this range BEFORE perspective division! (anything outside this range is clipped)
// create perspective projection matrix.
// (fov in radians, aspect ratio: width/height, near clipping plane distance, far plane distance.)
glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);


// ==========================
// ==========================
// More random transformations

// I assume this is really a model matrix vvv

// glm::mat4 trans = glm::mat4(1.0f);
// trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
// // (tran, radians, axis of rotation)
// trans = glm::rotate(trans, static_cast<float>(glfwGetTime() * 3), glm::vec3{0.0f, 0.0f, 1.0f});
// trans = glm::scale(trans, static_cast<float>(sin(glfwGetTime() * 2)) * glm::vec3{1.0f});

// unsigned int transform_loc = glGetUniformLocation(my_shader.program_id, "transform");
// // (location, how many matrices to send, transpose?, //value ptr is like address-of operator)
// glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));
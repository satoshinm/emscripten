#include <stdio.h>
#include <assert.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

int result = 1;

GLFWwindow* g_window;

void render();
void error_callback(int error, const char* description);

int joy_connected = -1;

void render() {
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  //printf("joy_connected = %d\n", joy_connected);
  if (joy_connected == -1) return;

  static struct {
    int axis_count;
    float axis[16];
    int button_count;
    unsigned char buttons[16];
  } last_gamepad_state = {0};


  int axis_count = 0;
  const float *axis = glfwGetJoystickAxes(joy_connected, &axis_count);

  int button_count = 0;
  const unsigned char *buttons = glfwGetJoystickButtons(joy_connected, &button_count);

  //printf("axis_count = %d, button_count = %d\n", axis_count, button_count);


  last_gamepad_state.axis_count = axis_count;
  for (int i = 0; i < axis_count; ++i) {
    if (last_gamepad_state.axis[i] != axis[i]) {
      printf("axis %d = %f\n", i, axis[i]);
    }
    //printf("axis unchanged %d = %f\n", i, axis[i]);

    last_gamepad_state.axis[i] = axis[i];
  }

  last_gamepad_state.button_count =  button_count;
  for (int i = 0; i < button_count; ++i) {
    if (last_gamepad_state.buttons[i] != buttons[i]) {
      printf("button %d = %d\n", i, buttons[i]);
    }
    //printf("button unchanged %d = %d\n", i, buttons[i]);

    last_gamepad_state.buttons[i] = buttons[i];
  }
}

void joystick_callback(int joy, int event)
{
  printf("joystick_callback %d %d\n", joy, event);
  if (event == GLFW_CONNECTED) {
    printf("Joystick %d was connected: %s\n", joy, glfwGetJoystickName(joy));
    joy_connected = joy; // use the most recently connected joystick
  } else if (event == GLFW_DISCONNECTED) {
    printf("Joystick %d was disconnected\n", joy);
    if (joy == joy_connected) joy_connected = -1;
  }
}

int main() {
  if (!glfwInit())
  {
    result = 0;
    printf("Could not create window. Test failed.\n");
#ifdef REPORT_RESULT
    REPORT_RESULT();
#endif
    return -1;
  }
  glfwWindowHint(GLFW_RESIZABLE , 1);
  g_window = glfwCreateWindow(600, 450, "GLFW joystick test", NULL, NULL);
  if (!g_window)
  {
    result = 0;
    printf("Could not create window. Test failed.\n");
#ifdef REPORT_RESULT
    REPORT_RESULT();
#endif
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(g_window);

  glfwSetJoystickCallback(joystick_callback);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(render, 0, 1);
#else
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
#endif

  glfwTerminate();

  return 0;
}

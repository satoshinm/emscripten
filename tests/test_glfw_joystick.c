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

void render() {
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void joystick_callback(int joy, int event)
{
  printf("joystick_callback %d %d\n", joy, event);
  if (event == GLFW_CONNECTED) {
    printf("Joystick %d was connected\n", joy);
  } else if (event == GLFW_DISCONNECTED) {
    printf("Joystick %d was disconnected\n", joy);
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

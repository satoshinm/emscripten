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

  last_gamepad_state.axis_count = axis_count;
  for (int i = 0; i < axis_count; ++i) {
    if (last_gamepad_state.axis[i] != axis[i]) {
      printf("axis %d = %f\n", i, axis[i]);
    }

    last_gamepad_state.axis[i] = axis[i];
  }

  last_gamepad_state.button_count =  button_count;
  for (int i = 0; i < button_count; ++i) {
    if (last_gamepad_state.buttons[i] != buttons[i]) {
      printf("button %d = %d\n", i, buttons[i]);
    }

    last_gamepad_state.buttons[i] = buttons[i];
  }
}

void joystick_callback(int joy, int event)
{
  if (event == GLFW_CONNECTED) {
    printf("Joystick %d was connected: %s\n", joy, glfwGetJoystickName(joy));
    joy_connected = joy; // use the most recently connected joystick
  } else if (event == GLFW_DISCONNECTED) {
    printf("Joystick %d was disconnected\n", joy);
    if (joy == joy_connected) joy_connected = -1;
  }
}

void main_2(void *arg) {
  emscripten_run_script("window.addNewGamepad('Pad Thai', 4, 16)");
  emscripten_run_script("window.addNewGamepad('Pad Kee Mao', 0, 4)");
  // Check that the joysticks exist properly.
  printf("glfwJoystickPresent(GLFW_JOYSTICK_1) = %d\n", glfwJoystickPresent(GLFW_JOYSTICK_1));
  printf("glfwJoystickPresent(GLFW_JOYSTICK_2) = %d\n", glfwJoystickPresent(GLFW_JOYSTICK_2));
  EM_ASM(alert(1));

  assert(glfwJoystickPresent(GLFW_JOYSTICK_1));
  assert(glfwJoystickPresent(GLFW_JOYSTICK_2));

  // TODO
  /*
  assert(!SDL_JoystickOpened(0));
  assert(!SDL_JoystickOpened(1));
  SDL_Joystick* pad1 = SDL_JoystickOpen(0);
  assert(SDL_JoystickOpened(0));
  assert(SDL_JoystickIndex(pad1) == 0);
  assert(strncmp(SDL_JoystickName(0), "Pad Thai", 9) == 0);
  assert(strncmp(SDL_JoystickName(1), "Pad Kee Mao", 12) == 0);
  assert(SDL_JoystickNumAxes(pad1) == 4);
  assert(SDL_JoystickNumButtons(pad1) == 16);

  // By default, SDL will automatically process events. Test this behavior, and then disable it.
  assert(SDL_JoystickEventState(SDL_QUERY) == SDL_ENABLE);
  SDL_JoystickEventState(SDL_DISABLE);
  assert(SDL_JoystickEventState(SDL_QUERY) == SDL_DISABLE);
  // Button events.
  emscripten_run_script("window.simulateGamepadButtonDown(0, 1)");
  // We didn't tell SDL to automatically update this joystick's state.
  assertNoJoystickEvent();
  SDL_JoystickUpdate();
  assertJoystickEvent(0, SDL_JOYBUTTONDOWN, 1, SDL_PRESSED);
  assert(SDL_JoystickGetButton(pad1, 1) == 1);
  // Enable automatic updates.
  SDL_JoystickEventState(SDL_ENABLE);
  assert(SDL_JoystickEventState(SDL_QUERY) == SDL_ENABLE);
  emscripten_run_script("window.simulateGamepadButtonUp(0, 1)");
  assertJoystickEvent(0, SDL_JOYBUTTONUP, 1, SDL_RELEASED);
  assert(SDL_JoystickGetButton(pad1, 1) == 0);
  // No button change: Should not result in a new event.
  emscripten_run_script("window.simulateGamepadButtonUp(0, 1)");
  assertNoJoystickEvent();
  // Joystick 1 is not opened; should not result in a new event.
  emscripten_run_script("window.simulateGamepadButtonDown(1, 1)");
  assertNoJoystickEvent();

  // Joystick wiggling
  emscripten_run_script("window.simulateAxisMotion(0, 0, 1)");
  assertJoystickEvent(0, SDL_JOYAXISMOTION, 0, 32767);
  assert(SDL_JoystickGetAxis(pad1, 0) == 32767);
  emscripten_run_script("window.simulateAxisMotion(0, 0, 0)");
  assertJoystickEvent(0, SDL_JOYAXISMOTION, 0, 0);
  assert(SDL_JoystickGetAxis(pad1, 0) == 0);
  emscripten_run_script("window.simulateAxisMotion(0, 1, -1)");
  assertJoystickEvent(0, SDL_JOYAXISMOTION, 1, -32768);
  assert(SDL_JoystickGetAxis(pad1, 1) == -32768);
  emscripten_run_script("window.simulateAxisMotion(0, 1, -1)");
  // No joystick change: Should not result in a new event.
  assertNoJoystickEvent();
  // Joystick 1 is not opened; should not result in a new event.
  emscripten_run_script("window.simulateAxisMotion(1, 1, -1)");
  assertNoJoystickEvent();

  SDL_JoystickClose(pad1);
  assert(!SDL_JoystickOpened(0));

  // Joystick 0 is closed; we should not process any new gamepad events from it.
  emscripten_run_script("window.simulateGamepadButtonDown(0, 1)");
  assertNoJoystickEvent();

  // End test.
  result = 2;
  printf("Test passed!\n");
  REPORT_RESULT();
  */
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

  emscripten_async_call(main_2, NULL, 3000); // avoid startup delays and intermittent errors

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

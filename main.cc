
#ifdef __EMSCRIPTEN__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#else
#include <GL/glew.h>
#endif
#include <GL/glut.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdlib>

#ifndef __EMSCRIPTEN__
int fullscreen;
#endif

void onDisplay() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glutSwapBuffers();
}

void onReshape(int width, int height) {
  glViewport(0, 0, width, height);
}

void onKeyboard(unsigned char key, int x, int y) {
}

void onIdle() {
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  printf("Hello WebGL!\n");

  glutInit(&argc, argv);
  glutInitWindowSize(1000, 1000);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  int win = glutCreateWindow(__FILE__);

  GLuint vbo;
  GLuint vao;
  GLuint vertex_shader, fragment_shader;
  const GLfloat vertices[] = {
    /*
       x,    y,
    */

    -1.f, -1.f,
     1.f, -1.f,
    -1.f,  1.f,

     1.f, -1.f,
     1.f,  1.f,
    -1.f,  1.f,
  };

	#ifndef __EMSCRIPTEN__
	  GLenum err = glewInit();
	  if (err != GLEW_OK) {
	    printf("error: %s\n", glewGetErrorString(err));
	    glutDestroyWindow(win);
	    return EXIT_FAILURE;
	  }
	#endif

  glEnable(GL_DEPTH_TEST);
  {
    const GLchar* str = "precision mediump float;\n"
                        "attribute vec2 aPosition;\n"
                        "varying vec2 vPosition;\n"
                        "void main()\n"
                        "{\n"
                        "  gl_Position = vec4(aPosition, 0.0, 1.0);\n"
                        "  vPosition = aPosition;\n"
                        "}\n";
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &str, NULL);
    glCompileShader(vertex_shader);
    {
      GLint params;
      glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &params);
      if (params == GL_FALSE) {
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &params);
        GLchar* log = static_cast<GLchar*>(malloc(params));
        glGetShaderInfoLog(vertex_shader, params, NULL, log);
        printf("%s", log);
        free(log);
        glutDestroyWindow(win);
        return EXIT_FAILURE;
      }
    }
  }
  {
    const GLchar* str = "precision mediump float;\n"
                        "varying vec2 vPosition;\n"
                        "void main()\n"
                        "{\n"
                        "  if (abs(vPosition.x) > 0.9 || abs(vPosition.y) > 0.9) {\n"
                        "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                        "  } else {\n"
                        "    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
                        "  }\n"
                        "}\n";
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &str, NULL);
    glCompileShader(fragment_shader);
    {
      GLint params;
      glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &params);
      if (params == GL_FALSE) {
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &params);
        GLchar* log = static_cast<GLchar*>(malloc(params));
        glGetShaderInfoLog(fragment_shader, params, NULL, log);
        printf("%s", log);
        free(log);
        glutDestroyWindow(win);
        return EXIT_FAILURE;
      }
    }
  }
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glBindAttribLocation(program, 0, "aPosition");
  glLinkProgram(program);
  {
    GLint params;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    if (params == GL_FALSE) {
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &params);
      GLchar* log = static_cast<GLchar*>(malloc(params));
      glGetProgramInfoLog(program, params, NULL, log);
      printf("%s", log);
      free(log);
      glutDestroyWindow(win);
      return EXIT_FAILURE;
    }
  }
  glUseProgram(program);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
  glEnableVertexAttribArray(0);

  glClearColor(0.f, 0.f, 0.4f, 0.f);

  printf("You should see a green rectangle with red borders.\n");
  printf("No matter 'Resize canvas' is checked or not, you should see the whole screen filled by the rectangle when in full screen, and after exiting, the rectangle should be restored in the window.\n");

  glutDisplayFunc(onDisplay);
  glutReshapeFunc(onReshape);
  glutKeyboardFunc(onKeyboard);
  glutIdleFunc(onIdle);
  glutMainLoop();

  glutDestroyWindow(win);

  return 0;
}

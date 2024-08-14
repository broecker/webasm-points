
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

#include "shader.h"

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

  const std::string vert_shader = R"(
    precision mediump float;
    attribute vec2 aPosition;
    varying vec2 vPosition;
  
    void main() {
      gl_Position = vec4(aPosition, 0.0, 1.0);
      vPosition = aPosition;
    }
  )";
  const std::string frag_shader = R"(
    precision mediump float;
    varying vec2 vPosition;
  
    void main() {
      if (abs(vPosition.x) > 0.9 || abs(vPosition.y) > 0.9) {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
      } else {
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
      }
    }
  )";
  std::map<GLuint, std::string> attributeMap;
  attributeMap[0] = "aPosition";

  WebGLShader shader(vert_shader, frag_shader, attributeMap);
  shader.bind();

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

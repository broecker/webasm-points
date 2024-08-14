#ifndef WEBGL_SHADER_INCLUDED
#define WEBGL_SHADER_INCLUDED

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

#include <string>
#include <map>

class WebGLShader final {
 public:
  WebGLShader(const std::string& vertex_source,
              const std::string& fragment_source,
              std::map<GLuint, std::string> vertex_attribute_map = {});
  virtual ~WebGLShader();

  void bind() const;
  void unbind() const;

 private:
  GLuint                          vertex_shader, fragment_shader, program;
  // index/name vertex attribute map.
  std::map<GLuint, std::string>   vertex_attribute_map;
 
  void load_and_compile(const std::string& vertex_shader, const std::string& fragment_shader);
};

#endif

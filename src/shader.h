// Copyright 2024 Markus Broecker <mbroecker@protonmail.ch>

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the “Software”), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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

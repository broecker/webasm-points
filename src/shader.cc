#include "shader.h"

#include <cstdio>
#include <cstdlib>

namespace {

  GLuint compile_shader(GLenum shader_type, const std::string& shader_source) {
    if (!(shader_type == GL_VERTEX_SHADER || shader_type == GL_FRAGMENT_SHADER)) {
      printf("Invalid shader type: %d, expected %d or %d \n", shader_type, GL_VERTEX_SHADER, GL_FRAGMENT_SHADER);
      return GL_INVALID_VALUE;
    }

    if (shader_type == GL_VERTEX_SHADER) {
      printf("Compiling vertex shader.\n");
    } else {
      printf("Compiling fragment shader.\n");
    }

    GLuint shader = glCreateShader(shader_type);

    const GLchar* sauce = shader_source.c_str();
    glShaderSource(shader, 1, &sauce, nullptr);
    glCompileShader(shader);
    {
      GLint params;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
      if (params == GL_FALSE) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &params);
        GLchar* log = static_cast<GLchar*>(malloc(params));
        glGetShaderInfoLog(shader, params, NULL, log);
        printf("Shader compilation failed.\n");
        printf("%s", log);
        free(log);

        printf("Source:\n%s\n", shader_source.c_str());

        return GL_INVALID_VALUE;
      }
    }
    return shader;
  }
}

WebGLShader::WebGLShader(const std::string& vertex_source,
                         const std::string& fragment_source,
                         std::map<GLuint, std::string> vertex_attribute_map) :
  vertex_attribute_map(vertex_attribute_map) {
  load_and_compile(vertex_source, fragment_source);
}

WebGLShader::~WebGLShader() {
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glDeleteProgram(program);
}

void WebGLShader::bind() const {
  glUseProgram(program);
  for (const auto entry : vertex_attribute_map) {
    glBindAttribLocation(program, entry.first, entry.second.c_str());
  }
}

void WebGLShader::unbind() const {
  glUseProgram(0);
}

void WebGLShader::load_and_compile(const std::string& vertex_source,
                                   const std::string& fragment_source) {
  vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_source);
  fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_source);

  if (vertex_shader == GL_INVALID_VALUE) {
    printf("Invalid vertex shader!\n");
  } else {
    printf("Vertex shader: %d\n", vertex_shader);
  }
  if (fragment_shader == GL_INVALID_VALUE) {
    printf("Invalid fragment shader!\n");
  } else {
    printf("Fragment shader: %d\n", fragment_shader);
  }



  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  {
    GLint params;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    if (params == GL_FALSE) {
      printf("Shader linking failed.\n");
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &params);
      GLchar* log = static_cast<GLchar*>(malloc(params));
      glGetProgramInfoLog(program, params, NULL, log);
      printf("%s", log);
      free(log);
    }
  }
}
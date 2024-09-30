#pragma once

#include <memory>

#include "utils.h"
#include "glcore.h"

class Framebuffer
{
public: 
  Framebuffer(unsigned int width, unsigned int height);
  ~Framebuffer();

  static std::shared_ptr<Framebuffer> create(unsigned int width, unsigned int height);

  inline void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); } 
  inline void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

  inline GLuint texture_id() const { return m_texture_id; }

  void rescale(unsigned int width, unsigned int height); 

private: 
  GLuint m_fbo; 
  GLuint m_rbo; 
  GLuint m_texture_id; 

  unsigned int m_width, m_height; 
};

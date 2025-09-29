#pragma once

#include "Utils.h"

#include <cstdint>
#include <memory>

class Framebuffer
{
public:
	Framebuffer() = default;
	Framebuffer(const uint32_t width, const uint32_t height);
	~Framebuffer();

	static std::shared_ptr<Framebuffer> create(const uint32_t width, const uint32_t height);

	inline void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }

	inline void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	inline GLuint texture_id() const { return m_texture_id; }

	void rescale(const uint32_t width, const uint32_t height);

private:
	GLuint m_fbo{};
	GLuint m_rbo{};
	GLuint m_texture_id{};

	uint32_t m_width{};
	uint32_t m_height{};
};

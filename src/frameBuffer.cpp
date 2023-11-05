#include "frameBuffer.h"

FrameBuffer::FrameBuffer(Window* window) : id(0)
{
	glGenFramebuffers(1, &id);
	SetSize(window->GetWidth(), window->GetHeight());
}

void FrameBuffer::SetSize(int width, int height)
{
	Bind();
	oldWidth = width;
	oldHeight = height;

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER,
	                      GL_DEPTH24_STENCIL8,
	                      width,
	                      height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(
	    GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually
	// complete now
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Frame buffer is not complete" << std::endl;

	Unbind();
}

void FrameBuffer::Resize(int newWidth, int newHeight)
{
	if(oldWidth == newWidth && oldHeight == newHeight) return;

	glViewport(0, 0, newWidth, newHeight);

	// Delete previous texture and renderbuffer (if any)
	if(textureColorbuffer != 0)
	{
		glDeleteTextures(1, &textureColorbuffer);
		textureColorbuffer = 0;
	}
	if(rbo != 0)
	{
		glDeleteRenderbuffers(1, &rbo);
		rbo = 0;
	}

	SetSize(newWidth, newHeight);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#include "RBuffer.h"

namespace Silent
{
	RBuffer::RBuffer(std::string name, LRenderer * renderer, int msaa, 
					 bool frame, bool color, bool depth) :
		Resource(name), _renderer(renderer), _MSAALevel(msaa), 
		useFrame(frame), useColor(color), useDepth(depth)
	{
	}

	int RBuffer::GetMultisampleLevel() const
	{
		return _MSAALevel;
	}

	Silent::BufferID RBuffer::GetBufferID() const
	{
		return _bufferID;
	}

	void RBuffer::BindColorbuffer()
	{
		_renderer->BindColorbuffer(_bufferID);
	}

	void RBuffer::BindDepthbuffer()
	{
		_renderer->BindDepthbuffer(_bufferID);
	}

	void RBuffer::Begin()
	{
		if(useColor) BindColorbuffer();
		if(useDepth) BindDepthbuffer();
		_renderer->BindFramebuffer(_bufferID);
	}

	void RBuffer::End()
	{
		_renderer->UnbindFramebuffer(_bufferID);
	}

	void RBuffer::Load()
	{
		_bufferID = _renderer->CreateBuffer(_MSAALevel, useFrame, useColor, useDepth);
	}

	void RBuffer::Cleanup()
	{

	}
}
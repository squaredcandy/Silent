#include "RBuffer.h"

namespace Silent
{
	RBuffer::RBuffer(std::string name)
	{

	}

	RBuffer::RBuffer(std::string name, Renderer * renderer,
									 bool useFramebuffer,  bool useColorbuffer, 
									 bool useDepthbuffer) : _renderer(renderer),
		useFrame(useFramebuffer), useColor(useColorbuffer), 
		useDepth(useDepthbuffer)
	{
		_bufferID = _renderer->CreateBuffer(_MSAALevel, useFramebuffer,
										   useColorbuffer, useDepthbuffer);
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
		_renderer->BindColorbufferToFramebuffer(_bufferID);
	}

	void RBuffer::BindDepthbuffer()
	{
		_renderer->BindDepthbufferToFramebuffer(_bufferID);
	}

	void RBuffer::Begin()
	{
		if(useColor) BindColorbuffer();
		if(useDepth) BindDepthbuffer();
		_renderer->BeginFramebufferDrawing(_bufferID);
	}

	void RBuffer::End()
	{
		// we need something here so that it doesnt push an image to imgui
		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();

		_renderer->EndFramebufferDrawing(_bufferID, size.x, size.y, 
										 pos.x, pos.y);
	}

	void RBuffer::Cleanup()
	{

	}

}
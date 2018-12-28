#include "Resource_Buffer.h"

namespace Silent
{
	Resource_Buffer::Resource_Buffer(std::string name)
	{

	}

	Resource_Buffer::Resource_Buffer(std::string name, Renderer * renderer,
									 bool useFramebuffer,  bool useColorbuffer, 
									 bool useDepthbuffer) : _renderer(renderer),
		useFrame(useFramebuffer), useColor(useColorbuffer), 
		useDepth(useDepthbuffer)
	{
		_bufferID = _renderer->CreateBuffer(_MSAALevel, useFramebuffer,
										   useColorbuffer, useDepthbuffer);
	}

	int Resource_Buffer::GetMultisampleLevel() const
	{
		return _MSAALevel;
	}

	Silent::BufferID Resource_Buffer::GetBufferID() const
	{
		return _bufferID;
	}

	void Resource_Buffer::BindColorbuffer()
	{
		_renderer->BindColorbufferToFramebuffer(_bufferID);
	}

	void Resource_Buffer::BindDepthbuffer()
	{
		_renderer->BindDepthbufferToFramebuffer(_bufferID);
	}

	void Resource_Buffer::Begin()
	{
		if(useColor) BindColorbuffer();
		if(useDepth) BindDepthbuffer();
		_renderer->BeginFramebufferDrawing(_bufferID);
	}

	void Resource_Buffer::End()
	{
		// we need something here so that it doesnt push an image to imgui
		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();

		_renderer->EndFramebufferDrawing(_bufferID, size.x, size.y, 
										 pos.x, pos.y);
	}

	void Resource_Buffer::Cleanup()
	{

	}

}
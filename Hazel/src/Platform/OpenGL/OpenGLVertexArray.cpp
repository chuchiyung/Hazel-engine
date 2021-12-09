#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Hazel {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Hazel::ShaderDataType::Float:   return GL_FLOAT;
			case Hazel::ShaderDataType::Float2:  return GL_FLOAT;
			case Hazel::ShaderDataType::Float3:  return GL_FLOAT;
			case Hazel::ShaderDataType::Float4:  return GL_FLOAT;
			case Hazel::ShaderDataType::Mat3:    return GL_FLOAT;
			case Hazel::ShaderDataType::Mat4:    return GL_FLOAT;
			case Hazel::ShaderDataType::Int:     return GL_INT;
			case Hazel::ShaderDataType::Int2:    return GL_INT;
			case Hazel::ShaderDataType::Int3:    return GL_INT;
			case Hazel::ShaderDataType::Int4:    return GL_INT;
			case Hazel::ShaderDataType::Bool:    return GL_BOOL;
		}
		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index + m_VertexBufferIndexOffset);
			glVertexAttribPointer(index + m_VertexBufferIndexOffset,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexBufferIndexOffset += layout.GetElements().size();
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
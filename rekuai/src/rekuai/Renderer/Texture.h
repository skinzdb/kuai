#pragma once

#include <string>

namespace kuai {
	/** \class Texture
	*	\brief A 2D texture that can support transparency.
	*/
	class Texture {
	public:
		enum class TextureFormat
		{
			RGB = 0,
			RGBA = 1
		};

		virtual ~Texture() = default;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;

		virtual void bind(uint32_t active_tex) const = 0;

		virtual uint32_t get_id() const = 0;

	public:
		static std::shared_ptr<Texture> create(const std::string& filename);
	};
}

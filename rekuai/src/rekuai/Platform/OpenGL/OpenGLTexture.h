#pragma once

namespace kuai {
	/** \class Texture
	*	\brief A 2D texture that can support transparency.
	*/
	class Texture {
	public:
		virtual ~Texture() = default;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};

	class Texture2D : public Texture {
	public:
	    static std::shared_ptr<Texture2D> create(const std::string& filename);
	};
}

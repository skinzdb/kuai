#pragma once

#include "Texture.h"

namespace kuai {
	/* Texture Array
	*
	* first texture of array is empty texture (equivalent to gmod missing texture)
	*
	*  - each texture will have its unique id
	*  - how to work out which texture will be used? a meshrenderer will have a material which in-turn has a texture
	*  - we get the id of this texture which will act as the index into the texturearray
	*  - what about multiple textures for one mesh? e.g. normal maps, height maps, etc.
	*  - doesnt matter i guess?
	*
	*  - assumption is that the dimensions of all textures we give are powers of 2
	*
	*  - one limitation of texture arrays is that each element must have the same dimensions
	*  - lets set the MAX element size to NxN = 256x256
	*  - what if we have smaller textures (nxn) < (256x256)? Solution -> put the texture in the top-left corner and send its dimensions to the shader; sample only (nxn) from the 256x256 element
	*
	*  - what if it's bigger ? Solution 1 -> have a maximum size N, if it's bigger then resize the image to NXN and send to shader as usual
	                                         (issue: picking an N too large will be wasteful for small textures, N too small will ruin image quality for bigger textures)
							   Solution 2 -> compress to nearest power of 2, let this be P, and divide into m = (P/N)^2 elements, let the shader know this texture will span m array elements
	*										 Inside the shader somehow rebuild this texture? is that possible?
	*
	*/
	class TextureArray
	{
	public:
		TextureArray(uint32_t width, uint32_t height, uint32_t layers);
		~TextureArray();

		void insert(const Texture& tex);
		void remove(const Texture& tex);

		void bind(uint32_t active_tex);


	private:
		uint32_t id;

		uint32_t width, height;
		uint32_t layers;

		int capacity;

		std::vector<uint32_t> tex_ids;
	};
}

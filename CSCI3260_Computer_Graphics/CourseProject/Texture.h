#pragma once

class Texture 
{
public:
	void setupTexture2D(const char* texturePath);
	void setupTexture2D();
	void setupTextureCubemap(const char* texturePaths[]);
	void bind2D(unsigned int slot) const;
	void bindCubemap(unsigned int slot) const;
	void unbind() const;

	unsigned int GetID() { return ID; }

private:
	unsigned int ID = 0;
	int Width = 0, Height = 0, BPP = 0;
};
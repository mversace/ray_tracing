#include <fstream>
#include <windows.h>
#include <wingdi.h>

import math;
import ray;

Math::Vec3 color(const Ray& r)
{
	Math::Vec3 direction = r.direction();
	direction.normalize();
	float t = 0.5f * (direction.y() + 1.0f);
	return (1.0f - t) * Math::Vec3(1.0f, 1.0f, 1.0f) + t * Math::Vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	constexpr auto width = 400;
	constexpr auto height = 200;
	
	BITMAPFILEHEADER fileHeader = { 0 };
	BITMAPINFOHEADER infoHeader = { 0 };
	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biWidth = width;
	infoHeader.biHeight = height;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biSizeImage = width * height * infoHeader.biBitCount / 8;

	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	// 实际为BM 小端模式
	fileHeader.bfType = ('M' << 8) | 'B';
	fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

	Math::Vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
	Math::Vec3 horizontal(4.0f, 0.0f, 0.0f);
	Math::Vec3 vertical(0.0f, 2.0f, 0.0f);
	Math::Vec3 origin(0.0f, 0.0f, 0.0f);

	RGBTRIPLE data[height][width];
	for (auto i = 0; i < height; ++i) {
		for (auto j = 0; j < width; ++j) {
			auto u = static_cast<float>(j) / static_cast<float>(width);
			auto v = static_cast<float>(i) / static_cast<float>(height);
			Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			Math::Vec3 col = color(r);
			data[i][j].rgbtRed = static_cast<unsigned char>(255.99 * col.x());
			data[i][j].rgbtGreen = static_cast<unsigned char>(255.99 * col.y());
			data[i][j].rgbtBlue = static_cast<unsigned char>(255.99 * col.z());
		}
	}

	FILE* pFile = nullptr;
	fopen_s(&pFile, "out.bmp", "wb");
	if (pFile) {
		fwrite(&fileHeader, sizeof(fileHeader), 1, pFile);
		fwrite(&infoHeader, sizeof(infoHeader), 1, pFile);
		fwrite(&data, sizeof(RGBTRIPLE), width * height, pFile);
		fclose(pFile);
	}

	return 0;
}
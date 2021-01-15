#include <fstream>
#include <windows.h>
#include <wingdi.h>
#include "world.h"

Math::Vec3 color(const Ray& r, const World& world)
{
	HitRec rec;
	if (world.hit(r, 0.0f, 1000.0f, rec)) {
		auto N = rec.normal;
		return 0.5 * Math::Vec3(N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f);
	}

	Math::Vec3 direction = r.direction();
	direction.normalize();
	float t = 0.5f * (direction.y() + 1.0f);
	return (1.0f - t) * Math::Vec3(1.0f, 1.0f, 1.0f) + t * Math::Vec3(0.5f, 0.7f, 1.0f);
}

void saveToBmp(int width, int height, void* pData)
{
	// 图片数据
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

	FILE* pFile = nullptr;
	fopen_s(&pFile, "out.bmp", "wb");
	if (pFile) {
		fwrite(&fileHeader, sizeof(fileHeader), 1, pFile);
		fwrite(&infoHeader, sizeof(infoHeader), 1, pFile);
		fwrite(pData, sizeof(RGBTRIPLE), width * height, pFile);
		fclose(pFile);
	}
}

int main()
{
	constexpr auto width = 400;
	constexpr auto height = 200;

	Math::Vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
	Math::Vec3 horizontal(4.0f, 0.0f, 0.0f);
	Math::Vec3 vertical(0.0f, 2.0f, 0.0f);
	Math::Vec3 origin(0.0f, 0.0f, 0.0f);

	World world;
	world.addObject({ Math::Vec3(0.0f, 0.0f, -1.0f), 0.5f });
	world.addObject({ Math::Vec3(0.0f, -100.5f, -1.0f), 100.0f });

	auto pData = std::make_shared<RGBTRIPLE[]>(width * height);
	for (auto i = 0; i < height; ++i) {
		for (auto j = 0; j < width; ++j) {
			auto u = static_cast<float>(j) / static_cast<float>(width);
			auto v = static_cast<float>(i) / static_cast<float>(height);
			Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			Math::Vec3 col = color(r, world);
			auto idx = i * width + j;
			pData[idx].rgbtRed = static_cast<unsigned char>(255.99 * col.x());
			pData[idx].rgbtGreen = static_cast<unsigned char>(255.99 * col.y());
			pData[idx].rgbtBlue = static_cast<unsigned char>(255.99 * col.z());
		}
	}

	saveToBmp(width, height, pData.get());

	return 0;
}
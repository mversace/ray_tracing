#include <fstream>
#include <windows.h>
#include <wingdi.h>
#include <random>
#include <time.h>
#include "world.h"
#include "camera.h"


inline float randF() {
	return static_cast<float>(rand()) / RAND_MAX;
}

Math::Vec3 randomInUnitSphere() {
	// Ϊ�˷�ֹ�ҵ��ĵ�������ڲ�

	Math::Vec3 p;
	do {
		p = 2.0f * Math::Vec3(randF(), randF(), randF()) - Math::Vec3(1.0f, 1.0f, 1.0f);
	} while (p.squaredLength() >= 1.0f);

	return p;
}

Math::Vec3 color(const Ray& r, const World& world)
{
	HitRec rec = { 0 };
	if (world.hit(r, 0.01f, 1000.0f, rec)) {
		// �������Ϊ���� ��ȡ��λ�������������
		auto target = rec.p + rec.normal + randomInUnitSphere();
		// ������ȥ����� ������������������� ����������ɫ����
		return 0.5f * color(Ray(rec.p, target - rec.p), world);
	}

	Math::Vec3 direction = r.direction();
	direction.normalize();
	float t = 0.5f * (direction.y() + 1.0f);


	return (1.0f - t) * Math::Vec3(1.0f, 1.0f, 1.0f) + t * Math::Vec3(0.5f, 0.7f, 1.0f);
}

void saveToBmp(int width, int height, void* pData)
{
	// ͼƬ����
	BITMAPFILEHEADER fileHeader = { 0 };
	BITMAPINFOHEADER infoHeader = { 0 };
	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biWidth = width;
	infoHeader.biHeight = height;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biSizeImage = width * height * infoHeader.biBitCount / 8;

	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	// ʵ��ΪBM С��ģʽ
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
	srand(time(nullptr));

	auto width = 800;
	auto height = 400;
	auto ns = 100;

	World world;
	world.addObject({ Math::Vec3(0.0f, 0.0f, -1.0f), 0.5f });
	world.addObject({ Math::Vec3(0.0f, -100.5f, -1.0f), 100.0f });

	Camera camera;

	auto pData = std::make_shared<RGBTRIPLE[]>(width * height);
	for (auto i = 0; i < height; ++i) {
		for (auto j = 0; j < width; ++j) {
			Math::Vec3 col{ 0.0f, 0.0f, 0.0f };
			for (auto k = 0; k < ns; ++k) {
				auto u = static_cast<float>(j + randF()) / static_cast<float>(width);
				auto v = static_cast<float>(i + randF()) / static_cast<float>(height);
				auto r = camera.getRay(u, v);
				col += color(r, world);
			}
			col /= float(ns);
			auto idx = i * width + j;
			pData[idx].rgbtRed = static_cast<unsigned char>(255.99 * col.x());
			pData[idx].rgbtGreen = static_cast<unsigned char>(255.99 * col.y());
			pData[idx].rgbtBlue = static_cast<unsigned char>(255.99 * col.z());
		}
	}

	saveToBmp(width, height, pData.get());

	return 0;
}
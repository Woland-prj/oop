#include "Filling.hpp"
#include <cstddef>
#include <queue>

struct Point
{
	int y;
	int x;
};

bool InBounds(const Image& img, int y, int x)
{
	return y >= 0 && y < img.size() && x >= 0 && x < img[y].size();
}

void BfsFill(Image& img, int startY, int startX)
{
	std::queue<Point> q;
	q.push({ startY, startX });

	while (!q.empty())
	{
		Point p = q.front();
		q.pop();

		const int dy[4] = { 1, -1, 0, 0 };
		const int dx[4] = { 0, 0, 1, -1 };

		for (int i = 0; i < 4; ++i)
		{
			int ny = p.y + dy[i];
			int nx = p.x + dx[i];

			if (!InBounds(img, ny, nx))
				continue;

			if (img[ny][nx] != k_imgEmptyCh)
				continue;

			img[ny][nx] = k_imgFillCh;
			q.push({ ny, nx });
		}
	}
}

void Fill(Image& img)
{
	for (int y = 0; y < img.size(); ++y)
	{
		for (int x = 0; x < img[y].size(); ++x)
		{
			if (img[y][x] == k_imgStartCh)
				BfsFill(img, y, x);
		}
	}
}

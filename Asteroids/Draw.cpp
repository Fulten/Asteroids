#include "Draw.h"

Draw::Draw(std::vector<CUSTOMVERTEX>* vertices, int* triCount) : 
	vertices(vertices),
	triCount(triCount)
{}

Draw::~Draw()
{
	//delete[] triCount;
	//delete[] windowSize;
}

void Draw::setWindowSize(myVec2* wSize)
{
	windowSize = wSize;
}

// draws a square at a location
void Draw::DrawSquare(myVec2 pos, int size, D3DCOLOR color)
{
	
	vertices->push_back({ pos.x			, pos.y			, 0.5f, 1.0f, color, });
	vertices->push_back({ pos.x + size	, pos.y			, 0.5f, 1.0f, color, });
	vertices->push_back({ pos.x + size	, pos.y	+ size	, 0.5f, 1.0f, color, });
	*triCount += 1;

	vertices->push_back({ pos.x			, pos.y + size	, 0.5f, 1.0f, color, });
	*triCount += 1;
	
	/* old 6 vertex system
	//first triangle
	vertices->push_back({ pos.x			, pos.y			, 0.5f, 1.0f, color, });
	vertices->push_back({ pos.x + size	, pos.y			, 0.5f, 1.0f, color, });
	vertices->push_back({ pos.x			, pos.y + size	, 0.5f, 1.0f, color, });
	*triCount += 1;
	//second triangle			
	vertices->push_back({ pos.x + size	, pos.y			, 0.5f, 1.0f, color, });
	vertices->push_back({ pos.x + size	, pos.y + size	, 0.5f, 1.0f, color, });
	vertices->push_back({ pos.x			, pos.y + size	, 0.5f, 1.0f, color, });
	*triCount += 1;
	*/
}

// draw a vector from the start and end points
void Draw::drawVector(myVec2 start, myVec2 end, D3DCOLOR color)
{
	myVec2 temp;

	temp = end - start;
	
	myVec2 wrap = myVec2(0, 0);
	float length = temp.getLength();
	temp.Normalize();
	
	if (length > 100) length = 100;

	for (int i = 0; i < length; i += 1)
	{
		if ((temp.x * i + start.x + wrap.x) < 0)
			wrap.x += windowSize->x;

		if ((temp.x * i + start.x + wrap.x ) >= windowSize->x)
			wrap.x -= windowSize->x;

		if ((temp.y * i + start.y + wrap.y) < 0)
			wrap.y += windowSize->y;

		if ((temp.y * i + start.y + wrap.y) >= windowSize->y)
			wrap.y -= windowSize->y;

		DrawSquare((temp * i + start + wrap + myVec2(0.5, 0.5)).round(), 1, color);
	}
}

void Draw::drawCharacter(myVec2 pos, char in, D3DCOLOR color, int size)
{
	std::string toDraw = "";
						 
	// 7 by 5			  
						  
	switch (in)			  
	{
	case 'A':
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += ".#####.";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		break;
	
	case 'B':
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += ".####..";
		toDraw += ".#...#.";
		toDraw += ".####..";
		break;

	case 'C':
		toDraw += "..####.";
		toDraw += ".#.....";
		toDraw += ".#.....";
		toDraw += ".#.....";
		toDraw += "..####.";
		break;

	case 'D':
		toDraw += ".####..";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += ".####..";
		break;

	case 'E':
		toDraw += "..###..";
		toDraw += ".#.....";
		toDraw += ".###...";
		toDraw += ".#.....";
		toDraw += "..###..";
		break;

	case 'F':
		toDraw += "..####.";
		toDraw += ".#.....";
		toDraw += ".####..";
		toDraw += ".#.....";
		toDraw += ".#.....";
		break;

	case 'G':
		toDraw += "..###..";
		toDraw += ".#.....";
		toDraw += ".#..##.";
		toDraw += ".#...#.";
		toDraw += "..###..";
		break;

	case 'H':
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += ".#####.";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		break;

	case 'I':
		toDraw += ".#####.";
		toDraw += "...#...";
		toDraw += "...#...";
		toDraw += "...#...";
		toDraw += ".#####.";
		break;

	case 'J':
		toDraw += "..####.";
		toDraw += ".....#.";
		toDraw += ".....#.";
		toDraw += ".#...#.";
		toDraw += "..###..";
		break;

	case 'K':
		toDraw += ".#...#.";
		toDraw += ".#..#..";
		toDraw += ".###...";
		toDraw += ".#..#..";
		toDraw += ".#...#.";
		break;

	case 'L':
		toDraw += ".#.....";
		toDraw += ".#.....";
		toDraw += ".#.....";
		toDraw += ".#.....";
		toDraw += "..####.";
		break;

	case 'M':
		toDraw += ".#...#.";
		toDraw += ".##.##.";
		toDraw += ".#.#.#.";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		break;

	case 'N':
		toDraw += ".#...#.";
		toDraw += ".##..#.";
		toDraw += ".#.#.#.";
		toDraw += ".#..##.";
		toDraw += ".#...#.";
		break;

	case 'O':
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += "..###..";
		break;

	case 'P':
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += ".####..";
		toDraw += ".#.....";
		toDraw += ".#.....";
		break;

	case 'Q':
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += ".#.#.#.";
		toDraw += "..#.##.";
		break;

	case 'R':
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += ".####..";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		break;

	case 'S':
		toDraw += "..####.";
		toDraw += ".#.....";
		toDraw += "..###..";
		toDraw += ".....#.";
		toDraw += ".####..";
		break;

	case 'T':
	{
		toDraw += ".#####.";
		toDraw += "...#...";
		toDraw += "...#...";
		toDraw += "...#...";
		toDraw += "...#...";
	}
		
		break;

	case 'U':
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += "..###..";
		break;

	case 'V':
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += "..#.#..";
		toDraw += "..#.#..";
		toDraw += "...#...";
		break;

	case 'W':
		toDraw += ".#...#.";
		toDraw += ".#...#.";
		toDraw += ".#.#.#.";
		toDraw += ".##.##.";
		toDraw += "..###..";
		break;

	case 'X':
		toDraw += ".#...#.";
		toDraw += "..#.#..";
		toDraw += "...#...";
		toDraw += "..#.#..";
		toDraw += ".#...#.";
		break;

	case 'Y':
		toDraw += ".#...#.";
		toDraw += "..#.#..";
		toDraw += "...#...";
		toDraw += "...#...";
		toDraw += "...#...";
		break;

	case 'Z':
		toDraw += ".#####.";
		toDraw += "....#..";
		toDraw += "...#...";
		toDraw += "..#....";
		toDraw += ".#####.";
		break;

	case ' ':
		toDraw += ".......";
		toDraw += ".......";
		toDraw += ".......";
		toDraw += ".......";
		toDraw += ".......";
		break;

	case ':':
		toDraw += ".#.....";
		toDraw += ".#.....";
		toDraw += ".......";
		toDraw += ".#.....";
		toDraw += ".#.....";
		break;

	case '1':
		toDraw += "...#...";
		toDraw += "..##...";
		toDraw += "...#...";
		toDraw += "...#...";
		toDraw += "..###..";
		break;

	case '2':
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += "....#..";
		toDraw += "...#...";
		toDraw += ".#####.";
		break;

	case '3':
		toDraw += ".####..";
		toDraw += ".....#.";
		toDraw += "...##..";
		toDraw += ".....#.";
		toDraw += ".####..";
		break;

	case '4':
		toDraw += "...##..";
		toDraw += "..#.#..";
		toDraw += ".#..#..";
		toDraw += ".#####.";
		toDraw += "....#..";
		break;

	case '5':
		toDraw += ".#####.";
		toDraw += ".#.....";
		toDraw += ".####..";
		toDraw += ".....#.";
		toDraw += ".####..";
		break;

	case '6':
		toDraw += "..###..";
		toDraw += ".#.....";
		toDraw += ".####..";
		toDraw += ".#...#.";
		toDraw += "..###..";
		break;

	case '7':
		toDraw += ".#####.";
		toDraw += ".....#.";
		toDraw += "....#..";
		toDraw += "...#...";
		toDraw += "..#....";
		break;

	case '8':
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += "..###..";
		break;

	case '9':
		toDraw += "..###..";
		toDraw += ".#...#.";
		toDraw += "..####.";
		toDraw += ".....#.";
		toDraw += "..###..";
		break;

	case '0':
		toDraw += "..###..";
		toDraw += ".##..#.";
		toDraw += ".#.#.#.";
		toDraw += ".#..##.";
		toDraw += "..###..";
		break;

	default:
		toDraw += "#######";
		toDraw += "#.#.#.#";
		toDraw += "#..#..#";
		toDraw += "#.#.#.#";
		toDraw += "#######";
		break;
	}

	for (int x = 0; x < 7; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			if (toDraw.at(x + y * 7) == '#')
			{
				DrawSquare(pos + myVec2(x * size, y * size), size, color);
			}
			else
			{
				//DrawSquare(pos + myVec2(x, y), 1, D3DCOLOR_XRGB(100, 100, 0));
			}
		}
	}
}

// draws a string
void Draw::drawString(myVec2 pos, std::string in, D3DCOLOR color, int size = 1)
{
	for (int i = 0; i < in.size(); i++)
	{
		drawCharacter(pos + myVec2(i * 7 * size, 0), in.at(i), color, size);
	}
}
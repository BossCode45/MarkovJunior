#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

enum Tile
{
	Black,
	White,
	Red,
	Blue,
	Green,
	DarkGreen,
	Brown,
	Yellow
	//A
};

struct Rule
{
	vector<Tile> toFind;
	vector<Tile> toReplace;
	int amnt;
};
struct Possibility
{
	int x;
	int y;
	int dir;
	int theRule;
};

const int boardW = 30;
const int boardH = 30;
Tile board[boardH][boardW];

vector<vector<Rule>> rules[] =
	{
		{
			{
				{{Black}, {White}, 1},
				{{Black}, {Red}, 1}
			},
			{
				{{Red, Black}, {Red, Red}, -1},
				{{White, Black}, {White, White}, -1}
			},
			{
				{{Red, White}, {Blue, Blue}, -1}
			},
			{
				{{Blue, White}, {Blue, Yellow}, -1},
				{{Blue, Red}, {Blue, Yellow}, -1}
			},
		},
		{
			{
				{{White}, {Brown}, 2},
				{{Red}, {Brown}, 2}
			}
		},
		{
			{
				{{White}, {DarkGreen}, -1},
				{{Red}, {DarkGreen}, -1},
			}
		},
		{
			{
				{{Brown, DarkGreen}, {Brown, Green}, -1}
			}
		}
	};
	/*
	{
		{
			{
				{{W, B, B}, {W, A, W}, -1}
			}
		},
		{
			{
				{{A}, {W}, -1}
			}
		}
	};
	*/

void drawBoard()
{
	for(int y = 0; y < boardH; y++)
	{
		//Print space at start
		if(y==0)
		{
			for(int x = 0; x < boardW; x++)
			{
				//cout << "  ";
			}
			//cout << endl;
		}
		//Print characters
		for(int x = 0; x < boardW; x++)
		{

			switch(board[y][x])
			{
				case White: cout << "\033[37m██\033[0m";break;
				case Black: cout << "\033[30m██\033[0m";break;
				//case A: cout << "\033[36m██\033[0m";break;
				case Red: cout << "\033[31m██\033[0m";break;
				case Blue: cout << "\033[36m██\033[0m";break;
				case Green: cout << "\033[32;1m██\033[0m";break;
				case DarkGreen: cout << "\033[38;5;28m██\033[0m";break;
				case Brown: cout << "\033[38;5;166m██\033[0m";break;
				case Yellow: cout << "\033[38;5;184m██\033[0m";break;
				/*
				case 'R': cout << "\033[31m██\033[0m";break;
				case 'O': cout << "\033[33m██\033[0m";break;
				case 'G': cout << "\033[32m██\033[0m";break;
				case 'L': cout << "\033[36m██\033[0m";break;
				*/
			}
			//cout << board[y][x];
		}
		cout << endl;
	}
}

bool check(int x, int y, int dir, vector<Tile> toFind)
{
	if(dir == 0 && boardW - toFind.size() >= x)
	{
		for(int i = 0; i < toFind.size(); i++)
		{
			if(board[y][x + i] != toFind[i])
			{
				return false;
			}
		}
		return true;
	}
	if(dir == 1 && boardH - toFind.size() >= y)
	{
		for(int i = 0; i < toFind.size(); i++)
		{
			if(board[y + i][x] != toFind[i])
			{
				return false;
			}
		}
		return true;
	}
	if(dir == 2 && x >= toFind.size()-1)
	{
		for(int i = 0; i < toFind.size(); i++)
		{
			if(board[y][x - i] != toFind[i])
			{
				return false;
			}
		}
		return true;
	}
	if(dir == 3 && y >= toFind.size()-1)
	{
		for(int i = 0; i < toFind.size(); i++)
		{
			if(board[y - i][x] != toFind[i])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void replace(int x, int y, int dir, vector<Tile> toReplace)
{
	if(dir == 0)
	{
		for(int i = 0; i < toReplace.size(); i++)
		{
			board[y][x+i] = toReplace[i];
		}
	}
	if(dir == 1)
	{
		for(int i = 0; i < toReplace.size(); i++)
		{
			board[y+i][x] = toReplace[i];
		}
	}
	if(dir == 2)
	{
		for(int i = 0; i < toReplace.size(); i++)
		{
			board[y][x-i] = toReplace[i];
		}
	}
	if(dir == 3)
	{
		for(int i = 0; i < toReplace.size(); i++)
		{
			board[y-i][x] = toReplace[i];
		}
	}
}

int main()
{
	srand(time(NULL));
	for(int y = 0; y < boardH; y++)
	{
		for(int x = 0; x < boardW; x++)
		{
			board[y][x] = Black;
		}
	}
	//board[rand()%boardH][rand()%boardW] = W;
	//drawBoard();
	bool found = false;
	for(int group = 0; group < sizeof(rules)/sizeof(rules[0]); group+=found?0:1)
	{
		found = false;
		for(int rulePriority = 0; rulePriority < rules[group].size(); rulePriority++)
		{
			vector<Possibility> possibilities;
			for(int theRule = 0; theRule < rules[group][rulePriority].size(); theRule++)
			{
				Rule rule = rules[group][rulePriority][theRule];
				if(rule.amnt == 0)
				{
					continue;
				}
				for(int y = 0; y < boardH; y++)
				{
					for(int x = 0; x < boardW; x++)
					{
						//Check
						for(int dir = 0; dir < 4; dir++)
						{
							if(check(x, y, dir, rule.toFind))
							{
								possibilities.push_back({x, y, dir, theRule});
							}
						}
					}
				}
			}
			if(possibilities.size() > 0)
			{
				Possibility choice = possibilities[rand()%possibilities.size()];
				//Replace
				replace(choice.x, choice.y, choice.dir, rules[group][rulePriority][choice.theRule].toReplace);
				rules[group][rulePriority][choice.theRule].amnt--;
				//cout << "x: " << choice.x << ", y: " << choice.y << ", dir: " << choice.dir << endl;
				found = true;
				break;
			}
		}
		//drawBoard();
	}
	drawBoard();
}

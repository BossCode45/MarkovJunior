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
	Yellow,
	LightBlue,
	SuperDarkGreen,
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

const int boardW = 50;
const int boardH = 50;
Tile board[boardH][boardW];

vector<vector<Rule>> rules[] =
	//River scene
	{
		//River
		{
			{
				{{Black}, {White}, 2},
				{{Black}, {Red}, 2}
			},
			{
				{{Red, Black}, {Red, Red}, -1},
				{{White, Black}, {White, White}, -1}
			},
			{
				{{Red, White}, {Blue, Blue}, -1}
			}
		},
		//Shore
		{
			{
				{{Blue, White}, {Blue, Yellow}, -1},
				{{Blue, Red}, {Blue, Yellow}, -1}
			},
		},
		//Widening
		{
			{
				{{Yellow}, {Blue}, -1},
			}
		},
		//Shore, again
		{
			{
				{{Blue, White}, {Blue, Yellow}, -1},
				{{Blue, Red}, {Blue, Yellow}, -1}
			}
		},
		//Shallows
		{
			{
				{{Yellow, Blue}, {Yellow, LightBlue}, -1}
			}
		},
		//Boat
		{
			{
				{{Blue}, {Brown}, 1},
				{{Blue, Brown}, {Brown, Brown}, 1}
			}
		},
		//Trees
		{
			{
				{{White}, {Brown}, 3},
				{{Red}, {Brown}, 3}
			}
		},
		//Forest
		{
			{
				{{White, Brown}, {DarkGreen, Brown}, -1},
				{{Red, Brown}, {DarkGreen, Brown}, -1},
				{{White, Yellow}, {Green, Yellow}, -1},
				{{Red, Yellow}, {Green, Yellow}, -1}
			},
			{
				{{Green, White}, {Green, Green}, -1},
				{{Green, Red}, {Green, Green}, -1},
				{{DarkGreen, White}, {DarkGreen, DarkGreen}, -1},
				{{DarkGreen, Red}, {DarkGreen, DarkGreen}, -1},
			}
		},
		//Leaves
		{
			{
				{{Brown, DarkGreen}, {Brown, SuperDarkGreen}, -1},
				{{Brown, Green}, {Brown, SuperDarkGreen}, -1},
				{{Brown, Yellow}, {Brown, SuperDarkGreen}, -1}
			}
		},
		//Bushes
		{
			{
				{{Green}, {White}, 5},
			},
			{
				{{White, Green}, {White, DarkGreen}, -1},
				{{White, Yellow}, {White, DarkGreen}, -1},
			},
			{
				{{White}, {DarkGreen}, -1}
			}
		}
	};
	/*
	//Basic maze
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
	printf("\033[%d;%dH", (0), (0));
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
				case White: cout << "\033[38;5;15m██\033[0m";break;
				case Black: cout << "\033[38;5;16m██\033[0m";break;
				case Red: cout << "\033[38;5;9m██\033[0m";break;
				case Blue: cout << "\033[38;5;19m██\033[0m";break;
				case Green: cout << "\033[38;5;112m██\033[0m";break;
				case DarkGreen: cout << "\033[38;5;28m██\033[0m";break;
				case Brown: cout << "\033[38;5;94m██\033[0m";break;
				case Yellow: cout << "\033[38;5;184m██\033[0m";break;
				case LightBlue: cout << "\033[38;5;21m██\033[0m";break;
				case SuperDarkGreen: cout << "\033[38;5;22m██\033[0m";break;
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
	int seed = time(NULL);
	srand(seed);
	system("clear");
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
		//if(found == false)
			//drawBoard();
		drawBoard();
	}
	cout << "SEED: " << seed << endl;
}

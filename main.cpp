#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unistd.h>
#include <fstream>

#include <nlohmann/json.hpp>

using std::cout;
using std::endl;
using std::string;
using std::vector;

using json = nlohmann::json;

enum Tile
{
	Any,
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
	Orange,
	Purple
	//A
};
static std::unordered_map<std::string, Tile> const tileMap = {
	{"Any", Tile::Any},
	{"Black", Tile::Black},
	{"White", Tile::White},
	{"Red", Tile::Red},
	{"Blue", Tile::Blue},
	{"Green", Tile::Green},
	{"DarkGreen", Tile::DarkGreen},
	{"Brown", Tile::Brown},
	{"Yellow", Tile::Yellow},
	{"LightBlue", Tile::LightBlue},
	{"SuperDarkGreen", Tile::SuperDarkGreen},
	{"Orange", Tile::Orange},
	{"Purple", Tile::Purple}
};

struct Rule
{
	vector<vector<Tile>> toFind;
	vector<vector<Tile>> toReplace;
	int amnt;
};
struct Dir
{
	int xDir;
	int yDir;
	bool swap;
};
const Dir dirList[] =
{
	{1, 1, false},
	{-1, 1, true},
	{-1, -1, false},
	{1, -1, true}
};
struct Possibility
{
	int x;
	int y;
	Dir dir;
	int theRule;
};

int boardW = 51;
int boardH = 51;
Tile** board;

vector<vector<vector<Rule>>> rules;// =
	//Rainbow
	/*
	{
		{
			{
				{{Black}, {Red}, 1},
				{{Black, Red}, {Orange, Red}, -1},
				{{Black, Orange}, {Yellow, Orange}, -1},
				{{Black, Yellow}, {Green, Yellow}, -1},
				{{Black, Green}, {LightBlue, Green}, -1},
				{{Black, LightBlue}, {Blue, LightBlue}, -1},
				{{Black, Blue}, {Purple, Blue}, -1},
				{{Black, Purple}, {Red, Purple}, -1},
			}
		}
	};
	*/
	//River scene
	/*
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
	*/
	//Basic maze
	/*
	{
		{
			{
				{{Black}, {White}, 1}
			}
		},
		{
			{
				{{White, Black, Black}, {White, Green, White}, -1}
			}
		},
		{
			{
				{{Green}, {White}, -1}
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
				case Red: cout << "\033[38;5;88m██\033[0m";break;
				case Blue: cout << "\033[38;5;19m██\033[0m";break;
				case Green: cout << "\033[38;5;112m██\033[0m";break;
				case DarkGreen: cout << "\033[38;5;28m██\033[0m";break;
				case Brown: cout << "\033[38;5;94m██\033[0m";break;
				case Yellow: cout << "\033[38;5;184m██\033[0m";break;
				case LightBlue: cout << "\033[38;5;21m██\033[0m";break;
				case SuperDarkGreen: cout << "\033[38;5;22m██\033[0m";break;
				case Orange: cout << "\033[38;5;166m██\033[0m";break;
				case Purple: cout << "\033[38;5;54m██\033[0m";break;
			}
			//cout << board[y][x];
		}
		cout << endl;
	}
}

bool check(int x, int y, Dir dir, vector<vector<Tile>> toFind)
{
	int xDir = dir.xDir;
	int yDir = dir.yDir;
	bool swap = dir.swap;
	for(int i = 0; i < toFind.size(); i++)
	{
		for(int j = 0; j < toFind[i].size(); j++)
		{
			int boardY = y + (i*(!swap) + j*swap)*yDir;
			int boardX = x + (i*swap + j*(!swap))*xDir;
			if(0 > boardX || boardX >= boardW)
				return false;
			if(0 > boardY || boardY >= boardH)
				return false;
			Tile toCheck = toFind[i][j];
			Tile tileInQuestion = board[boardY][boardX];
			if(toCheck != tileInQuestion)
				return false;
		}
	}
	return true;
}

void replace(int x, int y, Dir dir, vector<vector<Tile>> toReplace)
{
	int xDir = dir.xDir;
	int yDir = dir.yDir;
	bool swap = dir.swap;
	for(int i = 0; i < toReplace.size(); i++)
	{
		for(int j = 0; j < toReplace[i].size(); j++)
		{
			int boardY = y + (i*(!swap) + j*swap)*yDir;
			int boardX = x + (i*swap + j*(!swap))*xDir;
			if(0 > boardX || boardX > boardW - 1)
				return;
			if(0 > boardY || boardY > boardH - 1)
				return;
			Tile replacement = toReplace[i][j];
			board[boardY][boardX] = replacement;
		}
	}
}

int main(int argc, char* argv[])
{
	string rulesFile = "rules.json";
	if(argc > 1)
	{
		rulesFile = argv[1];
	}

	//Rand setup
	int seed = time(NULL)+getpid();
	srand(seed);
	system("clear");

	//Rule file setup
	std::ifstream f(rulesFile);
	json rulesJson = json::parse(f);


	//Rule file parsing
	for(json ruleGroup : rulesJson["rules"])
	{
		vector<vector<Rule>> ruleGroupElements;
		for(json rulePriority : ruleGroup)
		{
			vector<Rule> rulePriorityElements;
			for(json rule : rulePriority)
			{
				vector<vector<Tile>> toMatch;
				vector<vector<Tile>> toReplace;
				for(json slice : rule[0])
				{
					vector<Tile> sliceElements;
					for(json element : slice)
					{
						sliceElements.push_back(tileMap.find(element)->second);
					}
					toMatch.push_back(sliceElements);
				}
				for(json slice : rule[1])
				{
					vector<Tile> sliceElements;
					for(json element : slice)
					{
						sliceElements.push_back(tileMap.find(element)->second);
					}
					toReplace.push_back(sliceElements);
				}
				Rule toAdd = {toMatch, toReplace, rule[2].get<int>()};
				rulePriorityElements.push_back(toAdd);
			}
			ruleGroupElements.push_back(rulePriorityElements);
		}
		rules.push_back(ruleGroupElements);
	}

	//Board init
	boardW = rulesJson["board"]["boardW"].get<int>();
	boardH = rulesJson["board"]["boardH"].get<int>();
	board = new Tile*[boardH];
	for(int y = 0; y < boardH; y++)
	{
		board[y] = new Tile[boardW];
		for(int x = 0; x < boardW; x++)
		{
			board[y][x] = Black;
		}
	}
	if(rulesJson["board"].contains("starts"))
	{
		for(json start : rulesJson["board"]["starts"])
		{
			int x = start["x"].get<int>();
			int y = start["y"].get<int>();
			Tile t = tileMap.find(start["tile"])->second;
			board[y][x] = t;
		}
	}
	//board[rand()%boardH][rand()%boardW] = W;
	//drawBoard();
	bool found = false;
	for(int group = 0; group < rules.size(); group+=found?0:1)
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
						for(Dir dir : dirList)
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
	//drawBoard();
	cout << "SEED:\n\t" << seed << endl;
	/*
	cout << "RULES:" << endl;
	for(json ruleGroup : rulesJson["rules"])
	{
		cout << "\tRule group:\n";
		for(json rulePriority : ruleGroup)
		{
			cout << "\t\tRule prio:\n";
			for(json rule : rulePriority)
			{
				cout << "\t\t\t" << rule << endl;
			}
		}
	}
	*/


	//Cleanup
	for(int i = 0; i < boardH; i++)
	{
		delete[] board[i];
	}
	delete[] board;
}

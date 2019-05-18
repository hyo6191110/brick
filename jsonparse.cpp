#include"jsonparse.h"
using namespace cocos2d;
using namespace rapidjson;
using namespace std;

bool getAllBrickWithFile(const std::string& fileName, std::vector<BrickData*>& vec)
{
	rapidjson::Document document;
	string content = FileUtils::getInstance()->getStringFromFile(fileName);
	document.Parse<0>(content.c_str());

	if (document.HasParseError())
	{
		return false;
	}

	const rapidjson::Value& V_brick = document["bricks"];
	for (int i = 0; i < V_brick.Size(); i++)
	{
		int type = V_brick[i]["type"].GetInt();
		int x = V_brick[i]["x"].GetInt();
		int y = V_brick[i]["y"].GetInt();
		log("%d,%d,%d", type, x, y);
		auto brickdata = BrickData::create();
		brickdata->setType(type);
		brickdata->setX(x);
		brickdata->setY(y);
		vec.push_back(brickdata);
	}
	return true;
}


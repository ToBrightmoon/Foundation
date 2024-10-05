#include <iostream>
#include "param.hpp"

int main()
{
	using namespace BaseTool::Param;
	
	BaseTool::Param::ParamParse paramParse;

	if (paramParse.Init(R"(D:\project\cpp\BaseTool\param\param.json)") != BaseTool::Param::ErrorCode::NO_ERROR)
	{
		std::cout << "Init Fail" << "\n";
		return -1;
	}

	std::string name = "";

	if (paramParse.GetValue("name", name) != ErrorCode::NO_ERROR)
	{
		std::cout << "Parse Fail" << "\n";
	}

	std::cout << name << "\n";

	if (paramParse.GetValueByPath("/name", name) != ErrorCode::NO_ERROR)
	{
		std::cout << "Parse Fail" << "\n";
	}

	if (paramParse.GetValueByPath("/names", name) != ErrorCode::NO_ERROR)
	{
		std::cout << (int)paramParse.GetValueByPath("/names", name) << " ";
		std::cout << "Parse Fail" << "\n";
	}

	std::cout << name << "\n";

	if (paramParse.GetValueByPath("/features/path", name) != ErrorCode::NO_ERROR)
	{
		std::cout << "Parse Fail" << "\n";
	}

	std::cout << name << "\n";

	std::vector<int> vec;
	if (paramParse.GetValueByPath("/features/array", vec) != ErrorCode::NO_ERROR)
	{
		std::cout << "Parse Fail" << "\n";
	}

    for (int i: vec)
        std::cout << i << " ";

    std::cout << "\n";

	std::vector<nlohmann::json> modules;
	if (paramParse.GetValue("modules", modules) != ErrorCode::NO_ERROR)
	{
		std::cout << "Parse Fail" << "\n";
	}

	for (size_t i = 0; i < modules.size(); i++)
	{
		std::cout << modules.at(i).at("name").get<std::string>() << " ";
	}
	return 0;
}
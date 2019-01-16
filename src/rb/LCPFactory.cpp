#include "LCPFactory.h"

#include "Constraint.h"

using namespace RB;

std::unordered_map<std::string,
  std::function<void(std::vector<std::shared_ptr<Constraint>>&)>> LCPFactory::functions;
std::string LCPFactory::currSolver;

void LCPFactory::registerFunc
(std::string _name, std::function<void(std::vector<std::shared_ptr<Constraint>>&)> _func)
{
  auto i = functions.find(_name);
  if (i == functions.end())
  {
    functions[_name] = _func;
  }
}
void LCPFactory::unregisterFunc(std::string _name)
{
  auto i = functions.find(_name);
  if (i != functions.end())
  {
    functions.erase(i);
  }
}

void LCPFactory::setGlobal(std::string _name)
{
  auto i = functions.find(_name);
  if (i != functions.end())
  {
    currSolver = _name;
  }
}

std::function<void(std::vector<std::shared_ptr<Constraint>>&)>
LCPFactory::getFunction(std::string _name)
{
  auto i = functions.find(_name);
  if (i != functions.end())
  {
    return i->second;
  }
  else
  {
    return nullptr;
  }
}

std::function<void(std::vector<std::shared_ptr<Constraint>>&)>
LCPFactory::getGlobalFunction()
{
  if (!currSolver.empty())
  {
    return functions.find(currSolver)->second;
  }
  else
  {
    return nullptr;
  }
}
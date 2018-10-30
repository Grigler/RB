#include "IntegratorFactory.h"

#include "Body.h"

using namespace RB;

std::unordered_map<std::string,
  std::function<void(std::shared_ptr<Body>,float)>> IntegratorFactory::m_functions;

void IntegratorFactory::registerFunc
(std::string _name, std::function<void(std::shared_ptr<Body>,float)> _func)
{
  auto i = m_functions.find(_name);
  if (i == m_functions.end())
  {
    m_functions[_name] = _func;
  }
}
void IntegratorFactory::unregisterFunc(std::string _name)
{
  auto i = m_functions.find(_name);
  if (i != m_functions.end())
  {
    m_functions.erase(i);
  }
}
std::function<void(std::shared_ptr<Body>,float)> IntegratorFactory::getFunction(std::string _name)
{
  auto i = m_functions.find(_name);
  if (i != m_functions.end())
  {
    return i->second;
  }
  else
  {
    return nullptr;
  }
}
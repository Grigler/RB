#include "IntegratorFactory.h"

using namespace RB;

std::unordered_map<
  std::string,
  std::function<std::shared_ptr<Integrator> ()>
> IntegratorFactory::m_integrators;

void IntegratorFactory::registerIntegrator
(std::string _name, std::function<std::shared_ptr<Integrator> ()> _f)
{
  auto i = m_integrators.find(_name);
  if (i == m_integrators.end())
  {
    m_integrators[_name] = _f;
  }
}
void IntegratorFactory::unregisterIntegrator
(std::string _name, std::function<std::shared_ptr<Integrator> ()> _f)
{
  auto i = m_integrators.find(_name);
  if(i != m_integrators.end())
  {
    m_integrators.erase(i);
  }
}
std::shared_ptr<Integrator> IntegratorFactory::getIntegrator(std::string _name)
{
  auto i = m_integrators.find(_name);
  if (i != m_integrators.end())
  {
    return i->second();
  }
  else
  {
    return nullptr;
  }
}
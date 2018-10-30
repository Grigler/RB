#ifndef INTEGRATORFACTORY_H_
#define INTEGRATORFACTORY_H_

#include <unordered_map>
#include <functional>

namespace RB
{

  class Integrator;

  class IntegratorFactory
  {
  public:
    static void registerIntegrator
    (std::string _name, std::function<std::shared_ptr<Integrator> ()> _f);
    static void unregisterIntegrator
    (std::string _name, std::function<std::shared_ptr<Integrator> ()> _f);
    static std::shared_ptr<Integrator> getIntegrator(std::string _name);

  private:
    //Map of <name,shared_ptr>
    static std::unordered_map <
      std::string,
      std::function<std::shared_ptr<Integrator> ()>
    > m_integrators;

  };

}

#endif
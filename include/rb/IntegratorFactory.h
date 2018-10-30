#ifndef INTEGRATORFACTORY_H_
#define INTEGRATORFACTORY_H_

#include <unordered_map>
#include <memory>
#include <functional>

namespace RB
{

  class Integrator;
  class Body;


  class IntegratorFactory
  {
  public:
    static void registerFunc
      (std::string _name, std::function<void(std::shared_ptr<Body>,float)> _func);
    static void unregisterFunc(std::string _name);
    static std::function<void(std::shared_ptr<Body>,float)> getFunction(std::string _name);

  private:
    static std::unordered_map<std::string,
      std::function<void(std::shared_ptr<Body>,float)> > m_functions;
  };

}

#endif
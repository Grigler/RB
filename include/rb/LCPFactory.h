#ifndef LCPFACTORY_H_
#define LCPFACTORY_H_

#include <unordered_map>
#include <memory>
#include <functional>
#include <string>

namespace RB
{
  class Constraint;

  class LCPFactory
  {
  public:
    static void registerFunc
    (std::string _name, std::function<void(std::vector<std::shared_ptr<Constraint>>&)> _func);
    static void unregisterFunc(std::string _name);

    static void setGlobal(std::string _name);

    static std::function<void(std::vector<std::shared_ptr<Constraint>>&)> 
      getFunction(std::string _name);
    static std::function<void(std::vector<std::shared_ptr<Constraint>>&)>
      getGlobalFunction();
  
  private:
    static std::unordered_map<std::string,
      std::function<void(std::vector<std::shared_ptr<Constraint>>&)>> functions;

    static std::string currSolver;
  };
}

#endif
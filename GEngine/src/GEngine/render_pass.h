#pragma once
#include "common.h"
#include "shader.h"
#include <string>

namespace GEngine {
class CRenderPass {
public:
  CRenderPass();
  CRenderPass(const std::string &name, int order);
  CRenderPass(const std::string &name, int order, GEngine::ERenderPassType type);
  virtual ~CRenderPass();

  // implemented by user
  virtual void Init() = 0;
  virtual void Tick() = 0;

  bool operator<(const CRenderPass& ohter) const;
  bool operator>(const CRenderPass& ohter) const;

  const std::string &GetName() const { return pass_name_; }
  void SetName(const std::string &name) { pass_name_ = name; }

  const int GetOrder() const { return pass_order_; }
  void SetOrder(int order) { pass_order_ = order; }

  GEngine::ERenderPassType GetType() const { return pass_type_; }
  void SetType(GEngine::ERenderPassType type) { pass_type_ = type; }

  std::shared_ptr<CShader> shader_;

private:
  std::string pass_name_;
  int pass_order_ = -1;
  GEngine::ERenderPassType pass_type_ = GEngine::ERenderPassType::Default;
};
} // namespace GEngine
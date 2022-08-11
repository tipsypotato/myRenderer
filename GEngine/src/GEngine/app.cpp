#include <iostream>
#include <memory>
#include <string>
#include "GEngine/app.h"
#include "GEngine/render_system.h"
#include "GEngine/input_system.h"
#include "GEngine/shader.h"
#include "GEngine/log.h"
#include "GEngine/model.h"
#include "GEngine/mesh.h"
#include "GEngine/texture.h"

GEngine::CApp::CApp()
{
}

GEngine::CApp::~CApp()
{
}

GLvoid GEngine::CApp::Init()
{
  GEngine::CLog::Init();
  CSingleton<CRenderSystem>()->GetOrCreateWindow()->Init();
  window_ = CSingleton<CRenderSystem>()->GetOrCreateWindow()->GetGLFWwindow();
  CSingleton<CInputSystem>()->Init(); 
  CSingleton<CRenderSystem>()->Init();
  // renderpass init
  for(auto& pass : CSingleton<CRenderSystem>()->GetRenderPass()) {
    pass->Init();
  }
}

GLvoid GEngine::CApp::RunMainLoop() {
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  auto basic_shader  = std::make_shared<CShader>(std::string("../../shaders/vert.glsl"),
                                                 std::string("../../shaders/frag.glsl"));
  auto sponza_shader = std::make_shared<CShader>(std::string("../../shaders/sponza_VS.glsl"), 
                                                 std::string("../../shaders/sponza_FS.glsl"));
  auto pbr_shader = std::make_shared<CShader>(std::string("../../shaders/PBR_MR_VS.glsl"),
                                              std::string("../../shaders/PBR_MR_FS.glsl"));
  auto light_shader = std::make_shared<CShader>(std::string("../../shaders/light_sphere_VS.glsl"),
                                                std::string("../../shaders/light_sphere_FS.glsl"));

  bool render_light = false;

  bool render_sponza_phong = false;
  bool render_sponza_pbr = true;

  bool render_pbr_sphere = false;

  // texture
  if(render_light) {
    std::string texture_path = "../../assets/textures/marble.jpg";
    auto marble_texture = std::make_shared<CTexture>(texture_path); 
    basic_shader->SetTexture("diffuse_marble", marble_texture);
  }

  // mesh
  // std::string model_path("../../assets/model/glTF/DamagedHelmet.gltf");
  // std::string model_path("../../assets/model/backpack/backpack.obj");
  // std::string model_path("../../assets/model/Lucy/Lucy.obj"); // Large model
  auto mesh_sponza = std::make_shared<GEngine::CMesh>();
  std::string sponza_model_path;
  if(render_sponza_phong) {
    // sponza_model_path = "../../assets/model/sponza/sponza.obj";
    sponza_model_path = "../../assets/model/sponza/Scale300Sponza.obj";
  } else if(render_sponza_pbr) {
    sponza_model_path = "../../assets/model/SponzaPBR/SponzaPBR.obj";
  }
  mesh_sponza->LoadMesh(sponza_model_path);

  // render loop
  while (!glfwWindowShouldClose(window_)) {
    CalculateTime();
    CSingleton<CRenderSystem>()->GetOrCreateMainCamera()->Tick();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ticking the render passes
    auto render_passes = CSingleton<CRenderSystem>()->GetRenderPass();
    for (int i = 0; i < render_passes.size(); i++) {
      if (render_passes[i]->GetOrder() == -1)
        continue;
      switch (render_passes[i]->GetType()) {
      case GEngine::CRenderPass::ERenderPassType::Once:
        render_passes[i]->Tick();
        render_passes[i]->SetOrder(-1);
        break;
      default:
        render_passes[i]->Tick();
        break;
      }
    }
    // render the objects
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = CSingleton<CRenderSystem>()->GetOrCreateMainCamera()->GetViewMatrix();
    glm::mat4 projection = CSingleton<CRenderSystem>()->GetOrCreateMainCamera()->GetProjectionMatrix();
    glm::mat4 projection_view_model = projection * view * model;
    if(render_light) {
      basic_shader->Use();
      basic_shader->SetMat4("projection_view_model", projection_view_model);
      // CSingleton<CRenderSystem>()->RenderCube(basic_shader); // render cube
      CSingleton<CRenderSystem>()->RenderSphere(basic_shader); // render sphere
    }   
    // render model
    if (render_sponza_phong) {
      sponza_shader->Use();
      model = glm::mat4(1.0f);
      // model = glm::scale(model, glm::vec3(10.0, 10.0, 10.0)); 
      model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
      sponza_shader->SetMat4("u_model", model);
      sponza_shader->SetMat4("u_view", view);
      sponza_shader->SetMat4("u_projection", projection);
      mesh_sponza->Render(sponza_shader); // render model]
    } else if (render_sponza_pbr) {
      pbr_shader->Use();
      model = glm::mat4(1.0f);
      // model = glm::translate(model, glm::vec3(2, 2, 2));
      model = glm::scale(model, glm::vec3(10.0, 10.0, 10.0));
      pbr_shader->SetMat4("u_model", model);
      pbr_shader->SetMat4("u_view", view);
      pbr_shader->SetMat4("u_projection", projection);
      pbr_shader->SetVec3("u_basecolor", glm::vec3(0.5, 0.0, 0.0));
      pbr_shader->SetFloat("u_metallic", 0.0);
      pbr_shader->SetFloat("u_roughness", 0.5);
      pbr_shader->SetBool("u_linear_diffuse", false);
      mesh_sponza->Render(pbr_shader);
    }

    if(render_pbr_sphere) {
      pbr_shader->Use();
      pbr_shader->SetMat4("u_view", view);
      pbr_shader->SetMat4("u_projection", projection);
      int nrRows = 7;
      int nrColumns = 7;
      float spacing = 2.5;
      pbr_shader->SetVec3("u_basecolor", glm::vec3(0.5, 0.0, 0.0));
      // pbr_shader->SetBool("u_linear_diffuse", false); // no need for constant diffuse
      for (int row = 0; row < nrRows; ++row) {
        pbr_shader->SetFloat("u_metallic", (float)row / (float)nrRows);
        for (int col = 0; col < nrColumns; ++col) {
          pbr_shader->SetFloat("u_roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
          model = glm::mat4(1.0f);
          model = glm::translate(model, glm::vec3((float)(col - (nrColumns / 2)) * spacing, (float)(row - (nrRows / 2)) * spacing, 0.0f));
          pbr_shader->SetMat4("u_model", model);
          CSingleton<CRenderSystem>()->RenderSphere(pbr_shader);
        }
      }
      float x = 7.5f;
      std::vector<glm::vec3> lights_pos = {
        glm::vec3(-x,  x, 10.0),
        glm::vec3(-x, -x, 10.0),
        glm::vec3( x,  x, 10.0),
        glm::vec3( x, -x, 10.0),
      };
      light_shader->Use();
      light_shader->SetMat4("u_view", view);
      light_shader->SetMat4("u_projection", projection);
      for(int i=0; i<4; i++) {
          model = glm::mat4(1.0f);
          model = glm::translate(model, lights_pos[i]);
          model = glm::scale(model, glm::vec3(0.5f));
          light_shader->SetMat4("u_model", model);
          CSingleton<CRenderSystem>()->RenderSphere(light_shader);
      }
    }

    // ticking main GUI
    CSingleton<CRenderSystem>()->GetOrCreateMainUI()->Tick();

    glfwPollEvents();
    glfwSwapBuffers(window_);
  }
  glfwTerminate();
}

void GEngine::CApp::CalculateTime() {
  currentTime_ = glfwGetTime();
  deltaTime_ = currentTime_ - lastFrameTime_;
  lastFrameTime_ = currentTime_;

  ++frameCounter_;
  deltaTimeCounter_ += deltaTime_;
  if(deltaTimeCounter_ > 1.0) {
    currentFPS_ = frameCounter_;
    frameCounter_ = 0;
    deltaTimeCounter_ = 0.0;
  }
}

GLdouble GEngine::CApp::GetCurrentTime() const {
  return currentTime_;
}

GLdouble GEngine::CApp::GetDeltaTime() const {
  return deltaTime_;
}

GLuint GEngine::CApp::GetFPS() const {
  return currentFPS_;
}

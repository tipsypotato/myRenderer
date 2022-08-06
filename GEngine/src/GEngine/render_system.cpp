#include "render_system.h"
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GEngine::CRenderSystem::~CRenderSystem()
{
}

GEngine::CRenderSystem::CRenderSystem()
{
}

void GEngine::CRenderSystem::Init() {
  // set up main camera
  if (!main_camera_) {
    main_camera_ = std::make_shared<CCamera>();
    main_camera_->Init();
  }
  // set up main UI
  if (!main_UI_) {
    main_UI_ = std::make_shared<CEditorUI>();
    main_UI_->Init();
  }
}

std::shared_ptr<GEngine::CGLFWWindow>
GEngine::CRenderSystem::GetOrCreateWindow() {
  if (!window_) {
    window_ = std::make_shared<GEngine::CGLFWWindow>();
  }
  return window_;
}

std::shared_ptr<GEngine::CCamera>
GEngine::CRenderSystem::GetOrCreateMainCamera() {
  if (!main_camera_) {
    main_camera_ = std::make_shared<GEngine::CCamera>();
  }
  return main_camera_;
}

std::shared_ptr<GEngine::CEditorUI>
GEngine::CRenderSystem::GetOrCreateMainUI() {
  if (!main_UI_) {
    main_UI_ = std::make_shared<GEngine::CEditorUI>();
  }
  return main_UI_;
}

std::shared_ptr<GEngine::CModel> &
GEngine::CRenderSystem::GetOrCreateModelByPath(const std::string &path) {
  std::string::size_type pos = (path.find_last_of('\\') + 1) == 0
                                   ? path.find_last_of('/') + 1
                                   : path.find_last_of('\\') + 1;
  auto filename = path.substr(pos + 1);
  if (model_map_.find(filename) == model_map_.end()) {
    model_map_[filename] = std::make_shared<GEngine::CModel>(path);
  }
  return model_map_[filename];
}

void GEngine::CRenderSystem::RenderCube() {
  glBindVertexArray(GetOrCreateCubeVAO());
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

int GEngine::CRenderSystem::GetOrCreateCubeVAO() {
  if(cube_VAO_ != 0) {
      return cube_VAO_;
  }
  float cube_vertices[] = {
    // positions          // normal            // uv
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
      1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
      1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
      1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
      1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
      1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
      1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f};

  cube_VAO_ = CreateVAO(cube_vertices, sizeof(cube_vertices), {3, 3, 2});
  return cube_VAO_;
}

int GEngine::CRenderSystem::CreateVAO(const GLvoid *vertex_data, int data_size,
              std::initializer_list<int> attribute_layout, const int indices[],
              int indices_size, int *voVBO) {
  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, data_size, vertex_data, GL_STATIC_DRAW);
  if(indices) {
    // todo
  }
  int offset = 0;
  int i = 0;
  int stride = std::accumulate(attribute_layout.begin(), attribute_layout.end(), 0);
  for(auto length : attribute_layout) {
      glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, length, GL_FLOAT, GL_FALSE, stride * sizeof(GL_FLOAT), (GLvoid*)(offset * sizeof(GL_FLOAT)));
    offset += length;
    i++;
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  if(voVBO) {
    // todo
  }
  return VAO;
}

void GEngine::CRenderSystem::RenderCube(std::shared_ptr<CShader> shader) {
  shader->Use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = GetOrCreateMainCamera()->GetViewMatrix();
  glm::mat4 projection = GetOrCreateMainCamera()->GetProjectionMatrix();
  glm::mat4 projection_view_model = projection * view * model;
  shader->SetMat4("projection_view_model", projection_view_model);
  if(cube_VAO_ != 0) {
    // cube already exists
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 1);
    glBindVertexArray(cube_VAO_);
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    return;
  }
  float cube_vertices[] = {
    // positions          // normal            // uv
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
      1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
      1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
      1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
      1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
      1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
      1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
      1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
      1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f};

  // setup VAO & VBO
     unsigned int cube_VBO;
     glGenVertexArrays(1, &cube_VAO_);
     glBindVertexArray(cube_VAO_);
     glGenBuffers(1, &cube_VBO);
     glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
     glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices,
                  GL_STATIC_DRAW);

     glEnableVertexAttribArray(0); // pos
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT),
                           (void *)0);
     glEnableVertexAttribArray(1); // normal
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT),
                           (void *)(3 * sizeof(GL_FLOAT)));
     glEnableVertexAttribArray(2); // uv
     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT),
                           (void *)(6 * sizeof(GL_FLOAT)));

     glBindVertexArray(cube_VAO_);
     glEnable(GL_DEPTH_TEST);
     glDrawArrays(GL_TRIANGLES, 0, 36);
     // reset
     glBindVertexArray(0);
     glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int GEngine::CRenderSystem::LoadTexture(const std::string &path) {

  unsigned int textureID;
  glGenTextures(1, &textureID);

  // flip on load
  stbi_set_flip_vertically_on_load(true);

  int width, height, nrComponents;
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1) {
      format = GL_RED;
    } else if (nrComponents == 3) {
      format = GL_RGB;
    } else if (nrComponents == 4) {
      format = GL_RGBA;
    } else {
      GE_ERROR("Texture format error: {}", path);
      stbi_image_free(data);
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

  } else {
    GE_ERROR("Texture failed to load at path: {}", path);
    stbi_image_free(data);
  }
  return textureID;
}

void GEngine::CRenderSystem::RegisterRenderPass(
    const std::shared_ptr<GEngine::CRenderPass> &render_pass) {
  if (render_pass == nullptr) {
    return;
  }
  render_passes_.push_back(render_pass);
  std::sort(render_passes_.begin(), render_passes_.end(),
            [](const std::shared_ptr<GEngine::CRenderPass> render_pass1,
               const std::shared_ptr<GEngine::CRenderPass> render_pass2) {
              return render_pass1 < render_pass2;
            });
  // render_passes_.insert(
  //     std::lower_bound(
  //         render_passes_.begin(), render_passes_.end(), render_pass,
  //         [](const std::shared_ptr<GEngine::CRenderPass> render_pass1,
  //            const std::shared_ptr<GEngine::CRenderPass> render_pass2) {
  //           render_pass1 < render_pass2
  //         }),
  //     render_pass);
}
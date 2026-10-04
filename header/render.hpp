#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "camera.hpp"

struct GameObjects;
class Renderer;

struct TransformData {
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model;
    glm::vec4 texture; //x,y for offset, z,w for scale
};

struct Vertex {
    float x,y,z;
    float tx, ty; // texture coordinates
};

struct TextureAtlas {
    SDL_GPUTexture* texture;
    SDL_GPUSampler* sampler;
    glm::vec2 size;
    glm::vec2 itemSize;
    glm::vec4 getTransform(int x, int y) {
        return glm::vec4(x*itemSize.x / size.x, y * itemSize.y / size.y, itemSize.x / size.x, itemSize.y / size.y);
    }
};

struct TextureData {
    SDL_GPUSampler* sampler;
    TextureAtlas tree;
    TextureAtlas floor;
    TextureAtlas player;
    TextureAtlas building;
    TextureAtlas road;
    void loadTextures(Renderer* renderer);
};

class Renderer {
private:
    SDL_Window* window;
    SDL_GPUDevice* device;

    TransformData transformData;
    bool isPerspective = false;

    Camera &camera;
    GameObjects& gameObjects;

    SDL_GPUTexture* depthTexture;
    SDL_GPUBuffer* quadVBO;
    SDL_GPUGraphicsPipeline* quadPipeline;

    TextureData textureData;

    Uint32 width = 640;
    Uint32 height = 480;
    SDL_GPUBuffer* createVertexBuffer();
    SDL_GPUGraphicsPipeline* createQuadPipeline();
    SDL_GPUTexture* createDepthTexture();
public:
    Renderer(Camera& camera, GameObjects& gameObjects);
    int updateRendering();
    int cleanup();
    
    SDL_GPUSampler* createSampler();
    SDL_GPUTexture* createTexture(const char* path);
    
    void updateWindowSize();
    void bindQuadPipeline(SDL_GPURenderPass* renderPass);
    void bindTextureAtlas(SDL_GPURenderPass* renderPass, TextureAtlas textureAtlas);
    void performQuadRender(SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, glm::mat4 model, glm::vec4 textureAtlasTransform);
    
};

#endif
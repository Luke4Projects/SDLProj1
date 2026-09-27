#include "header/render.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "header/game.hpp"

static Vertex quadVertices[] = {
    {-0.5f,-0.5f, 0.0f, 1.0f, 1.0f},
    { 0.5f,-0.5f, 0.0f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
    {-0.5f, 0.5f, 0.0f, 1.0f, 0.0f},
    { 0.5f, 0.5f, 0.0f, 0.0f, 0.0f},
    { 0.5f,-0.5f, 0.0f, 0.0f, 1.0f},
};

void TextureData::loadTextures(Renderer* renderer) {
    sampler = sampler = renderer->createSampler();
    tree = TextureAtlas{
        .texture = renderer->createTexture("assets/Trees/Golden-Tree.png"),
        .sampler = sampler,
        .size = glm::vec2(1344, 1200),
        .itemSize = glm::vec2(108, 368),
    };
    floor = TextureAtlas{
        .texture = renderer->createTexture("assets/floor.png"),
        .sampler = sampler,
        .size = glm::vec2(16, 16),
        .itemSize = glm::vec2(512, 512),
    };
    player = TextureAtlas{
        .texture = renderer->createTexture("assets/Character/Idle/Idle-Sheet.png"),
        .sampler = sampler,
        .size = glm::vec2(256, 80),
        .itemSize = glm::vec2(64, 80)
    };
}

Renderer::Renderer(Camera& camera, GameObjects& gameObjects) : camera(camera), gameObjects(gameObjects) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("window", 640, 480, SDL_WINDOW_RESIZABLE);

    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);

    SDL_ClaimWindowForGPUDevice(device, window);

    quadVBO = createVertexBuffer();
    quadPipeline = createQuadPipeline();

    textureData.loadTextures(this);

    transformData = TransformData{
        .proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f),
        //.proj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f),
        .view = glm::mat4(1.0f),
        .model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)),
        .texture = textureData.tree.getTransform(0, 0)
    };
    transformData.model = glm::scale(transformData.model, glm::vec3(1.8f, 3.6f, 1.0f));

}

SDL_GPUBuffer* Renderer::createVertexBuffer() {

    SDL_GPUBufferCreateInfo vertexBufferInfo{
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof(quadVertices),
    };
    SDL_GPUBuffer* vertexBuffer = SDL_CreateGPUBuffer(device, &vertexBufferInfo);

    SDL_GPUTransferBufferCreateInfo transferBufferInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeof(quadVertices),
    };
    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferBufferInfo);
    Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(device, transferBuffer, true);
    SDL_memcpy(data, quadVertices, sizeof(quadVertices));
    SDL_UnmapGPUTransferBuffer(device, transferBuffer);

    SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);
    SDL_GPUTransferBufferLocation transferBufferLocation{
        .transfer_buffer = transferBuffer,
        .offset = 0,
    };
    SDL_GPUBufferRegion vertexBufferLocation{
        .buffer = vertexBuffer,
        .offset = 0,
        .size = sizeof(quadVertices)
    };
    SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &vertexBufferLocation, true);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmdBuffer);
    SDL_ReleaseGPUTransferBuffer(device, transferBuffer);

    return vertexBuffer;
}

SDL_GPUTexture* Renderer::createTexture(const char* path) {
    SDL_Surface* loadedImage = SDL_LoadPNG(path);
    if(loadedImage == NULL) {
        SDL_Log("Failed to load image: %s", SDL_GetError());
        return NULL;
    }
    SDL_Surface* imageData = SDL_ConvertSurface(loadedImage, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(loadedImage);

    SDL_GPUTextureCreateInfo textureInfo{
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = SDL_static_cast(Uint32, imageData->w),
        .height = SDL_static_cast(Uint32, imageData->h),
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
    };
    SDL_GPUTexture* texture = SDL_CreateGPUTexture(device, &textureInfo);

    const Uint32 uploadSize = SDL_static_cast(Uint32, imageData->pitch * imageData->h);

    SDL_GPUTransferBufferCreateInfo transferBufferInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = uploadSize
    };
    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferBufferInfo);
    Uint8* data = (Uint8*)SDL_MapGPUTransferBuffer(device, transferBuffer, false);
    SDL_memcpy(data, imageData->pixels, uploadSize);
    SDL_UnmapGPUTransferBuffer(device, transferBuffer);

    SDL_GPUTextureTransferInfo transferInfo{
        .transfer_buffer = transferBuffer,
        .offset = 0,
        .pixels_per_row = SDL_static_cast(Uint32, imageData->w),
        .rows_per_layer = SDL_static_cast(Uint32, imageData->h),
    };
    SDL_GPUTextureRegion destRegion{
        .texture = texture,
        .mip_level = 0,
        .layer = 0,
        .x = 0,
        .y = 0,
        .z = 0,
        .w = SDL_static_cast(Uint32, imageData->w),
        .h = SDL_static_cast(Uint32, imageData->h),
        .d = 1,
    };
    SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);
    SDL_UploadToGPUTexture(copyPass, &transferInfo, &destRegion, false);
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmdBuffer);
    SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
    SDL_DestroySurface(imageData);

    return texture;
}

SDL_GPUSampler* Renderer::createSampler() {
    SDL_GPUSamplerCreateInfo samplerInfo{
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .mag_filter = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
    };
    return SDL_CreateGPUSampler(device, &samplerInfo);
}

SDL_GPUGraphicsPipeline* Renderer::createQuadPipeline() {

    size_t vertCodeSize;
    size_t fragCodeSize;
    void* vertCode = SDL_LoadFile("shape.vert.spv", &vertCodeSize);
    void* fragCode = SDL_LoadFile("colored.frag.spv", &fragCodeSize);
    SDL_GPUShaderCreateInfo vertInfo{
        .code_size = vertCodeSize,
        .code = (const Uint8*)vertCode,
        .entrypoint = "main",
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = SDL_GPU_SHADERSTAGE_VERTEX,
        .num_samplers = 0,
        .num_storage_textures = 0,
        .num_storage_buffers = 0,
        .num_uniform_buffers = 1,
    };
    SDL_GPUShaderCreateInfo fragInfo{
        .code_size = fragCodeSize,
        .code = (const Uint8*)fragCode,
        .entrypoint = "main",
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
        .num_samplers = 1,
        .num_storage_textures = 0,
        .num_storage_buffers = 0,
        .num_uniform_buffers = 1,
    };
    SDL_GPUShader* vertShader = SDL_CreateGPUShader(device, &vertInfo);
    SDL_GPUShader* fragShader = SDL_CreateGPUShader(device, &fragInfo);
    SDL_free(vertCode);
    SDL_free(fragCode);

    SDL_GPUVertexBufferDescription vertexBufferDescriptions[1];
    vertexBufferDescriptions[0] = {
        .slot = 0,
        .pitch = sizeof(Vertex),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
    };
    SDL_GPUVertexAttribute vertexAttributes[2];
    vertexAttributes[0] = {
        .location = 0,
        .buffer_slot = 0,
        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
        .offset = 0,
    };
    vertexAttributes[1] = {
        .location = 1,
        .buffer_slot = 0,
        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
        .offset = sizeof(float) * 3,
    };

    SDL_GPUColorTargetDescription colorTargetDescriptions[1];
    colorTargetDescriptions[0] = {
        .format = SDL_GetGPUSwapchainTextureFormat(device, window)
    };

    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{
        .vertex_shader = vertShader,
        .fragment_shader = fragShader,
        .vertex_input_state = {
            .vertex_buffer_descriptions = vertexBufferDescriptions,
            .num_vertex_buffers = 1,
            .vertex_attributes = vertexAttributes,
            .num_vertex_attributes = 2,
        },
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .rasterizer_state = {},
        .multisample_state = {},
        .depth_stencil_state = {},
        .target_info = {
            .color_target_descriptions = colorTargetDescriptions,
            .num_color_targets = 1
        },
    };

    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
    SDL_ReleaseGPUShader(device, vertShader);
    SDL_ReleaseGPUShader(device, fragShader);

    return pipeline;
}

int Renderer::updateRendering() {

    transformData.view = camera.getViewMatrix();

    SDL_GPUTexture *swapchainTexture;
    Uint32 width, height;
    SDL_GPUCommandBuffer *cmdBuffer = SDL_AcquireGPUCommandBuffer(device);

    SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, window, &swapchainTexture, &width, &height);
    if (swapchainTexture == NULL)
    {
        SDL_SubmitGPUCommandBuffer(cmdBuffer);
        return 1;
    }

    SDL_GPUColorTargetInfo colorTargetInfo{
        .texture = swapchainTexture,
        .clear_color = {0.0f, 0.5f, 1.0f, 1.0f},
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE,
    };

    SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(cmdBuffer, &colorTargetInfo, 1, NULL);

    bindQuadPipeline(renderPass);

    SDL_GPUTextureSamplerBinding floorBinding{
        .texture = textureData.floor.texture,
        .sampler = textureData.sampler
    };
    SDL_BindGPUFragmentSamplers(renderPass, 0, &floorBinding, 1);
    transformData.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0));
    transformData.model = glm::scale(transformData.model, glm::vec3(50.0f, 50.0f, 1.0f));
    transformData.texture = textureData.floor.getTransform(0, 0);
    SDL_PushGPUVertexUniformData(cmdBuffer, 0, &transformData, sizeof(transformData));
    SDL_DrawGPUPrimitives(renderPass, 6, 1, 0, 0);

    bindTextureAtlas(renderPass, textureData.tree);

    for(int i = 0; i < 20; i++) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i - 10, 0, i - 20));
        model = glm::scale(model, glm::vec3(textureData.tree.itemSize.x/100, textureData.tree.itemSize.y/100, 1) );
        performQuadRender(renderPass, cmdBuffer, model, textureData.tree.getTransform(0, 0));
    }

    gameObjects.player.renderAsQuad(*this, renderPass, cmdBuffer, textureData.player);

    SDL_EndGPURenderPass(renderPass);

    SDL_SubmitGPUCommandBuffer(cmdBuffer);

    return 0;
}

int Renderer::cleanup() {
    SDL_ReleaseGPUTexture(device, textureData.tree.texture);
    SDL_ReleaseGPUSampler(device, textureData.sampler);
    SDL_DestroyGPUDevice(device);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void Renderer::bindQuadPipeline(SDL_GPURenderPass* renderPass) {
    SDL_BindGPUGraphicsPipeline(renderPass, quadPipeline);
    SDL_GPUBufferBinding bufferingBindings[1];
    bufferingBindings[0] = {
        .buffer = quadVBO,
        .offset = 0,
    };
    SDL_BindGPUVertexBuffers(renderPass, 0, bufferingBindings, 1);
}

void Renderer::bindTextureAtlas(SDL_GPURenderPass* renderPass, TextureAtlas textureAtlas) {
    SDL_GPUTextureSamplerBinding textureBinding{
        .texture = textureAtlas.texture,
        .sampler = textureAtlas.sampler,
    };
    SDL_BindGPUFragmentSamplers(renderPass, 0, &textureBinding, 1);
}

void Renderer::performQuadRender(SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, glm::mat4 model, glm::vec4 textureAtlasTransform) {
    transformData.model = model;
    transformData.texture = textureAtlasTransform;
    SDL_PushGPUVertexUniformData(cmdBuffer, 0, &transformData, sizeof(transformData));

    SDL_DrawGPUPrimitives(renderPass, 6, 1, 0, 0);
}
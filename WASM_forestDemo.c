//emcc -o WASM_forestDemo.js WASM_forestDemo.c -s EXPORTED_FUNCTIONS='["_initialize", "_renderPass", "_getFrameBuffer", "_deleteWebContext", "_initializeFromObj", "_renderPassWireFrame", "_updateExplodeScalar", "_updateColorBuffer", "_updateLightVector", "_addMatrixValue", "_addMatrixSlot","_randomizeColorBuffer", "_misc"]' -s --preload-file forestPondFIXED.obj
#include <string.h>
#include <emscripten.h>
#include "GraphicsEngine/raster/rasterizer.c"
#include "GraphicsEngine/3Dmath/operations.c"
#include "GraphicsEngine/engineTypes.h"
#include "GraphicsEngine/graphicsEngineFunctions.c"
#include "WASM_transforms.c"
#include "commonCoords.c"
#include "utahTeapot.c"
#include "GraphicsEngine/OBJParser/parser.c"


EMSCRIPTEN_KEEPALIVE
webContext* initialize(int height, int width){
    webContext* wc = malloc(sizeof(webContext));
    wc->rc = createRenderContext_RGBA(height, width);

    vertexBuffer* vb0 = createVertexBuffer(108);
    memcpy(vb0->inputVertices, correctedCube, sizeof(float) * 108);
    normalBuffer* nb0 = generateNormals(vb0);
    colorBuffer* cb0 = createColorBuffer(108);
    for(int i = 0; i < cb0->length; i++) cb0->inputColors[i] = 127;
    mesh* mesh0 = meshify(vb0, cb0, nb0);

    wc->sc = createScene(1);
    wc->sc->meshes[0] = mesh0;
    wc->sc->lightVector = malloc(sizeof(vec3)); 
    wc->sc->lightVector->x = 0; wc->sc->lightVector->y = 0; wc->sc->lightVector->z = -1;

    wc->ts = malloc(sizeof(transformSpec));
    wc->ts->translateX = 0;
    wc->ts->translateY = 0;
    wc->ts->translateZ = 5;
    wc->ts->rotateX = 45.0;
    wc->ts->rotateY = 45.0;
    wc->ts->rotateZ = 0.0;
    wc->ts->explodeScalar = 0.0;
    wc->ts->matrixArrayLength = 0;
    wc->ts->matrixArray = NULL;
    return wc;
}


EMSCRIPTEN_KEEPALIVE
webContext* initializeFromObj(int height, int width){
    webContext* wc = malloc(sizeof(webContext));
    wc->rc = createRenderContext_RGBA(height, width);

    object* obj = parseNoMTL("forestPondFIXED.obj");

    vertexBuffer* vb0 = createVertexBuffer(obj->faceCount * 9);
    memcpy(vb0->inputVertices, obj->faces, sizeof(float) * obj->faceCount * 9);
    normalBuffer* nb0 = generateNormals(vb0);
    colorBuffer* cb0 = createColorBuffer(obj->faceCount * 9);
    for(int i = 0; i < cb0->length; i++) cb0->inputColors[i] = 127;
    mesh* mesh0 = meshify(vb0, cb0, nb0);
    deleteObjNoMtl(obj);

    wc->sc = createScene(1);
    wc->sc->meshes[0] = mesh0;
    wc->sc->lightVector = malloc(sizeof(vec3)); 
    wc->sc->lightVector->x = 0; wc->sc->lightVector->y = 0; wc->sc->lightVector->z = -1;

    wc->ts = malloc(sizeof(transformSpec));
    wc->ts->translateX = 0;
    wc->ts->translateY = 2;
    wc->ts->translateZ = 10;
    wc->ts->rotateX = 180.0;
    wc->ts->rotateY = -90.0;
    wc->ts->rotateZ = 0.0;
    wc->ts->explodeScalar = 0.0;
    wc->ts->matrixArrayLength = 0;
    wc->ts->matrixArray = NULL;
    return wc;
}

EMSCRIPTEN_KEEPALIVE
void renderPass(webContext* wc, float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ){
    cleanRenderContext_RGBA(wc->rc);
    wc->ts->translateX += translateX;
    wc->ts->translateY += translateY;
    wc->ts->translateZ += translateZ;
    wc->ts->rotateX += rotateX;
    wc->ts->rotateY += rotateY;
    wc->ts->rotateZ += rotateZ;
    transform(wc->rc, wc->ts, wc->sc, wc->sc->meshes[0]->vb, wc->sc->meshes[0]->cb, wc->sc->meshes[0]->nb);
    rasterize_RGBA(wc->rc, wc->sc->meshes[0]->vb, wc->sc->meshes[0]->cb);
}

EMSCRIPTEN_KEEPALIVE
void renderPassWireFrame(webContext* wc, float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ){
    cleanRenderContext_RGBA(wc->rc);
    wc->ts->translateX += translateX;
    wc->ts->translateY += translateY;
    wc->ts->translateZ += translateZ;
    wc->ts->rotateX += rotateX;
    wc->ts->rotateY += rotateY;
    wc->ts->rotateZ += rotateZ;
    transform(wc->rc, wc->ts, wc->sc, wc->sc->meshes[0]->vb, wc->sc->meshes[0]->cb, wc->sc->meshes[0]->nb);
    rasterizeNoScanline_RGBA(wc->rc, wc->sc->meshes[0]->vb, wc->sc->meshes[0]->cb); 
}

EMSCRIPTEN_KEEPALIVE
void updateExplodeScalar(webContext* wc, float scalar){
    wc->ts->explodeScalar = scalar;
}

EMSCRIPTEN_KEEPALIVE
void randomizeColorBuffer(webContext* wc){
    for(int i = 0; i < wc->sc->meshes[0]->vb->length; i += 3){
        wc->sc->meshes[0]->cb->inputColors[i] = rand() % 255;
        wc->sc->meshes[0]->cb->inputColors[i + 1] = rand() % 255;
        wc->sc->meshes[0]->cb->inputColors[i + 2] = rand() % 255;
    }
}

EMSCRIPTEN_KEEPALIVE
void updateColorBuffer(webContext* wc, unsigned char r, unsigned char g, unsigned char b){
    for(int i = 0; i < wc->sc->meshes[0]->vb->length; i += 3){
        wc->sc->meshes[0]->cb->inputColors[i] = r;
        wc->sc->meshes[0]->cb->inputColors[i + 1] = g;
        wc->sc->meshes[0]->cb->inputColors[i + 2] = b;
    }
}

EMSCRIPTEN_KEEPALIVE
void updateLightVector(webContext* wc, float x, float y, float z){
    wc->sc->lightVector->x = x;
    wc->sc->lightVector->y = y;
    wc->sc->lightVector->z = z;
    normalizeVector(wc->sc->lightVector);
}

EMSCRIPTEN_KEEPALIVE
uint8_t* getFrameBuffer(webContext* wc){
    return wc->rc->frameBuffer;
}


EMSCRIPTEN_KEEPALIVE
void deleteWebContext(webContext* wc){
    deleteRenderContext(wc->rc);
    deleteScene(wc->sc);
    free(wc->ts);
    free(wc);
}

EMSCRIPTEN_KEEPALIVE
void addMatrixSlot(webContext* wc){
    float* new = realloc(wc->ts->matrixArray, sizeof(float) * (wc->ts->matrixArrayLength + 16));
     wc->ts->matrixArray = new;
}

EMSCRIPTEN_KEEPALIVE
void addMatrixValue(webContext* wc, float newVal){
    wc->ts->matrixArray[wc->ts->matrixArrayLength] = newVal;
    wc->ts->matrixArrayLength++;
}

//for debugging purposes
EMSCRIPTEN_KEEPALIVE
int misc(webContext* wc){
    return wc->sc->meshes[0]->vb->length;
}


// int main(){
//     webContext* wc = initializeFromObj(800, 600);
//     renderPass(wc, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
//     renderPass(wc, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
//     // unsigned char* ptr = getFrameBuffer(wc);
//     renderPass(wc, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
//     for(int i = 0; i < wc->rc->height * wc->rc->width * 4; i+=4){
//         if(wc->rc->frameBuffer[i] != 0 && wc->rc->frameBuffer[i] != 254)
//         printf(" %d, %d, %d, %d : %p\n ", wc->rc->frameBuffer[i], wc->rc->frameBuffer[i + 1],  wc->rc->frameBuffer[i + 2],  wc->rc->frameBuffer[i + 3], wc->rc->frameBuffer + i);
//     }
// } 
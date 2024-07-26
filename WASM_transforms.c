//this may be best as designated for user input
//there is a reason they use programmable vertex shaders
//this could be quite restrictive
void transform(renderContext* rc, transformSpec* ts, scene* sc, vertexBuffer* vb, colorBuffer* cb, normalBuffer* nb){
    matrix4x4 rotationMatrixX, rotationMatrixY, rotationMatrixZ, translationMatrix, scalingMatrix, perspectiveProjectionMatrix, screenSpaceMatrix;
    createRotationMatrixX(ts->rotateX, rotationMatrixX);
    createRotationMatrixY(ts->rotateY, rotationMatrixY);
    createRotationMatrixZ(ts->rotateZ, rotationMatrixZ);
    createTranslationMatrix(ts->translateX, ts->translateY, ts->translateZ, translationMatrix);
    createPerspectiveProjectionMatrix(45.0, 1.0, 10.0, 1000.0/700.0, perspectiveProjectionMatrix);
    for(int i = 0; i < vb->length; i += 3){ 
        vec4 temp;
        temp.x = vb->inputVertices[i];
        temp.y = vb->inputVertices[i + 1];
        temp.z = vb->inputVertices[i + 2];
        
        temp.w = 1.0f;

        vec3 normTemp;
        normTemp.x = nb->normals[i];
        normTemp.y = nb->normals[i + 1];
        normTemp.z = nb->normals[i + 2];

        vec4 normTempH = homogenizeVector(normTemp);
        // vec4 zeroVec; zeroVec.x = 0; zeroVec.y = 0; zeroVec.z = 0; zeroVec.w = 0;
        temp = addVectors(temp, scaleVector(normTempH, ts->explodeScalar));
        // temp = addVectors(temp, zeroVec);
        
        vecByMatrix4x4(&temp, rotationMatrixX);
        
        vecByMatrix4x4(&temp, rotationMatrixY);
        vecByMatrix4x4(&temp, rotationMatrixZ);
        
        vecByMatrix4x4(&normTempH, rotationMatrixX);
        vecByMatrix4x4(&normTempH, rotationMatrixY);
        vecByMatrix4x4(&normTempH, rotationMatrixZ);
        
        vecByMatrix4x4(&temp, translationMatrix);

        for(int i = 0; i < ts->matrixArrayLength; i += 16){
            vecByMatrix4x4(&temp, &ts->matrixArray[i]);
        }
        
        perspectiveProjection(&temp, perspectiveProjectionMatrix);

        normTemp = dehomogenizeVector(normTempH);
        normalizeVector(&normTemp);
        float lightScalar = dotProduct(normTemp, *sc->lightVector);
        //printf("%f, %f, %f\n", normTemp.x, normTemp.y, normTemp.z);
        //printf("%f\n", lightScalar);
        lightScalar += 1;
        if(dotProduct(normTemp, *sc->cameraVector) < -0.5){
            vb->indexBuffer[i/3] = 0;
        }else vb->indexBuffer[i/3] = 1;
        cb->colors[i] = RGBClamp(cb->inputColors[i] * lightScalar);
        cb->colors[i + 1] = RGBClamp(cb->inputColors[i + 1] * lightScalar);
        cb->colors[i + 2] = RGBClamp(cb->inputColors[i + 2] * lightScalar);
        
        
        
        perspectiveDivide(&temp);
        NDCToScreenSpace(&temp, 1.0, 100.0, rc->height, rc->width);
        //printf("%f, %f, %f, %f\n", temp.x, temp.y, temp.z, temp.w);
        //create the temporary VBO
        vb->vertices[i] = temp.x;
        vb->vertices[i + 1] = temp.y;
        //we are now w-buffering, maybe a more thorough implementation would be good
        vb->vertices[i + 2] = temp.w * 25;
        // printf("%f\n", temp.w);
    }
}
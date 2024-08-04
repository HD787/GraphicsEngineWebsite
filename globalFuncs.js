function transform(){
    let tx = 0.0, ty = 0.0, tz = 0.0, rx = 0.0, ry = 0.0, rz = 0.0;
   
    if (keyStates['KeyW']){
        ty -= 0.05;
    }
    if (keyStates['KeyA']){
        tx -= 0.05;
    }
    if (keyStates['KeyS']){
        ty += 0.05;
    }
    if (keyStates['KeyD']){
        tx += 0.05;
    }
    if (keyStates['Space']){
        tz += 0.05;
    }
    if (keyStates['KeyC']){
        tz -= 0.05;
    }
    if (keyStates['KeyE']){
        ry += 1.0;
    }
    if (keyStates['KeyF']){
        ry -= 1.0;
    }
    if (keyStates['KeyR']){
        rx += 1.0;
    }
    if (keyStates['KeyG']){
        rx -= 1.0;
    }
    if (keyStates['KeyT']){
        rz += 1.0;
    }
    if (keyStates['KeyH']){
        rz -= 1.0;
    }
    if(renderMode === 1){
        renderPassWireFrame(wcPtr, tx, ty, tz, rx, ry, rz);
        ctx.putImageData(imageData, 0, 0);
    }
    else if(renderMode === 0){
        renderPass(wcPtr, tx, ty, tz, rx, ry, rz);
        ctx.putImageData(imageData, 0, 0);
    }
}

function changeMenus(){
    event.target.blur()
    const menuIds = ["controls", "transforms", "rasterization"];
    menuIds.forEach(id => {
        document.getElementById(id).classList.add("hidden");
    });
    document.getElementById(this.dataset.target).classList.remove("hidden");
}

function submitNewExplodeScalar(scalar){
    event.target.blur()
    updateExplodeScalar(wcPtr, scalar);
    render();
    document.getElementById("explodeScalarValue").textContent = scalar.toString();
}

function submitNewColor(r, g, b){
    event.target.blur()
    r = clampUnsignedChar(r);
    g = clampUnsignedChar(g);
    b = clampUnsignedChar(b);
    updateColorBuffer(wcPtr, r, g, b);
    render();
}

function randomizeColors(){
    console.log("here");
    event.target.blur();
    randomizeColorBuffer(wcPtr);
    render();
}

function submitNewLightVector(x, y, z){
    event.target.blur()
    updateLightVector(wcPtr, x, y, z)
    render();
}

function submitNewMatrix(){
    event.target.blur()
    const matrixInputs = document.querySelectorAll('#matrixInput input[type="number"]');
    const matrix = Array.from(matrixInputs).map(input => parseFloat(input.value));
    addMatrixSlot(wcPtr)
    for(i = 0; i < 16; i++){
       addMatrixValue(wcPtr, matrix[i]);
    }
    render();
}

function clampUnsignedChar(value){
    if(value > 255) return 255;
    if(value < 0) return 0;
    return value;
}

function changeRenderMode(){
    event.target.blur()
    if(renderMode == 0){
        renderMode = 1;
        document.getElementById("renderWireFrame").innerText = "Enable Scanline"
        render()
        return;
    }
    if(renderMode == 1){
        renderMode = 0;
        document.getElementById("renderWireFrame").innerText = "Disable Scanline"
    }
    render();
}

function render(){
    if(renderMode === 1){
        renderPassWireFrame(wcPtr, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        ctx.putImageData(imageData, 0, 0);
        return;
    }
    renderPass(wcPtr, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    ctx.putImageData(imageData, 0, 0);
}




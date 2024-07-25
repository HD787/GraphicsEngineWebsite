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
    renderPass(wcPtr, tx, ty, tz, rx, ry, rz);
    ctx.putImageData(imageData, 0, 0);
}

function changeMenus(){
    const menuIds = ["controls", "transforms", "rasterization"];
    menuIds.forEach(id => {
        document.getElementById(id).classList.add("hidden");
    });
    console.log(this.target);
    document.getElementById(this.dataset.target).classList.remove("hidden");
}

function submitNewExplodeScalar(scalar){
    updateExplodeScalar(wcPtr, scalar);
    renderPass(wcPtr, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    ctx.putImageData(imageData, 0, 0);
    document.getElementById("explodeScalarValue").textContent = scalar.toString();
}

function submitNewColor(r, g, b){
    r = clampUnsignedChar(r);
    g = clampUnsignedChar(g);
    b = clampUnsignedChar(b);
    updateColorBuffer(wcPtr, r, g, b);
    renderPass(wcPtr, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    ctx.putImageData(imageData, 0, 0);
}

function submitNewLightVector(x, y, z){
    updateLightVector(wcPtr, x, y, z)
    renderPass(wcPtr, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    ctx.putImageData(imageData, 0, 0);
}

function clampUnsignedChar(value){
    if(value > 255) return 255;
    if(value < 0) return 0;
    return value;
}




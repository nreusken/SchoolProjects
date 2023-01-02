class Cube{
  constructor(){
    this.type = "cube";
    this.color = [1.0, 1.0, 1.0, 1.0];
    this.matrix = new Matrix4();
    this.normalMatrix = new Matrix4();
    this.textureNum = -2;
  
  }

  render(){
    var rgba = this.color;

    gl.uniform1i(u_whichTexture, this.textureNum);

  
    // Pass the color of a point to u_FragColor variable
    gl.uniform4f(u_FragColor, rgba[0], rgba[1], rgba[2], rgba[3]);
    //pass the size of the point to u_Size variable
    gl.uniformMatrix4fv(u_ModelMatrix, false, this.matrix.elements);
    gl.uniformMatrix4fv(u_NormalMatrix, false, this.normalMatrix.elements);
    // Draw
 

    //Front of cube
    drawTrianlge3DUVNormal([0.0, 0.0, 0.0,  1.0, 1.0, 0.0,  1.0, 0.0, 0.0], [0,0, 1,1, 1,0], [0,0,-1, 0,0,-1 ,0,0,-1]);
    drawTrianlge3DUVNormal([0.0, 0.0, 0.0,  0.0, 1.0, 0.0,  1.0, 1.0, 0.0], [0,0, 0,1, 1,1], [0,0,-1, 0,0,-1 ,0,0,-1]);

    //gl.uniform4f(u_FragColor, rgba[0]*.9, rgba[1]*.9, rgba[2]*.9, rgba[3]);
    //top of cube
    drawTrianlge3DUVNormal([0.0, 1.0, 0.0,  0.0, 1.0, 1.0,  1.0, 1.0, 1.0], [0,0, 0,1, 1,1], [0,1,0, 0,1,0 ,0,1,0]);
    drawTrianlge3DUVNormal([0.0, 1.0, 0.0,  1.0, 1.0, 1.0,  1.0, 1.0, 0.0], [0,0, 1,1, 1,0], [0,1,0, 0,1,0 ,0,1,0]);
    //gl.uniform4f(u_FragColor, rgba[0]*.8, rgba[1]*.8, rgba[2]*.8, rgba[3]);

    //left side of the cube
    drawTrianlge3DUVNormal([0.0, 0.0, 0.0,  0.0, 1.0, 1.0,  0.0, 1.0, 0.0], [1,0, 0,1, 1,1], [-1,0,0, -1,0,0 ,-1,0,0]);
    drawTrianlge3DUVNormal([0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0, 1.0], [1,0, 0,0, 0,1], [-1,0,0, -1,0,0 ,-1,0,0]);

    //right side of the cube
    drawTrianlge3DUVNormal([1.0, 0.0, 0.0,  1.0, 1.0, 1.0,  1.0, 1.0, 0.0], [0,0, 1,1, 0,1], [1,0,0, 1,0,0 ,1,0,0]);
    drawTrianlge3DUVNormal([1.0, 0.0, 0.0,  1.0, 0.0, 1.0,  1.0, 1.0, 1.0], [0,0, 1,0, 1,1], [1,0,0, 1,0,0 ,1,0,0]);

    //gl.uniform4f(u_FragColor, rgba[0]*.7, rgba[1]*.7, rgba[2]*.7, rgba[3]);

    //back of the cube
    drawTrianlge3DUVNormal([0.0, 0.0, 1.0,  0.0, 1.0, 1.0,  1.0, 1.0, 1.0], [1,0, 1,1, 0,1], [0,0,1, 0,0,1 ,0,0,1]);
    drawTrianlge3DUVNormal([0.0, 0.0, 1.0,  1.0, 0.0, 1.0,  1.0, 1.0, 1.0], [1,0, 0,0, 0,1], [0,0,1, 0,0,1 ,0,0,1]);

    //gl.uniform4f(u_FragColor, rgba[0]*.6, rgba[1]*.6, rgba[2]*.6, rgba[3]);
    //bottum of the cube
    drawTrianlge3DUVNormal([0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  1.0, 0.0, 1.0], [0,0, 0,1, 1,1], [0,-1,0, 0,-1,0 ,0,-1,0]);
    drawTrianlge3DUVNormal([0.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 1.0], [0,0, 1,0, 1,1], [0,-1,0, 0,-1,0 ,0,-1,0]);
  } 

  renderFast(){
    var rgba = this.color;

    gl.uniform1i(u_whichTexture, this.textureNum);

  
    // Pass the color of a point to u_FragColor variable
    gl.uniform4f(u_FragColor, rgba[0], rgba[1], rgba[2], rgba[3]);
    //pass the size of the point to u_Size variable
    gl.uniformMatrix4fv(u_ModelMatrix, false, this.matrix.elements);
    // Draw
 
    var allVerts = [];
    var allUVVerts = []
    allVerts = allVerts.concat([0.0, 0.0, 0.0,  1.0, 1.0, 0.0,  1.0, 0.0, 0.0]);
    allUVVerts = allUVVerts.concat([0,0, 1,1, 1,0]);
    allVerts = allVerts.concat([0.0, 0.0, 0.0,  0.0, 1.0, 0.0,  1.0, 1.0, 0.0]);
    allUVVerts = allUVVerts.concat([0,0, 0,1, 1,1]);


    allVerts = allVerts.concat([0.0, 1.0, 0.0,  0.0, 1.0, 1.0,  1.0, 1.0, 1.0]);
    allUVVerts = allUVVerts.concat([0,0, 0,1, 1,1]);
    allVerts = allVerts.concat([0.0, 1.0, 0.0,  1.0, 1.0, 1.0,  1.0, 1.0, 0.0]);
    allUVVerts = allUVVerts.concat([0,0, 1,1, 1,0]);

    allVerts = allVerts.concat([0.0, 0.0, 0.0,  0.0, 1.0, 1.0,  0.0, 1.0, 0.0]);
    allUVVerts = allUVVerts.concat([1,0, 0,1, 1,1]);
    allVerts = allVerts.concat([0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0, 1.0]);
    allUVVerts = allUVVerts.concat([1,0, 0,0, 0,1]);

    allVerts = allVerts.concat([1.0, 0.0, 0.0,  1.0, 1.0, 1.0,  1.0, 1.0, 0.0]);
    allUVVerts = allUVVerts.concat([0,0, 1,1, 0,1]);
    allVerts = allVerts.concat([1.0, 0.0, 0.0,  1.0, 0.0, 1.0,  1.0, 1.0, 1.0]);
    allUVVerts = allUVVerts.concat([0,0, 1,0, 1,1]);

    allVerts = allVerts.concat([0.0, 0.0, 1.0,  0.0, 1.0, 1.0,  1.0, 1.0, 1.0]);
    allUVVerts = allUVVerts.concat([1,0, 1,1, 0,1]);
    allVerts = allVerts.concat([0.0, 0.0, 1.0,  1.0, 0.0, 1.0,  1.0, 1.0, 1.0]);
    allUVVerts = allUVVerts.concat([1,0, 0,0, 0,1]);

    

    allVerts = allVerts.concat([0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  1.0, 0.0, 1.0]);
    allUVVerts = allUVVerts.concat([0,0, 0,1, 1,1]);
    allVerts = allVerts.concat([0.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 1.0]);
    allUVVerts = allUVVerts.concat([0,0, 1,0, 1,1]);

    drawTrianlge3DUV(allVerts, allUVVerts);
  }
}
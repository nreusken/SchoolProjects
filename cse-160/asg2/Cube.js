class Cube{
  constructor(){
    this.type = "cube";
    this.color = [1.0, 1.0, 1.0, 1.0];
    this.matrix = new Matrix4();
  }

  render(){
    var rgba = this.color;

    // Pass the position of a point to a_Position variable
  
    // Pass the color of a point to u_FragColor variable
    gl.uniform4f(u_FragColor, rgba[0], rgba[1], rgba[2], rgba[3]);
    //pass the size of the point to u_Size variable
    gl.uniformMatrix4fv(u_ModelMatrix, false, this.matrix.elements);
    // Draw
 

    //Front of cube
    drawTrianlge3D([0.0, 0.0, 0.0,  1.0, 1.0, 0.0,  1.0, 0.0, 0.0]);
    drawTrianlge3D([0.0, 0.0, 0.0,  0.0, 1.0, 0.0,  1.0, 1.0, 0.0]);

    gl.uniform4f(u_FragColor, rgba[0]*.9, rgba[1]*.9, rgba[2]*.9, rgba[3]);
    //top of cube
    drawTrianlge3D([0.0, 1.0, 0.0,  0.0, 1.0, 1.0,  1.0, 1.0, 1.0]);
    drawTrianlge3D([0.0, 1.0, 0.0,  1.0, 1.0, 1.0,  1.0, 1.0, 0.0]);
    gl.uniform4f(u_FragColor, rgba[0]*.8, rgba[1]*.8, rgba[2]*.8, rgba[3]);

    //left side of the cube
    drawTrianlge3D([0.0, 0.0, 0.0,  0.0, 1.0, 1.0,  0.0, 1.0, 0.0]);
    drawTrianlge3D([0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0, 1.0]);

    //left right of the cube
    drawTrianlge3D([1.0, 0.0, 0.0,  1.0, 1.0, 1.0,  1.0, 1.0, 0.0]);
    drawTrianlge3D([1.0, 0.0, 0.0,  1.0, 0.0, 1.0,  1.0, 1.0, 1.0]);

    gl.uniform4f(u_FragColor, rgba[0]*.7, rgba[1]*.7, rgba[2]*.7, rgba[3]);

    //back of the cube
    drawTrianlge3D([0.0, 0.0, 1.0,  0.0, 1.0, 1.0,  1.0, 1.0, 1.0]);
    drawTrianlge3D([0.0, 0.0, 1.0,  1.0, 0.0, 1.0,  1.0, 1.0, 1.0]);

    gl.uniform4f(u_FragColor, rgba[0]*.6, rgba[1]*.6, rgba[2]*.6, rgba[3]);
    //bottum of the cube
    drawTrianlge3D([0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  1.0, 0.0, 1.0]);
    drawTrianlge3D([0.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 1.0]);
  }
}
// ColoredPoint.js (c) 2012 matsuda
// Vertex shader program
var VSHADER_SOURCE =
  'precision mediump float;\n' +
  'attribute vec4 a_Position;\n' +
  'attribute vec2 a_UV;\n' +
  'attribute vec3 a_Normal;\n' +
  'varying vec2 v_UV;\n' +
  'varying vec3 v_Normal;\n' +
  'varying vec4 v_VertPos;\n' +
  'uniform mat4 u_ModelMatrix;\n' +
  'uniform mat4 u_NormalMatrix;\n' +
  'uniform mat4 u_GlobalRotateMatrix;\n' +
  'uniform mat4 u_ViewMatrix;\n' +
  'uniform mat4 u_ProjectionMatrix;\n' +
  'void main() {\n' +
  '  gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_GlobalRotateMatrix * u_ModelMatrix * a_Position;\n' +
  '  v_UV = a_UV;\n' +
  '  //v_Normal = a_Normal;\n' +
  '  v_Normal = normalize(vec3(u_NormalMatrix * vec4(a_Normal, 1)));\n' +
  '  v_VertPos = u_ModelMatrix * a_Position;\n' +
  '}\n';

// Fragment shader program
var FSHADER_SOURCE =
  'precision mediump float;\n' +
  'uniform vec4 u_FragColor;\n' +  // uniform
  'varying vec2 v_UV;\n' +
  'varying vec3 v_Normal;\n' +
  'uniform sampler2D u_Sampler0;\n' +
  'uniform sampler2D u_Sampler1;\n' +
  'uniform vec3 u_lightPos;\n' +
  'uniform vec3 u_cameraPos;\n' +
  'uniform int u_whichTexture;\n' +
  'uniform bool u_lightOn;\n' +
  'varying vec4 v_VertPos;\n' +
  'void main() {\n' +
  '  if(u_whichTexture == -3){\n' +
  '    gl_FragColor = vec4((v_Normal+1.0)/2.0, 1.0);\n' +
  '  }else if(u_whichTexture == -2){\n' +
  '    gl_FragColor = u_FragColor;\n' +
  '  }else if(u_whichTexture == -1){\n' +
  '   gl_FragColor = vec4(v_UV, 1.0, 1.0);\n' +
  '  }else if(u_whichTexture == 0){\n' +
  '   gl_FragColor = texture2D(u_Sampler0,v_UV);\n' +
  '  }else if(u_whichTexture == 1){\n' +
  '   gl_FragColor = texture2D(u_Sampler1,v_UV);\n' +
  '  }else{\n' +
  '   gl_FragColor = vec4(0,.7,.9, 1);\n' +
  '  }\n' +
  '  vec3 lightVector = u_lightPos-vec3(v_VertPos);\n' +
  '  float r = length(lightVector);\n' +
  '  //if(r < 1.0){\n' +
  '  // gl_FragColor = vec4(1,0,0,1);\n' +
  '  //} else if(r<2.0){\n' +
  '  // gl_FragColor = vec4(0,1,0,1);\n' +
  '  //}\n' +
  '  vec3 L = normalize(lightVector);\n' +
  '  vec3 N = normalize(v_Normal);\n' +
  '  float nDotL = max(dot(N,L), 0.0);\n' +
  '  vec3 R = reflect(-L,N);\n' +
  '  vec3 E = normalize(u_cameraPos-vec3(v_VertPos));\n' + 
  '  float specular = pow(max(0.0, dot(E,R)), 10.0);\n' + 
  '  vec3 diffuse = vec3(gl_FragColor) * nDotL * 0.7;\n' +
  '  vec3 ambient = vec3(gl_FragColor) * 0.3;\n' +
  '  if(u_lightOn){\n' +
  '   if(u_whichTexture == 1){\n' +
  '     gl_FragColor = vec4(specular + diffuse + ambient, 1.0);\n' +
  '   }else{\n' +
  '     gl_FragColor = vec4(diffuse + ambient, 1.0);\n' +
  '   };\n' +
  '  };\n' +
  '}\n';

//Global variables
let canvas;
let gl;
let a_Position;
let a_UV;
let a_Normal;
let u_FragColor;
let u_Size;
let u_ModelMatrix;
let u_GlobalRotateMatrix;
let u_Sampler0;
let u_Sampler1;
let u_whichTexture;
let u_ProjectionMatrix;
let u_ViewMatrix;
let u_lightPos;
let u_cameraPos;
let u_lightOn;
let u_NormalMatrix;
function setupWebGL(){
  // Retrieve <canvas> element
  canvas = document.getElementById('webgl');

  // Get the rendering context for WebGL
  gl = canvas.getContext("webgl", {preserveDrawingBuffer: true});
  if (!gl) {
    console.log('Failed to get the rendering context for WebGL');
    return;
  }

  gl.enable(gl.DEPTH_TEST);
}

function connectVariablesToGLSL(){
  // Initialize shaders
  if (!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)) {
    console.log('Failed to intialize shaders.');
    return;
  }

  // // Get the storage location of a_Position
  a_Position = gl.getAttribLocation(gl.program, 'a_Position');
  if (a_Position < 0) {
    console.log('Failed to get the storage location of a_Position');
    return;
  }

  // // Get the storage location of a_UV
  a_UV = gl.getAttribLocation(gl.program, 'a_UV');
  if (a_UV < 0) {
    console.log('Failed to get the storage location of a_UV');
    return;
  }

  // // Get the storage location of a_Normal
  a_Normal = gl.getAttribLocation(gl.program, 'a_Normal');
  if (a_Normal < 0) {
    console.log('Failed to get the storage location of a_Normal');
    return;
  }


  // Get the storage location of u_FragColor
  u_FragColor = gl.getUniformLocation(gl.program, 'u_FragColor');
  if (!u_FragColor) {
    console.log('Failed to get the storage location of u_FragColor');
    return;
  }

  // Get the storage location of u_ModelMatrix
  u_ModelMatrix = gl.getUniformLocation(gl.program, 'u_ModelMatrix');
  if (!u_ModelMatrix) {
    console.log('Failed to get the storage location of u_ModelMatrix');
    return;
  }

  // Get the storage location of u_GlobalRotateMatrix
  u_GlobalRotateMatrix = gl.getUniformLocation(gl.program, 'u_GlobalRotateMatrix');
  if (!u_GlobalRotateMatrix) {
    console.log('Failed to get the storage location of u_GlobalRotateMatrix');
    return;
  }

  // Get the storage location of u_Sampler0
  u_Sampler0 = gl.getUniformLocation(gl.program, 'u_Sampler0');
   if (!u_Sampler0) {
     console.log('Failed to get the storage location of u_Sampler0');
     return false;
   }

  // Get the storage location of u_Sampler1
  u_Sampler1 = gl.getUniformLocation(gl.program, 'u_Sampler1');
   if (!u_Sampler1) {
     console.log('Failed to get the storage location of u_Sampler1');
     return false;
   }


  // Get the storage location of u_whichTexture
  u_whichTexture = gl.getUniformLocation(gl.program, 'u_whichTexture');
   if (!u_whichTexture) {
     console.log('Failed to get the storage location of u_whichTexture');
     return false;
   }
  // Get the storage location of u_ProjectionMatrix
  u_ProjectionMatrix = gl.getUniformLocation(gl.program, 'u_ProjectionMatrix');
   if (!u_ProjectionMatrix) {
     console.log('Failed to get the storage location of u_ProjectionMatrix');
     return false;
   }
  // Get the storage location of u_ViewMatrix
  u_ViewMatrix = gl.getUniformLocation(gl.program, 'u_ViewMatrix');
   if (!u_ViewMatrix) {
     console.log('Failed to get the storage location of u_ViewMatrix');
     return false;
   }
  
  // Get the storage location of u_lightPos
  u_lightPos = gl.getUniformLocation(gl.program, 'u_lightPos');
   if (!u_lightPos) {
     console.log('Failed to get the storage location of u_lightPos');
     return false;
   }

   // Get the storage location of u_cameraPos
  u_cameraPos = gl.getUniformLocation(gl.program, 'u_cameraPos');
  if (!u_cameraPos) {
    console.log('Failed to get the storage location of u_cameraPos');
    return false;
  }

   // Get the storage location of u_lightOn
   u_lightOn = gl.getUniformLocation(gl.program, 'u_lightOn');
   if (!u_lightOn) {
     console.log('Failed to get the storage location of u_lightOn');
     return false;
   }

   // Get the storage location of u_NormalMatrix
   u_NormalMatrix = gl.getUniformLocation(gl.program, 'u_NormalMatrix');
   if (!u_NormalMatrix) {
     console.log('Failed to get the storage location of u_NormalMatrix');
     return false;
   }
}

function initTextures() {
  // Create the image object
  var image = new Image();
  if (!image) {
    console.log('Failed to create the image object');
    return false;
  }

  var image1 = new Image();
  if (!image1) {
    console.log('Failed to create the image1 object');
    return false;
  }

  // Register the event handler to be called when image loading is completed
  image.onload = function(){ sendTextureToGLSL(image, image1); };
  // Tell the browser to load an Image
  image.src = 'brick.jpg';
  image1.src = 'drit.jpg';
  

  return true;
}

function sendTextureToGLSL(image, image1) {
  // Create a texture object
  var texture = gl.createTexture();
  if (!texture) {
    console.log('Failed to create the texture object');
    return false;
  }

  gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);  // Flip the image's y axis
  // Activate texture unit0
  gl.activeTexture(gl.TEXTURE0);
  // Bind the texture object to the target
  gl.bindTexture(gl.TEXTURE_2D, texture);

  // Set the texture parameter
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
  // Set the image to texture
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image);
  
  // Set the texture unit 0 to the sampler
  gl.uniform1i(u_Sampler0, 0);


  // Create a texture object
  var texture1 = gl.createTexture();
  if (!texture1) {
    console.log('Failed to create the texture1 object');
    return false;
  }

  gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);  // Flip the image's y axis
  // Activate texture unit0
  gl.activeTexture(gl.TEXTURE1);
  // Bind the texture object to the target
  gl.bindTexture(gl.TEXTURE_2D, texture1);

  // Set the texture parameter
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
  // Set the image to texture
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image1);
  
  // Set the texture unit 0 to the sampler
  gl.uniform1i(u_Sampler1, 1);
  
  // Clear <canvas>
  gl.clear(gl.COLOR_BUFFER_BIT);

  // Draw the rectangle
  // gl.drawArrays(gl.TRIANGLE_STRIP, 0, n);
}


//constants
const POINT = 0;
const TRIANGLE = 1;
const CIRCLE = 2;

//globals related to UI elements
let g_selectedColor = [1.0, 1.0, 1.0, 1.0];
let g_selectedSize = 5;
let g_selectedType = POINT;
let g_selectedSegements = 10;
let g_globalAngle = 0;
let g_armAngle = 225;
let g_bottomArmAngle = -45;
let g_animation = false;
let g_specialAnimation = false;
let g_rightEye = 0;
let g_leftEye = 0;
let g_legs = 0;
let g_rightLegs = 0;
let g_color = [.15, .15, .15, 1];
let g_normalOn = false;
let g_lightPos = [0,1,-2];
let g_lightOn = true;
function addActionsforHtmlUI(){

  document.getElementById("lightSliderX").addEventListener("mousemove", function(ev){ if(ev.buttons == 1){g_lightPos[0]=this.value/100; renderScene();}});
  document.getElementById("lightSliderY").addEventListener("mousemove", function(ev){ if(ev.buttons == 1){g_lightPos[1]=(this.value/100)+2.3; renderScene();}});
  document.getElementById("lightSliderZ").addEventListener("mousemove", function(ev){ if(ev.buttons == 1){g_lightPos[2]=this.value/100; renderScene();}});


  document.getElementById("animationOff").onclick = function() {g_animation = false;g_color = g_color = [.15, .15, .15, 1];};
  document.getElementById("animationOn").onclick = function() {g_animation = true;};

  document.getElementById("normalOff").onclick = function() {g_normalOn = false};
  document.getElementById("normalOn").onclick = function() {g_normalOn = true;};

  document.getElementById("lightOff").onclick = function() {g_lightOn = false};
  document.getElementById("lightOn").onclick = function() {g_lightOn = true;};

  document.getElementById("armSlider").addEventListener("mousemove", function(){ g_armAngle = this.value; renderScene(); });
  document.getElementById("bottomArmSlider").addEventListener("mousemove", function(){ g_bottomArmAngle = this.value; renderScene(); });
  

}

function main() {
  
  //sets up canvas and GL variables
  setupWebGL();

  //Set up GLSL shader programs and connect GLSL variables
  connectVariablesToGLSL();

  addActionsforHtmlUI();

  document.onkeydown = keydown;

  initTextures();

  // Register function (event handler) to be called on a mouse press
  canvas.onmousedown = click;
  canvas.onmousemove = function(ev) { if(ev.buttons == 1){click(ev) } };
  // Specify the color for clearing <canvas>
  gl.clearColor(0.0, 0.0, 0.0, 1.0);

  // Clear <canvas>
  renderScene();
  requestAnimationFrame(tick);
}

var g_startTime = performance.now()/1000.0;
var g_seconds = performance.now()/1000.0-g_startTime;


function tick(){
  g_seconds = performance.now()/500.0-g_startTime;

  updateAnimateAngles();

  renderScene();

  requestAnimationFrame(tick);
}

function updateAnimateAngles(){
  if(g_animation){
    g_armAngle = 245 + (20*Math.sin(g_seconds));
    g_bottomArmAngle = -45 + (30*Math.sin(g_seconds));
    g_lightPos[0] = Math.cos(g_seconds);
    
  }
  if(g_specialAnimation){
   
    g_armAngle = 245 + (20*Math.sin(4*g_seconds));
    g_bottomArmAngle = -45 + (30*Math.sin(4*g_seconds));
    g_rightEye = (Math.sin(4*g_seconds))/20;
    g_color = [Math.random(), Math.random(), Math.random(), 1];
  }
  
  
}

function click(ev) {
  if(ev.shiftKey){
    
    var x_cord = Math.round(g_at.elements[0])+23;
    var y_cord = Math.round(g_at.elements[2])+20;
    console.log(y_cord);
    if(g_block[x_cord][y_cord] == 0){
      g_block[x_cord][y_cord] = 1;
    }
    else{
      g_block[x_cord][y_cord] = 0;

    }
    
  }

  //Extract the event click and return it in WebGL coordinates
  let [x, y] = convertCoordinatesEventToGL(ev);

  var at_p = new Vector3([0, 0, 0]);
  
  at_p.set(g_at);
  at_p.sub(g_eye);
  var theta = (Math.atan2(at_p.elements[2], at_p.elements[0]));
  theta += x/10;
  at_p.elements[0] = Math.cos(theta);
  at_p.elements[1] = 0;
  at_p.elements[2] = Math.sin(theta);
  at_p.add(g_eye)
  g_at.set(at_p);
}
var g_shapeList = [];


//Extract the event click and return it in WebGL coordinates
function convertCoordinatesEventToGL(ev){
  var x = ev.clientX; // x coordinate of a mouse pointer
  var y = ev.clientY; // y coordinate of a mouse pointer
  var rect = ev.target.getBoundingClientRect();

  x = ((x - rect.left) - canvas.width/2)/(canvas.width/2);
  y = (canvas.height/2 - (y - rect.top))/(canvas.height/2);

  return([x, y]);
}

function keydown(ev){
  //forward
  if(ev.keyCode == 87){
    var d = new Vector3();
    d.set(g_at);
    d.sub(g_eye);
    g_eye.add(d.normalize());
    g_at.add(d.normalize());
  }
  //backward
  else if(ev.keyCode == 83){
    var d = new Vector3();
    d.set(g_at);
    d.sub(g_eye);
    g_eye.sub(d.normalize());
    g_at.sub(d.normalize());
  }
  //left
  else if(ev.keyCode == 65){
    var d = new Vector3();
    d.set(g_at);
    d.sub(g_eye);
    d.normalize();
    g_eye.sub(Vector3.cross(d, g_up));
    g_at.sub(Vector3.cross(d, g_up));
  }
  //right
  else if(ev.keyCode == 68){
    var d = new Vector3();
    d.set(g_at);
    d.sub(g_eye);
    d.normalize();
    g_eye.add(Vector3.cross(d, g_up));
    g_at.add(Vector3.cross(d, g_up));
  }
  //look right
  else if(ev.keyCode == 69){
    var at_p = new Vector3([0, 0, 0]);
  
    at_p.set(g_at);
    at_p.sub(g_eye);
    var theta = (Math.atan2(at_p.elements[2], at_p.elements[0]));
    theta += 0.0872664626;
    at_p.elements[0] = Math.cos(theta);
    at_p.elements[1] = 0;
    at_p.elements[2] = Math.sin(theta);
    at_p.add(g_eye)
    g_at.set(at_p);
    
  }
  //look left
  else if(ev.keyCode == 81){
    var at_p = new Vector3([0, 0, 0]);
    at_p.set(g_at);
    at_p.sub(g_eye);
    var theta = (Math.atan2(at_p.elements[2], at_p.elements[0]));
    theta -= 0.0872664626;
    at_p.elements[0] = Math.cos(theta);
    at_p.elements[1] = 0;
    at_p.elements[2] = Math.sin(theta);
    at_p.add(g_eye)
    g_at.set(at_p);
    
  }
}

var g_map = [
  [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
  [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],

];

function drawMap(){
  for(x=0; x < 16; x++){
    for(y = 0; y < 16; y++){
      if(g_map[x][y] == 1){
        var wall = new Cube();
        wall.color[1, 1, 1, 1];
        wall.textureNum = 0;
        wall.matrix.scale(3, 3, 3, 1);
        wall.matrix.translate(x-8, -0.25, y-8);
        wall.renderFast();
      }
    }
  }
}


var g_block = [
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1],

];

function drawblocks(){
  for(x=0; x < 48; x++){
    for(y = 0; y < 48; y++){
      if(g_block[x][y] == 1){
        var wall = new Cube();
        wall.color[1, 1, 1, 1];
        wall.textureNum = 1;
        wall.matrix.scale(1, 1, 1, 1);
        wall.matrix.translate(x-24, -.75, y-24);
        wall.renderFast();
      }
    }
  }
}

var g_eye = new Vector3([0, .75, 3]);
var g_at = new Vector3([0, .75, -100]);
var g_up = new Vector3([0, 1, 0]);

//Draw every shape that is supposed to be in the canvas
function renderScene(){
  
  var projMat = new Matrix4();
  projMat.setPerspective(90, canvas.width/canvas.height, 1, 100)
  gl.uniformMatrix4fv(u_ProjectionMatrix, false, projMat.elements);

  var viewMat = new Matrix4();
  viewMat.setLookAt(g_eye.elements[0],g_eye.elements[1],g_eye.elements[2], g_at.elements[0],g_at.elements[1],g_at.elements[2], g_up.elements[0],g_up.elements[1],g_up.elements[2])
  gl.uniformMatrix4fv(u_ViewMatrix, false, viewMat.elements);

  var globalRotMat = new Matrix4().rotate(-g_globalAngle, 0, 1, 0);
  gl.uniformMatrix4fv(u_GlobalRotateMatrix, false, globalRotMat.elements);
  // Clear <canvas>
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  gl.clear(gl.COLOR_BUFFER_BIT);

  gl.uniform3f(u_lightPos, g_lightPos[0], g_lightPos[1], g_lightPos[2]);
  gl.uniform3f(u_cameraPos, g_eye.elements[0], g_eye.elements[1], g_eye.elements[2]);
  gl.uniform1i(u_lightOn, g_lightOn);

  var light = new Cube();
  light.color = [2, 2, 0, 1];
  light.matrix.translate(g_lightPos[0], g_lightPos[1], g_lightPos[2]);
  light.textureNum=-2;
  light.matrix.scale(-.1, -.1, -.1);
  light.matrix.translate(-.5, -.5, -.5);
  light.render()

  //floor
  var floor = new Cube();
  floor.color = [1.0, 0.0, 0.0, 1.0];
  floor.textureNum = 0;
  if (g_normalOn) floor.textureNum = -3;
  floor.matrix.translate(0, -2.49, 0.0);
  floor.matrix.scale(10, 0.01, 10);
  floor.matrix.translate(-.5, 0, -0.5);
  floor.render();

  //block
  var block = new Cube();
  block.color = [1.0, 1.0, 1.0, 1.0];
  block.textureNum = -2;
  if (g_normalOn) block.textureNum = -3;
  block.matrix.translate(0, -2.49, 0.0);
  block.matrix.scale(-10, -10, -10);
  block.matrix.translate(-.5, -.99, -0.5);
  block.render();

  var sphere = new Sphere();
  sphere.color = [1.0, 1.0, 1.0, 1.0];
  sphere.textureNum = 1;
  if (g_normalOn) sphere.textureNum = -3;
  sphere.matrix.scale(.5,.5,.5);
  sphere.matrix.translate(1,.5,0)
  sphere.render();

  drawBlockyAnimal([-0.7,-0.5,0]);

}


function drawBlockyAnimal(move){
  //head
  var head = new Cube();
  head.color = [.95, .95, .95, 1];
  if (g_normalOn) head.textureNum = -3;
  head.matrix.translate(-.3+move[0], .15+move[1], 0.0+move[2]);
  head.matrix.scale(.6, .35, .5);
  head.render();

  //ear
  var leftEar = new Cube();
  leftEar.color = g_color;
  if (g_normalOn) leftEar.textureNum = -3;
  leftEar.matrix.translate(-.29+move[0], .45+move[1], 0.2+move[2]);
  leftEar.matrix.scale(.2, .2, .2);
  leftEar.render();
  var leftEar = new Cube();
  leftEar.color = [.05, .05, .05, 1];
  if (g_normalOn) leftEar.textureNum = -3;
  leftEar.matrix.translate(-.24+move[0], .45+move[1], 0.175+move[2]);
  leftEar.matrix.scale(.1, .15, .2);
  leftEar.render();

  var rigtEar = new Cube();
  rigtEar.color = g_color;
  if (g_normalOn) rigtEar.textureNum = -3;
  rigtEar.matrix.translate(.09+move[0], .45+move[1], 0.2+move[2]);
  rigtEar.matrix.scale(.2, .2, .2);
  rigtEar.render();
  var rigtEar = new Cube();
  if (g_normalOn) rigtEar.textureNum = -3;
  rigtEar.color = [.05, .05, .05, 1];
  rigtEar.matrix.translate(.14+move[0], .45+move[1], 0.175+move[2]);
  rigtEar.matrix.scale(.1, .15, .2);
  rigtEar.render();

  //nose
  var nose = new Cube();
  nose.color = g_color;
  if (g_normalOn) nose.textureNum = -3;
  nose.matrix.translate(-.05+move[0], .275+move[1], -0.025+move[2]);
  nose.matrix.scale(.1, .05 , .2);
  nose.render();

  var nose = new Cube();
  nose.color = g_color;
  if (g_normalOn) nose.textureNum = -3;
  nose.matrix.translate(-.04+move[0], .25+move[1], -0.025+move[2]);
  nose.matrix.scale(.075, .05 , .2);
  nose.render();

  //eye
  var eye = new Cube();
  eye.color = g_color;
  if (g_normalOn) eye.textureNum = -3;
  eye.matrix.translate(-.25+move[0], .35+move[1], -0.0005+move[2]);
  eye.matrix.scale(.2, .1 , .2);
  eye.render();

  var eye = new Cube();
  eye.color = [1, 1, 1, 1];
  if (g_normalOn) eye.textureNum = -3;
  eye.matrix.translate(.14 + g_rightEye+move[0], .375+move[1], -0.005+move[2]);
  eye.matrix.scale(.025, .025 , .2);
  eye.render();

  var eye = new Cube();
  eye.color = g_color;
  if (g_normalOn) eye.textureNum = -3;
  eye.matrix.translate(.05+move[0], .35+move[1], -0.0005+move[2]);
  eye.matrix.scale(.2, .1 , .2);
  eye.render();

  var eye = new Cube();
  eye.color = [1, 1, 1, 1];
  if (g_normalOn) eye.textureNum = -3;
  eye.matrix.translate(-.16 + g_rightEye+move[0], .375+move[1], -0.005+move[2]);
  eye.matrix.scale(.025, .025 , .2);
  eye.render();
  
  //mouth
  var mouth = new Cube();
  mouth.color = g_color;
  if (g_normalOn) mouth.textureNum = -3;
  mouth.matrix.translate(-.15+move[0], .175+move[1], -0.005+move[2]);
  mouth.matrix.scale(.3, .025 , .2);
  mouth.render();
  var mouth = new Cube();
  mouth.color = g_color;
  if (g_normalOn) mouth.textureNum = -3;
  mouth.matrix.translate(-.175+move[0], .2+move[1], -0.005+move[2]);
  mouth.matrix.scale(.35, .025 , .2);
  mouth.render();

  //belly
  var belly = new Cube();
  belly.color = [.95, .95, .95, 1];
  if (g_normalOn) belly.textureNum = -3;
  belly.matrix.translate(-.25+move[0], -.5+move[1], 0.0+move[2]);
  belly.matrix.scale(.5, .6, .5);
  belly.render();

  var belly = new Cube();
  belly.color = [1, 1, 1, 1];
  if (g_normalOn) belly.textureNum = -3;
  belly.matrix.translate(-.2+move[0], -.45+move[1], -0.05+move[2]);
  belly.matrix.scale(.4, .5, .6);
  belly.render();

  var belly = new Cube();
  belly.color = g_color;
  if (g_normalOn) belly.textureNum = -3;
  belly.matrix.translate(-.2+move[0], -.55+move[1], 0.05+move[2]);
  belly.matrix.scale(.4, .75, .4);
  belly.render();

  var belly = new Cube();
  belly.color = [1, 1, 1, 1];
  if (g_normalOn) belly.textureNum = -3;
  belly.matrix.translate(-.3+move[0], -.45+move[1], 0.05+move[2]);
  belly.matrix.scale(.6, .5, .4);
  belly.render();

  //arm  
  var leftTopArm = new Cube();
  leftTopArm.color = g_color;
  if (g_normalOn) leftTopArm.textureNum = -3;
  leftTopArm.matrix.translate(0.2+move[0], 0.2+move[1], 0.025+move[2]);
  leftTopArm.matrix.rotate(g_armAngle, 0, 0, 1);
  var leftTopArmCoordinatesMat = new Matrix4(leftTopArm.matrix);
  leftTopArm.matrix.scale(.2, .45, .45);
  leftTopArm.normalMatrix.setInverseOf(leftTopArm.matrix).transpose();
  leftTopArm.render();
  
  var leftBottomArm = new Cube();
  leftBottomArm.color = g_color;
  if (g_normalOn) leftBottomArm.textureNum = -3;
  leftBottomArm.matrix = leftTopArmCoordinatesMat;
  leftBottomArm.matrix.translate(.0, 0.42, 0.0125);
  leftBottomArm.matrix.rotate(g_bottomArmAngle, 0, 0, 1);
  leftBottomArm.matrix.scale(.2, .3, .425);
  leftBottomArm.normalMatrix.setInverseOf(leftBottomArm.matrix).transpose();
  leftBottomArm.render();

  var rightTopArm = new Cube();
  rightTopArm.color = g_color;
  if (g_normalOn) rightTopArm.textureNum = -3;
  rightTopArm.matrix.translate(-0.2+move[0], 0.2+move[1], 0.025+move[2]);
  rightTopArm.matrix.rotate(-g_armAngle, 0, 0, 1);
  var rightTopArmCoordinatesMat = new Matrix4(rightTopArm.matrix);
  rightTopArm.matrix.scale(-.2, .45, .45);
  rightTopArm.normalMatrix.setInverseOf(rightTopArm.matrix).transpose();
  rightTopArm.render();

  var rightBottomArm = new Cube();
  rightBottomArm.color = g_color;
  if (g_normalOn) rightBottomArm.textureNum = -3;
  rightBottomArm.matrix = rightTopArmCoordinatesMat;
  rightBottomArm.matrix.translate(.0, 0.42, 0.0125);
  rightBottomArm.matrix.rotate(-g_bottomArmAngle, 0, 0, 1);
  rightBottomArm.matrix.scale(-.2, .3, .425);
  rightBottomArm.normalMatrix.setInverseOf(rightBottomArm.matrix).transpose();
  rightBottomArm.render();

  //legs
  var rightLeg = new Cube();
  rightLeg.color = g_color;
  if (g_normalOn) rightLeg.textureNum = -3;
  rightLeg.matrix.translate(-0.2+move[0], -0.9+move[1], move[2]);
  rightLeg.matrix.rotate(g_rightLegs, 1, 0, 0);
  rightLeg.matrix.scale(.175, .4, .3);
  rightLeg.render();

  var leftLeg = new Cube();
  leftLeg.color = g_color;
  if (g_normalOn) leftLeg.textureNum = -3;
  leftLeg.matrix.translate(0.025+move[0], -0.9+move[1], move[2]);
  leftLeg.matrix.rotate(g_legs, 1, 0, 0);
  leftLeg.matrix.scale(.175, .4, .3);
  leftLeg.render();
}

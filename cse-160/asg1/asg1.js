// ColoredPoint.js (c) 2012 matsuda
// Vertex shader program
var VSHADER_SOURCE =
  'attribute vec4 a_Position;\n' +
  'uniform float u_Size;\n' +
  'void main() {\n' +
  '  gl_Position = a_Position;\n' +
  '  gl_PointSize = u_Size;\n' +
  '}\n';

// Fragment shader program
var FSHADER_SOURCE =
  'precision mediump float;\n' +
  'uniform vec4 u_FragColor;\n' +  // uniform変数
  'void main() {\n' +
  '  gl_FragColor = u_FragColor;\n' +
  '}\n';

//Global variables
let canvas;
let gl;
let a_Position;
let u_FragColor;
let u_Size;

function setupWebGL(){
  // Retrieve <canvas> element
  canvas = document.getElementById('webgl');

  // Get the rendering context for WebGL
  gl = canvas.getContext("webgl", {preserveDrawingBuffer: true});
  if (!gl) {
    console.log('Failed to get the rendering context for WebGL');
    return;
  }
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

  // Get the storage location of u_FragColor
  u_FragColor = gl.getUniformLocation(gl.program, 'u_FragColor');
  if (!u_FragColor) {
    console.log('Failed to get the storage location of u_FragColor');
    return;
  }

  // Get the storage location of u_Size
  u_Size = gl.getUniformLocation(gl.program, 'u_Size');
  if (!u_Size) {
    console.log('Failed to get the storage location of u_Size');
    return;
  }
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
function addActionsforHtmlUI(){
  //Button Events
  document.getElementById("clearButton").onclick = function() {g_shapeList = []; renderAllShapes();};
  document.getElementById("drawButton").onclick = function() {g_shapeList = []; drawPicture()};
  document.getElementById("pointButton").onclick = function() {g_selectedType = POINT;};
  document.getElementById("triButton").onclick = function() {g_selectedType = TRIANGLE;};
  document.getElementById("circleButton").onclick = function() {g_selectedType = CIRCLE;};

  //color slider events
  document.getElementById("redSlider").addEventListener("mouseup", function(){ g_selectedColor[0] = this.value/100; });
  document.getElementById("greenSlider").addEventListener("mouseup", function(){ g_selectedColor[1] = this.value/100; });
  document.getElementById("blueSlider").addEventListener("mouseup", function(){ g_selectedColor[2] = this.value/100; });
  
  //size slider events
  document.getElementById("sizeSlider").addEventListener("mouseup", function(){ g_selectedSize = this.value; });
  document.getElementById("segmentSlider").addEventListener("mouseup", function(){ g_selectedSegements = this.value; });

  
}

function main() {
  
  //sets up canvas and GL variables
  setupWebGL();

  //Set up GLSL shader programs and connect GLSL variables
  connectVariablesToGLSL();

  addActionsforHtmlUI();

  // Register function (event handler) to be called on a mouse press
  canvas.onmousedown = click;
  canvas.onmousemove = function(ev) { if(ev.buttons == 1){click(ev) } };
  // Specify the color for clearing <canvas>
  gl.clearColor(0.0, 0.0, 0.0, 1.0);

  // Clear <canvas>
  gl.clear(gl.COLOR_BUFFER_BIT);
}

var g_shapeList = [];


function click(ev) {
  
  //Extract the event click and return it in WebGL coordinates
  let [x, y] = convertCoordinatesEventToGL(ev);

  let point;
  if(g_selectedType == POINT){
    point = new Point();
  }
  else if (g_selectedType == TRIANGLE){
    point = new Triangle();
  }
  else{
    point = new Circle();
    point.segments = g_selectedSegements;
  }

  point.position = [x, y];
  point.color = g_selectedColor.slice();
  point.size = g_selectedSize;
  g_shapeList.push(point);
  renderAllShapes();
}

//Extract the event click and return it in WebGL coordinates
function convertCoordinatesEventToGL(ev){
  var x = ev.clientX; // x coordinate of a mouse pointer
  var y = ev.clientY; // y coordinate of a mouse pointer
  var rect = ev.target.getBoundingClientRect();

  x = ((x - rect.left) - canvas.width/2)/(canvas.width/2);
  y = (canvas.height/2 - (y - rect.top))/(canvas.height/2);

  return([x, y]);
}

//Draw every shape that is supposed to be in the canvas
function renderAllShapes(){
  // Clear <canvas>
  gl.clear(gl.COLOR_BUFFER_BIT);

  var len = g_shapeList.length;
  for(var i = 0; i < len; i++) {
    g_shapeList[i].render();
  }
}

function drawPicture(){
  g_shapeList = []; 
  renderAllShapes();
  rgba = [0, 0, 0, 1];
  gl.uniform4f(u_FragColor, g_selectedColor[0], g_selectedColor[1], g_selectedColor[2], g_selectedColor[3]);
  //Draws head
  drawTrianlge([0.0, 0.3, 0.4, 0.75, -0.4, 0.75]);
  
  //Draws neck
  drawTrianlge([0.0, 0.3, 0.1, 0.3, 0, 0.6]);
  drawTrianlge([0.0, 0.3, -0.1, 0.3, 0, 0.6]);

  //draws ears
  drawTrianlge([0.3, .9, 0.2, 0.75, 0.4, 0.75]);
  drawTrianlge([-0.3, .9, -0.2, 0.75, -0.4, 0.75]);
  //Draws torso
  drawTrianlge([0, -.5, 0.45, 0.3, -0.45, 0.3]);
  //Draws arms
  drawTrianlge([0.65, -.2, 0.45, 0.3, 0.2, 0.2]);
  drawTrianlge([-0.65, -.2, -0.45, 0.3, -0.2, 0.2]);
  //draws hands
  drawTrianlge([0.625, -.15, 0.7, -.25, 0.6, -.25]);
  drawTrianlge([-0.625, -.15, -0.7, -.25, -0.6, -.25]);


  //Draws legs
  drawTrianlge([0, -.5, 0.4, -1, 0.1, 0.2]);
  drawTrianlge([0, -.5, -0.4, -1, -0.1, 0.2]);

  //Draws feet
  drawTrianlge([-0.35, -1, -0.55, -1, -0.35, -0.9]);
  drawTrianlge([0.35, -1, 0.55, -1, 0.35, -0.9]);

  rgba = [0, 0, 0, 1];
  gl.uniform4f(u_FragColor, rgba[0], rgba[1], rgba[2], rgba[3]);
  
  //Draws ears
  drawTrianlge([0.3, .88, 0.22, 0.75, 0.38, 0.75]);
  drawTrianlge([-0.3, .88, -0.22, 0.75, -0.38, 0.75]);

  //Draws torse
  drawTrianlge([0, -.30, 0.25, 0.2, -0.25, 0.2]);

  //Draws mouth
  drawTrianlge([0.0, 0.4, 0.1, 0.5, -0.1, 0.5]);
  
  //draws nose
  drawTrianlge([0.0, 0.55, 0, 0.6, -0.025, 0.575]);

  //Draws eyes
  drawTrianlge([0.175, 0.625, 0.25, 0.675, 0.1, 0.675]);
  drawTrianlge([-0.175, 0.625, -0.25, 0.675, -0.1, 0.675]);
}
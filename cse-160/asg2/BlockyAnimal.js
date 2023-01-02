// ColoredPoint.js (c) 2012 matsuda
// Vertex shader program
var VSHADER_SOURCE =
  'attribute vec4 a_Position;\n' +
  'uniform mat4 u_ModelMatrix;\n' +
  'uniform mat4 u_GlobalRotateMatrix;\n' +
  'void main() {\n' +
  '  gl_Position = u_GlobalRotateMatrix * u_ModelMatrix * a_Position;\n' +
  '}\n';

// Fragment shader program
var FSHADER_SOURCE =
  'precision mediump float;\n' +
  'uniform vec4 u_FragColor;\n' +  // uniform
  'void main() {\n' +
  '  gl_FragColor = u_FragColor;\n' +
  '}\n';

//Global variables
let canvas;
let gl;
let a_Position;
let u_FragColor;
let u_Size;
let u_ModelMatrix;

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

  // var identityM = new Matrix4();
  // gl.uniformMatrix4fv(u_ModelMatrix, false, identityM.elements);
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
let g_globalAngle = 30;
let g_armAngle = 225;
let g_bottomArmAngle = -45;
let g_animation = false;
let g_specialAnimation = false;
let g_rightEye = 0;
let g_leftEye = 0;
let g_legs = 0;
let g_rightLegs = 0;
let g_color = [.15, .15, .15, 1];
function addActionsforHtmlUI(){
  
  document.getElementById("animationOff").onclick = function() {g_animation = false;};
  document.getElementById("animationOn").onclick = function() {g_animation = true;};
  document.getElementById("animationOn").onclick = function() {g_animation = true;};



  document.getElementById("armSlider").addEventListener("mousemove", function(){ g_armAngle = this.value; renderScene(); });
  document.getElementById("bottomArmSlider").addEventListener("mousemove", function(){ g_bottomArmAngle = this.value; renderScene(); });
  
  document.getElementById("angleSlider").addEventListener("mousemove", function(){ g_globalAngle = this.value; renderScene();});

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
  renderScene();
  requestAnimationFrame(tick);
}

var g_startTime = performance.now()/1000.0;
var g_seconds = performance.now()/1000.0-g_startTime;


function tick(){
  g_seconds = performance.now()/1000.0-g_startTime;

  updateAnimateAngles();

  renderScene();

  requestAnimationFrame(tick);
}

function updateAnimateAngles(){
  if(g_animation){
    g_armAngle = 245 + (20*Math.sin(g_seconds));
    g_bottomArmAngle = -45 + (30*Math.sin(g_seconds));
    g_legs = 5+(15 * Math.sin(2*g_seconds));
    g_rightLegs = 5-(15 * (Math.sin(2*g_seconds)));
  }
  if(g_specialAnimation){
    g_armAngle = 245 + (20*Math.sin(4*g_seconds));
    g_bottomArmAngle = -45 + (30*Math.sin(4*g_seconds));
    g_rightEye = (Math.sin(4*g_seconds))/20;
    g_legs = 5+(15 * Math.sin(6*g_seconds));
    g_rightLegs = 5-(15 * (Math.sin(6*g_seconds)));
    g_color = [Math.random(), Math.random(), Math.random(), 1];
  }
}

function click(ev) {
  if(ev.shiftKey){
    if(g_specialAnimation){
      g_specialAnimation = false;
      g_color = [.15, .15, .15, 1];
    }
    else{
      g_specialAnimation = true;
    }
  }

  //Extract the event click and return it in WebGL coordinates
  let [x, y] = convertCoordinatesEventToGL(ev);

  g_globalAngle = -180*x;
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

//Draw every shape that is supposed to be in the canvas
function renderScene(){
  var globalRotMat = new Matrix4().rotate(g_globalAngle, 0, 1, 0);
  gl.uniformMatrix4fv(u_GlobalRotateMatrix, false, globalRotMat.elements);
  // Clear <canvas>
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  gl.clear(gl.COLOR_BUFFER_BIT);



  //head
  var head = new Cube();
  head.color = [.95, .95, .95, 1];
  head.matrix.translate(-.3, .15, 0.0);
  head.matrix.scale(.6, .35, .5);
  head.render();

  //ear
  var leftEar = new Cube();
  leftEar.color = g_color;
  leftEar.matrix.translate(-.29, .45, 0.2);
  leftEar.matrix.scale(.2, .2, .2);
  leftEar.render();
  var leftEar = new Cube();
  leftEar.color = [.05, .05, .05, 1];
  leftEar.matrix.translate(-.24, .45, 0.175);
  leftEar.matrix.scale(.1, .15, .2);
  leftEar.render();

  var rigtEar = new Cube();
  rigtEar.color = g_color;
  rigtEar.matrix.translate(.09, .45, 0.2);
  rigtEar.matrix.scale(.2, .2, .2);
  rigtEar.render();
  var rigtEar = new Cube();
  rigtEar.color = [.05, .05, .05, 1];
  rigtEar.matrix.translate(.14, .45, 0.175);
  rigtEar.matrix.scale(.1, .15, .2);
  rigtEar.render();

  //nose
  var nose = new Cube();
  nose.color = g_color;
  nose.matrix.translate(-.05, .275, -0.025);
  nose.matrix.scale(.1, .05 , .2);
  nose.render();

  var nose = new Cube();
  nose.color = g_color;
  nose.matrix.translate(-.04, .25, -0.025);
  nose.matrix.scale(.075, .05 , .2);
  nose.render();

  //eye
  var eye = new Cube();
  eye.color = g_color;
  eye.matrix.translate(-.25, .35, -0.0005);
  eye.matrix.scale(.2, .1 , .2);
  eye.render();

  var eye = new Cube();
  eye.color = [1, 1, 1, 1];
  eye.matrix.translate(.14 + g_rightEye, .375, -0.005);
  eye.matrix.scale(.025, .025 , .2);
  eye.render();

  var eye = new Cube();
  eye.color = g_color;
  eye.matrix.translate(.05, .35, -0.0005);
  eye.matrix.scale(.2, .1 , .2);
  eye.render();

  var eye = new Cube();
  eye.color = [1, 1, 1, 1];
  eye.matrix.translate(-.16 + g_rightEye, .375, -0.005);
  eye.matrix.scale(.025, .025 , .2);
  eye.render();
  
  //mouth
  var mouth = new Cube();
  mouth.color = g_color;
  mouth.matrix.translate(-.15, .175, -0.005);
  mouth.matrix.scale(.3, .025 , .2);
  mouth.render();
  var mouth = new Cube();
  mouth.color = g_color;
  mouth.matrix.translate(-.175, .2, -0.005);
  mouth.matrix.scale(.35, .025 , .2);
  mouth.render();

  //belly
  var belly = new Cube();
  belly.color = [.95, .95, .95, 1];
  belly.matrix.translate(-.25, -.5, 0.0);
  belly.matrix.scale(.5, .6, .5);
  belly.render();

  var belly = new Cube();
  belly.color = [1, 1, 1, 1];
  belly.matrix.translate(-.2, -.45, -0.05);
  belly.matrix.scale(.4, .5, .6);
  belly.render();

  var belly = new Cube();
  belly.color = g_color;
  belly.matrix.translate(-.2, -.55, 0.05);
  belly.matrix.scale(.4, .75, .4);
  belly.render();

  var belly = new Cube();
  belly.color = [1, 1, 1, 1];
  belly.matrix.translate(-.3, -.45, 0.05);
  belly.matrix.scale(.6, .5, .4);
  belly.render();

  //arm  
  var leftTopArm = new Cube();
  leftTopArm.color = g_color;
  leftTopArm.matrix.translate(0.2, 0.2, 0.025);
  leftTopArm.matrix.rotate(g_armAngle, 0, 0, 1);
  var leftTopArmCoordinatesMat = new Matrix4(leftTopArm.matrix);
  leftTopArm.matrix.scale(.2, .45, .45);
  leftTopArm.render();
  
  var leftBottomArm = new Cube();
  leftBottomArm.color = g_color;
  leftBottomArm.matrix = leftTopArmCoordinatesMat;
  leftBottomArm.matrix.translate(.0, 0.42, 0.0125);
  leftBottomArm.matrix.rotate(g_bottomArmAngle, 0, 0, 1);
  leftBottomArm.matrix.scale(.2, .3, .425);
  leftBottomArm.render();

  var rightTopArm = new Cube();
  rightTopArm.color = g_color;
  rightTopArm.matrix.translate(-0.2, 0.2, 0.025);
  rightTopArm.matrix.rotate(-g_armAngle, 0, 0, 1);
  var rightTopArmCoordinatesMat = new Matrix4(rightTopArm.matrix);
  rightTopArm.matrix.scale(-.2, .45, .45);
  rightTopArm.render();

  var rightBottomArm = new Cube();
  rightBottomArm.color = g_color;
  rightBottomArm.matrix = rightTopArmCoordinatesMat;
  rightBottomArm.matrix.translate(.0, 0.42, 0.0125);
  rightBottomArm.matrix.rotate(-g_bottomArmAngle, 0, 0, 1);
  rightBottomArm.matrix.scale(-.2, .3, .425);
  rightBottomArm.render();

  //legs
  var rightLeg = new Cube();
  rightLeg.color = g_color;
  rightLeg.matrix.translate(-0.2, -0.5, 0.4);
  rightLeg.matrix.rotate(g_rightLegs, 1, 0, 0);
  rightLeg.matrix.scale(.175, -.4, -.3);
  rightLeg.render();

  var leftLeg = new Cube();
  leftLeg.color = g_color;
  leftLeg.matrix.translate(0.025, -0.5, 0.4);
  leftLeg.matrix.rotate(g_legs, 1, 0, 0);
  leftLeg.matrix.scale(.175, -.4, -.3);
  leftLeg.render();

}


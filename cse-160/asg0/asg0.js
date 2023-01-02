// DrawRectangle.js
function main() {
// Retrieve <canvas> element <- (1)
  var canvas = document.getElementById('example');
  if (!canvas) {
    console.log('Failed to retrieve the <canvas> element');
    return;
  }
  
  // Get the rendering context for 2DCG <- (2)
  var ctx = canvas.getContext('2d');
 
  // Draw a blue rectangle <- (3)
  ctx.fillStyle = 'rgba(0, 0, 0, 1.0)'; // Set a blue color
  ctx.fillRect(0, 0, 400, 400); // Fill a rectangle with the color

  
}

function drawVector(v, color){
  var canvas = document.getElementById('example');
  var ctx = canvas.getContext('2d');
  ctx.strokeStyle = color;
  ctx.beginPath();
  ctx.moveTo(200,200);
  ctx.lineTo(200+(v.elements[0] * 20), 200-(v.elements[1] * 20));
  ctx.stroke();
}

function handleDrawEvent(){
  var canvas = document.getElementById('example');
  var ctx = canvas.getContext('2d');
  ctx.fillStyle = 'rgba(0, 0, 0, 1.0)';
  ctx.fillRect(0, 0, 400, 400); 
  var v1 = new Vector3([document.getElementById("v1.x").value, document.getElementById("v1.y").value, 0]);
  drawVector(v1, "red");
  var v2 = new Vector3([document.getElementById("v2.x").value, document.getElementById("v2.y").value, 0]);
  drawVector(v2, "blue");
}

function handleDrawOperationEvent(){
  var canvas = document.getElementById('example');
  var ctx = canvas.getContext('2d');
  ctx.fillStyle = 'rgba(0, 0, 0, 1.0)';
  ctx.fillRect(0, 0, 400, 400); 
  var v1 = new Vector3([document.getElementById("v1.x").value, document.getElementById("v1.y").value, 0]);
  drawVector(v1, "red");
  var v2 = new Vector3([document.getElementById("v2.x").value, document.getElementById("v2.y").value, 0]);
  drawVector(v2, "blue");
  if(document.getElementById("operation").value == "add"){
    var v3 = v1.add(v2);
    drawVector(v3, "green");
  }
  else if(document.getElementById("operation").value == "sub"){
    var v3 = v1.sub(v2);
    drawVector(v3, "green");
  }
  else if(document.getElementById("operation").value == "mul"){
    var v3 = v1.mul(document.getElementById("scalar").value);
    drawVector(v3, "green");
    var v4 = v2.mul(document.getElementById("scalar").value);
    drawVector(v4, "green");
  }
  else if(document.getElementById("operation").value == "div"){
    var v3 = v1.div(document.getElementById("scalar").value);
    drawVector(v3, "green");
    var v4 = v2.div(document.getElementById("scalar").value);
    drawVector(v4, "green");
  }
  else if(document.getElementById("operation").value == "magnitude"){
    console.log("Magnitude v1:",v1.magnitude());
    console.log("Magnitude v2:", v2.magnitude());
  }
  else if(document.getElementById("operation").value == "normalize"){
    drawVector(v1.normalize(), "green");
    drawVector(v2.normalize(), "green");
  }
  else if(document.getElementById("operation").value == "angle"){
    console.log("Angle:", angleBetween(v1, v2));
  }
  else if(document.getElementById("operation").value == "area"){
    console.log("Area of the triangle:", areaTriangle(v1, v2));
  }
}

function angleBetween(v1, v2){
  d = Vector3.dot(v1, v2);
  d = d / (v1.magnitude()*v2.magnitude());
  d = Math.acos(d);
  d = d * (180/ Math.PI);
  return d;
}

function areaTriangle(v1, v2){
  var v3 = Vector3.cross(v1,v2);
  return (v3.magnitude())/2;
}
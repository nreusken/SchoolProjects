
import * as THREE from 'https://threejsfundamentals.org/threejs/resources/threejs/r122/build/three.module.js';
import {OBJLoader} from 'https://threejsfundamentals.org/threejs/resources/threejs/r122/examples/jsm/loaders/OBJLoader.js'
import {MTLLoader} from 'https://threejsfundamentals.org/threejs/resources/threejs/r122/examples/jsm/loaders/MTLLoader.js'
import {GUI} from 'https://threejs.org/examples/jsm/libs/lil-gui.module.min.js';
import {OrbitControls} from 'https://threejsfundamentals.org/threejs/resources/threejs/r122/examples/jsm/controls/OrbitControls.js';
import {RectAreaLightUniformsLib} from 'https://threejsfundamentals.org/threejs/resources/threejs/r122//examples/jsm/lights/RectAreaLightUniformsLib.js';
import {RectAreaLightHelper} from 'https://threejsfundamentals.org/threejs/resources/threejs/r122//examples/jsm/helpers/RectAreaLightHelper.js';

class ColorGUIHelper {
  constructor(object, prop) {
    this.object = object;
    this.prop = prop;
  }
  get value() {
    return `#${this.object[this.prop].getHexString()}`;
  }
  set value(hexString) {
    this.object[this.prop].set(hexString);
  }
}


function main() {
  const canvas = document.querySelector('#c');
  const renderer = new THREE.WebGLRenderer({
    canvas,
    logarithmicDepthBuffer: true,
  });
  RectAreaLightUniformsLib.init();

  const fov = 75;
  const aspect = 1.25;  // the canvas default
  const near = 0.00001;
  const far = 100;
  const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
  camera.position.z = 2;
  camera.position.y = 1;

  const scene = new THREE.Scene();
  scene.background = new THREE.Color('white');

  //fog
  {
    const near = 2;
    const far = 40;
    const color = 'white';
    scene.fog = new THREE.Fog(color, near, far);
    scene.background = new THREE.Color(color);
  }
 
  //hemisphere light
  const skyColor = 0xB1E1FF;  // light blue
  const groundColor = 0xB97A20;  // brownish orange
  const hintensity = 0.33;
  const hlight = new THREE.HemisphereLight(skyColor, groundColor, hintensity);
  hlight.position.set(-1, 2, 4);
  scene.add(hlight);
  const color = 0xFFFFFF;
  const intensity = 1;
  const slight = new THREE.SpotLight(color, intensity);
  slight.penumbra = .5;
  slight.position.set(-3, 3, 7);
  slight.target.position.set(-1, 0, 9);
  scene.add(slight);
  scene.add(slight.target);
 
  //rectangle light 
  const width = 2;
  const height = 32;
  const rectlight = new THREE.RectAreaLight(color, 5, width, height);
  rectlight.position.set(-15, 10, 0);
  rectlight.rotation.x = THREE.MathUtils.degToRad(-90);
  scene.add(rectlight);
  const helper = new RectAreaLightHelper(rectlight);
  rectlight.add(helper);

  const loader = new THREE.TextureLoader();
  const texture = loader.load(
    'background.jpg',
    () => {
      const rt = new THREE.WebGLCubeRenderTarget(texture.image.height);
      rt.fromEquirectangularTexture(renderer, texture);
      scene.background = rt.texture;
    });
  // const loader = new THREE.TextureLoader();
  //cubes and renders the walls and floor
  const boxWidth = 1;
  const boxHeight = 1;
  const boxDepth = 1;
  const geometry = new THREE.BoxGeometry(boxWidth, boxHeight, boxDepth);
  const cubes = [];
  loader.load('brick.jpg', (texture) => {
    const material = new THREE.MeshStandardMaterial({
      map: texture,
    });
    for(let x=-16; x < 16; x++){
      for(let z = -16; z < 16; z++){
        if(x==5){
          for(let y = 0; y < 5; y++){
            const cube = new THREE.Mesh(geometry, material);
            scene.add(cube);
            cube.position.y = y;
            cube.position.x = x;
            cube.position.z = z;
          }
        }
        const cube = new THREE.Mesh(geometry, material);
        scene.add(cube);
        cube.position.y = -1;
        cube.position.x = x;
        cube.position.z = z;
        
      }
      
    }
  });


  const shadowTexture = loader.load('https://threejsfundamentals.org/threejs/resources/images/roundshadow.png');
  const sphereShadowBases = [];
  //spheres
  const radius = .5;  // ui: radius
  const widthSegments = 8;  // ui: widthSegments
  const heightSegments = 6;  // ui: heightSegments
  const sphereGeometry = new THREE.SphereGeometry(radius, widthSegments, heightSegments);
  const numSpheres = 10;

  const planeSize = 1;
  const shadowGeo = new THREE.PlaneGeometry(planeSize, planeSize);

  for (let i = -numSpheres; i < numSpheres; ++i) {
    // make a base for the shadow and the sphere
    // so they move together.
    const base = new THREE.Object3D();
    scene.add(base);
   
  
    const shadowMat = new THREE.MeshBasicMaterial({
      map: shadowTexture,
      transparent: true,    // so we can see the ground
      depthWrite: false,    // so we don't have to sort
    });
    const shadowMesh = new THREE.Mesh(shadowGeo, shadowMat);
    shadowMesh.position.z = i*1.5;
    shadowMesh.position.x = -15;
    shadowMesh.position.y = -0.499;
    shadowMesh.rotation.x = Math.PI * -.5;
    const shadowSize = radius * 4;
    shadowMesh.scale.set(shadowSize, shadowSize, shadowSize);
    base.add(shadowMesh);
   
    // add the sphere to the base
    const u = i / numSpheres;   // goes from 0 to 1 as we iterate the spheres.
    const sphereMat = new THREE.MeshStandardMaterial();
    sphereMat.color.setHSL(u, 1, .75);
    const sphereMesh = new THREE.Mesh(sphereGeometry, sphereMat);
    sphereMesh.position.z = i*1.5;
    sphereMesh.position.x = -15;
    sphereMesh.position.y = radius+1.5;
    base.add(sphereMesh);
   
    // remember all 3 plus the y position
    sphereShadowBases.push({base, sphereMesh, shadowMesh, y: sphereMesh.position.y});
  }
    


  const coneradius = 3;  // ui: radius
  const coneheight = 6;  // ui: height
  const radialSegments = 16;  // ui: radialSegments
  const conegeometry = new THREE.ConeGeometry(coneradius, coneheight, radialSegments);
  const conematerial = new THREE.MeshPhysicalMaterial({color: '#CA8'});
  const cone = new THREE.Mesh(conegeometry, conematerial);
  scene.add(cone);
  cone.position.x = -12;
  cone.position.z = 12;
  cone.position.y = .7;
    

  const mtlLoader = new MTLLoader();
  const objLoader = new OBJLoader();
  mtlLoader.load('BarTable/materials.mtl', (mtl) => {
    mtl.preload();
    for(let x=-0; x > -4; x--){
      for(let z = 0; z > -4; z--){
        objLoader.load('BarTable/model.obj', (root) => {
          scene.add(root);
          root.position.x = x*3;
          root.position.z = z*4;
        });
      }
    }
  });

  mtlLoader.load('Tavern/materials.mtl', (mtl) => {
    mtl.preload();
    objLoader.setMaterials(mtl);
    objLoader.load('Tavern/model.obj', (root) => {
      scene.add(root);
      root.rotation.y = 3.14159;
      root.position.x = 0;
      root.position.z = 10;
      root.position.y = .7;
    });
    
    
  });

  mtlLoader.load('Bar/materials.mtl', (mtl) => {
    mtl.preload();
    objLoader.setMaterials(mtl);
    
    for(let z = 0; z > -4; z--){
      objLoader.load('Bar/model.obj', (root) => {
        scene.add(root);
        root.position.x = 3;
        root.position.z = z* 4;
        root.position.y = .7;
      });
    }
    
  });

  function updateCamera() {
    camera.updateProjectionMatrix();
  }
   
  const gui = new GUI();
  gui.addColor(new ColorGUIHelper(hlight, 'color'), 'value').name('skyColor');
  gui.addColor(new ColorGUIHelper(hlight, 'groundColor'), 'value').name('groundColor');
  gui.add(hlight, 'intensity', 0, 2, 0.01);
 
  const controls = new OrbitControls(camera, canvas);
 
  function render(time) {
    time *= 0.001;  // convert time to seconds

    sphereShadowBases.forEach((sphereShadowBase, ndx) => {
      const {base, sphereMesh, shadowMesh, y} = sphereShadowBase;
   
      // u is a value that goes from 0 to 1 as we iterate the spheres
      const u = ndx / sphereShadowBases.length;
   
    
   
      // yOff is a value that goes from 0 to 1
      const yOff = Math.abs(Math.sin(time * 2 + ndx));
      // move the sphere up and down
      sphereMesh.position.y = y + THREE.MathUtils.lerp(-2, 2, yOff);
      // fade the shadow as the sphere goes up
      shadowMesh.material.opacity = THREE.MathUtils.lerp(1, .25, yOff);
    });

    renderer.render(scene, camera);

    requestAnimationFrame(render);
  }
  requestAnimationFrame(render);

}

main();


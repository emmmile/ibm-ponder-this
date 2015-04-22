var cube = "WWWWWWWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWWWWWWWWWWWWWWWPPPPPWWPPPPPWWWPPPWWWPPPPPWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWPPPPPWPPPWPPPWPPPPPWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWWWWWWWWWWWWWWWPPPPPWWPPPPPWWPWPWPWWPPPPPWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWWWWWWW";
var container = document.getElementById("cube-container");
var ratio = container.clientWidth / 7;

// check WebGL availability
function webglAvailable() {
  try {
    var canvas = document.createElement( 'canvas' );
    return !!( window.WebGLRenderingContext && (
      canvas.getContext( 'webgl' ) ||
      canvas.getContext( 'experimental-webgl' ) )
    );
  } catch ( e ) {
    return false;
  }
}

function init() {
  /*if ( webglAvailable() ) {
    renderer = new THREE.WebGLRenderer({antialias:true});
  } else {
    renderer = new THREE.CanvasRenderer();
  }*/

  renderer = new THREE.CanvasRenderer({ alpha: true } );
  renderer.setClearColor(0xffffff, 0.0);


  renderer.setPixelRatio( window.devicePixelRatio );
  scene = new THREE.Scene();
  var w = container.clientWidth; //window.innerWidth;
  var h = container.clientHeight; //window.innerHeight;
  camera = new THREE.OrthographicCamera( w / -ratio, w / ratio, h / ratio, h / -ratio, 1, 1000 );
  camera.position.z = 10;

  renderer.setSize( w, h );
  container.appendChild( renderer.domElement );

  stats = new Stats();
  stats.domElement.style.position = 'absolute';
  stats.domElement.style.top = '0px';
  container.appendChild( stats.domElement );


  // add the particles
  group = new THREE.Group();
  for ( el = 0; el < cube.length; ++el ) {
    addPoint(cube[el], el);         
  }
  scene.add( group );

  container.addEventListener('mousemove', onMouseMove, false);
  container.addEventListener('mousedown', onMouseDown, false);
  container.addEventListener('mouseup', onMouseUp, false);
  window.addEventListener('resize', onWindowResize, false );

  console.log(w, h);
}

function addPoint(character, index) {
  var material = new THREE.SpriteCanvasMaterial({
    color: ( character == "W" ? 0xff0000 : 0x00ff00 ),
    program: function ( context ) {
      context.beginPath();
      context.arc( 0, 0, 0.5, 0, Math.PI * 2, true );
      context.fill();
    }
  });

  particle = new THREE.Sprite( material );
  particle.position.x = Math.floor((index % 49) % 7) - 3;
  particle.position.y = Math.floor(index / 49) - 3;
  particle.position.z = Math.floor((index % 49) / 7) - 3;
  particle.scale.x = particle.scale.y = 0.1;
  group.add( particle );
};

var render = function () {
  requestAnimationFrame( render );

  group.rotation.x += 0.001;
  group.rotation.y += 0.001;

  renderer.render(scene, camera);
  stats.update();
};


// mouse stuff
var mouseDown = false,
  mouseX = 0,
  mouseY = 0;

function onMouseMove(event) {
    if (!mouseDown) {
        return;
    }

    event.preventDefault();

    var deltaX = event.clientX - mouseX,
        deltaY = event.clientY - mouseY;
    mouseX = event.clientX;
    mouseY = event.clientY;
    rotateScene(deltaX, deltaY);
}

function onMouseDown(event) {
    event.preventDefault();

    mouseDown = true;
    mouseX = event.clientX;
    mouseY = event.clientY;
}

function onMouseUp(event) {
    event.preventDefault();

    mouseDown = false;
}

function onWindowResize() {
  camera.lookAt(new THREE.Vector3(0,0,0));
  ratio = container.clientWidth / 7;


  var w = container.clientWidth; //window.innerWidth;
  var h = container.clientHeight; //window.innerHeight;

  camera.left = w / -ratio;
  camera.right = w / ratio;
  camera.top = h / ratio;
  camera.bottom = h / -ratio;
  camera.updateProjectionMatrix();

  renderer.setSize( w, h );
  console.log(w, h);
}

function rotateScene(deltaX, deltaY) {
  group.rotation.y += deltaX / 100;
  group.rotation.x += deltaY / 100;
}

init();
render();
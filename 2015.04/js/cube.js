var cube = "WWWWWWWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWWWWWWWWWWWWWWWPPPPPWWPPPPPWWWPPPWWWPPPPPWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWPPPPPWPPPWPPPWPPPPPWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWPPPPPWWWWWWWWWWWWWWWWPPPPPWWPPPPPWWPWPWPWWPPPPPWWPPPPPWWPPPPPWWWWWWWWWPPPPPWWWWWWWWWPPPPPWWPPPPPWWWWWWWWWWWWWWW";
var ratio = 100;

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

  renderer = new THREE.CanvasRenderer();


  renderer.setPixelRatio( window.devicePixelRatio );
  scene = new THREE.Scene();
  var w = window.innerWidth;
  var h = window.innerHeight;
  camera = new THREE.OrthographicCamera( w / -ratio, w / ratio, h / ratio, h / -ratio, 1, 1000 );
  camera.position.z = 10;

  renderer.setSize( w, h );
  document.body.appendChild( renderer.domElement );

  stats = new Stats();
  stats.domElement.style.position = 'absolute';
  stats.domElement.style.top = '0px';
  document.body.appendChild( stats.domElement );


  // add the particles
  group = new THREE.Group();
  for ( el = 0; el < cube.length; ++el ) {
    addPoint(cube[el], el);         
  }
  scene.add( group );

  document.addEventListener('mousemove', onMouseMove, false);
  document.addEventListener('mousedown', onMouseDown, false);
  document.addEventListener('mouseup', onMouseUp, false);
  window.addEventListener('resize', onWindowResize, false );
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

  camera.left = window.innerWidth / -ratio;
  camera.right = window.innerWidth / ratio;
  camera.top = window.innerHeight / ratio;
  camera.bottom = window.innerHeight / -ratio;
  camera.updateProjectionMatrix();

  renderer.setSize( window.innerWidth, window.innerHeight );
}

function rotateScene(deltaX, deltaY) {
  group.rotation.y += deltaX / 100;
  group.rotation.x += deltaY / 100;
}

init();
render();
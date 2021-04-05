/*
*
* Practica_02_base.js
* Videojuegos (30262) - Curso 2019-2020
*
* Parte adaptada de: Alex Clarke, 2016, y Ed Angel, 2015.
*
* Modificada por Andrés Otero y Néstor Monzón
* 05/04/2021
*/

// Variable to store the WebGL rendering context
var gl;

//----------------------------------------------------------------------------
// MODEL DATA
//----------------------------------------------------------------------------

//Define points' position vectors
const cubeVerts = [
	[ 0.5, 0.5, 0.5, 1], //0
	[ 0.5, 0.5,-0.5, 1], //1
	[ 0.5,-0.5, 0.5, 1], //2
	[ 0.5,-0.5,-0.5, 1], //3
	[-0.5, 0.5, 0.5, 1], //4
	[-0.5, 0.5,-0.5, 1], //5
	[-0.5,-0.5, 0.5, 1], //6
	[-0.5,-0.5,-0.5, 1], //7
];

const wireCubeIndices = [
//Wire Cube - use LINE_STRIP, starts at 0, 30 vertices
	0,4,6,2,0, //front
	1,0,2,3,1, //right
	5,1,3,7,5, //back
	4,5,7,6,4, //right
	4,0,1,5,4, //top
	6,7,3,2,6, //bottom
];

const cubeIndices = [
//Solid Cube - use TRIANGLES, starts at 0, 36 vertices
	0,4,6, //front
	0,6,2,
	1,0,2, //right
	1,2,3,
	5,1,3, //back
	5,3,7,
	4,5,7, //left
	4,7,6,
	4,0,1, //top
	4,1,5,
	6,7,3, //bottom
	6,3,2,
];

const pointsAxes = [];
pointsAxes.push([ 2.0, 0.0, 0.0, 1.0]); //x axis is green
pointsAxes.push([-2.0, 0.0, 0.0, 1.0]);
pointsAxes.push([ 0.0, 2.0, 0.0, 1.0]); //y axis is red
pointsAxes.push([ 0.0,-2.0, 0.0, 1.0]);
pointsAxes.push([ 0.0, 0.0, 2.0, 1.0]); //z axis is blue
pointsAxes.push([ 0.0, 0.0,-2.0, 1.0]);

const pointsWireCube = [];
for (let i=0; i < wireCubeIndices.length; i++)
{
	pointsWireCube.push(cubeVerts[wireCubeIndices[i]]);
}

const pointsCube = [];
for (let i=0; i < cubeIndices.length; i++)
{
	pointsCube.push(cubeVerts[cubeIndices[i]]);
}

const shapes = {
	wireCube: {Start: 0, Vertices: 30},
	cube: {Start: 0, Vertices: 36},
	axes: {Start: 0, Vertices: 6}
};

const red =			[1.0, 0.0, 0.0, 1.0];
const green =		[0.0, 1.0, 0.0, 1.0];
const blue =		[0.0, 0.0, 1.0, 1.0];
const lightred =	[1.0, 0.5, 0.5, 1.0];
const lightgreen =	[0.5, 1.0, 0.5, 1.0];
const lightblue = 	[0.5, 0.5, 1.0, 1.0];
const white =		[1.0, 1.0, 1.0, 1.0];

const colorsAxes = [
	green, green, //x
	red, red,     //y
	blue, blue,   //z
];

const colorsWireCube = [
	white, white, white, white, white,
	white, white, white, white, white,
	white, white, white, white, white,
	white, white, white, white, white,
	white, white, white, white, white,
	white, white, white, white, white,
];

const colorsCube = [
	lightblue, lightblue, lightblue, lightblue, lightblue, lightblue,
	lightgreen, lightgreen, lightgreen, lightgreen, lightgreen, lightgreen,
	lightred, lightred, lightred, lightred, lightred, lightred,
	blue, blue, blue, blue, blue, blue,
	red, red, red, red, red, red,
	green, green, green, green, green, green,
];

//----------------------------------------------------------------------------
// OTHER DATA
//----------------------------------------------------------------------------

var model = new mat4();   		// create a model matrix and set it to the identity matrix
var view = new mat4();   		// create a view matrix and set it to the identity matrix
var projection = new mat4();	// create a projection matrix and set it to the identity matrix

var eye, target, up;			// for view matrix

var rotAngle = 0.0;
var rotChange = 0.5;

var program;
var uLocations = {};
var aLocations = {};

const pixelPerDegree = 20.0 //100 pixels = 1 degree

var programInfo = {
			program,
			uniformLocations: {},
			attribLocations: {},
};

var objectsToDraw = [
		{
		  programInfo: programInfo,
		  pointsArray: pointsAxes,
		  colorsArray: colorsAxes,
		  uniforms: {
			u_colorMult: [1.0, 1.0, 1.0, 1.0],
			u_model: new mat4(),
		  },
		  primType: "lines",
		},
		{
		  programInfo: programInfo,
		  pointsArray: pointsWireCube,
		  colorsArray: colorsWireCube,
		  uniforms: {
			u_colorMult: [1.0, 1.0, 1.0, 1.0],
			u_model: new mat4(),
		  },
		  primType: "line_strip",
		},
		{
		  programInfo: programInfo,
		  pointsArray: pointsCube,
		  colorsArray: colorsCube,
		  uniforms: {
			u_colorMult: [1.0, 1.0, 1.0, 1.0],
			u_model: new mat4(),
		  },
		  primType: "triangles",
		},
		{
		  programInfo: programInfo,
		  pointsArray: pointsCube,
		  colorsArray: colorsCube,
		  uniforms: {
			u_colorMult: [0.5, 0.5, 0.5, 1.0],
			u_model: new mat4(),
		  },
		  primType: "triangles",
		},
];

var pitch = 0;
var yaw = 0;
var projection_type;
var default_proj;
var ortho_proj;
var fov;
var aspect;

//----------------------------------------------------------------------------
// Initialization function
//----------------------------------------------------------------------------

function copyMat(col1, fil1, col2, fil2) {
	for (var i = 0; i < array.length; i++) {
		array[i]
	}
}


var translation = new mat4();

window.onload = function init() {

	// Set up a WebGL Rendering Context in an HTML5 Canvas
	var canvas = document.getElementById("gl-canvas");
	gl = WebGLUtils.setupWebGL(canvas);
	if (!gl) {
		alert("WebGL isn't available");
	}

	//  Configure WebGL
	gl.clearColor(0.0, 0.0, 0.0, 1.0);
	gl.enable(gl.DEPTH_TEST);
	window.addEventListener("keydown", keyPressedHandler);
	var click_x;
	var click_y;
	var original_view;

	var transform;
	window.addEventListener("mousedown", function (event) {
		click_x = event.clientX;
		click_y = event.clientY;
		//original_view = mat4(gl.getUniform(program, gl.getUniformLocation(program, "view")));
	});

	window.addEventListener("mousemove", function (event) {
		// Cambiar para que al llegar a determinado punto de la pantalla, se llegue al máximo de rotación
		// para ello, se puede rotar en el ángulo que queda para llegar a esa posición
		mousePressedHandler(event, click_x, click_y);
		updateView();
		click_x = event.clientX;
		click_y = event.clientY;
		//original_view = mat4(gl.getUniform(program, gl.getUniformLocation(program, "view")));
	});


	// ------- Add our stuff:
	let nCubes = 20;
	addCubes(objectsToDraw, nCubes);

	setPrimitive(objectsToDraw);

	// Set up a WebGL program
	// Load shaders and initialize attribute buffers
	program = initShaders(gl, "vertex-shader", "fragment-shader");

	// Save the attribute and uniform locations
	uLocations.model = gl.getUniformLocation(program, "model");
	uLocations.view = gl.getUniformLocation(program, "view");
	uLocations.projection = gl.getUniformLocation(program, "projection");
	uLocations.colorMult = gl.getUniformLocation(program, "colorMult");
	aLocations.vPosition = gl.getAttribLocation(program, "vPosition");
	aLocations.vColor = gl.getAttribLocation(program, "vColor");

	programInfo.uniformLocations = uLocations;
	programInfo.attribLocations = aLocations;
	programInfo.program = program;

	gl.useProgram(programInfo.program);

	// Set up viewport
	gl.viewport(0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight);

	// Set up camera
	// Projection matrix
	fov = 45.0;
	aspect = canvas.width / canvas.height;
	default_proj = perspective(fov, aspect, 0.1, 100.0);
	ortho_proj = ortho(-canvas.width/100, canvas.width/100, -canvas.height/100, canvas.height/100, 0.1, 100.0);
	projection = default_proj;
	projection_type = "perspective"
	gl.uniformMatrix4fv( programInfo.uniformLocations.projection, gl.FALSE, projection ); // copy projection to uniform value in shader
    // View matrix (static cam)
	eye = vec3(-5.0, 5.0, 10.0);
	target =  vec3(0.0, 0.0, 0.0);
	up =  vec3(0.0, 1.0, 0.0);
	view = lookAt(eye,target,up);
	gl.uniformMatrix4fv(programInfo.uniformLocations.view, gl.FALSE, view); // copy view to uniform value in shader
	translation = view;
	requestAnimFrame(render);

};

function keyPressedHandler(event) {
	transform = null;
	switch (event.key) {
		case "Down":
		case "ArrowDown":
			if(transform == null) transform = translate(0.0, 0.0, 1.0) // Z axis towards camera
		case "Up":
		case "ArrowUp":
			if(transform == null) transform = translate(0.0, 0.0, -1.0)
		case "Left":
		case "ArrowLeft":
			if(transform == null) transform = translate(-1.0, 0.0, 0.0)
		case "Right":
		case "ArrowRight":
			if(transform == null) transform = translate(1.0, 0.0, 0.0)

			if (transform != null) {
				//original = mat4(gl.getUniform(program, gl.getUniformLocation(program, "view")));
				translation = mult(inverse(transform), translation)
				updateView();
			}
			break;
		case "P":
		case "p":
			console.log("P");
			fov = 45.0
			projection = default_proj;
			projection_type = "perspective"
			gl.uniformMatrix4fv(programInfo.uniformLocations.projection, gl.FALSE, projection);
			break;
		case "O":
		case "o":
			console.log("O");
			projection = ortho_proj;
			projection_type = "orthogonal"
			gl.uniformMatrix4fv(programInfo.uniformLocations.projection, gl.FALSE, projection);
			break;
		case "Add":
		case "+":
			console.log("+");
			if (fov < 179) {
				proj = mat4(gl.getUniform(program, gl.getUniformLocation(program, "projection")));
				fov = changeFOV(proj, fov, 1.0);
				if (projection_type == "perspective") {
					//projection = default_proj;
					gl.uniformMatrix4fv(programInfo.uniformLocations.projection, gl.FALSE, proj);
				}
			}
			break;
		case "Substract":
		case "-":
			console.log("-");
			if (fov > 6.0) {
				proj = mat4(gl.getUniform(program, gl.getUniformLocation(program, "projection")));
				fov = changeFOV(proj, fov, -1.0);
				if (projection_type == "perspective") {
					//projection = default_proj;
					gl.uniformMatrix4fv(programInfo.uniformLocations.projection, gl.FALSE, proj);
				}
			}
			break;
	}
	return null;
}

function mousePressedHandler(event, x0, y0) {
	let ejeX = vec3(1.0, 0.0, 0.0);
	let ejeY = vec3(0.0, 1.0, 0.0);
	if (event.buttons == 1) {
		rotP = (event.clientX - x0) / pixelPerDegree;
		if (pitch + rotP >= 90) {
			rotP = pitch - 90;
			pitch = 90
		}
		else if (pitch + rotP <= -90) {
			rotP = pitch + 90;
			pitch = -90
		}
		else {
			pitch += rotP;
		}

		rotY = (event.clientY - y0) / pixelPerDegree;
		if (yaw + rotY >= 90) {
			rotY = yaw - 90;
			yaw = 90

		}
		else if (yaw + rotY <= -90) {
			rotY = yaw + 90;
			yaw = -90
		}
		else {
			yaw += rotY;
		}
	}
}

// Returns a random rgb color
function randColor() {
	let end = false;
	let c = [];
	while (!end) {
		c = [Math.random(),Math.random(),Math.random(),1] // random color with alpha=1
		let sum = 0;
		for (var i = 0; i < c.length-1; i++) {
			sum += c[i];
		}
		end = sum>1.5;
	}
	return c;
}

// Returns a random speed (between 0.3 and 1, either sign)
function randSpeed() {
	let s = 2.0*(Math.random()-0.5);
	let mins = 0.3;
	if (Math.abs(s) < mins) {
		s = (s<0) ? -mins : mins;
	}
	return s;
}

// Not really uniform spherical distribution but probably good enough here:
function randNormalVec3() {
	return normalize(vec3(Math.random(), Math.random(), Math.random()));
}

// ------------------------------------
// Add our n cubes to objects:
function addCubes(objects, n) {
	let maxX = 12
	let maxY = 8
	let maxZ = 10
	for (var i = 0; i < n; i++) {
		let pos = vec3(maxX*(Math.random()-0.5), maxY*(Math.random()-0.5), maxZ*(Math.random()-0.5));
		let color = randColor();
		objects.push({
			programInfo: programInfo,
			pointsArray: pointsCube,
			colorsArray: Array(36).fill(color),
			uniforms: {
			u_colorMult: [0.5, 0.5, 0.5, 1.0],
			u_model: new mat4(),
			},
			primType: "triangles",
			translation: translate(pos[0], pos[1], pos[2]),
			locRotAxis: randNormalVec3(),
			locRotSpeed: randSpeed(),
			// world rot axis must be perpendicular to pos-origin, so we cross it with a random vector:
			worldRotAxis: normalize(cross(randNormalVec3(),pos)),
			worldRotSpeed: randSpeed(),
		});
	}
}

//----------------------------------------------------------------------------
// Rendering Event Function
//----------------------------------------------------------------------------

function render() {

	gl.clear(gl.DEPTH_BUFFER_BIT | gl.COLOR_BUFFER_BIT);

	//----------------------------------------------------------------------------
	// MOVE STUFF AROUND
	//----------------------------------------------------------------------------

	let ejeY = vec3(0.0, 1.0, 0.0);
	let R = rotate(rotAngle, ejeY);

	objectsToDraw[2].uniforms.u_model = translate(1.0, 1.0, 3.0);
	objectsToDraw[2].uniforms.u_model = mult(objectsToDraw[2].uniforms.u_model, R);

	objectsToDraw[3].uniforms.u_model = translate(1.0, 0.0, 3.0);
	objectsToDraw[3].uniforms.u_model = mult(R, objectsToDraw[3].uniforms.u_model);

	// Our rotations:
	for (var i = 4; i < objectsToDraw.length; i++) {
		// Local rotation
		let R = rotate(rotAngle*objectsToDraw[i].locRotSpeed*5.0, objectsToDraw[i].locRotAxis);
		objectsToDraw[i].uniforms.u_model = mult(objectsToDraw[i].translation, R);
		// World rotation:
		R = rotate(rotAngle*objectsToDraw[i].worldRotSpeed*5.0, objectsToDraw[i].worldRotAxis);
		//objectsToDraw[i].uniforms.u_model = objectsToDraw[i].translation;
		objectsToDraw[i].uniforms.u_model = mult(R, objectsToDraw[i].uniforms.u_model);
	}


	//----------------------------------------------------------------------------
	// DRAW
	//----------------------------------------------------------------------------

	objectsToDraw.forEach(function(object) {

		gl.useProgram(object.programInfo.program);

		// Setup buffers and attributes
		setBuffersAndAttributes(object.programInfo, object.pointsArray, object.colorsArray);

		// Set the uniforms
		setUniforms(object.programInfo, object.uniforms);

		// Draw
		gl.drawArrays(object.primitive, 0, object.pointsArray.length);
    });

	rotAngle += rotChange;

	// Transform view:
	requestAnimationFrame(render);

}

// Updates the gl view according to the pitch, yaw (x and y rotations), and the translation
// Called every time either changes
function updateView() {
	v = new mat4();
	v = mult(translation,  v);
	v = mult(rotate(pitch, vec3(0.0, 1.0, 0.0)), v);
	v = mult(rotate(yaw, vec3(1.0, 0.0, 0.0)), v);
	gl.uniformMatrix4fv(programInfo.uniformLocations.view, gl.FALSE, v);
}

//----------------------------------------------------------------------------
// Utils functions
//----------------------------------------------------------------------------

function setPrimitive(objectsToDraw) {

	objectsToDraw.forEach(function(object) {
		switch(object.primType) {
		  case "lines":
			object.primitive = gl.LINES;
			break;
		  case "line_strip":
			object.primitive = gl.LINE_STRIP;
			break;
		  case "triangles":
		    object.primitive = gl.TRIANGLES;
		    break;
		  default:
			object.primitive = gl.TRIANGLES;
		}
	});
}

function setUniforms(pInfo, uniforms) {
	// Copy uniform model values to corresponding values in shaders
	gl.uniform4f(pInfo.uniformLocations.colorMult, uniforms.u_colorMult[0], uniforms.u_colorMult[1], uniforms.u_colorMult[2], uniforms.u_colorMult[3]);
	gl.uniformMatrix4fv(pInfo.uniformLocations.model, gl.FALSE, uniforms.u_model);
}

function setBuffersAndAttributes(pInfo, ptsArray, colArray) {
	// Load the data into GPU data buffers
	// Vertices
	var vertexBuffer = gl.createBuffer();
	gl.bindBuffer( gl.ARRAY_BUFFER, vertexBuffer );
	gl.bufferData( gl.ARRAY_BUFFER,  flatten(ptsArray), gl.STATIC_DRAW );
	gl.vertexAttribPointer( pInfo.attribLocations.vPosition, 4, gl.FLOAT, gl.FALSE, 0, 0 );
	gl.enableVertexAttribArray( pInfo.attribLocations.vPosition );

	// Colors
	var colorBuffer = gl.createBuffer();
	gl.bindBuffer( gl.ARRAY_BUFFER, colorBuffer );
	gl.bufferData( gl.ARRAY_BUFFER,  flatten(colArray), gl.STATIC_DRAW );
	gl.vertexAttribPointer( pInfo.attribLocations.vColor, 4, gl.FLOAT, gl.FALSE, 0, 0 );
	gl.enableVertexAttribArray( pInfo.attribLocations.vColor );
}

function changeFOV(proj, fov0, delta) {
	console.log("changeFov:", fov0, delta)
	//Multiplicamos por el valor del anterior fov y realizamos de nuevo el cálculo de las dos líneas anteriores
	var f = Math.tan( radians(fov0) / 2 ) / Math.tan( radians(fov0 + delta) / 2 );

	proj[0] *= f;
	proj[5] *= f;
	//Asignamos el nuevo valor de fov0
	fov0 += delta;
	return fov0;
}

// This function takes the projection matrix, the translation, and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// The given projection matrix is also a 4x4 matrix stored as an array in column-major order.
// You can use the MatrixMult function defined in project4.html to multiply two 4x4 matrices in the same format.
function GetModelViewProjection( projectionMatrix, translationX, translationY, translationZ, rotationX, rotationY )
{
	// [TO-DO] Modify the code below to form the transformation matrix.
	var rotationZ = 0
	var trans = [
		Math.cos(rotationZ)*Math.cos(rotationY), Math.cos(rotationZ)*Math.sin(rotationY)*Math.sin(rotationX) - Math.sin(rotationZ)*Math.cos(rotationX), Math.cos(rotationZ)*Math.sin(rotationY)*Math.cos(rotationX) +  Math.sin(rotationZ)*Math.sin(rotationX), 0,
		Math.sin(rotationZ)*Math.cos(rotationY), Math.sin(rotationZ)*Math.sin(rotationY)*Math.sin(rotationX) + Math.cos(rotationZ)*Math.cos(rotationX), Math.sin(rotationZ)*Math.sin(rotationY)*Math.cos(rotationX) -  Math.cos(rotationZ)*Math.sin(rotationX), 0,
		-Math.sin(rotationY), Math.cos(rotationY)*Math.sin(rotationX), Math.cos(rotationY)*Math.cos(rotationX), 0,
		translationX, translationY, translationZ, 1
	];
	var mvp = MatrixMult( projectionMatrix, trans );
	return mvp;
}


// [TO-DO] Complete the implementation of the following class.

class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
		// Compile the shader program
		this.prog = InitShaderProgram(objVS, objFS);
		// Get the ids of the uniform variables in the shaders
		this.mvp = gl.getUniformLocation( this.prog, 'mvp' );
		// Get the ids of the vertex attributes in the shaders
		this.vertPos = gl.getAttribLocation( this.prog, 'pos' );
		this.txc = gl.getAttribLocation( this.prog, 'txc' );
		this.vertbuffer = gl.createBuffer();
		this.texcoordbuffer = gl.createBuffer();
		this.usingTexture = gl.getUniformLocation( this.prog, 'usingTexture');
		this.isswapYZ = gl.getUniformLocation( this.prog, 'swapYZ');
	}
	// This method is called every time the user opens an OBJ file.
	// The arguments of this function is an array of 3D vertex positions
	// and an array of 2D texture coordinates.
	// Every item in these arrays is a floating point value, representing one
	// coordinate of the vertex position or texture coordinate.
	// Every three consecutive elements in the vertPos array forms one vertex
	// position and every three consecutive vertex positions form a triangle.
	// Similarly, every two consecutive elements in the texCoords array
	// form the texture coordinate of a vertex.
	// Note that this method can be called multiple times.
	setMesh( vertPos, texCoords )
	{
		// [TO-DO] Update the contents of the vertex buffer objects.
		this.numTriangles = vertPos.length / 3;
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.texcoordbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);
	}
	
	// This method is called when the user changes the state of the
	// "Swap Y-Z Axes" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	swapYZ( swap )
	{
		gl.useProgram(this.prog);
		if(swap){
			gl.uniform1f(this.isswapYZ, 1.0); 
		}else{
			gl.uniform1f(this.isswapYZ, 0.0); 
		}
	}
	
	// This method is called to draw the triangular mesh.
	// The argument is the transformation matrix, the same matrix returned
	// by the GetModelViewProjection function above.
	draw( trans )
	{
		// [TO-DO] Complete the WebGL initializations before drawing
		gl.useProgram(this.prog);
		gl.uniformMatrix4fv(this.mvp, false, trans);
		gl.bindBuffer( gl.ARRAY_BUFFER, this.vertbuffer );
		gl.vertexAttribPointer( this.vertPos, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray(this.vertPos);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.texcoordbuffer);
		gl.vertexAttribPointer(this.txc, 2, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.txc);

		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
		console.log(img);
		gl.useProgram(this.prog);
		// [TO-DO] Bind the texture
		gl.activeTexture(gl.TEXTURE0);
		var texture = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, texture);
		// You can set the texture image data using the following command.
		gl.texParameteri(
			gl.TEXTURE_2D,
			gl.TEXTURE_MAG_FILTER,
			gl.LINEAR
		);
		gl.texImage2D( gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img );
		gl.generateMipmap(gl.TEXTURE_2D);


		// [TO-DO] Now that we have a texture, it might be a good idea to set
		// some uniform parameter(s) of the fragment shader, so that it uses the texture.
		var sampler = gl.getUniformLocation(this.prog, 'tex');
		gl.uniform1i(sampler, 0);
	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		gl.useProgram(this.prog);
		if(show){
			gl.uniform1f(this.usingTexture, 1.0); 
		}else{
			gl.uniform1f(this.usingTexture, 0.0); 
		}
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify if it should use the texture.
	}
	
}
// Vertex shader source code
var objVS = `
	attribute vec3 pos;
	attribute vec2 txc;
	uniform mat4 mvp;
	varying vec2 texCoord;
	uniform float swapYZ;

	void main()
	{
		if(swapYZ > 0.5){
			gl_Position = mvp * vec4(pos[0], pos[2], pos[1],1);
		}else{
			gl_Position = mvp * vec4(pos,1);
		}
		texCoord = txc;
	}
`;
// Fragment shader source code
var objFS = `
	precision mediump float;
	uniform sampler2D tex;
	varying vec2 texCoord;
	uniform float usingTexture;

	void main()
	{
		if(usingTexture > 0.5){
			gl_FragColor = texture2D(tex, texCoord);
		}else{
			gl_FragColor = vec4(1,gl_FragCoord.z*gl_FragCoord.z,0,1);
		}
	}
`;
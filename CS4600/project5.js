// This function takes the translation and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// You can use the MatrixMult function defined in project5.html to multiply two 4x4 matrices in the same format.
function GetModelViewMatrix( translationX, translationY, translationZ, rotationX, rotationY )
{
	// [TO-DO] Modify the code below to form the transformation matrix.
	var rotationZ = 0
	var trans = [
		Math.cos(rotationZ)*Math.cos(rotationY), Math.cos(rotationZ)*Math.sin(rotationY)*Math.sin(rotationX) - Math.sin(rotationZ)*Math.cos(rotationX), Math.cos(rotationZ)*Math.sin(rotationY)*Math.cos(rotationX) +  Math.sin(rotationZ)*Math.sin(rotationX), 0,
		Math.sin(rotationZ)*Math.cos(rotationY), Math.sin(rotationZ)*Math.sin(rotationY)*Math.sin(rotationX) + Math.cos(rotationZ)*Math.cos(rotationX), Math.sin(rotationZ)*Math.sin(rotationY)*Math.cos(rotationX) -  Math.cos(rotationZ)*Math.sin(rotationX), 0,
		-Math.sin(rotationY), Math.cos(rotationY)*Math.sin(rotationX), Math.cos(rotationY)*Math.cos(rotationX), 0,
		translationX, translationY, translationZ, 1
	];
	var mv = trans;
	return mv;
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
		this.mv = gl.getUniformLocation( this.prog, 'mv' );
		this.normal = gl.getUniformLocation(this.prog, 'normal');
		this.lightDir = gl.getUniformLocation(this.prog, 'ltDir');
		this.usingTexture = gl.getUniformLocation( this.prog, 'usingTexture');
		this.isswapYZ = gl.getUniformLocation( this.prog, 'swapYZ');
		this.alpha = gl.getUniformLocation(this.prog, 'alpha');
		// Get the ids of the vertex attributes in the shaders
		this.vertPos = gl.getAttribLocation( this.prog, 'vert_pos' );
		this.txc = gl.getAttribLocation( this.prog, 'vert_txc' );
		this.vertNormal = gl.getAttribLocation(this.prog, 'vert_n');
		this.vertPosbuffer = gl.createBuffer();
		this.texcoordbuffer = gl.createBuffer();
		this.vertNormalbuffer = gl.createBuffer();
	}
	
	// This method is called every time the user opens an OBJ file.
	// The arguments of this function is an array of 3D vertex positions,
	// an array of 2D texture coordinates, and an array of vertex normals.
	// Every item in these arrays is a floating point value, representing one
	// coordinate of the vertex position or texture coordinate.
	// Every three consecutive elements in the vertPos array forms one vertex
	// position and every three consecutive vertex positions form a triangle.
	// Similarly, every two consecutive elements in the texCoords array
	// form the texture coordinate of a vertex and every three consecutive 
	// elements in the normals array form a vertex normal.
	// Note that this method can be called multiple times.
	setMesh( vertPos, texCoords, normals )
	{
		this.numTriangles = vertPos.length / 3;
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertPosbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.texcoordbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertNormalbuffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW);
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
	// The arguments are the model-view-projection transformation matrixMVP,
	// the model-view transformation matrixMV, the same matrix returned
	// by the GetModelViewProjection function above, and the normal
	// transformation matrix, which is the inverse-transpose of matrixMV.
	draw( matrixMVP, matrixMV, matrixNormal )
	{
		//uniform variables
		gl.useProgram(this.prog);
		gl.uniformMatrix4fv(this.mvp, false, matrixMVP);
		gl.uniformMatrix4fv(this.mv, false, matrixMV);
		gl.uniformMatrix3fv(this.normal, false, matrixNormal);
		//vertex attributes
		//vertbuffer to vertPos
		gl.bindBuffer( gl.ARRAY_BUFFER, this.vertPosbuffer );
		gl.vertexAttribPointer( this.vertPos, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray(this.vertPos);
		//normalbuffer to vertNormal
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vertNormalbuffer);
		gl.vertexAttribPointer(this.vertNormal, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.vertNormal);
		//texcoordbuffer to txc
		gl.bindBuffer(gl.ARRAY_BUFFER, this.texcoordbuffer);
		gl.vertexAttribPointer(this.txc, 2, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray(this.txc);
		//draw
		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
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

		// You can set the texture image data using the following command.
		gl.texImage2D( gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img );
		gl.generateMipmap(gl.TEXTURE_2D);


		// [TO-DO] Now that we have a texture, it might be a good idea to set
		// some uniform parameter(s) of the fragment shader, so that it uses the texture.
		var sampler = gl.getUniformLocation(this.prog, 'tex');
		gl.uniform1f(this.usingTexture, 1.0); 
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
	}
	
	// This method is called to set the incoming light direction
	setLightDir( x, y, z )
	{
		var ltDir = [x, y, z];
		gl.useProgram(this.prog);
		gl.uniform3fv(this.lightDir, ltDir);
	}
	
	// This method is called to set the shininess of the material
	setShininess( shininess )
	{
		gl.useProgram(this.prog);
		gl.uniform1f(this.alpha, shininess);
	}
}
// Vertex shader source code
var objVS = `
	attribute vec3 vert_pos;
	attribute vec3 vert_n;
	attribute vec2 vert_txc;

	uniform mat4 mvp;
	uniform mat4 mv;
	uniform mat3 normal;
	uniform float swapYZ;

	varying vec2 frag_txc;
	varying vec3 frag_n; //surface normal
	varying vec4 frag_pos;
	varying float f_swapYZ;

	void main()
	{
		if(swapYZ > 0.5){
			gl_Position = mvp * vec4(vert_pos[0], vert_pos[2], vert_pos[1],1);
			frag_pos = mv * vec4(vert_pos[0], vert_pos[2], vert_pos[1],1);
			frag_n = normal * vec3(vert_n[0], vert_n[2], vert_n[1]);
		}else{
			gl_Position = mvp * vec4(vert_pos,1);
			frag_pos = mv * vec4(vert_pos,1);
			frag_n = normal * vert_n;
		}
		frag_txc = vert_txc;
	}
`;
// Fragment shader source code
var objFS = `
	precision mediump float;

	uniform sampler2D tex;
	uniform float usingTexture;
	uniform vec3 ltDir;
	uniform float alpha;

	varying vec2 frag_txc;
	varying vec3 frag_n;
	varying vec4 frag_pos;

	void main()
	{
		vec4 color;
		if(usingTexture > 0.5){
			color = texture2D(tex, frag_txc);
		}else{
			color = vec4(0.85, 0.85, 0.85,1);
		}
		vec3 diffuseColor = color.rgb;
		vec3 h = normalize(ltDir - frag_pos.xyz);
		vec3 specularColor = vec3(1,1,1);
		vec3 specular = (pow(max(dot(frag_n, h), 0.0), alpha)/dot(frag_n, ltDir))*specularColor;
		
		vec3 ambient = color.rgb * 0.2;
		vec3 ltColor = vec3(1, 1, 1);
		vec3 lt = ltColor * max(dot(frag_n, ltDir), 0.0);

		vec3 C = ambient + (lt * (diffuseColor+specular));
		gl_FragColor = vec4(C, color.a);


	}
`;

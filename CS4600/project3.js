// [TO-DO] Complete the implementation of the following class and the vertex shader below.

class CurveDrawer {
	constructor()
	{
		this.prog   = InitShaderProgram( curvesVS, curvesFS );
		// [TO-DO] Other initializations should be done here.
		// [TO-DO] This is a good place to get the locations of attributes and uniform variables.

		// Get the ids of the uniform variables in the shaders
		this.mvp = gl.getUniformLocation( this.prog, 'mvp' );
		// Get the ids of the vertex attributes in the shaders
		console.log("getUniformLocation");
		this.vertP0 = gl.getUniformLocation( this.prog, 'p0' );
		console.log(this.vertP0);
		this.vertP1 = gl.getUniformLocation( this.prog, 'p1' );
		this.vertP2 = gl.getUniformLocation( this.prog, 'p2' );
		this.vertP3 = gl.getUniformLocation( this.prog, 'p3' );
		this.vertt = gl.getAttribLocation( this.prog, 't' );
		console.log(this.vertt);
		console.log("getUniformLocation done");
		// Create the vertex buffer object
		this.buffer = gl.createBuffer();
		// We are not filling the contents of the buffer here,
		// because we will put the control points into this buffer.
		
		// Initialize the attribute buffer
		this.steps = 100;
		var tv = [];
		for ( var i=0; i<this.steps; ++i ) {
			tv.push( i / (this.steps-1) );
		}
		gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tv), gl.STATIC_DRAW);
		// [TO-DO] This is where you can create and set the contents of the vertex buffer object
		// for the vertex attribute we need.
	}
	setViewport( width, height )
	{
		// [TO-DO] This is where we should set the transformation matrix.
		// [TO-DO] Do not forget to bind the program before you set a uniform variable value.

		// Compute the orthographic projection matrix and send it to the shader
		var trans = [ 2/width,0,0,0,  0,-2/height,0,0, 0,0,1,0, -1,1,0,1 ];
		gl.useProgram( this.prog );
		gl.uniformMatrix4fv( this.mvp, false, trans );
	}
	updatePoints( pt )
	{
		// [TO-DO] The control points have changed, we must update corresponding uniform variables.
		// [TO-DO] Do not forget to bind the program before you set a uniform variable value.
		// [TO-DO] We can access the x and y coordinates of the i^th control points using
		// var x = pt[i].getAttribute("cx");
		// var y = pt[i].getAttribute("cy");

		// The control points have changed, so we must update 
		// the data in the in the vertex buffer
		var p = [];
		for ( var i=0; i<4; ++i ) {
			var x = pt[i].getAttribute("cx");
			var y = pt[i].getAttribute("cy");
			p.push(x);
			p.push(y);
		}
		gl.useProgram( this.prog );
		gl.uniform2fv(this.vertP0, [p[0], p[1]]);
		gl.uniform2fv(this.vertP1, [p[2], p[3]]);
		gl.uniform2fv(this.vertP2, [p[4], p[5]]);
		gl.uniform2fv(this.vertP3, [p[6], p[7]]);

	}
	draw()
	{
		// [TO-DO] This is where we give the command to draw the curve.
		// [TO-DO] Do not forget to bind the program and set the vertex attribute.

		// Draw the line segments
		gl.useProgram( this.prog );
		gl.bindBuffer( gl.ARRAY_BUFFER, this.buffer );
		gl.vertexAttribPointer( this.vertt, 2, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.vertt )
		gl.drawArrays( gl.LINE_STRIP, 0, 50 );
	}
}

// Vertex Shader
var curvesVS = `
	uniform vec2 p0;
	attribute float t;
	uniform mat4 mvp;
	uniform vec2 p1;
	uniform vec2 p2;
	uniform vec2 p3;

	void main()
	{
		// [TO-DO] Replace the following with the proper vertex shader code
		gl_Position =  mvp * (pow(1.0 - t, 3.0) * vec4(p0, 0, 1) +3.0*pow(1.0 - t, 2.0)*t * vec4(p1, 0, 1) + 3.0*(1.0 - t)*pow(t, 2.0) * vec4(p2, 0, 1) + pow(t, 3.0) * vec4(p3, 0, 1));
	}
`;

//(pow(1.0 - t, 3.0) * vec4(p0, 0, 1) +3.0*pow(1.0 - t, 2.0)*t * vec4(p1, 0, 1) + 3.0*(1.0 - t)*pow(t, 2.0) * vec4(p2, 0, 1) + pow(t, 3.0) * vec4(p3, 0, 1))

// Fragment Shader
var curvesFS = `
	precision mediump float;
	void main()
	{
		gl_FragColor = vec4(1,0,0,1);
	}
`;
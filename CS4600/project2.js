// Returns a 3x3 transformation matrix as an array of 9 values in column-major order.
// The transformation first applies scale, then rotation, and finally translation.
// The given rotation value is in degrees.
function GetTransform( positionX, positionY, rotation, scale )
{

	return Array( scale * Math.cos(-rotation/180 * 3.14159), -Math.sin(-rotation/180 * 3.14159), 0, Math.sin(-rotation/180 * 3.14159), Math.cos(-rotation/180 * 3.14159), 0, positionX, positionY, 1 );
}

// Returns a 3x3 transformation matrix as an array of 9 values in column-major order.
// The arguments are transformation matrices in the same format.
// The returned transformation first applies trans1 and then trans2.
function ApplyTransform( trans1, trans2 )
{
	let trans3 = new Array(9);
	let id = 0;
	for(let i = 0; i <=6; i+=3){
		for(let j = 0; j<=2; j++){
			trans3[id] = trans1[i] * trans2[j] + trans1[i+1] * trans2[j+3] + trans1[i+2] * trans2[j+6];
			id++;
		}
	}

	return trans3;
}

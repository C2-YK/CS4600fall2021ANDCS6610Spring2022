var raytraceFS = `
struct Ray {
	vec3 pos;
	vec3 dir;
};

struct Material {
	vec3  k_d;	// diffuse coefficient
	vec3  k_s;	// specular coefficient
	float n;	// specular exponent
};

struct Sphere {
	vec3     center;
	float    radius;
	Material mtl;
};

struct Light {
	vec3 position;
	vec3 intensity;
};

struct HitInfo {
	float    t;
	vec3     position;
	vec3     normal;
	Material mtl;
};

uniform Sphere spheres[ NUM_SPHERES ];
uniform Light  lights [ NUM_LIGHTS  ];
uniform samplerCube envMap;
uniform int bounceLimit;

bool IntersectRay( inout HitInfo hit, Ray ray );

// Shades the given point and returns the computed color.
vec3 Shade( Material mtl, vec3 position, vec3 normal, vec3 view )
{
	vec3 color = vec3(0,0,0);
	for ( int i=0; i<NUM_LIGHTS; ++i ) {
		vec3 ltDir = normalize(lights[i].position - position);
		HitInfo hit;
		Ray ray;
		ray.pos = position;
		ray.dir = ltDir;
		if(IntersectRay(hit, ray)){

		}else{
			vec3 ltTens = lights[i].intensity;
			vec3 diffuseColor = mtl.k_d;
			vec3 specularColor = mtl.k_s;
			vec3 h = normalize(ltDir+view);
			float alpha = 100.0;

			vec3 specular = (pow(max(dot(normal, h), 0.0), alpha) / dot(normal, ltDir)) * specularColor;

			vec3 lt = ltTens * max(dot(normal, ltDir), 0.0);		
			color += lt * (diffuseColor + specular);
			
		}
	}
	return color;
}

// Intersects the given ray with all spheres in the scene
// and updates the given HitInfo using the information of the sphere
// that first intersects with the ray.
// Returns true if an intersection is found.
bool IntersectRay( inout HitInfo hit, Ray ray )
{
	hit.t = 1e30;
	bool foundHit = false;
	for ( int i=0; i<NUM_SPHERES; ++i ) {
		vec3 sc = spheres[i].center;
		float sr = spheres[i].radius;
		vec3 rp = ray.pos;
		vec3 rd = ray.dir;

		float a = dot(rd,rd);
		float b = dot( 2.0 * rd, rp - sc);
		float c = dot(rp - sc, rp - sc) - sr * sr;

		float delta = b * b - 4.0 * a * c;
		float bias = 0.001;
		if(delta >= 0.0){
			float t = (- b - sqrt(delta)) / (2.0 * a);
			if(hit.t > t && t > bias){
				foundHit = true;
				hit.t = t;
				hit.position = rp + rd * hit.t;
				hit.mtl = spheres[i].mtl;
				hit.normal = normalize(hit.position - sc);
			}
		}
	}
	return foundHit;
}

// Given a ray, returns the shaded color where the ray intersects a sphere.
// If the ray does not hit a sphere, returns the environment color.
vec4 RayTracer( Ray ray )
{
	HitInfo hit;
	if ( IntersectRay( hit, ray ) ) {
		vec3 view = normalize( -ray.dir );
		vec3 clr = Shade( hit.mtl, hit.position, hit.normal, view );
		// Compute reflections
		vec3 k_s = hit.mtl.k_s;
		for ( int bounce=0; bounce<MAX_BOUNCES; ++bounce ) {
			if ( bounce >= bounceLimit ) break;
			if ( hit.mtl.k_s.r + hit.mtl.k_s.g + hit.mtl.k_s.b <= 0.0 ) break;
			
			HitInfo h;	// reflection hit info
			Ray r;	// this is the reflection ray
			r.dir = normalize(2.0 * (dot(view, hit.normal)) * hit.normal - view);
			r.pos = hit.position;
			
			
			if ( IntersectRay( h, r ) ) {
				view = normalize( -r.dir );
				clr += k_s * Shade( h.mtl, h.position, h.normal, view );
				k_s *= h.mtl.k_s;
				ray = r;
				hit = h;
			} else {
				// The refleciton ray did not intersect with anything,
				// so we are using the environment color
				clr += k_s * textureCube( envMap, r.dir.xzy ).rgb;
				break;	// no more reflections
			}
		}
		return vec4( clr, 1 );	// return the accumulated color, including the reflections
	} else {
		return vec4( textureCube( envMap, ray.dir.xzy ).rgb, 0 );	// return the environment color
	}
}
`;
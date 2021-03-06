#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int is_spot;
uniform float angle;

uniform vec3 object_color;
layout(location = 0) out vec4 out_color;

void main()
{
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * max(dot(normalize(world_normal), L), 0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
	}

	// TODO: compute light
	float light = 0;

	if (is_spot > 0)
	{
		float rads = radians(angle);
		float spotlight = dot(-L, light_direction);
		float spot_light_limit = cos(rads);

		if (spotlight > spot_light_limit)
		{
			float linear_att = (spotlight - spot_light_limit) / (1.0f - spot_light_limit);
			float light_fact = linear_att * linear_att;
			light = ambient_light + light_fact * (diffuse_light + specular_light);
		} else
		{
			light = ambient_light;
		}
	} else
	{
		float dist = distance(light_position, world_position);
		float att_fact = 1.0f / max (dist * dist, 1.0f);
		light = ambient_light + att_fact * (diffuse_light + specular_light);
	}
	// TODO: write pixel out color
	vec3 color = object_color * light;
	out_color = vec4(color, 1.0f);
}
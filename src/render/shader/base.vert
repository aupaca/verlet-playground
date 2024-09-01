#version 320 es 

layout (location = 0) in vec2 a_vertPos;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in vec2 a_ballCenterPos;
layout (location = 3) in int a_ballColorId;
layout (location = 4) in float a_ballRadius;

out vec4 v_color;
out vec2 v_texCoord;

uniform mat4 u_proj;
uniform vec4 u_primaryColor;
uniform vec4 u_secondaryColor;

void main()
{
	vec2 pos = a_vertPos * a_ballRadius + a_ballCenterPos;
	gl_Position = u_proj * vec4(pos, 0.0, 1.0);
	v_texCoord = a_texCoord;
	if (a_ballColorId == 1)
	{
	    v_color = u_primaryColor;
	}
	else
	{
	    v_color = u_secondaryColor;
	}
}


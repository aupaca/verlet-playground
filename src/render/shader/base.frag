#version 320 es 

precision mediump float;

in vec4 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture;

out vec4 outColor;

void main()
{
	vec4 texColor = texture(u_texture, v_texCoord);
	if (texColor.a < 0.1)
		discard;
	outColor = texColor * v_color;
}

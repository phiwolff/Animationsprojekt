#version 330 core

in vec4 interp_color;
in vec3 interp_normal;
// hier wird uniform varibale verwendet um die Richtung,
// aus der das Licht kommt, zu definieren
uniform vec3 light_dir;

out vec4 frag_color;


void main()
{
    float light = dot(interp_normal, light_dir);
	// x, y in clamp(light, x, y) geben an wie hell Objekt ist
    frag_color = clamp(light, 0.5, 1.0) * interp_color;
	//frag_color = interp_color;
}

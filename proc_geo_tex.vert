#version 330 core
layout (location = 0) in vec3 position;
// vec4 color braucht einen buffer
// ich habe hier die locations von color und normal vertauscht !!!
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

out vec4 interp_color;
out vec3 interp_normal;

void main()
{
    gl_Position = proj_mat * view_mat * model_mat * vec4(position.x, position.y, position.z, 1.0);
    interp_color = color;
    interp_normal = normalize((transpose(inverse(model_mat)) * vec4(normal, 0.0)).xyz);
}



// anscheinend muss dem für die textur dem vertex shader die anzahl der koordinaten der geometrie
// übergeben werden, die die geometrie enthält damit das image(textur) auf die geometrie gesampelt
// werden kann. der vertex shader gibt das an den fragmentshader weiter, der die werte dazwischen interpoliert
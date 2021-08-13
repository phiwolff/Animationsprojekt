

#version 330 core

layout (location = 0) in vec3 position;

out vec4 variColor;

void main()
{	

    gl_Position = vec4(position.x, position.y, position.z, 1.0);

	variColor = vec4(0.f, 0.f, 0.f, 0.f);

	if (position.x == -0.5 && position.y ==-0.5){
		variColor = vec4(1.f, 1.f, 1.f, 1.f); 
	}

	if (position.x == -0.3 && position.y == -0.5){
			variColor = vec4(1.f, 1.f, 1.f, 1.f);
	}

	if (position.x == -0.1 && position.y == -0.5){
			variColor = vec4(1.f, 1.f, 1.f, 1.f);
	}

	if (position.x == 0.1 && position.y == -0.5){
			variColor = vec4(1.f, 1.f, 1.f, 1.f);
	}

	if (position.x == 0.3 && position.y == -0.5){
			variColor = vec4(1.f, 1.f, 1.f, 1.f);
	}

	if (position.x == 0.5 && position.y == -0.5){
			variColor = vec4(1.f, 1.f, 1.f, 1.f);
	}



}








#include "OnionBase.h"

#include <fstream>

bool Onion::glfwInitted = false;
bool Onion::glewInitted = false;

bool Onion::onionInitGlfw(int antiAliasing, int glVersion)
{
    if (!glfwInitted)
    {
        fprintf(stdout,"Initting GLFW");
        if(!glfwInit())
        {
            fprintf(stderr,"No GLFW for you");
            return false;
        }


        glfwWindowHint(GLFW_SAMPLES, antiAliasing);

        fprintf (stdout, "%d.%d\n",glVersion / 10, glVersion % 10);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersion / 10);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersion % 10);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Not the old opengl

        glfwInitted = true;
    }
    return true;
}

bool Onion::onionInitGlew()
{
    if (!glewInitted)
    {
        fprintf(stdout,"Initting GLEW\n");
        glewExperimental = true;
        if (glewInit() != GLEW_OK)
        {
            fprintf(stderr,"No GLEW for you");
            return false;
        }
        glewInitted = true;
    }
    return true;
}

GLfloat Onion::onionGoodFOV(float monitorSize, float monitorDistance)
{
    return 2 * glm::atan(monitorSize/2, monitorDistance);
}

GLuint Onion::onionLoadShaders(std::string vertexFilename, std::string fragmentFilename)
{
    const char * vertex_file_path = vertexFilename.c_str();
    const char * fragment_file_path = fragmentFilename.c_str();

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
    {
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
    {
		printf("%s is not here. The fuck?\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open())
    {
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Vertex shader: %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
    {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Fragment shader: %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
    {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
    {
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void Onion::transform(GLfloat*& points, glm::mat4 trans, int numVertices)
{
    GLfloat* buffer = new GLfloat[4*numVertices];
    for (int i = 0; i < numVertices; ++i)
    {
        buffer[4*i] = points[3*i];
        buffer[4*i+1] = points[3*i+1];
        buffer[4*i+2] = points[3*i+2];
        buffer[4*i+3] = 1;
    }

    for (int c = 0; c < numVertices; ++c)
    {
        for (int r = 0; r < 4; ++r)
        {
            float sum = 0;
            for (int i = 0; i < 4; ++i)
            {
                sum += trans[i][r] * points[4*c+i];
            }
            buffer[4*c+r] = sum;
        }
    }

    delete points;
    points = buffer;
}

void Onion::translate(GLfloat*& points, GLfloat dx, GLfloat dy, GLfloat dz, int numVertices)
{
    for (int i = 0; i < 3 * numVertices; i += 3)
    {
        points[i] += dx;
        points[i+1] += dy;
        points[i+2] += dz;
    }
}

void Onion::rotate(GLfloat*& points, GLfloat angle, GLfloat axisX, GLfloat axisY, GLfloat axisZ, int numVertices)
{
    transform(points, glm::rotate(angle, glm::vec3(axisX,axisY,axisZ)), numVertices);
}


GLfloat Onion::cubicBezier(GLfloat x0, GLfloat x1, GLfloat x2, GLfloat x3, GLfloat t)
{
    GLfloat finalValue = 0;
    finalValue += (1-t) * (1-t) * (1-t) * x0;
    finalValue += 3 * (1-t) * (1-t) * t * x1;
    finalValue += 3 * (1-t) * t * t * x2;
    finalValue += t * t * t * x3;

    return finalValue;
}

#include <Game.h>

static bool flip;

Game::Game(sf::ContextSettings settings) : window(sf::VideoMode(800, 600), "OpenGL Cube Vertex and Fragment shaders", sf::Style::Default, settings)
{
}

Game::~Game() {}

void Game::run()
{
	initialize();
	sf::Event event;
	while (isRunning)
	{
		#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
		#endif
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				isRunning = false;
			}
			switch (event.key.code)
			{
			case sf::Keyboard::Q:
				m_scale = Matrix3::scale(1.1, 1.1, 1.1);
				applyTransformation(m_scale);
				break;
			case sf::Keyboard::E:
				m_scale = Matrix3::scale(0.9, 0.9, 0.9);
				applyTransformation(m_scale);
				break;
			case sf::Keyboard::Left:
				m_rotate = Matrix3::rotation(Matrix3::Axis::Y, -3);
				applyTransformation(m_rotate);
				break;
			case sf::Keyboard::Right:
				m_rotate = Matrix3::rotation(Matrix3::Axis::Y, 3);
				applyTransformation(m_rotate);
				break;
			case sf::Keyboard::W:
				translate(0.1, Matrix3::Axis::Y);
				break;
			case sf::Keyboard::S:
				translate(-0.1, Matrix3::Axis::Y);
				break;
			case sf::Keyboard::D:
				translate(0.1, Matrix3::Axis::X);
				break;
			case sf::Keyboard::A:
				translate(-0.1, Matrix3::Axis::X);
				break;
			case sf::Keyboard::R:
				translate(0.1, Matrix3::Axis::Z);
				break;
			case sf::Keyboard::T:
				translate(-0.1, Matrix3::Axis::Z);
				break;
			case sf::Keyboard::Up:
				m_rotate = Matrix3::rotation(Matrix3::Axis::X, -3);
				applyTransformation(m_rotate);
				break;
			case sf::Keyboard::Down:
				m_rotate = Matrix3::rotation(Matrix3::Axis::X, 3);
				applyTransformation(m_rotate);
				break;
			default:
				break;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[4];
	float texel[2];
} Vertex;

Vertex vertex[8];
GLubyte triangles[36];

/* Variable to hold the VBO identifier and shader data */
GLuint	index,		//Index to draw
		vsid,		//Vertex Shader ID
		fsid,		//Fragment Shader ID
		progID,		//Program ID
		vao = 0,	//Vertex Array ID
		vbo[1],		// Vertex Buffer ID
		positionID, //Position ID
		colorID,	// Color ID
		to,			// Texture ID 1 to 32
		textureID,	//Texture ID
		texelID;	// Texel ID

//const string filename = "texture.tga";
//const string filename = "cube.tga";

const std::string filename = "minecraft.tga";

int width; //width of texture
int height; //height of texture
int comp_count; //Component of texture
const int number = 4; //4 = RGBA

unsigned char* img_data;
void Game::initialize()
{
	GLint isCompiled = 0;
	GLint isLinked = 0;
	glEnable(GL_DEPTH_TEST | GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	isRunning = true;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, window.getSize().x / window.getSize().y, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -6.0f);
	glewInit();

	/* Vertices counter-clockwise winding */
	//FRONT
	vertex[0].coordinate[0] = 0.5f;		//tr f
	vertex[0].coordinate[1] = 0.5f;
	vertex[0].coordinate[2] = 0.5f;

	vertex[1].coordinate[0] = -0.5f;	//tl f
	vertex[1].coordinate[1] = 0.5f;
	vertex[1].coordinate[2] = 0.5f;

	vertex[2].coordinate[0] = -0.5f;	//bl f
	vertex[2].coordinate[1] = -0.5f;
	vertex[2].coordinate[2] = 0.5f;

	vertex[3].coordinate[0] = 0.5f;		//br b
	vertex[3].coordinate[1] = -0.5f;
	vertex[3].coordinate[2] = 0.5f;




	//BACK
	vertex[4].coordinate[0] = 0.5f;		//tr b
	vertex[4].coordinate[1] = 0.5f;
	vertex[4].coordinate[2] = -0.5f;

	vertex[5].coordinate[0] = -0.5f;	//tl b
	vertex[5].coordinate[1] = 0.5f;
	vertex[5].coordinate[2] = -0.5f;

	vertex[6].coordinate[0] = -0.5f;	//bl b
	vertex[6].coordinate[1] = -0.5f;
	vertex[6].coordinate[2] = -0.5f;

	vertex[7].coordinate[0] = 0.5f;		//br b
	vertex[7].coordinate[1] = -0.5f;
	vertex[7].coordinate[2] = -0.5f;

	vertex[0].color[0] = 1.0f;		//Nevajo white
	vertex[0].color[1] = 0.870588f;
	vertex[0].color[2] = 0.678431;

	vertex[1].color[0] = 0.254902f; //Royal blue
	vertex[1].color[1] = 0.411765f;
	vertex[1].color[2] = 0.882353;

	vertex[2].color[0] = 1.0f;		//red
	vertex[2].color[1] = 0.0f;
	vertex[2].color[2] = 0.0f;

	vertex[3].color[0] = 1.0f;		//magenta
	vertex[3].color[1] = 0.0f;
	vertex[3].color[2] = 1.0f;

	vertex[4].color[0] = 0.2f;		//Blue
	vertex[4].color[1] = 0.0f;
	vertex[4].color[2] = 1.0f;

	vertex[5].color[0] = 0.0f;		//cyan
	vertex[5].color[1] = 1.0f;
	vertex[5].color[2] = 1.0f;

	vertex[6].color[0] = 0.0f;		//green
	vertex[6].color[1] = 1.0f;
	vertex[6].color[2] = 0.0f;

	vertex[7].color[0] = 1.0f;		//yellow
	vertex[7].color[1] = 1.0f;
	vertex[7].color[2] = 0.0f;

	//TEXELS
	vertex[0].texel[0] = 0.0f;	//tr f
	vertex[0].texel[1] = 0.0f;

	vertex[1].texel[0] = 0.0f;	//tl f
	vertex[1].texel[1] = 0.0f;

	vertex[2].texel[0] = 0.0f;  //bl f
	vertex[2].texel[1] = 0.0f;

	vertex[3].texel[0] = 0.0f;  //br f
	vertex[3].texel[1] = 0.0f;

	vertex[4].texel[0] = 0.0f;  //tr b
	vertex[4].texel[1] = 0.0f;

	vertex[5].texel[0] = 0.0f;  //tl b
	vertex[5].texel[1] = 0.0f;

	vertex[6].texel[0] = 0.0f;  //bl b
	vertex[6].texel[1] = 0.0f;

	vertex[7].texel[0] = 0.0f;  //br b
	vertex[7].texel[1] = 0.0f;

	/*Index of Poly / Triangle to Draw */
	//front
	triangles[0] = 0;   triangles[1] = 1;   triangles[2] = 2;
	triangles[3] = 3;   triangles[4] = 0;   triangles[5] = 2;

	//Top
	triangles[6] = 5;	triangles[7] = 1;	triangles[8] = 0;
	triangles[9] = 5;	triangles[10] = 0;	triangles[11] = 4;

	//Back
	triangles[12] = 6;	triangles[13] = 5;	triangles[14] = 4;
	triangles[15] = 6;	triangles[16] = 4;	triangles[17] = 7;

	//Bottom
	triangles[18] = 2;	triangles[19] = 6;	triangles[20] = 7;
	triangles[21] = 2;	triangles[22] = 7;	triangles[23] = 3;

	//right
	triangles[24] = 7;	triangles[25] = 4;	triangles[26] = 0;
	triangles[27] = 7;	triangles[28] = 0;	triangles[29] = 3;

	//left
	triangles[30] = 2;	triangles[31] = 1;	triangles[32] = 5;
	triangles[33] = 2;	triangles[34] = 5;	triangles[35] = 6;



	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Vertex Shader which would normally be loaded from an external file */

	std::string temp = loadShaderFile("Vertex", fileContentsVertex);
	const char* vs_src = temp.c_str(); //Vertex Shader Src

	/* Vertex Shader which would normally be loaded from an external file */
	//const char* vs_src = "#version 400\n\r"
	//	"in vec4 sv_position;"
	//	"in vec4 sv_color;"
	//	"out vec4 color;"
	//	"void main() {"
	//	"	color = sv_color;"
	//	"	gl_Position = sv_position;"
	//	"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER); //Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL); // Set the shaders source

	glCompileShader(vsid); //Check that the shader compiles

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}
	std::string temp2 = loadShaderFile("Fragment", fileContentsFragment);
	/* Fragment Shader which would normally be loaded from an external file */
	const char* fs_src = temp2.c_str();; //Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);
	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	//Create program in GPU
	glAttachShader(progID, vsid); //Attach Vertex Shader to Program
	glAttachShader(progID, fsid); //Attach Fragment Shader to Program
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);
	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to);
	glBindTexture(GL_TEXTURE_2D, to);
	
	//Wrap around
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Filtering
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Bind to OpenGL
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(GL_TEXTURE_2D, //2D texture image
				 0, //Minmapping Level
				 GL_RGBA, //GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
				 width, //width
				 height, //height
				 0, //Border
				 GL_RGBA, //bitmap
				 GL_UNSIGNED_BYTE, img_data);

	// Find variables in the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
	texelID = glGetAttribLocation(progID, "sv_texel");
	textureID = glGetAttribLocation(progID, "f_texture");
}

void Game::update()
{
	glCullFace(GL_BACK);

	elapsed = clock.getElapsedTime();

	if (elapsed.asSeconds() >= 1.0f)
	{
		clock.restart();

		if (!flip)
		{
			flip = true;
		}
		else
			flip = false;
	}

	if (flip)
	{
		rotationAngle += 0.005f;

		if (rotationAngle > 360.0f)
		{
			rotationAngle -= 360.0f;
		}
	}

	//Change vertex data
	//vertex[0].coordinate[0] += -0.0001f;
	//vertex[0].coordinate[1] += -0.0001f;
	//vertex[0].coordinate[2] += -0.0001f;

	//vertex[1].coordinate[0] += -0.0001f;
	//vertex[1].coordinate[1] += -0.0001f;
	//vertex[1].coordinate[2] += -0.0001f;

	//vertex[2].coordinate[0] += -0.0001f;
	//vertex[2].coordinate[1] += -0.0001f;
	//vertex[2].coordinate[2] += -0.0001f;

#if (DEBUG >= 2)
	DEBUG_MSG("Update up...");
#endif

}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Drawing...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain
		model components that 'are' and 'are not' to be drawn )	*/

	//Set active texture
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);

	// Set pointers for each parameter
	// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(texelID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(texelID);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	window.display();

}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);
	glDeleteBuffers(1, vbo);
	stbi_image_free(img_data);
}
/// <summary>
/// loading the vertex shader details from txt file
/// </summary>
/// <returns></returns>
std::string Game::loadShaderFile(std::string type, std::string contents)
{
	std::ifstream myReadFile;
	std::string string;
	myReadFile.open(type + "Shader.txt");
	while (!myReadFile.eof()) // To get you all the lines.
	{
		getline(myReadFile, string); // Saves the line in STRING.
		contents.append(string + "\n"); // Prints our STRING.
	}
	myReadFile.close();
	std::cout << contents;
	return contents;
}

void Game::applyTransformation(const Matrix3 & transform)
{
	for (unsigned int i = 0u; i < 8; i++)
	{
		Vector3 point = Vector3(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2]);
		point = transform * point;
		vertex[i].coordinate[0] = point.getX();
		vertex[i].coordinate[1] = point.getY();
		vertex[i].coordinate[2] = point.getZ();
	}
}

void Game::translate(float offset, Matrix3::Axis axis)
{
	for (unsigned int i = 0u; i < 8; i++)
	{
		Vector3 point = Vector3(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2]);
		vertex[i].coordinate[0] = Matrix3::translate(point, offset, axis).getX();
		vertex[i].coordinate[1] = Matrix3::translate(point, offset, axis).getY();
		vertex[i].coordinate[2] = Matrix3::translate(point, offset, axis).getZ();
	}
}


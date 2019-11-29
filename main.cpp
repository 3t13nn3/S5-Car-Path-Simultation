/*ETIENNE (づ｡◕‿‿◕｡)づ PENAULT */

#include "Graphe.hpp"
#include "Road.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "shader.hpp"


/*WINDOW SIZE*/
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(0.4f, 0.5f, 5.0f);


int main(int argc, char ** argv){
    /*INIT ARG*/
    int NB_VOITURE     = atoi(argv[2]);
    int NB_POINT       = atoi(argv[1]);

    /*INITIALIZE WINDOW*/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3t13nn3", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    /*INITIALIZE SHADER*/
    Shader lightingShader("Shaders/color.fs", "Shaders/color.vs");

    /*SQUARE VERTICES*/
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    /*CREATING VAO & VBO*/
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);




	srand(time(NULL));
 
    /*CREATING OUR ECOSYSTEM*/
	Road r(NB_POINT, NB_VOITURE);

    /*RECOVERING POINTS AND THERE COORDINATES*/
	std::vector<std::vector<float>> lines = r.GetEdges();
    std::vector<point> points             = r.GetPoints();

	std::vector<GLuint> lineVBOs(lines.size());
	std::vector<GLuint> lineVAOs(lines.size());

	//std::cout << "Ici " << lines[i].size() << std::endl;


	/*FILLING VAO/VBO*/
	for(size_t i = 0; i< lineVBOs.size(); ++i){
		//std::cout << "Ici " << lines[i].size() << std::endl;
		glGenBuffers(1, &lineVBOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, lineVBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, lines[i].size() * sizeof(float), lines[i].data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &lineVAOs[i]);
		glBindVertexArray(lineVAOs[i]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    	glEnableVertexAttribArray(0);
	}

    /*GENERATE RANDOM COLORS FOR EACH LINES*/
	std::vector<glm::vec3> colors;
	for(size_t i = 0; i < lineVAOs.size(); ++i){
		float x = (rand()%651) + 350;
		float y = (rand()%651) + 350;
		float z = (rand()%651) + 350;
		x/=1000;
		y/=1000;
		z/=1000;

		colors.emplace_back(glm::vec3(x,y,z));
	}

    while (!glfwWindowShouldClose(window)){

        /*TIME MANAGEMENT*/
        float currentFrame = glfwGetTime(); 
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*LIGHTING OPTIONS*/
        lightingShader.use();
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        glm::vec3 lightColor(1.0, 1.0 , 1.0);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); 
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        lightingShader.setVec3("light.ambient", ambientColor);
        lightingShader.setVec3("light.diffuse", diffuseColor);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        lightingShader.setVec3("material.ambient", 0.0215f, 0.1745f, 0.0215f);
        lightingShader.setVec3("material.diffuse", 0.07568f, 0.61424f, 0.07568f);
        lightingShader.setVec3("material.specular", 0.633f, 0.727811,0.633f);
        lightingShader.setFloat("material.shininess", 0.6f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);


        glm::mat4 model = glm::mat4(1.0f);

        /*ROTATE OUR GRAPH*/
        float angle = (float)glfwGetTime();
        model = glm::rotate(model, glm::radians(angle*5), glm::vec3(0.5f, 0.7f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5));
        glm::mat4 rot = model;
        lightingShader.setMat4("model", model);
        
        /*SHAPE WE WANT TO USE*/
	    glBindVertexArray(cubeVAO);
        
        /*DRAWING OUR NODES, GREEN CUBES*/
        for(size_t i =0; i < points.size(); ++i){
            glm::mat4 model = rot;
            model = glm::translate(model, points[i]);
            //scaling our nodes size in fuction of the number of nodes into our graph
            model = glm::scale(model, glm::vec3(0.275/log(NB_POINT)));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);

        }


        /*DRAWING EACH OF OUR CARS, BLUE/RED/ORANGE CUBES*/
        for(size_t i =0; i < NB_VOITURE; ++i){
            //resetting our matrices to be at the right place
            glm::mat4 model = rot;
            glm::vec3 diffuseColor;


            /*SETTING THE COLOR OF OUR CAR BY IT STATE*/
            if(r.GetCarState(i) == 4) {
                diffuseColor = glm::vec3(1.0f, 0.5, 0.0); 
            } else if(r.GetCarState(i) > 2 && r.GetCarState(i) < 4){
                diffuseColor = glm::vec3(1.0f, 0.0, 0.0); 
            } else {
                diffuseColor = glm::vec3(0.0f, 0.0f, 0.75f); 
            }
                glm::vec3 ambientColor = diffuseColor * glm::vec3(1.5f);
                lightingShader.setVec3("light.ambient", ambientColor);
                lightingShader.setVec3("material.ambient", 1.0f, 0.75f, 1.0f);
                lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
                lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
                lightingShader.setFloat("material.shininess", 32.0f);
            

            /*RECOVERING DATA WE NEED FROM OU GRAPH FOR TRANSITION LIKE POSITIONS, TIME, FUTUR NODES*/
            std::vector<int> car_nodes = r.GetCarNodes();
            std::vector<int> next_nodes = r.GetNextNodes();
            std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> starts = r.GetStarts();
            std::vector<int> to_wait = r.GetWaitTimes();

            /*CALCULATING CURRENT POSITION OF OUR CURRENT CAR IN FUNCTION OF TIME AND DEPARTUR/ARRIVAL NODES*/
            int tmp(std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now()-starts[i]).count());
            
        #ifdef __APPLE__
            tmp(std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - starts[i]).count());
        #endif

            float ratio = (float)tmp/to_wait[i];
            point a = points[car_nodes[i]] * (1 - ratio);
            point b = points[next_nodes[i]] * (ratio);
            point final = a + b;
            
            /*AT THE BEGINNING, SOME CAR WILL NOTE MOVE THEN WE NEED TO HANDLE THIS SITUATION BY CHECKING THE TRAVEL TIME*/
            if(to_wait[i] == 0)
                model = glm::translate(model, points[car_nodes[i]]);
            else
                model = glm::translate(model, final);

            //scaling our nodes size in fuction of the number of nodes into our graph
            model = glm::scale(model, glm::vec3(0.3/log(NB_POINT)));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        
        //resetting our matrices to be at the right place
        model = rot;
        lightingShader.setMat4("model", model);

        /*DRAWING LINES*/
        for(size_t i = 0; i< lineVAOs.size(); ++i){
        	//std::cout << "x: " << colors[i].x << "y: " << colors[i].y<< "z: " << colors[i].z << std::endl;

            //Using selected colors we have generate before to got some readability :)
	        glm::vec3 diffuseColor = (colors[i]) * glm::vec3(1.5f); 
	        glm::vec3 ambientColor = diffuseColor * glm::vec3(1.f); // low influence
	        lightingShader.setVec3("light.ambient", ambientColor);
	        lightingShader.setVec3("material.ambient", colors[i]);
	        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
	        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
	        lightingShader.setFloat("material.shininess", 32.0f);
	        glBindVertexArray(lineVAOs[i]);
	        glDrawArrays(GL_LINES, 0, 2);
	    }

        lightingShader.setMat4("model", model);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*FREE*/
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    for(size_t i = 0; i< lineVAOs.size(); ++i){
		glDeleteVertexArrays(1, &lineVAOs[i]);
    	glDeleteBuffers(1, &lineVBOs[i]);
	}

    glfwTerminate();

    /*TRICKY EXIT BECAUSE PROGRAM WILL CONTINUE IF WE JUST QUIT THE WINDOWS*/
    /*MAYBE THERE IS A SIGNAL TO WARN EVERY THREAD THAT WINDOW IS COLSED ??*/
    exit(0);
    r.Join();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
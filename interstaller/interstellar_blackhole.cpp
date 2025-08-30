#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

// Window dimensions
const unsigned int WIDTH = 1200, HEIGHT = 800;

// Black hole properties
glm::vec2 blackHolePos(WIDTH / 2, HEIGHT / 2);
float blackHoleRadius = 60.0f;
float accretionDiskRadius = 150.0f;
float innerDiskRadius = 80.0f;
bool isDragging = false;

// Star structure
struct Star {
    glm::vec2 position;
    float size;
    float brightness;
};

// Particle structures
struct AccretionParticle {
    float angle;
    float distance;
    float speed;
    float size;
    glm::vec3 color;
    float brightness;
};

struct PhotonParticle {
    int ring;
    float angle;
    float radius;
    float size;
};

// Containers
std::vector<Star> stars;
std::vector<AccretionParticle> accretionParticles;
std::vector<PhotonParticle> photonParticles;

// Shader sources
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 ourColor;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * vec4(aPos.x, aPos.y, 0.0, 1.0);
        ourColor = aColor;
    }
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)glsl";

// Function declarations
void initStars();
void initAccretionParticles();
void initPhotonParticles();
void updateAccretionParticles();
void updatePhotonParticles();
void renderScene();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Interstellar Black Hole - C++/OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Build and compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Initialize data
    initStars();
    initAccretionParticles();
    initPhotonParticles();
    
    // Set up projection matrix
    glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
    
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        if (isDragging) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            blackHolePos.x = xpos;
            blackHolePos.y = ypos;
        }
        
        // Update particles
        updateAccretionParticles();
        updatePhotonParticles();
        
        // Render
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        renderScene();
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void initStars() {
    stars.resize(400);
    std::srand(std::time(0));
    
    for (auto& star : stars) {
        star.position = glm::vec2(std::rand() % WIDTH, std::rand() % HEIGHT);
        star.size = (std::rand() % 25 + 5) / 10.0f;
        star.brightness = std::rand() % 155 + 100;
    }
}

void initAccretionParticles() {
    accretionParticles.resize(600);
    
    for (auto& particle : accretionParticles) {
        particle.angle = (std::rand() % 360) * 3.14159f / 180.0f;
        particle.distance = innerDiskRadius + std::rand() % (int)(accretionDiskRadius - innerDiskRadius);
        particle.speed = 0.02f * (60.0f / particle.distance);
        particle.size = (std::rand() % 25 + 15) / 10.0f;
        
        // Doppler effect
        float dopplerFactor = std::cos(particle.angle);
        if (dopplerFactor > 0) {
            // Approaching side - blue shift
            particle.color = glm::vec3(
                0.6f + 0.4f * dopplerFactor,
                0.6f + 0.4f * dopplerFactor,
                1.0f
            );
            particle.brightness = 0.8f + 0.2f * dopplerFactor;
        } else {
            // Receding side - red shift
            particle.color = glm::vec3(
                1.0f,
                0.6f + 0.4f * std::abs(dopplerFactor),
                0.6f + 0.4f * std::abs(dopplerFactor)
            );
            particle.brightness = 0.8f + 0.2f * std::abs(dopplerFactor);
        }
    }
}

void initPhotonParticles() {
    photonParticles.resize(300);
    int index = 0;
    
    for (int ring = 0; ring < 3; ring++) {
        float ringRadius = blackHoleRadius * (1.5f + ring * 0.5f);
        for (int i = 0; i < 100; i++) {
            photonParticles[index].ring = ring;
            photonParticles[index].angle = (std::rand() % 360) * 3.14159f / 180.0f;
            photonParticles[index].radius = ringRadius;
            photonParticles[index].size = (std::rand() % 10 + 5) / 10.0f;
            index++;
        }
    }
}

void updateAccretionParticles() {
    for (auto& particle : accretionParticles) {
        particle.angle += particle.speed;
        
        // Adjust particle distance based on black hole size
        if (particle.distance > accretionDiskRadius) {
            particle.distance = accretionDiskRadius;
        } else if (particle.distance < innerDiskRadius) {
            particle.distance = innerDiskRadius;
        }
    }
}

void updatePhotonParticles() {
    for (auto& particle : photonParticles) {
        particle.angle += 0.01f / (particle.ring + 1);
    }
}

void renderScene() {
    // Render stars with gravitational lensing
    std::vector<float> starVertices;
    std::vector<float> starColors;
    
    for (const auto& star : stars) {
        // Calculate distance to black hole
        float dx = star.position.x - blackHolePos.x;
        float dy = star.position.y - blackHolePos.y;
        float distance = std::sqrt(dx*dx + dy*dy);
        
        glm::vec2 pos = star.position;
        float size = star.size;
        float brightness = star.brightness / 255.0f;
        
        // Gravitational lensing effect
        if (distance < blackHoleRadius * 4) {
            float distortion = 1.0f - (distance / (blackHoleRadius * 4.0f));
            pos.x += dx * distortion * 0.7f;
            pos.y += dy * distortion * 0.7f;
            brightness = std::min(1.0f, brightness + distortion * 0.6f);
            size += distortion * 2;
        }
        
        // Create a quad for the star
        float halfSize = size / 2.0f;
        starVertices.insert(starVertices.end(), {
            pos.x - halfSize, pos.y - halfSize,
            pos.x + halfSize, pos.y - halfSize,
            pos.x - halfSize, pos.y + halfSize,
            pos.x + halfSize, pos.y + halfSize,
            pos.x - halfSize, pos.y + halfSize,
            pos.x + halfSize, pos.y - halfSize
        });
        
        for (int i = 0; i < 6; i++) {
            starColors.insert(starColors.end(), {brightness, brightness, brightness});
        }
    }
    
    // Render accretion disk particles
    std::vector<float> accretionVertices;
    std::vector<float> accretionColors;
    
    for (const auto& particle : accretionParticles) {
        glm::vec2 pos = blackHolePos + glm::vec2(
            std::cos(particle.angle) * particle.distance,
            std::sin(particle.angle) * particle.distance
        );
        
        float halfSize = particle.size / 2.0f;
        accretionVertices.insert(accretionVertices.end(), {
            pos.x - halfSize, pos.y - halfSize,
            pos.x + halfSize, pos.y - halfSize,
            pos.x - halfSize, pos.y + halfSize,
            pos.x + halfSize, pos.y + halfSize,
            pos.x - halfSize, pos.y + halfSize,
            pos.x + halfSize, pos.y - halfSize
        });
        
        glm::vec3 color = particle.color * particle.brightness;
        for (int i = 0; i < 6; i++) {
            accretionColors.insert(accretionColors.end(), {color.r, color.g, color.b});
        }
    }
    
    // Render photon ring particles
    std::vector<float> photonVertices;
    std::vector<float> photonColors;
    
    for (const auto& particle : photonParticles) {
        glm::vec2 pos = blackHolePos + glm::vec2(
            std::cos(particle.angle) * particle.radius,
            std::sin(particle.angle) * particle.radius
        );
        
        float halfSize = particle.size / 2.0f;
        photonVertices.insert(photonVertices.end(), {
            pos.x - halfSize, pos.y - halfSize,
            pos.x + halfSize, pos.y - halfSize,
            pos.x - halfSize, pos.y + halfSize,
            pos.x + halfSize, pos.y + halfSize,
            pos.x - halfSize, pos.y + halfSize,
            pos.x + halfSize, pos.y - halfSize
        });
        
        for (int i = 0; i < 6; i++) {
            photonColors.insert(photonColors.end(), {0.8f, 0.86f, 1.0f});
        }
    }
    
    // Draw event horizon (black hole)
    std::vector<float> bhVertices;
    std::vector<float> bhColors;
    
    int segments = 64;
    for (int i = 0; i < segments; i++) {
        float angle1 = (i / (float)segments) * 2.0f * 3.14159f;
        float angle2 = ((i + 1) / (float)segments) * 2.0f * 3.14159f;
        
        bhVertices.insert(bhVertices.end(), {
            blackHolePos.x, blackHolePos.y,
            blackHolePos.x + std::cos(angle1) * blackHoleRadius, blackHolePos.y + std::sin(angle1) * blackHoleRadius,
            blackHolePos.x + std::cos(angle2) * blackHoleRadius, blackHolePos.y + std::sin(angle2) * blackHoleRadius
        });
        
        for (int j = 0; j < 3; j++) {
            bhColors.insert(bhColors.end(), {0.0f, 0.0f, 0.0f});
        }
    }
    
    // Draw gravitational lensing effect
    std::vector<float> lensingVertices;
    std::vector<float> lensingColors;
    
    for (int i = 0; i < 20; i++) {
        float radius = blackHoleRadius * 1.5f + i * 3.0f;
        for (int j = 0; j < segments; j++) {
            float angle1 = (j / (float)segments) * 2.0f * 3.14159f;
            float angle2 = ((j + 1) / (float)segments) * 2.0f * 3.14159f;
            
            lensingVertices.insert(lensingVertices.end(), {
                blackHolePos.x + std::cos(angle1) * radius, blackHolePos.y + std::sin(angle1) * radius,
                blackHolePos.x + std::cos(angle2) * radius, blackHolePos.y + std::sin(angle2) * radius
            });
            
            float intensity = (20.0f - i) / 20.0f;
            lensingColors.insert(lensingColors.end(), {0.2f * intensity, 0.2f * intensity, 0.47f * intensity});
            lensingColors.insert(lensingColors.end(), {0.2f * intensity, 0.2f * intensity, 0.47f * intensity});
        }
    }
    
    // Render all elements
    unsigned int VBO, VAO, colorVBO;
    
    // Stars
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &colorVBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, starVertices.size() * sizeof(float), starVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, starColors.size() * sizeof(float), starColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    
    glDrawArrays(GL_TRIANGLES, 0, starVertices.size() / 2);
    
    // Accretion disk
    std::vector<float> combinedVertices = accretionVertices;
    combinedVertices.insert(combinedVertices.end(), photonVertices.begin(), photonVertices.end());
    
    std::vector<float> combinedColors = accretionColors;
    combinedColors.insert(combinedColors.end(), photonColors.begin(), photonColors.end());
    
    glBufferData(GL_ARRAY_BUFFER, combinedVertices.size() * sizeof(float), combinedVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, combinedColors.size() * sizeof(float), combinedColors.data(), GL_STATIC_DRAW);
    
    glDrawArrays(GL_TRIANGLES, 0, combinedVertices.size() / 2);
    
    // Lensing effect
    glBufferData(GL_ARRAY_BUFFER, lensingVertices.size() * sizeof(float), lensingVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, lensingColors.size() * sizeof(float), lensingColors.data(), GL_STATIC_DRAW);
    
    glDrawArrays(GL_LINES, 0, lensingVertices.size() / 2);
    
    // Black hole
    glBufferData(GL_ARRAY_BUFFER, bhVertices.size() * sizeof(float), bhVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, bhColors.size() * sizeof(float), bhColors.data(), GL_STATIC_DRAW);
    
    glDrawArrays(GL_TRIANGLES, 0, bhVertices.size() / 2);
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &colorVBO);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            float distance = std::sqrt(
                (xpos - blackHolePos.x) * (xpos - blackHolePos.x) + 
                (ypos - blackHolePos.y) * (ypos - blackHolePos.y)
            );
            if (distance <= blackHoleRadius) {
                isDragging = true;
            }
        } else if (action == GLFW_RELEASE) {
            isDragging = false;
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    blackHoleRadius += yoffset * 5.0f;
    if (blackHoleRadius < 20.0f) blackHoleRadius = 20.0f;
    if (blackHoleRadius > 120.0f) blackHoleRadius = 120.0f;
    
    accretionDiskRadius = blackHoleRadius * 2.5f;
    innerDiskRadius = blackHoleRadius * 1.3f;
    
    // Reinitialize particles for new size
    initAccretionParticles();
    initPhotonParticles();
}

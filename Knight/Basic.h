#define BASIC_H
#define STB_IMAGE_IMPLEMENTATION

#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include"Shader.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include<sstream>

float modulus(float value)
{
    if (value >= 0)
        return value;

    if (value < 0)
        return -value;

}
float distance(glm::vec3 a, glm::vec3 b)
{
    float c;
    c = sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z)));
    return c;
};
float distance(glm::vec4 a, glm::vec4 b)
{
    float c;
    c = sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z)));
    return c;
};

class objecttexture
{
public:
    unsigned int texture;
    int width, height, nrchannels;
    unsigned char* image;
    objecttexture(const char imagename[], int imagetype /* 1->PNG , 2->JPG*/)
    {
        int imgt;
        if (imagetype == 1)
            imgt = GL_RGBA;
        else if (imagetype == 2)
            imgt = GL_RGB;


        stbi_set_flip_vertically_on_load(1);
        image = stbi_load(imagename, &width, &height, &nrchannels, 0);


        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        glTexImage2D(GL_TEXTURE_2D, 0, imgt, width, height, 0, imgt, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        stbi_image_free(image);
    }
    objecttexture()
    {

    };
};

class objectspace
{
public:
    unsigned int vao, vbo, ebo;
    Shader shader;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 viewtranslate = glm::mat4(1.0f);
    glm::mat4 viewscale = glm::mat4(1.0f);
    glm::mat4 viewrotate = glm::mat4(1.0f);
    glm::mat4 transform = glm::mat4(1.0f);
    float vertex1[20];
    glm::vec4 center, length, center1,length1;
    float speed = 1.3f;
    float nsprites = 10;
    glm::vec3 viewtranslatevec = glm::vec3(0.0f); //used to store the data in the file for changing the center
    glm::vec4 newmodelscale = glm::vec4(1.0f); //used to store the data in the file for changing the size of sprite
    int ontop =0;
    int onside = 0;
    int contact = 0;

    void intitialize(float vertices[], unsigned int indices[], const char* vertexshader = "bg.vs", const char* fragmentshader = "bg.fs",int texdirection =1)
    {

        shader = Shader(vertexshader, fragmentshader);

        
        unsigned int indices1[6];
        for (int i = 0; i <= 19; i++)
        {
            vertex1[i] = vertices[i];
        }
        for (int i = 0; i <= 5; i++)
        {
            indices1[i] = indices[i];
        }

        for (int i = 0; i <= 19; i = i + 5)
        {
            center.x += vertex1[i];
            center.y += vertex1[i + 1];
            center.z += vertex1[i + 2];
        }
        center.x = center.x / 4;
        center.y = center.y / 4;
        center.z = center.z / 4;
        center.w = 1.0f;

        center1 = center;

        length.x = 2*modulus(center.x - vertex1[0]);
        length.y = 2*modulus(center.y - vertex1[0]);
        length.z = 2*modulus(center.z - vertex1[0]);
        length.w = 1.0f;

        length1 = length;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex1), vertex1, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("viewtranslate", viewtranslate);
        shader.setMat4("viewscale", viewscale);
        shader.setMat4("viewrotate", viewrotate);

        shader.setMat4("transform", transform);
        shader.setInt("texdirection", texdirection);
        shader.setFloat("addy", -10.0f);

    }
    
    void setmodel(float scalefactor, glm::vec3 translate, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotateaxis = glm::vec3(0.0f, 0.0f, 1.0f), float rotateangle = 0)
    {
        
        center = glm::translate(model,translate) * center;
        center1 = center;
        model = glm::translate(model, translate);
        
        model = glm::rotate(model, glm::radians(rotateangle), rotateaxis);
        if (scalefactor == 0)
        {
            model = glm::scale(model, scale);
            length = glm::scale(glm::mat4(1.0f), scale) * length;
        }
        else
        {
            model = glm::scale(model, glm::vec3(scalefactor));
            length = scalefactor * length;
        }
        length1 = length;
        shader.use();
        shader.setMat4("model", model);

    }
    
    void changemodel(float scalefactor, glm::vec3 translate, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotateaxis = glm::vec3(0.0f, 0.0f, 1.0f), float rotateangle = 0)
    {
        model = glm::translate(model, translate);
        newmodelscale= glm::vec4(scale, 1.0f)*newmodelscale;

        length1 = length *newmodelscale;

        model = glm::rotate(model, glm::radians(rotateangle), rotateaxis);
        if (scalefactor == 0)
            model = glm::scale(model, scale);
        else
            model = glm::scale(model, glm::vec3(scalefactor));

        shader.use();
        shader.setMat4("model", model);

    }
    
    void setview(float scalefactor, glm::vec3 translate, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotateaxis = glm::vec3(0.0f, 0.0f, 1.0f), float rotateangle = 0)
    {
        viewtranslatevec += translate;
        
        viewtranslate = glm::translate(viewtranslate, translate);
        center1 =viewtranslate*center;
        viewrotate = glm::rotate(viewrotate, glm::radians(rotateangle), rotateaxis);
        if (scalefactor == 0)
        {
            viewscale = glm::scale(viewscale, scale);
            length = glm::scale(glm::mat4(1.0f), scale) * length;
        }
        else
        {
            viewscale = glm::scale(viewscale, glm::vec3(scalefactor));
            length = scalefactor * length;
        }
        shader.use();
        shader.setMat4("viewtranslate", viewtranslate);
        shader.setMat4("viewscale", viewscale);

            shader.setMat4("viewrotate", viewrotate);
    }
    
    void drawquad()
    {
        shader.use();
        shader.setInt("texture1", 0);
        shader.setMat4("transform", transform);

       
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
    void writedata(const char *spritename,std::ofstream *spritedatawrite,int arrayno =-1)
    {
        glm::mat4 temp;
        float tempa[16];

        glm::mat4 view = viewtranslate * viewscale * viewrotate;
        temp = view*model;
        for (int i = 0; i <= 3; i++)
        {
            for (int j = 0; j <= 3; j++)
            {
                tempa[(i * 4) + j] = temp[i][j];
            }

        }
        if (arrayno == -1)
            *spritedatawrite << spritename << "\n";

        else
            *spritedatawrite << spritename << arrayno << "\n";

        for (int i = 0; i <= 15; i++)
        {
            *spritedatawrite << tempa[i] << "\t";
        };

        *spritedatawrite << "\n";

        
        *spritedatawrite << viewtranslatevec.x << "\t";
        *spritedatawrite << viewtranslatevec.y << "\t";
        *spritedatawrite << viewtranslatevec.z << "\t";

        *spritedatawrite << "\n";

        *spritedatawrite << newmodelscale.x << "\t";
        *spritedatawrite << newmodelscale.y << "\t";
        *spritedatawrite << newmodelscale.z;

        *spritedatawrite << "\n";
    }
    
    void readdata(const char *spritename,std::ifstream *file,int arrayno =-1)
    {
        float tempa2[16];
        glm::mat4 temp2;
        glm::vec4 temptranslate = glm::vec4(0.0f);
        glm::vec4 tempscale = glm::vec4(1.0f);

        std::string name;
        std::string line;
        std::stringstream geek;
            
        if (arrayno == -1)
            name = spritename;
        else
            name = spritename + std::to_string(arrayno);


        for (int i = 1; i > 0; i++)
        {
            std::getline(*file, line);

            if (line == name)
            {
                break;
            };
        }
        std::getline(*file, line);
        geek = std::stringstream(line);
        for (int i = 0; i <= 15; i++)
        {
            geek >> tempa2[i];
        }

        for (int i = 0; i <= 3; i++)
        {
            for (int j = 0; j <= 3; j++)
            {
                temp2[i][j] = tempa2[(i * 4) + j];
            }

        };
        model = glm::mat4(1.0f);
        model = temp2;
        shader.use();
        shader.setMat4("model", model);

        std::getline(*file, line);
        geek = std::stringstream(line);

        geek >> temptranslate.x;
        
        geek >> temptranslate.y;

        geek >> temptranslate.z;


        center1 += temptranslate;
        center = center1;
        viewtranslatevec = glm::vec3(temptranslate);

        std::getline(*file, line);
        
        geek = std::stringstream(line);

        geek >> tempscale.x;

        geek >> tempscale.y;

        geek >> tempscale.z;

        length1 *= tempscale;
        //length = length1;
        newmodelscale = tempscale;
    }
    
    void settranform(float scalefactor, glm::vec3 translate, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotateaxis = glm::vec3(0.0f, 0.0f, 1.0f), float rotateangle = 360)
    {

        transform = glm::translate(transform, translate);
        center1 = glm::vec4(translate, 1.0f) + center1;

        transform = glm::rotate(transform, glm::radians(rotateangle), rotateaxis);
        if (scalefactor == 0)
            transform = glm::scale(transform, scale);
        else
            transform = glm::scale(transform, glm::vec3(scalefactor));
    }
    
    void deinitialize()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(shader.ID);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};

void mousetranslatesprite(GLFWwindow* window, objectspace *sprite, int* changecursor)
{
    GLFWcursor* shift = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    
    int windowx, windowy;
    double xmouse, ymouse;

    glfwGetCursorPos(window, &xmouse, &ymouse);

    glfwGetWindowSize(window, &windowx, &windowy);

    xmouse = (2 * xmouse / windowx) - 1;
    ymouse = -((2 * (ymouse) / windowy) - 1);


    if (distance(glm::vec4(xmouse, ymouse, 0.0f, 1.0f), sprite->center1) <= 0.1f)
    {
        glfwSetCursor(window, shift);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            sprite->setview(0, glm::vec3(xmouse, ymouse, 0.0f) - glm::vec3(sprite->center1.x, sprite->center1.y, sprite->center1.z));
        }
        *changecursor = *changecursor +1;
    }
}

void mouseresizesprite(GLFWwindow* window,objectspace* sprite,int* changecursor)
{
    int windowx, windowy;
    double xmouse, ymouse;
    objectspace temp;
    GLFWcursor* Hresize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);

    GLFWcursor* Vresize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);


    glfwGetCursorPos(window, &xmouse, &ymouse);

    glfwGetWindowSize(window, &windowx, &windowy);

    xmouse = (2 * xmouse / windowx) - 1;
    ymouse = -((2 * (ymouse) / windowy) - 1);

    temp = *sprite;

    if (((modulus(xmouse - temp.center1.x) > (0.45 * temp.length1.x)) && (modulus(xmouse - temp.center1.x) < (0.55 * temp.length1.x)) && modulus(ymouse - temp.center1.y) < (0.5 * temp.length1.y)) || ((modulus(ymouse - temp.center1.y) > (0.45 * temp.length1.y)) && (modulus(ymouse - temp.center1.y) < (0.55 * temp.length1.y)) && (modulus(xmouse - temp.center1.x) < (0.5 * temp.length1.x))))
    {
        if (modulus(xmouse - temp.center1.x) > 0.45 * temp.length1.x)
        {
            glfwSetCursor(window, Hresize);
            *changecursor = *changecursor + 1;
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                sprite->changemodel(0, glm::vec3(0.0f), glm::vec3(1.05 * (modulus(xmouse - temp.center1.x) / (temp.length1.x / 2)), 1.0f, 1.0f));

            }
        }
        else if (modulus(ymouse - temp.center1.y) > 0.45 * temp.length1.y)
        {
            glfwSetCursor(window, Vresize);
            *changecursor = *changecursor +1;

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                sprite->changemodel(0, glm::vec3(0.0f), glm::vec3(1.0f, 1.05 * (modulus(ymouse - temp.center1.y) / (temp.length1.y / 2)), 1.0f));
            }
        }
    }
}


#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include"Shader.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include"Basic.h"
#include<fstream>

int scrwidth = 1900;
int scrheight = 1080;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);
}
 
class Movement
{
public:
    
    //related to jumping
    int jumpframeno = 0;
    int jumping = 0;
    double jumptime = 0, jumptime1 = 0;
    double jumpspeedy = 0.35f;
    double jumpspeedx = 0.4f;
    float g = 0.1f;
    glm::vec3 posbeforejump = glm::vec3(0.0f);
    float groundposy = -1.0f;

    //related to attack
    int attacking = 0;
    double attacktime = 0;
    unsigned int attackframeno = 0;

    //related to walking
    int walking = 0;
    float walkspeed = 0.25f;

    //related to running
    int running = 0;
    float runspeed = 0.4f;

    //related to falling
    int falling = 0;
    double falltime = 0 , falltime1 = 0;

    //related to frames
    int FPS = 120;
    int framenumber = 1;
    float add;
    double framestart, frameend, deltatime = 0;
    
    //related to idling
    int idling = 0;

    int gamestart = 0;
    int playerdirectionx = 1;
    

    void feedframedata(int framenumber1,double deltatime1)
    {
        framenumber = framenumber1;
        deltatime = deltatime1;
    }

    bool idles(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture knightidles)
    {
        
        if (jumptime == 0 && attacktime == 0)
        {
            if (idling == 0)
                *knighttex = knightidles;
   

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            {
                gamestart = 1;
                idling = 1;
                knight->shader.use();
                add = (1 / knight->nsprites) * (int)((framenumber * knight->speed * knight->nsprites) / FPS);
               
                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", 0.0f);
                knight->shader.setInt("texdirection", 1);
                knight->settranform(0.0f, glm::vec3(walkspeed*deltatime, 0.0f, 0.0f));

                walking = 1;
                playerdirectionx = 1;

            }
            else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            {
                gamestart = 1;
                idling = 1;
                knight->shader.use();
                add = (1 / knight->nsprites) * (int)((framenumber * knight->speed * knight->nsprites) / FPS);

                knight->shader.setFloat("add", -add);
                knight->shader.setFloat("addy", 0.0f);
                knight->shader.setInt("texdirection", -1);
                knight->settranform(0.0f, glm::vec3(-walkspeed * deltatime , 0.0f, 0.0f));

                walking = 1;
                playerdirectionx = -1;
            }
            else
                walking = 0;

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            {
                gamestart = 1;
                idling = 1;
                knight->shader.use();
                add = (1 / knight->nsprites) * (int)((framenumber * knight->speed * knight->nsprites) / FPS);
                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", -2.0f);
                knight->shader.setInt("texdirection", 1);
                knight->settranform(0.0f, glm::vec3(runspeed*deltatime, 0.0f, 0.0f));
                running = 1;

                playerdirectionx = 1;


            }
            else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            {
                gamestart = 1;
                idling = 1;
                knight->shader.use();
                add = (1 / knight->nsprites) * (int)((framenumber * knight->speed * knight->nsprites) / FPS);
                knight->shader.setFloat("add", -add);
                knight->shader.setInt("texdirection", -1);
                knight->shader.setFloat("addy", -2.0f);
                knight->settranform(0.0f, glm::vec3(-runspeed*deltatime, 0.0f, 0.0f));
                running = 1;
                playerdirectionx = -1;

            }
            else
                running = 0;


            if (walking == 0 && running == 0)
            {
                idling = 1;
                knight->shader.use();
                add = (1 / knight->nsprites) * (int)((framenumber * knight->speed * knight->nsprites) / FPS);
                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", -1.0f);
                knight->shader.setInt("texdirection", playerdirectionx);
            }
            else
                return 0;
        }

    }

    void jump(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture* knightjump)
    {
        if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && jumping == 0) || jumping != 0)
        {
            gamestart = 1;
            idling = 0;
            if (jumping == 0)
            {
                jumpframeno = 0;
                jumptime = 0;
                posbeforejump = knight->center1;
            }
            if (jumpframeno == FPS / knight->speed)
                jumpframeno = 1;



            jumptime1 = jumptime;
            jumptime += 7 * deltatime;

            *knighttex = *knightjump;

            if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && jumping == 0) || jumping == 1)
            {

                jumping = 1;
                knight->shader.use();
                add = 0.1f;

                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", -10.0f);
                knight->shader.setInt("texdirection", 1);

                knight->settranform(0, glm::vec3(jumpspeedx*deltatime, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }
            else if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && jumping == 0) || jumping == 2)
            {
                jumping = 2;
                knight->shader.use();
                add = 0.1f;
                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", -10.0f);
                knight->shader.setInt("texdirection", -1);

                knight->settranform(0, glm::vec3(-jumpspeedx * deltatime , ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }

            else if (jumping == 0||jumping ==3)
            {
                jumping = 3;
                knight->shader.use();
                add = 0.5f;
                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", -10.0f);

                knight->settranform(0, glm::vec3(0.0f, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }

            if (knight->center1.y <= (groundposy+ knight->length1.y/2))
            {
                knight->settranform(0, glm::vec3(0.0f, ((groundposy + knight->length1.y / 2) - knight->center1.y), 0.0f));
                knight->center1.y = (groundposy + knight->length1.y / 2);
                jumping = 0;
                jumptime = 0;
                jumpframeno = 0;
            }

            jumpframeno++;
        }


    }

    void attack(GLFWwindow* window,objectspace *knight,objecttexture *knighttex,objecttexture *knightattack)
    {
        if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && attacking==0) || attacking==1)
        {
            gamestart = 0;
            idling = 0;
            if (attacking==0)
            {
                attacktime = 0;
                attackframeno = 0;
            }
            attackframeno++;
            attacktime += deltatime;

            attacking = 1;
            *knighttex = *knightattack;
            knight->shader.use();
            add = (1 / knight->nsprites) * (int)((attackframeno * knight->speed * knight->nsprites) / FPS);
            if (playerdirectionx == -1)
                add = 1 - add;

            knight->shader.setFloat("add", add);
            knight->shader.setFloat("addy", -10.0f);
            knight->shader.setInt("texdirection", playerdirectionx);

            if (attacktime >= 0.72f)
            {
                attacking = 0;
                attacktime = 0;
            }
        }

    }

    void fall(objectspace* knight, objectspace* box)
    {
        if (box->ontop == 1 ||falling ==1)
        {
            if (glm::distance(knight->center1.x, box->center1.x) >= (box->length1.x / 2 + knight->length1.x / 2))
            {
                if (falling == 0)
                {
                    falltime = 0;
                    falltime1 = 0;

                }
                falltime1 = falltime;
                falltime += deltatime;
                
                falling = 1;
                knight->settranform(0, glm::vec3(0, -5.0 * g * ((falltime*falltime) - (falltime1*falltime1)), 0));

                if (knight->center1.y <= (groundposy + knight->length1.y / 2))
                {
                    knight->settranform(0, glm::vec3(0.0f, ((groundposy + knight->length1.y / 2) - knight->center1.y), 0.0f));
                    knight->center1.y = (groundposy + knight->length1.y / 2);
                    falling = 0;
                    falltime = 0;
                    falltime1 = 0;
                    box->ontop = 0;
                }
            }
              
        }

    }

    void sidecollisionsolid(objectspace *knight, objectspace *box)
    {
        float distance =0;
        if (glm::distance(knight->center1.x, box->center1.x) <= (knight->length1.x + box->length1.x) / 2 && glm::distance(knight->center1.y, box->center1.y) <= (knight->length1.y + box->length1.y) / 2)
        {

            if (walking == 1)
                distance = walkspeed * deltatime;

            else if (running == 1)
                distance = runspeed * deltatime;

            else if (jumping == 1)
                distance = jumpspeedx * deltatime;

            if (jumping != 0)
                jumpspeedx = 0;


            if (playerdirectionx == 1)
                knight->settranform(0, glm::vec3(-distance, 0.0f, 0.0f));

            else if (playerdirectionx == -1)
                knight->settranform(0, glm::vec3(distance, 0.0f, 0.0f));  

            running = 0;
            walking = 0;
        }
        else
            jumpspeedx = 0.4f;
    }

    void topcollisionsolid(objectspace* knight, objectspace *box)
    {
        if (jumping != 0 || falling!=0)
        {
            if (glm::distance(knight->center1.x, box->center1.x) <= (knight->length1.x + box->length1.x) / 2 && glm::distance(knight->center1.y, box->center1.y) <= (knight->length1.y + box->length1.y) / 2 && knight->center1.y > box->center1.y)
            {
                if(jumping!=0)
                    knight->settranform(0, glm::vec3(0.0f, -((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

                else if(falling!=0)
                    knight->settranform(0, glm::vec3(0.0f, (5.0 * g * ((falltime * falltime) - (falltime1*falltime1))), 0.0f));


                box->ontop = 1;

                falling = 0;
                jumping = 0;
                jumptime = 0;
                jumptime1 = 0;
                falltime = 0;
                falltime1 = 0;
            }

            else
                box->ontop = 0;
        }

    }
};

class all
{
public:
    float objvertices[100] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // top left 
    };
    unsigned int indices[10] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    float bgvertices[100] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   15.f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,    15.f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,    -15.f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,    -15.f, 1.0f  // top left 
    };
    float spritevertices[100] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   0.1f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,    0.1f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // top left 
    };

    objectspace bg;

    objectspace knight;

    objectspace container;

    objectspace container2;

    objectspace temp;

    objecttexture bgtex = objecttexture("textures/grass.png", 1);

    objecttexture knighttex;

    objecttexture knightattack = objecttexture("textures/attack.png", 1);

    objecttexture knightjump = objecttexture("textures/jump.png", 1);

    objecttexture containertex = objecttexture("textures/container.jpg", 2);

    objecttexture knightidles = objecttexture("textures/idles.png", 1);


    //related to the window
    int windowx, windowy;
    double xmouse, ymouse;
    
    //related to background moving
    float slidebgspeed = 0.01f;

    //general
    int changecursor = 0;

    //related to frames
    int FPS = 120;
    int framenumber = 1;
    float add;
    double framestart, frameend, deltatime = 0;
    float spriteanimationspeed = 1.3f; 
    
    GLFWcursor* shift = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    GLFWcursor* Hresize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);

    GLFWcursor* Vresize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

    Movement knightmovement;


    void transform()
    {
        framenumber++;
        if (framenumber == FPS / spriteanimationspeed)
            framenumber = 1;

    }

    void initialize()
    {
        bg.intitialize(bgvertices, indices, "shaders/bg.vs", "shaders/bg.fs");
        bg.setmodel(0, glm::vec3(0.0f, -0.75f, 0.0f), glm::vec3(20.0f, 0.5f, 1.0f));

        knight.intitialize(spritevertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        knight.setmodel(0, glm::vec3(0.5f, -0.7f, 0.0f), glm::vec3(0.5f));
        knighttex = knightidles;
        knight.shader.setFloat("addy", 0.0f);
        
        container.intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container.setmodel(0, glm::vec3(0.5f, 0.7f, 0.0f), glm::vec3(0.5f));

        container2.intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container2.setmodel(0, glm::vec3(0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

    }

    void mousetranslate(GLFWwindow* window)
    {
        glfwGetCursorPos(window, &xmouse, &ymouse);

        glfwGetWindowSize(window, &windowx, &windowy);

        xmouse = (2 * xmouse / windowx) - 1;
        ymouse = -((2 * (ymouse) / windowy) - 1);


        if (distance(glm::vec4(xmouse, ymouse, 0.0f, 1.0f), knight.center1) <= 0.1f)
        {
            glfwSetCursor(window, shift);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                knight.setview(0, glm::vec3(xmouse, ymouse, 0.0f) - glm::vec3(knight.center1.x, knight.center1.y, knight.center1.z));
            }
            changecursor++;
        }

        else if (distance(glm::vec4(xmouse, ymouse, 0.0f, 1.0f), container.center1) <= 0.1f)
        {
            glfwSetCursor(window, shift);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                container.setview(0, glm::vec3(xmouse, ymouse, 0.0f) - glm::vec3(container.center1.x, container.center1.y, container.center1.z));
            }
            changecursor++;
        }

        else if (distance(glm::vec4(xmouse, ymouse, 0.0f, 1.0f), container2.center1) <= 0.1f)
        {
            glfwSetCursor(window, shift);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                container2.setview(0, glm::vec3(xmouse, ymouse, 0.0f) - glm::vec3(container2.center1.x, container2.center1.y, container2.center1.z));
            }
            changecursor++;
        }

    }

    void mouseresize(GLFWwindow* window)
    {
        glfwGetCursorPos(window, &xmouse, &ymouse);

        glfwGetWindowSize(window, &windowx, &windowy);

        xmouse = (2 * xmouse / windowx) - 1;
        ymouse = -((2 * (ymouse) / windowy) - 1);

        temp = container;
        
        if (((modulus(xmouse - temp.center1.x) >(0.45*temp.length1.x)) &&(modulus(xmouse - temp.center1.x)<(0.55*temp.length1.x))&& modulus(ymouse - temp.center1.y) <(0.5*temp.length1.y)) || ((modulus(ymouse - temp.center1.y) > (0.45 * temp.length1.y)) && (modulus(ymouse - temp.center1.y) < (0.55 * temp.length1.y)) && (modulus(xmouse - temp.center1.x) <(0.5*temp.length1.x))))
        {
            if (modulus(xmouse - temp.center1.x) > 0.45 * temp.length1.x)
            {
                glfwSetCursor(window, Hresize);
                changecursor++;
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    container.changemodel(0, glm::vec3(0.0f), glm::vec3(1.05 * (modulus(xmouse - temp.center1.x) / (temp.length1.x / 2)), 1.0f, 1.0f));
                
                }
            }
            else if (modulus(ymouse - temp.center1.y) > 0.45 * temp.length1.y)
            {
                glfwSetCursor(window, Vresize);
                changecursor++;

                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    container.changemodel(0, glm::vec3(0.0f), glm::vec3(1.0f,1.05 * (modulus(ymouse - temp.center1.y) / (temp.length1.y / 2)), 1.0f));
                }
            }
        }

        temp = container2;

        if (((modulus(xmouse - temp.center1.x) > (0.45 * temp.length1.x)) && (modulus(xmouse - temp.center1.x) < (0.55 * temp.length1.x)) && modulus(ymouse - temp.center1.y) < (0.5 * temp.length1.y)) || ((modulus(ymouse - temp.center1.y) > (0.45 * temp.length1.y)) && (modulus(ymouse - temp.center1.y) < (0.55 * temp.length1.y)) && (modulus(xmouse - temp.center1.x) < (0.5 * temp.length1.x))))
        {
            if (modulus(xmouse - temp.center1.x) > 0.45 * temp.length1.x)
            {
                glfwSetCursor(window, Hresize);
                changecursor++;
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    container2.changemodel(0, glm::vec3(0.0f), glm::vec3(1.05 * (modulus(xmouse - temp.center1.x) / (temp.length1.x / 2)), 1.0f, 1.0f));

                }
            }
            else if (modulus(ymouse - temp.center1.y) > 0.45 * temp.length1.y)
            {
                glfwSetCursor(window, Vresize);
                changecursor++;

                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    container2.changemodel(0, glm::vec3(0.0f), glm::vec3(1.0f, 1.05 * (modulus(ymouse - temp.center1.y) / (temp.length1.y / 2)), 1.0f));
                }
            }
        }

        temp = knight;
        if (((modulus(xmouse - temp.center1.x) > (0.45 * temp.length1.x)) && (modulus(xmouse - temp.center1.x) < (0.55 * temp.length1.x)) && modulus(ymouse - temp.center1.y) < (0.5 * temp.length1.y)) || ((modulus(ymouse - temp.center1.y) > (0.45 * temp.length1.y)) && (modulus(ymouse - temp.center1.y) < (0.55 * temp.length1.y)) && (modulus(xmouse - temp.center1.x) < (0.5 * temp.length1.x))))
        {
            if (modulus(xmouse - temp.center1.x) > 0.45 * temp.length1.x)
            {
                glfwSetCursor(window, Hresize);
                changecursor++;
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    knight.changemodel(0, glm::vec3(0.0f), glm::vec3(1.05 * (modulus(xmouse - temp.center1.x) / (temp.length1.x / 2)), 1.0f, 1.0f));

                }
            }
            else if (modulus(ymouse - temp.center1.y) > 0.45 * temp.length1.y)
            {
                glfwSetCursor(window, Vresize);
                changecursor++;

                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    knight.changemodel(0, glm::vec3(0.0f), glm::vec3(1.0f, 1.05 * (modulus(ymouse - temp.center1.y) / (temp.length1.y / 2)), 1.0f));

                }
            }
        }

    }

    void writematrix()
    {
        std::ofstream file("modeldata.txt");

        knight.writedata("knight", &file);

        container.writedata("cont", &file);

        container2.writedata("cont2", &file);

        file.close();
    }

    void getmatrix()
    {
        std::ifstream file2("modeldata.txt");

        knight.readdata("knight", &file2);

        container.readdata("cont", &file2);

        container2.readdata("cont2", &file2);

        file2.close();
    
    }
    
    void slidebackground(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            bg.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
            knight.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
            container.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
            container2.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
        
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            bg.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
            knight.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
            container.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
            container2.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));

        }
        
    }

    void processmovement(GLFWwindow* window)
    {
        
        knightmovement.feedframedata(framenumber, deltatime);
        
        knightmovement.topcollisionsolid(&knight, &container);
        knightmovement.topcollisionsolid(&knight, &container2);
        
        knightmovement.jump(window, &knight, &knighttex, &knightjump);

        knightmovement.fall(&knight, &container);
        knightmovement.fall(&knight, &container2);


        knightmovement.attack(window, &knight, &knighttex, &knightattack);
        
        knightmovement.idles(window, &knight, &knighttex, knightidles);

        knightmovement.sidecollisionsolid(&knight, &container);
        knightmovement.sidecollisionsolid(&knight, &container2);

    }

    void draw()
    {
        
        knight.drawquad(knighttex);


        container.drawquad(containertex);

        container2.drawquad(containertex);

        bg.drawquad(bgtex);

    }

    void deleteall()
    {
        bg.deinitialize();
        knight.deinitialize();
        container.deinitialize();
        container2.deinitialize();
    }
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(scrwidth, scrheight, "A Knight's Tale", glfwGetPrimaryMonitor(), NULL);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    all game;
    game.initialize();

    game.getmatrix();

    while (glfwWindowShouldClose(window) != 1)
    {
        game.framestart = glfwGetTime();
        game.changecursor = 0;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS&&game.knightmovement.gamestart ==0) 
          //Change Position Mode only before moving any character
        {
            game.mouseresize(window);
            game.mousetranslate(window);
            
            if (game.changecursor == 0)
                glfwSetCursor(window, NULL);
        }
        else //normal mode
        {
            game.transform();
            game.processmovement(window);

            glfwSetCursor(window, NULL);
        }

        game.slidebackground(window);

        game.draw();

        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        glfwPollEvents();

        game.frameend = glfwGetTime();
        game.deltatime = game.frameend - game.framestart;
    };

    game.writematrix();

    glfwTerminate();
    return 0;
}
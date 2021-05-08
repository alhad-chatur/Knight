
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
    float groundposy = -0.95f;

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
    double falltime = 0, falltime1 = 0;

    //related to frames
    int FPS = 120;
    int framenumber = 1;
    float add;
    double framestart, frameend, deltatime = 0;

    //related to idling
    int idling = 0;

    int gamestart = 0;
    int playerdirectionx = 1;


    void feedframedata(int framenumber1, double deltatime1)
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


            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            {
                gamestart = 1;
                idling = 1;
                knight->shader.use();
                add = (1 / knight->nsprites) * (int)((framenumber * knight->speed * knight->nsprites) / FPS);

                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", -0.0f);
                knight->shader.setInt("texdirection", 1);
                knight->settranform(0.0f, glm::vec3(walkspeed * deltatime, 0.0f, 0.0f));

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
                knight->shader.setFloat("addy", -0.0f);
                knight->shader.setInt("texdirection", -1);
                knight->settranform(0.0f, glm::vec3(-walkspeed * deltatime, 0.0f, 0.0f));

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
                knight->settranform(0.0f, glm::vec3(runspeed * deltatime, 0.0f, 0.0f));
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
                knight->settranform(0.0f, glm::vec3(-runspeed * deltatime, 0.0f, 0.0f));
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
        if (((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && jumping == 0) || jumping != 0) && falling == 0)
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

                knight->settranform(0, glm::vec3(jumpspeedx * deltatime, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }
            else if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && jumping == 0) || jumping == 2)
            {
                jumping = 2;
                knight->shader.use();
                add = 0.1f;
                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", -10.0f);
                knight->shader.setInt("texdirection", -1);

                knight->settranform(0, glm::vec3(-jumpspeedx * deltatime, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }

            else if (jumping == 0 || jumping == 3)
            {
                jumping = 3;
                knight->shader.use();
                add = 0.5f;
                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", -10.0f);

                knight->settranform(0, glm::vec3(0.0f, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }

            if (knight->center1.y <= (groundposy + knight->length1.y / 2))
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

    void attack(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture* knightattack)
    {
        if (((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && attacking == 0) || attacking == 1) && gamestart ==1)
        {
            idling = 0;
            if (attacking == 0)
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

    void fall(objectspace* knight)
    {
        if ( falling == 1)
        {
            if (falling == 0)
            {
                falltime = 0;
                falltime1 = 0;

            }
            falltime1 = falltime;
            falltime += deltatime;

            falling = 1;
            knight->settranform(0, glm::vec3(0, -20.0 * g * ((falltime * falltime) - (falltime1 * falltime1)), 0));

            if (knight->center1.y <= (groundposy + knight->length1.y / 2))
            {
                knight->settranform(0, glm::vec3(0.0f, ((groundposy + knight->length1.y / 2) - knight->center1.y), 0.0f));
                knight->center1.y = (groundposy + knight->length1.y / 2);
                falling = 0;
                falltime = 0;
                falltime1 = 0;
            }


        };

    }

    void sidecollisionsolid(objectspace *knight, objectspace *box)
    {
        float distance =0;
        if (glm::distance(knight->center1.x, box->center1.x) <= (knight->length1.x + box->length1.x) / 2 && glm::distance(knight->center1.y, box->center1.y) <= (knight->length1.y + box->length1.y) / 2 && box->ontop ==0)
        {
            box->onside = 1;

            if (walking == 1)
                distance = walkspeed * deltatime;

            else if (running == 1)
                distance = runspeed * deltatime;

            else if (jumping == 1 || jumping ==2)
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

        {
            box->onside = 0;
            jumpspeedx = 0.4f;
        }
    }

    void topcollisionsolid(objectspace* knight, objectspace *box)
    {
        if (jumping != 0 || falling!=0)
        {
            if (glm::distance(knight->center1.x, box->center1.x) <= (knight->length1.x + box->length1.x) / 2 && glm::distance(knight->center1.y, box->center1.y) <= (knight->length1.y + box->length1.y) / 2 && knight->center1.y > (box->center1.y + box->length1.y/2))
            {
                if(jumping!=0)
                    knight->settranform(0, glm::vec3(0.0f, -((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

                else if(falling!=0)
                    knight->settranform(0, glm::vec3(0.0f, (20.0 * g * ((falltime * falltime) - (falltime1*falltime1))), 0.0f));


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
        else if((glm::distance(knight->center1.x, box->center1.x) >= (box->length1.x / 2 + knight->length1.x / 2) && falling == 0) && box->ontop==1)
        {
            falling = 1;
            falltime = 0;
            falltime1 = 0;
            
        }

    }

    void bottomcollisionsolid(objectspace* knight, objectspace* box)
    {
        if (jumping != 0 &&falling ==0)
        {
            if (glm::distance(knight->center1.x, box->center1.x) <= (knight->length1.x + box->length1.x) / 2 && glm::distance(knight->center1.y, box->center1.y) <= (knight->length1.y + box->length1.y) / 2 && knight->center1.y < box->center1.y)
            {
                jumping = 0;
                jumptime = 0;
                jumptime1 = 0;
                falling = 1;
                falltime = 0;
                falltime1 = 0;
            }


       }


    }

    void collectiblecollision(objectspace* knight, objectspace* coin)
    {
        if (glm::distance(knight->center1.x, coin->center1.x) <= (knight->length1.x + coin->length1.x) / 2 && glm::distance(knight->center1.y, coin->center1.y) <= (knight->length1.y + coin->length1.y) / 2 && coin->contact ==0)
        {
            coin->contact = 1;
        }
    }
};

class Level0
{
public:

    objectspace bg;

    objectspace knight;

    objectspace container[3];

    int containerno = 3; //when adding new containers do uncomment the game.getmatrix() function for one time

    objectspace coins[4];

    int coinnumber = 4;
    
    objectspace temp;

    objecttexture bgtex = objecttexture("textures/grass.png", 1);

    objecttexture knighttex;

    objecttexture knightattack = objecttexture("textures/attack.png", 1);

    objecttexture knightjump = objecttexture("textures/jump.png", 1);

    objecttexture containertex = objecttexture("textures/container.jpg", 2);

    objecttexture knightidles = objecttexture("textures/idles.png", 1);

    objecttexture cointex = objecttexture("textures/coins.png", 1);


    GLFWcursor* shift = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    GLFWcursor* Hresize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);

    GLFWcursor* Vresize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);


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
        
        container[0].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container[0].setmodel(0, glm::vec3(0.5f, 0.7f, 0.0f), glm::vec3(0.5f));

        container[1].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container[1].setmodel(0, glm::vec3(0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        container[2].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container[2].setmodel(0, glm::vec3(-0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

       
        for (int i = 0; i < coinnumber; i++)
        {
            coins[i].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
            coins[i].setmodel(0.1f, glm::vec3(-0.7f, 0.7f, 0.0f));
        }
        

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
        else
        {
            for (int i = 0; i < containerno; i++)
            {
                if (distance(glm::vec4(xmouse, ymouse, 0.0f, 1.0f), container[i].center1) <= 0.1f)
                {
                    glfwSetCursor(window, shift);
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                    {
                        container[i].setview(0, glm::vec3(xmouse, ymouse, 0.0f) - glm::vec3(container[i].center1.x, container[i].center1.y, container[i].center1.z));
                    }
                    changecursor++;
                    break;
                }
            }
            for (int i = 0; i < coinnumber; i++)
            {
                if (distance(glm::vec4(xmouse, ymouse, 0.0f, 1.0f), coins[i].center1) <= 0.1f)
                {
                    glfwSetCursor(window, shift);
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                    {
                        coins[i].setview(0, glm::vec3(xmouse, ymouse, 0.0f) - glm::vec3(coins[i].center1.x, coins[i].center1.y, coins[i].center1.z));
                    }
                    changecursor++;
                    break;
                }
            }


        }
    }

    void mouseresize(GLFWwindow* window)
    {
        glfwGetCursorPos(window, &xmouse, &ymouse);

        glfwGetWindowSize(window, &windowx, &windowy);

        xmouse = (2 * xmouse / windowx) - 1;
        ymouse = -((2 * (ymouse) / windowy) - 1);

        for (int i = 0; i < containerno; i++)
        {
            temp = container[i];

            if (((modulus(xmouse - temp.center1.x) > (0.45 * temp.length1.x)) && (modulus(xmouse - temp.center1.x) < (0.55 * temp.length1.x)) && modulus(ymouse - temp.center1.y) < (0.5 * temp.length1.y)) || ((modulus(ymouse - temp.center1.y) > (0.45 * temp.length1.y)) && (modulus(ymouse - temp.center1.y) < (0.55 * temp.length1.y)) && (modulus(xmouse - temp.center1.x) < (0.5 * temp.length1.x))))
            {
                if (modulus(xmouse - temp.center1.x) > 0.45 * temp.length1.x)
                {
                    glfwSetCursor(window, Hresize);
                    changecursor++;
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                    {
                        container[i].changemodel(0, glm::vec3(0.0f), glm::vec3(1.05 * (modulus(xmouse - temp.center1.x) / (temp.length1.x / 2)), 1.0f, 1.0f));

                    }
                }
                else if (modulus(ymouse - temp.center1.y) > 0.45 * temp.length1.y)
                {
                    glfwSetCursor(window, Vresize);
                    changecursor++;

                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                    {
                        container[i].changemodel(0, glm::vec3(0.0f), glm::vec3(1.0f, 1.05 * (modulus(ymouse - temp.center1.y) / (temp.length1.y / 2)), 1.0f));
                    }
                }
                break;
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

    void writematrix(const char* filename)
    {
        std::ofstream file(filename);

        knight.writedata("knight", &file);

        for (int i = 0; i < containerno; i++)
        {
            container[i].writedata("cont", &file,i);
        }

        for (int i = 0; i < coinnumber; i++)
        {
            coins[i].writedata("coin", &file, i);
        }

        file.close();
    }

    void getmatrix(const char* filename)
    {
        std::ifstream file2(filename);

        knight.readdata("knight", &file2);

        for (int i = 0; i < containerno; i++)
        {

            container[i].readdata("cont", &file2, i);
        }
        for (int i = 0; i < coinnumber; i++)
        {
          coins[i].readdata("coin", &file2, i);
        }

        file2.close();
    
    }
    
    void slidebackground(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            if (knightmovement.gamestart == 1)
            {
                bg.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                knight.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));

                for (int i = 0; i < containerno; i++)
                {
                    container[i].settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                  coins[i].settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                }

            }
            else
            {

                bg.setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                knight.setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                for (int i = 0; i < containerno; i++)
                {
                    container[i].setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                }

            }
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            if (knightmovement.gamestart == 1)
            {
                bg.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                knight.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                for (int i = 0; i < containerno; i++)
                {
                    container[i].settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                }
            }
            else
            {

                bg.setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                knight.setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                for (int i = 0; i < containerno; i++)
                {
                    container[i].setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                }

            }
        }
        
    }

    void processmovement(GLFWwindow* window)
    {
        //knight
        knightmovement.feedframedata(framenumber, deltatime);

        for (int i = 0; i < containerno; i++)
        {
            knightmovement.topcollisionsolid(&knight, &container[i]);
        }
        for (int i = 0; i < containerno; i++)
        {
            knightmovement.bottomcollisionsolid(&knight, &container[i]);
        }

        knightmovement.jump(window, &knight, &knighttex, &knightjump);

        knightmovement.fall(&knight);

        knightmovement.attack(window, &knight, &knighttex, &knightattack);
        
        knightmovement.idles(window, &knight, &knighttex, knightidles);

        for (int i = 0; i < containerno; i++)
        {
            knightmovement.sidecollisionsolid(&knight, &container[i]);
        }

        for(int i =0;i<coinnumber;i++)
        knightmovement.collectiblecollision(&knight, &coins[i]);

        //coins

    }

    void draw()
    {
        
        knight.drawquad(knighttex);

        for(int i=0;i<containerno;i++)
        container[i].drawquad(containertex);

        for (int i = 0; i < coinnumber; i++)
        {
            if (coins[i].contact == 0)
                coins[i].drawquad(cointex);
        }
        
        bg.drawquad(bgtex);

    }

    void deleteall()
    {
        bg.deinitialize();
        knight.deinitialize();
        
        for(int i=0;i<containerno;i++)
        container[i].deinitialize();
    }
};

class Menu
{
public:

    GLFWcursor* shift = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

    GLFWcursor* Hresize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);

    GLFWcursor* Vresize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);


    objectspace bg;

    objectspace container[3];

    int containerno = 3; //when adding new containers do uncomment the game.getmatrix() function for one time

    objectspace pointer;

    objectspace temp;

    objecttexture bgtex = objecttexture("textures/grass.png", 1);

    objecttexture containertex = objecttexture("textures/container.jpg", 2);

    objecttexture starttex = objecttexture("textures/start.png", 1); //container[0]

    objecttexture optionstex = objecttexture("textures/options.png", 1); //container[1]

    objecttexture exittex = objecttexture("textures/exit.png", 1); //container[2]

    objecttexture arrowtex = objecttexture("textures/arrow.png", 1); //container[containerno-1]

    //related to the window
    int windowx, windowy;
    double xmouse, ymouse;

    //general
    int changecursor = 0;

    //related to frames
    int FPS = 120;
    int framenumber = 1;
    float add;
    double framestart, frameend, deltatime = 0;
    float spriteanimationspeed = 1.3f;

    //related to pointer
    int pointerpos = 0;
    float pointertime = 0;
    float pointerspeed = 0.18f;


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

        container[0].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container[0].setmodel(0, glm::vec3(0.5f, 0.7f, 0.0f), glm::vec3(0.5f));

        container[1].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container[1].setmodel(0, glm::vec3(0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        container[2].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container[2].setmodel(0, glm::vec3(-0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        pointer.intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        pointer.setmodel(0, glm::vec3(-0.5f, 0.7f, 0.0f), glm::vec3(0.25f));
    }

    void mousetranslate(GLFWwindow* window)
    {
        glfwGetCursorPos(window, &xmouse, &ymouse);

        glfwGetWindowSize(window, &windowx, &windowy);

        xmouse = (2 * xmouse / windowx) - 1;
        ymouse = -((2 * (ymouse) / windowy) - 1);

            for (int i = 0; i < containerno; i++)
            {
                if (distance(glm::vec4(xmouse, ymouse, 0.0f, 1.0f), container[i].center1) <= 0.05f)
                {
                    glfwSetCursor(window, shift);
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                    {
                        container[i].setview(0, glm::vec3(xmouse, ymouse, 0.0f) - glm::vec3(container[i].center1.x, container[i].center1.y, container[i].center1.z));
                    }
                    changecursor++;
                    break;
                }
            }
            if (distance(glm::vec4(xmouse, ymouse, 0.0f, 1.0f), pointer.center1) <= 0.05f)
            {
                glfwSetCursor(window, shift);
                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    pointer.setview(0, glm::vec3(xmouse, ymouse, 0.0f) - glm::vec3(pointer.center1.x,pointer.center1.y, pointer.center1.z));
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

        for (int i = 0; i < containerno; i++)
        {
            temp = container[i];

            if (((modulus(xmouse - temp.center1.x) > (0.45 * temp.length1.x)) && (modulus(xmouse - temp.center1.x) < (0.55 * temp.length1.x)) && modulus(ymouse - temp.center1.y) < (0.5 * temp.length1.y)) || ((modulus(ymouse - temp.center1.y) > (0.45 * temp.length1.y)) && (modulus(ymouse - temp.center1.y) < (0.55 * temp.length1.y)) && (modulus(xmouse - temp.center1.x) < (0.5 * temp.length1.x))))
            {
                if (modulus(xmouse - temp.center1.x) > 0.45 * temp.length1.x)
                {
                    glfwSetCursor(window, Hresize);
                    changecursor++;
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                    {
                        container[i].changemodel(0, glm::vec3(0.0f), glm::vec3(1.05 * (modulus(xmouse - temp.center1.x) / (temp.length1.x / 2)), 1.0f, 1.0f));

                    }
                }
                else if (modulus(ymouse - temp.center1.y) > 0.45 * temp.length1.y)
                {
                    glfwSetCursor(window, Vresize);
                    changecursor++;

                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                    {
                        container[i].changemodel(0, glm::vec3(0.0f), glm::vec3(1.0f, 1.05 * (modulus(ymouse - temp.center1.y) / (temp.length1.y / 2)), 1.0f));
                    }
                }
                break;
            }
            temp = pointer;

            if (((modulus(xmouse - temp.center1.x) > (0.45 * temp.length1.x)) && (modulus(xmouse - temp.center1.x) < (0.55 * temp.length1.x)) && modulus(ymouse - temp.center1.y) < (0.5 * temp.length1.y)) || ((modulus(ymouse - temp.center1.y) > (0.45 * temp.length1.y)) && (modulus(ymouse - temp.center1.y) < (0.55 * temp.length1.y)) && (modulus(xmouse - temp.center1.x) < (0.5 * temp.length1.x))))
            {
                if (modulus(xmouse - temp.center1.x) > 0.45 * temp.length1.x)
                {
                    glfwSetCursor(window, Hresize);
                    changecursor++;
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                    {
                        pointer.changemodel(0, glm::vec3(0.0f), glm::vec3(1.05 * (modulus(xmouse - temp.center1.x) / (temp.length1.x / 2)), 1.0f, 1.0f));

                    }
                }
                else if (modulus(ymouse - temp.center1.y) > 0.45 * temp.length1.y)
                {
                    glfwSetCursor(window, Vresize);
                    changecursor++;

                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                    {
                        pointer.changemodel(0, glm::vec3(0.0f), glm::vec3(1.0f, 1.05 * (modulus(ymouse - temp.center1.y) / (temp.length1.y / 2)), 1.0f));
                    }
                }

            }
        }



    }

    void writematrix(const char* filename)
    {
        std::ofstream file(filename);

        for (int i = 0; i < containerno; i++)
        {
            container[i].writedata("cont", &file, i);
        }
        pointer.writedata("pointer", &file);

        file.close();
    }

    void getmatrix(const char* filename)
    {
        std::ifstream file2(filename);

        for (int i = 0; i < containerno; i++)
        {

            container[i].readdata("cont", &file2, i);
        }
        pointer.readdata("pointer", &file2);

        file2.close();

    }

    void togglepointer(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && pointertime ==0)
        {
            pointerpos++;
            pointertime = pointerspeed;
            if (pointerpos > containerno - 1) //2 bcoz last container is pointer itself
                pointerpos = 0;
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && pointertime ==0)
        {
            pointerpos--;
            pointertime = pointerspeed;
            if (pointerpos < 0)
                pointerpos = containerno - 1;
        }
        pointer.setview(0, glm::vec3((container[pointerpos].center1.x - pointer.center1.x) -container[pointerpos].length1.x/2 -0.05f, (container[pointerpos].center1.y - pointer.center1.y), 0));
        pointertime -= deltatime;
        if (pointertime < 0)
            pointertime = 0;
    }

    void pointerfunctions(GLFWwindow* window,int *currentclass)
    {
        pointer.shader.setInt("texdirection", -1);
        
        if (pointerpos == 0 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
            *currentclass = 0;
        
        if (pointerpos == 2 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
            glfwSetWindowShouldClose(window,1);
    }

    void draw()
    {
            container[0].drawquad(starttex);
            container[1].drawquad(optionstex);
            container[2].drawquad(exittex);
            pointer.drawquad(arrowtex);

    }

    void deleteall()
    {
        bg.deinitialize();

        for (int i = 0; i < containerno; i++)
            container[i].deinitialize();
    }

};

class Renderer
{
public:
    Level0 level0; //0
    Menu menu; //1

    int currentclass = 1;

    void preset()
    {
        level0.initialize();
        level0.getmatrix("Levels/level0.txt");
        menu.initialize();
        menu.getmatrix("Levels/menu.txt");

    }

    void loopprocess(GLFWwindow *window)
    {

        if (currentclass == 0)
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                currentclass = 1;

            level0.framestart = glfwGetTime();
            level0.changecursor = 0;

            if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && level0.knightmovement.gamestart == 0)
                //Change Position Mode only before moving any character
            {
                level0.mouseresize(window);
                level0.mousetranslate(window);

                if (level0.changecursor == 0)
                    glfwSetCursor(window, NULL);
            }
            else //normal mode
            {
                level0.transform();
                level0.processmovement(window);

                glfwSetCursor(window, NULL);
            }
            level0.slidebackground(window);
        }
        else if (currentclass == 1)
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            menu.framestart = glfwGetTime();
            menu.changecursor = 0;
            
            if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
            {
                menu.mouseresize(window);
                menu.mousetranslate(window);

                if (menu.changecursor == 0)
                   glfwSetCursor(window, NULL);
            }
            else //normal mode
            {
                menu.transform();
                menu.togglepointer(window);
                menu.pointerfunctions(window,&currentclass);
                glfwSetCursor(window, NULL);
            }

        }
    }

    void draw()
    {
        if(currentclass ==0)
        level0.draw();

        else if(currentclass ==1)
        menu.draw();
    }

    void loopend(GLFWwindow* window)
    {
        
        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (currentclass == 0)
        {
            level0.frameend = glfwGetTime();
            level0.deltatime = level0.frameend - level0.framestart;
        }
        else if (currentclass == 1)
        {
            menu.frameend = glfwGetTime();
            menu.deltatime = menu.frameend - menu.framestart;
        }
    }

    void postset()
    {
      level0.writematrix("Levels/level0.txt");
      menu.writematrix("Levels/menu.txt");
    }


};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(scrwidth, scrheight, "A Knight's Tale", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Renderer game;

    game.preset();

    while (glfwWindowShouldClose(window) != 1)
    {
        game.loopprocess(window);
        
        game.draw();

        game.loopend(window);
    };
    game.postset();

    glfwTerminate();
    return 0;
}
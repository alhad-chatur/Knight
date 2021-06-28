
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>
#include"Shader.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include"Basic.h"
#include<fstream>
#include<irrklang/irrKlang.h>


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
     0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // top left 
};

int scrwidth = 1900;
int scrheight = 1080;

irrklang::ISoundEngine* soundengine = irrklang::createIrrKlangDevice();

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);
}
 
class Movement
{
private:
    void dynamicspritespaceloop(objectspace* knight, int* number1, int* number, unsigned int* frameno, float anispeed, float FPS, float* sumx, float* sumy, int playerdirectionx, int width, int height, float spritex[], float spritey[], int* activity, int spritenumber)
    {
        *number1 = *number;
        *number = (int)(((*frameno) * anispeed * (knight->nsprites)) / FPS);

        float add = 0;
        float add1 = 0;
        for (int i = 0; i <= *number; i++)
        {
            add = add + ((double)spritex[i] / (double)width);
        };
        for (int i = 0; i < *number; i++)
        {
            add1 = add1 + ((double)spritex[i] / (double)width);
        };
        float addy = 1 - ((double)spritey[*number] / (double)height);


        if (*number == spritenumber)
        {
            *activity = 0;
            *frameno = 0;

            knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)spritex[0] / (double)spritex[spritenumber - 1], (double)spritey[0] / (double)spritey[spritenumber - 1], 1.0f));
            knight->settranform(0, glm::vec3(0.0f, -*sumy / 2, 0.0f));

            if (playerdirectionx == 1)
                knight->settranform(0, glm::vec3(-*sumx / 2, 0.0f, 0.0f));

            else
                knight->settranform(0, glm::vec3(*sumx / 2, 0.0f, 0.0f));

            *number = 0;
            *number1 = 0;
        }

        if ((*number1 != *number) && *activity == 1)
        {
            float length1x = knight->length1.x;
            float length1y = knight->length1.y;
            knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)spritex[*number] / (double)spritex[*number - 1], (double)spritey[*number] / (double)spritey[*number - 1], 1.0f));

            if (playerdirectionx == 1)
                knight->settranform(0, glm::vec3((knight->length1.x - length1x) / 2, (knight->length1.y - length1y) / 2, 0.0f));

            else if (playerdirectionx == -1)
                knight->settranform(0, glm::vec3(-(knight->length1.x - length1x) / 2, (knight->length1.y - length1y) / 2, 0.0f));

            *sumy += knight->length1.y - length1y;
            *sumx += knight->length1.x - length1x;

        }

        if (playerdirectionx == -1)
        {
            add = add + add1;     //a little swap between add and add1
            add1 = add - add1;
            add = add - add1;
        }

        knight->shader.use();

        knight->shader.setFloat("add1", add1);

        knight->shader.setFloat("add", add);
        knight->shader.setFloat("addy", addy);
        knight->shader.setInt("texdirection", 1);

    }

    void dynamicspritespacecont(objectspace* knight, int* number1, int* number, unsigned int* framenumber, float anispeed, float FPS, float* sumx, float* sumy, int playerdirectionx, int width, int height, float spritex[], float spritey[], int activity, int spritenumber)
    {
        *number1 = *number;
        *number = (int)(((*framenumber) * anispeed * (knight->nsprites)) / FPS);

        if (*number == spritenumber)
        {
            knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)spritex[0] / (double)spritex[*number - 1], (double)spritey[0] / (double)spritey[*number - 1], 1.0f));
            knight->settranform(0, glm::vec3(0.0f, -*sumy / 2, 0.0f));

            if (activity == 1)
                knight->settranform(0, glm::vec3(-*sumx / 2, 0.0f, 0.0f));

            else if (activity == -1)
                knight->settranform(0, glm::vec3(*sumx / 2, 0.0f, 0.0f));


            *framenumber = 0;
            *number = 0;
            *number1 = 0;
            *sumx = 0;
            *sumy = 0;
        }
        float add = 0;
        float add1 = 0;
        for (int i = 0; i <= *number; i++)
        {
            add = add + ((double)spritex[i] / (double)width);
        };
        for (int i = 0; i < *number; i++)
        {
            add1 = add1 + ((double)spritex[i] / (double)width);
        };
        float addy = 1 - ((double)spritey[*number] / (double)height);

        if (*number1 != *number)
        {
            float length1x = knight->length1.x;
            float length1y = knight->length1.y;
            knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)spritex[*number] / (double)spritex[*number - 1], (double)spritey[*number] / (double)spritey[*number - 1], 1.0f));

            if (activity == 1)
                knight->settranform(0, glm::vec3((knight->length1.x - length1x) / 2, (knight->length1.y - length1y) / 2, 0.0f));

            else if (activity == -1)
                knight->settranform(0, glm::vec3(-(knight->length1.x - length1x) / 2, (knight->length1.y - length1y) / 2, 0.0f));

            *sumy += knight->length1.y - length1y;
            *sumx += knight->length1.x - length1x;
        }

        if (activity == -1)
        {
            add = add + add1;
            add1 = add - add1;
            add = add - add1;
        }

        knight->shader.setFloat("add1", add1);

        knight->shader.setFloat("add", add);
        knight->shader.setFloat("addy", addy);
        knight->shader.setInt("texdirection", 1);

    }

public:
    int idlewidth = 438;
    int idleheight = 610;


    //related to jumping
    int jumpframeno = 0;
    int jumping = 0;
    double jumptime = 0, jumptime1 = 0;
    double jumpspeedy = 0.35f;
    double jumpspeedx = 0.4f;
    float g = 0.1f;
    glm::vec3 posbeforejump = glm::vec3(0.0f);
    float groundposy = -0.78f;

    //related to attack
    int attacking = 0;
    unsigned int attackframeno = 0;
    int attnumber = 0;
    int attnumber1 =0;
    float attackx[12]
    {
        435,389,363,352,353,413,467,468,470,494,464,435
    };
    float attacky[12]
    {
        615,617,616,614,614,616,600,597,594,607,615,615
    };
    float attanispeed = 1.7f;
    int attspritesno = 12;
    float attsumx = 0;
    float attsumy = 0;

    //related to walking
    int walking = 0;
    float walkspeed = 0.25f;
    float walkx[10]
    {
      407,398,388,393,393,399,398,402,405,407
    };
    float walky[10]
    {
        633,657,660,638,640,632,652,650,635,637
    };
    int walknumber = 0;
    int walknumber1 = 0;
    float walkanispeed = 1.5f;
    int walkspritesno = 10;
    float walksumx = 0;
    float walksumy = 0;
    unsigned int walkframenumber = 0;

    //related to running
    int running = 0;
    float runspeed = 0.4f;
    float runx[10]
    {
        403,370,376,381,380,400,417,409,415,429

    };
    float runy[10]
    {
        631,662,630,624,629,631,649,627,624,634
    };
    int runnumber = 0;
    int runnumber1 = 0;
    float runanispeed = 1.5f;
    int runspritesno = 10;
    float runsumx = 0;
    float runsumy = 0;
    unsigned int runframenumber = 0;

    //related to falling
    int falling = 0;
    double falltime = 0, falltime1 = 0;

    //related to frames
    int FPS = 120;
    int framenumber = 1;
    float add = 0;
    float add1=0;
    float addy = 0;
    double framestart, frameend, deltatime = 0;

    //related to sliding
    int sliding = 0;
    unsigned int slideframeno = 0;
    float slidex[12] =
    {
        438,435,504,589,657,657,657,657,657,654,501,438
    };
    float slidey[12] =
    {
        610,624,605,521,447,447,447,447,447,472,582,610
    };
    int slidenumber = 0 , slidenumber1 = 0;
    float slideanispeed = 1.25f;
    int slidespritesno = 12;
    float slidesumy = 0;
    float slidesumx = 0;
    float slidespeed = 0.003f;

    int gamestart = 0;
    int playerdirectionx = 1;


    void feedframedata(int framenumber1, double deltatime1)
    {
        framenumber = framenumber1;
        deltatime = deltatime1;
    }

    void idle(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture knightidle)
    {

        if (jumptime == 0 && attacking ==0 &&sliding ==0)
        {

            if (walking == 0 && running == 0)
            {
                knight->shader.use();
                *knighttex = knightidle;
                add = (1 / knight->nsprites) * (int)((framenumber * knight->speed * knight->nsprites) / FPS);
                
                if (playerdirectionx == -1)
                    add = 1 - add;

                add1 = add - 0.1f;
                knight->shader.setFloat("add1", 0.0f);

                knight->shader.setFloat("add", 0.1f);
                knight->shader.setFloat("addy", 0.0f);
                knight->shader.setInt("texdirection", playerdirectionx);
            }
        }

    }

    void run(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture knightrun, int runkeyright = NULL,int runkeyleft = NULL, int walkkey2 = NULL)
    {
        if (jumptime == 0 && attacking == 0 && sliding == 0)
        {
            if (glfwGetKey(window, runkeyright) == GLFW_PRESS && glfwGetKey(window, walkkey2) == GLFW_RELEASE || glfwGetKey(window, runkeyleft) == GLFW_PRESS && glfwGetKey(window, walkkey2) == GLFW_RELEASE)
            {
                runframenumber++;
                if (running == 0)  //when starting to run
                {
                    knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)runx[0] / (double)idlewidth, (double)runy[0] / (double)idleheight, 1.0f));
                    gamestart = 1;
                    runframenumber = 0;
                    runsumx = 0;
                    runsumy = 0;
                }
                if (glfwGetKey(window, runkeyright) == GLFW_PRESS && glfwGetKey(window, walkkey2) == GLFW_RELEASE)
                {
                    playerdirectionx = 1;
                    running = 1;
                }
                else
                {
                    playerdirectionx = -1;
                    running = -1;
                }
                if (running == 1)
                   knight->settranform(0.0f, glm::vec3(runspeed * deltatime, 0.0f, 0.0f));

                else if (running == -1)
                   knight->settranform(0.0f, glm::vec3(-runspeed * deltatime, 0.0f, 0.0f));

                knight->shader.use();
                *knighttex = knightrun;
                dynamicspritespacecont(knight, &runnumber1, &runnumber, &runframenumber, runanispeed, FPS, &runsumx, &runsumy, playerdirectionx, knightrun.width, knightrun.height, runx, runy, running, runspritesno);
            }

            else if (running == 1 ||running ==-1) //when the running is about to stop 
            {
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)runx[runnumber], (double)idleheight / (double)runy[runnumber], 1.0f));
                knight->settranform(0, glm::vec3(0.0f, -runsumy / 2, 0.0f));

                if(running ==1)
                knight->settranform(0, glm::vec3(-runsumx / 2, 0.0f, 0.0f));

                else if(running ==-1)
                    knight->settranform(0, glm::vec3(runsumx / 2, 0.0f, 0.0f));

                running = 0;
                runnumber = 0;
                runframenumber = 0;
                runnumber1 = 0;
                running = 0;
            }
            else
            {
                runnumber = 0;
                runnumber1 = 0;
                runframenumber = 0;
                running = 0;
            }
        }
    }

    void walk(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture knightwalk, int walkkeyright = NULL,int walkkeyleft = NULL, int walkkey2 = NULL)
    {
        if (jumptime == 0 && attacking == 0 && sliding == 0)
        {
            if ((glfwGetKey(window, walkkeyright) == GLFW_PRESS && glfwGetKey(window, walkkey2) == GLFW_PRESS) ||(glfwGetKey(window, walkkeyleft) == GLFW_PRESS && glfwGetKey(window, walkkey2) == GLFW_PRESS))
            {
                walkframenumber++;
                if (walking == 0)  //when starting to walk
                {
                    knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)walkx[0] / (double)idlewidth, (double)walky[0] / (double)idleheight, 1.0f));
                    gamestart = 1;
                    walkframenumber = 0;
                    walksumx = 0;
                    walksumy = 0;
                }
                if (glfwGetKey(window, walkkeyright) ==GLFW_PRESS && glfwGetKey(window, walkkey2) == GLFW_PRESS)
                {
                    playerdirectionx = 1;
                    walking = 1;
                }
                else if(glfwGetKey(window,walkkeyleft) ==GLFW_PRESS && glfwGetKey(window,walkkey2) == GLFW_PRESS)
                {
                    playerdirectionx = -1;
                    walking = -1;
                }
                if (walking == 1)
                    knight->settranform(0.0f, glm::vec3(walkspeed * deltatime, 0.0f, 0.0f));

                else if (walking == -1)
                    knight->settranform(0.0f, glm::vec3(-walkspeed * deltatime, 0.0f, 0.0f));

                knight->shader.use();
                *knighttex = knightwalk;
                dynamicspritespacecont(knight, &walknumber1, &walknumber, &walkframenumber, walkanispeed, FPS, &walksumx, &walksumy, playerdirectionx, knightwalk.width, knightwalk.height, walkx, walky, walking, walkspritesno);

            }
            else if(walking ==1 ||walking ==-1)
            {
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)walkx[walknumber], (double)idleheight / (double)walky[walknumber], 1.0f));
                knight->settranform(0, glm::vec3(0.0f, -walksumy / 2, 0.0f));

                if (walking == 1)
                    knight->settranform(0, glm::vec3(-walksumx / 2, 0.0f, 0.0f));

                else if (walking == -1)
                    knight->settranform(0, glm::vec3(walksumx / 2, 0.0f, 0.0f));

                walking = 0;
                walknumber = 0;
                walkframenumber = 0;
                walknumber1 = 0;
                walking = 0;
            }
            else
            {
                walknumber = 0;
                walknumber1 = 0;
                walkframenumber = 0;
                walking = 0;  
            }
        }
    }

    void jump(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture* knightjump,int jumpkey = NULL)
    {

        if (((glfwGetKey(window, jumpkey) == GLFW_PRESS && jumping == 0) || jumping != 0) && falling == 0 &&sliding ==0)
        {
            gamestart = 1;
            
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
                add1 = add - 0.1f;
                knight->shader.setFloat("add1", add1);


                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", 0.0f);
                knight->shader.setInt("texdirection", 1);

                knight->settranform(0, glm::vec3(jumpspeedx * deltatime, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }
            else if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && jumping == 0) || jumping == 2)
            {
                jumping = 2;
                knight->shader.use();
                add = 0.1f;
                add1 = add - 0.1f;
                knight->shader.setFloat("add1", add1);

                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", 0.0f);
                knight->shader.setInt("texdirection", -1);

                knight->settranform(0, glm::vec3(-jumpspeedx * deltatime, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }

            else if (jumping == 0 || jumping == 3)
            {
                jumping = 3;
                knight->shader.use();
                add = 0.5f;
                add1 = add - 0.1f;
                knight->shader.setFloat("add1", add1);
                knight->shader.setFloat("add", add);
                knight->shader.setFloat("addy", 0.0f);

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

    void attack(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture* knightattack,int attackkey = NULL)
    {
        
        if (((glfwGetMouseButton(window, attackkey) == GLFW_PRESS && attacking == 0) || attacking == 1) && gamestart ==1)
        {
            
            if (attacking == 0)
            {
                attackframeno = 0;
                attsumx = 0;
                attsumy = 0;
            }
            attackframeno++;

            attacking = 1;
            *knighttex = *knightattack;
            knight->shader.use();
            dynamicspritespaceloop(knight, &attnumber1, &attnumber, &attackframeno, attanispeed, FPS, &attsumx, &attsumy, playerdirectionx, knightattack->width, knightattack->height, attackx, attacky, &attacking, attspritesno);
        }

    }

    void slide(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture knightslide,int slidekey = NULL)
    {
        
        if (((glfwGetKey(window,slidekey) == GLFW_PRESS && sliding == 0) || sliding == 1) && gamestart == 1 && jumping ==0)
        {
            
            if (sliding == 0)
            {
                slideframeno = 0;
                slidesumy = 0;
                slidesumx = 0;
            }
            slideframeno++;
            knight->shader.use();
            sliding = 1;
            *knighttex =knightslide;

            knight->settranform(0, glm::vec3(playerdirectionx * slidespeed, 0.0f, 0.0f));

            dynamicspritespaceloop(knight, &slidenumber1, &slidenumber, &slideframeno, slideanispeed, FPS, &slidesumx, &slidesumy, playerdirectionx, knightslide.width, knightslide.height, slidex, slidey, &sliding, slidespritesno);
        }
    };

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

            if (walking != 0)
            {
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)walkx[walknumber], (double)idleheight / (double)walky[walknumber], 1.0f));
                knight->settranform(0, glm::vec3(0.0f, -walksumy / 2, 0.0f));

                if (walking == 1)
                    knight->settranform(0, glm::vec3(-walksumx / 2, 0.0f, 0.0f));

                else if (walking == -1)
                    knight->settranform(0, glm::vec3(walksumx / 2, 0.0f, 0.0f));

                distance = walkspeed * deltatime;

                walknumber = 0;
                walknumber1 = 0;
                walkframenumber = 0;
                walking = 0;

            }
            else if (running != 0)
            {

                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)runx[runnumber], (double)idleheight / (double)runy[runnumber], 1.0f));
                knight->settranform(0, glm::vec3(0.0f, -runsumy / 2, 0.0f));

                distance = runspeed * deltatime;

                runnumber = 0;
                runnumber1 = 0;
                runframenumber = 0;
                running = 0;
            }
            else if (jumping == 1 || jumping == 2)
            {
                running = 0;
                walking = 0;
                distance = jumpspeedx * deltatime;
                jumpspeedx = 0;
            }
            else if (jumping == 3)
                jumpspeedx = 0;
            else if (sliding != 0)
            {
               
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)slidex[0] / (double)slidex[slidenumber], (double)slidey[0] / (double)slidey[slidenumber], 1.0f));
                knight->settranform(0, glm::vec3(0.0f, -slidesumy / 2, 0.0f));

                distance = slidespeed * deltatime;
                             
                sliding = 0;
                slidenumber = 0;
                slidenumber1 = 0;
                slideframeno = 0;

            }


            if (playerdirectionx == 1)
                knight->settranform(0, glm::vec3(-distance, 0.0f, 0.0f));

            else if (playerdirectionx == -1)
                knight->settranform(0, glm::vec3(distance, 0.0f, 0.0f));  

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
            if (glm::distance(knight->center1.x, box->center1.x) <= ((knight->length1.x + box->length1.x) / 2)-box->length1.x/10 && glm::distance(knight->center1.y, box->center1.y) <= (knight->length1.y + box->length1.y) / 2 && knight->center1.y > (box->center1.y + box->length1.y/2))
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

    void collectiblecollision(objectspace* knight, objectspace* coin,const char* audio)
    {
        if (glm::distance(knight->center1.x, coin->center1.x) <= (knight->length1.x + coin->length1.x) / 2 && glm::distance(knight->center1.y, coin->center1.y) <= (knight->length1.y + coin->length1.y) / 2 && coin->contact ==0)
        {
            coin->contact = 1;
            soundengine->play2D(audio);
        }
    }
};

class Knight
{
public:
    objectspace knight;
    
    Movement knightmovement;

    objecttexture knighttex;

    objecttexture knightattack = objecttexture("textures/attack1.png", 1);

    objecttexture knightjump = objecttexture("textures/jump.png", 1);

    objecttexture knightidle = objecttexture("textures/idle.png", 1);

    objecttexture knightslide = objecttexture("textures/slide.png", 1);

    objecttexture knightrun = objecttexture("textures/run.png", 1);

    objecttexture knightwalk = objecttexture("textures/walk.png", 1);

    void initialize()
    {
        knight.intitialize(spritevertices, indices, "attacksprite.vs", "attacksprite.fs");
        knight.setmodel(0, glm::vec3(0.5f, -0.9f, 0.0f), glm::vec3(0.25f));
        knighttex = knightidle;
        knight.shader.setFloat("addy", 0.0f);
    }

    void knightanimations(GLFWwindow *window)
    {

        knightmovement.jump(window, &knight, &knighttex, &knightjump,GLFW_KEY_SPACE);

        knightmovement.fall(&knight);

        knightmovement.attack(window, &knight, &knighttex, &knightattack,GLFW_MOUSE_BUTTON_LEFT);

        knightmovement.walk(window, &knight, &knighttex, knightwalk,GLFW_KEY_D,GLFW_KEY_A,GLFW_KEY_LEFT_CONTROL);

        knightmovement.run(window, &knight, &knighttex, knightrun,GLFW_KEY_D,GLFW_KEY_A,GLFW_KEY_LEFT_CONTROL);

        knightmovement.slide(window, &knight, &knighttex, knightslide,GLFW_KEY_LEFT_SHIFT);

        knightmovement.idle(window, &knight, &knighttex, knightidle); //always put this in last
    }
    
    void draw()
    {
        knight.drawquad(knighttex);
    }
    
    void deinitialize()
    {
        knight.deinitialize();
    }

};

class Level0
{
public:

    float coinvertices[100] =
    {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   0.166, 1.0f, // top right
         0.5f, -0.5f, 0.0f,    0.166, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // top left 
    };
    float slimevertices[100] =
    {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   0.04, 1.0f, // top right
         0.5f, -0.5f, 0.0f,    0.04, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // top left 
    };

    float texmapvertices[100]
    {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,    1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,    0.0f, 1.0f  // top left 
    };
    
    objectspace bg;
    
    objectspace bg2;

    objectspace container[3];

    int containerno = 3; //when adding new containers do uncomment the game.getmatrix() function for one time

    objectspace coins[4];

    int coinnumber = 4;

    objectspace slime;

    objecttexture bgtex = objecttexture("textures/bg.png", 1);

    objecttexture containertex = objecttexture("textures/container.jpg", 2);

    objecttexture cointex = objecttexture("textures/coins.png", 1);

    objecttexture tilemaptex = objecttexture("textures/level0.png", 1);

    objecttexture slimeidle = objecttexture("textures/slimeidle.png", 1);

    objecttexture slimehurt = objecttexture("textures/slimehurt.png", 1);

    objecttexture slimedie = objecttexture("textures/slimedie.png", 1);


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
   

    Knight knightclass;

    void transform()
    {
        framenumber++;
        if (framenumber == FPS/spriteanimationspeed)
            framenumber = 1;

    }

    void initialize()
    {
        bg.intitialize(texmapvertices, indices, "shaders/bg.vs", "shaders/bg.fs");
        bg.model = glm::mat4(1.0f);
        bg.setmodel(0, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));

        bg2.intitialize(texmapvertices, indices, "shaders/bg.vs", "shaders/bg.fs");
        bg2.model = glm::mat4(1.0f);
        bg2.setmodel(0, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));

        knightclass.initialize();

        container[0].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container[0].model = glm::mat4(1.0f);
        container[0].setmodel(0, glm::vec3(0.5f, 0.7f, 0.0f), glm::vec3(0.5f));

        container[1].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container[1].model = glm::mat4(1.0f);
        container[1].setmodel(0, glm::vec3(0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        container[2].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container[2].model = glm::mat4(1.0f);
        container[2].setmodel(0, glm::vec3(-0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

       
        for (int i = 0; i < coinnumber; i++)
        {
            coins[i].intitialize(coinvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
            coins[i].model = glm::mat4(1.0f);
            coins[i].setmodel(0.1f, glm::vec3(-0.7f, 0.7f, 0.0f));
       
        }
        slime.intitialize(slimevertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        slime.model = glm::mat4(1.0f);
        slime.setmodel(0.1f, glm::vec3(-0.7f, 0.7f, 0.0f));   

    }

    void restart()
    {
        bg.center1 = bg.center;
        bg.length1 = bg.length;
        bg.transform = glm::mat4(1.0f);

        bg2.center1 = bg2.center;
        bg2.length1 = bg2.length;
        bg2.transform = glm::mat4(1.0f);

        for (int i = 0; i < containerno; i++)
        {
            container[i].center1 = container[i].center;        
            container[i].length1 = container[i].length;
            container[i].transform = glm::mat4(1.0f);
        }

        for (int i = 0; i < coinnumber; i++)
        {
            coins[i].center1 = coins[i].center;
            coins[i].length1 = coins[i].length;
            coins[i].transform = glm::mat4(1.0f);
            coins[i].contact = 0;
        }

        knightclass.knight.center1 = knightclass.knight.center;
        knightclass.knight.length1 = knightclass.knight.length;
        knightclass.knight.transform = glm::mat4(1.0f);

       slime.center1 = slime.center;
        slime.length1 = slime.length;
        slime.transform = glm::mat4(1.0f);

    }

    void mousetranslate(GLFWwindow* window)
    {
        mousetranslatesprite(window, &knightclass.knight, &changecursor);    
       
            for (int i = 0; i < containerno; i++)
            {
                mousetranslatesprite(window, &container[i], &changecursor);
            }
            for (int i = 0; i < coinnumber; i++)
            {
                mousetranslatesprite(window, &coins[i], &changecursor);
            }
            mousetranslatesprite(window, &slime, &changecursor);
    }

    void mouseresize(GLFWwindow* window)
    {
        for (int i = 0; i < containerno; i++)
        {
            mouseresizesprite(window, &container[i], &changecursor);
        }
        for (int i = 0; i < coinnumber; i++)
        {
            mouseresizesprite(window, &coins[i], &changecursor);
        }
        mouseresizesprite(window, &slime, &changecursor);
        mouseresizesprite(window, &knightclass.knight, &changecursor);
       
    }

    void writematrix(const char* filename)
    {
        std::ofstream file(filename);

        knightclass.knight.writedata("knight", &file);

        for (int i = 0; i < containerno; i++)
        {
            container[i].writedata("cont", &file,i);
        }

        for (int i = 0; i < coinnumber; i++)
        {
            coins[i].writedata("coin", &file, i);
        }
        bg.writedata("bg", &file);

        bg2.writedata("bgb", &file);

        slime.writedata("slime", &file);

        file.close();
    }

    void getmatrix(const char* filename)
    {
        std::ifstream file2(filename);

        knightclass.knight.readdata("knight", &file2);

        for (int i = 0; i < containerno; i++)
        {

            container[i].readdata("cont", &file2, i);
        }
        for (int i = 0; i < coinnumber; i++)
        {
          coins[i].readdata("coin", &file2, i);
        }
        bg.readdata("bg",&file2);
        bg2.readdata("bgb", &file2);

        slime.readdata("slime", &file2);

        file2.close();
        
    }
    
    void slidebackground(GLFWwindow* window)    
    {
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            if (knightclass.knightmovement.gamestart == 1)
            {
                bg.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                bg2.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));

                knightclass.knight.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));

                for (int i = 0; i < containerno; i++)
                {
                    container[i].settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                  coins[i].settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                }
                slime.settranform(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));


            }
            else
            {

                bg.setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                bg2.setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));

                knightclass.knight.setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                for (int i = 0; i < containerno; i++)
                {
                    container[i].setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));
                
                }
                slime.setview(0, glm::vec3(-slidebgspeed, 0.0f, 0.0f));

            }
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            if (knightclass.knightmovement.gamestart == 1)
            {
                bg.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                bg2.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));

                knightclass.knight.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                for (int i = 0; i < containerno; i++)
                {
                    container[i].settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                
                }
                slime.settranform(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));

            }
            else
            {

                bg.setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                bg2.setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));

                knightclass.knight.setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                for (int i = 0; i < containerno; i++)
                {
                    container[i].setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));
                
                }
                slime.setview(0, glm::vec3(slidebgspeed, 0.0f, 0.0f));


            }
        }
        
    }

    void processmovement(GLFWwindow* window)
    {
        //knight collisions
        knightclass.knightmovement.feedframedata(framenumber, deltatime);

        for (int i = 0; i < containerno; i++)
        {
            knightclass.knightmovement.topcollisionsolid(&knightclass.knight, &container[i]);
            std::cout << container[i].ontop;
        }
        for (int i = 0; i < containerno; i++)
        {
            knightclass.knightmovement.bottomcollisionsolid(&knightclass.knight, &container[i]);
        }
        knightclass.knightanimations(window);

        for (int i = 0; i < containerno; i++)
        {
            knightclass.knightmovement.sidecollisionsolid(&knightclass.knight, &container[i]);
           
        }
        std::cout << "\n";

        for(int i =0;i<coinnumber;i++)
        knightclass.knightmovement.collectiblecollision(&knightclass.knight, &coins[i],"audio/coin touch.wav");

        //coins
        float add = 0;
        for (int i = 0; i < coinnumber; i++)
        {
            coins[i].nsprites = 6;
            
            add = (1 / coins[i].nsprites) * (int)((framenumber *2.3 * coins[i].nsprites) / FPS);
            coins[i].shader.use();
            coins[i].shader.setFloat("add", add);

        }
        slime.nsprites = 25;

        add = (1 / slime.nsprites) * (int)((framenumber * 1.5 * slime.nsprites) / FPS);
        slime.shader.use();
        slime.shader.setFloat("add", add);


    }

    void draw()
    {

        bg2.drawquad(bgtex);
        
        bg.drawquad(tilemaptex);
        
        knightclass.draw();

        if (knightclass.knightmovement.gamestart == 0)
        {
            for (int i = 0; i < containerno; i++)
                container[i].drawquad(containertex);
        }

        for (int i = 0; i < coinnumber; i++)
        {
            if (coins[i].contact == 0)
                coins[i].drawquad(cointex);
        }
        slime.drawquad(slimeidle);

    }

    void deleteall()
    {
        bg.deinitialize();
        knightclass.deinitialize();
        
        for(int i=0;i<containerno;i++)
        container[i].deinitialize();
    }
};

class Menu
{
public:

    objectspace bg;

    objectspace container[3];

    objectspace container1[4];

    int containerno = 3;//when adding new containers do uncomment the game.getmatrix() function for one time

    int containerno1 = 4;

    objectspace pointer;

    objectspace pointer1;

    objecttexture bgtex = objecttexture("textures/grass.png", 1);

    objecttexture containertex = objecttexture("textures/container.jpg", 2);

    objecttexture starttex = objecttexture("textures/start.png", 1); //container[0]

    objecttexture optionstex = objecttexture("textures/options.png", 1); //container[1]

    objecttexture exittex = objecttexture("textures/exit.png", 1); //container[2]

    objecttexture arrowtex = objecttexture("textures/arrow.png", 1); 

    objecttexture resumetex = objecttexture("textures/resume.png", 1);

    objecttexture restarttex = objecttexture("textures/restart.png", 1);


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

    int menustate = 0; //0->start menu , 1->Resume menu

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

        container1[0].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container1[0].setmodel(0, glm::vec3(0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        container1[1].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container1[1].setmodel(0, glm::vec3(-0.5f, 0.7f, 0.0f), glm::vec3(0.5f));

        container1[2].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container1[2].setmodel(0, glm::vec3(-0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        container1[3].intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        container1[3].setmodel(0, glm::vec3(-0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        pointer.intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        pointer.setmodel(0, glm::vec3(-0.5f, 0.7f, 0.0f), glm::vec3(0.25f));

        pointer1.intitialize(objvertices, indices, "shaders/sprite.vs", "shaders/sprite.fs");
        pointer1.setmodel(0, glm::vec3(-0.5f, 0.7f, 0.0f), glm::vec3(0.25f));
    }

    void mousetranslate(GLFWwindow* window)
    {
        if (menustate == 0)
        {
            for (int i = 0; i < containerno; i++)
            {
                mousetranslatesprite(window, &container[i], &changecursor);
            }
            mousetranslatesprite(window, &pointer, &changecursor);
        }
        else if (menustate == 1)
        {
            for (int i = 0; i < containerno1; i++)
            {
                mousetranslatesprite(window, &container1[i], &changecursor);
            }
            mousetranslatesprite(window, &pointer1, &changecursor);
        }
    }

    void mouseresize(GLFWwindow* window)
    {
        if (menustate == 0)
        {
            for (int i = 0; i < containerno; i++)
            {
                mouseresizesprite(window, &container[i], &changecursor);
            }
            mouseresizesprite(window, &pointer, &changecursor);
        }
        else if (menustate == 1)
        {
            for (int i = 0; i < containerno1; i++)
            {
                mouseresizesprite(window, &container1[i], &changecursor);
            }
            mouseresizesprite(window, &pointer1, &changecursor);
        }
    }

    void writematrix(const char* filename)
    {
        std::ofstream file(filename);

        for (int i = 0; i < containerno; i++)
        {
            container[i].writedata("cont", &file, i);
        }
        for (int i = 0; i < containerno1; i++)
        {
            container1[i].writedata("contr", &file, i);
        }

        pointer.writedata("pointer", &file);

        pointer1.writedata("pointerr", &file);

        file.close();
    }

    void getmatrix(const char* filename)
    {
        std::ifstream file2(filename);

        for (int i = 0; i < containerno; i++)
        {
            container[i].readdata("cont", &file2, i);
        }
        for (int i = 0; i < containerno1; i++)
        {
            container1[i].readdata("contr", &file2, i);
        }
        pointer.readdata("pointer", &file2);

        pointer1.readdata("pointerr", &file2);

        file2.close();
        
    }

    void togglepointer(GLFWwindow* window)
    {
        if (menustate == 0)
        {
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && pointertime == 0)
            {
                pointerpos++;
                pointertime = pointerspeed;
                if (pointerpos > containerno - 1)
                    pointerpos = 0;
                soundengine->play2D("audio/solid.wav");
            }
            else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && pointertime == 0)
            {
                pointerpos--;
                pointertime = pointerspeed;
                if (pointerpos < 0)
                    pointerpos = containerno - 1;
                soundengine->play2D("audio/solid.wav");
            }
            pointertime -= deltatime;
            if (pointertime < 0)
                pointertime = 0;
            pointer.setview(0, glm::vec3((container[pointerpos].center1.x - pointer.center1.x) - container[pointerpos].length1.x / 2 - 0.05f, (container[pointerpos].center1.y - pointer.center1.y), 0));
        }

        else if (menustate == 1)
        {
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && pointertime == 0)
            {
                pointerpos++;
                pointertime = pointerspeed;
                if (pointerpos > containerno1 - 1)
                    pointerpos = 0;
                soundengine->play2D("audio/solid.wav");
            }
            else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && pointertime == 0)
            {
                pointerpos--;
                pointertime = pointerspeed;
                if (pointerpos < 0)
                    pointerpos = containerno1 - 1;
                soundengine->play2D("audio/solid.wav");
            }
            pointertime -= deltatime;
            if (pointertime < 0)
                pointertime = 0;
            pointer1.setview(0, glm::vec3((container1[pointerpos].center1.x - pointer1.center1.x) - container1[pointerpos].length1.x / 2 - 0.05f, (container1[pointerpos].center1.y - pointer1.center1.y), 0));
        }


    }

    void pointerfunctions(GLFWwindow* window,int *currentclass,int *restart)
    {
        if (menustate == 0)
        {
            pointer.shader.setInt("texdirection", -1);

            if (pointerpos == 0 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                *currentclass = 0;

            if (pointerpos == 2 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, 1);
        }
        if (menustate == 1)
        {
            pointer1.shader.setInt("texdirection", -1);

            if (pointerpos == 0 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                *currentclass = 0;

            if (pointerpos == 1 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                *restart = 1;

            if (pointerpos == 3 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, 1);
        }
    }

    void draw()
    {
        if (menustate == 0)
        {
            container[0].drawquad(starttex);

            container[1].drawquad(optionstex);
            container[2].drawquad(exittex);
            pointer.drawquad(arrowtex);
        }
        else if (menustate == 1)
        {
            container1[0].drawquad(resumetex);

            container1[1].drawquad(restarttex);
            container1[2].drawquad(optionstex);
            container1[3].drawquad(exittex);
            pointer1.drawquad(arrowtex);
        }
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
    int restart = 0;

    void preset()
    {
        level0.initialize();
        level0.getmatrix("Levels/level0.txt");
        menu.initialize();
        menu.getmatrix("Levels/menu.txt");
        level0.knightclass.knightmovement.gamestart == 0;
        soundengine->play2D("audio/solid.wav"); //dummy audio for irrklang to get started

    }

    void loopprocess(GLFWwindow *window)
    {

        if (currentclass == 0)
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                currentclass = 1;
                menu.menustate = 1;
            }
            level0.framestart = glfwGetTime();
            level0.changecursor = 0;

            if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && level0.knightclass.knightmovement.gamestart == 0) //Change Position Mode only before moving any character
            {
                level0.transform();
                level0.mousetranslate(window);

                if (level0.changecursor == 0)
                    glfwSetCursor(window, NULL);
            }
            else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && level0.knightclass.knightmovement.gamestart == 0) //Change Position Mode only before moving any character
            {
                level0.transform();
                level0.mouseresize(window);

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
            level0.framestart = glfwGetTime();
            menu.changecursor = 0;

            
            if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
            {
                menu.mousetranslate(window);

                if (menu.changecursor == 0)
                   glfwSetCursor(window, NULL);
            }
            else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            {
                menu.mouseresize(window);  

                if (menu.changecursor == 0)
                    glfwSetCursor(window, NULL);
            }
            else //normal mode
            {
                menu.transform();
                menu.togglepointer(window);
                menu.pointerfunctions(window,&currentclass,&restart);
                
                glfwSetCursor(window, NULL);
            }

            if (restart == 1)
            {
                restart = 0;
                menu.menustate = 0;
                level0.restart();
                
                soundengine->play2D("audio/solid.wav"); //dummy audio for irrklang to get started

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
  
            level0.frameend = glfwGetTime();
            level0.deltatime = level0.frameend - level0.framestart;
        
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

    GLFWwindow* window = glfwCreateWindow(scrwidth, scrheight, "A Knight's Tale", NULL , NULL);

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
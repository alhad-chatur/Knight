
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

int scrwidth = 1920;
int scrheight = 1080;
int FPS = 60;

irrklang::ISoundEngine* soundengine = irrklang::createIrrKlangDevice();

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);
}
 
class Movement
{
private:
    void dynamicspritespaceloop(objectspace* knight, int* number1, int* number, unsigned int* frameno, float anispeed, float FPS, float* sumx, float* sumy, int playerdirectionx, int width, int height, float spritex[], float spritey[], int* activity, int spritenumber,int idlewidth,int idleheight)
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

            knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)spritex[spritenumber - 1], (double)idleheight / (double)spritey[spritenumber - 1], 1.0f));
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

        knight->add1 = add1;
        knight->add = add;
        knight->addy = addy;
        knight->texdirectionx = 1;
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

        knight->add1 = add1;
        knight->add = add;
        knight->addy = addy;
        knight->texdirectionx = 1;
    }

public:
    int idlewidth;
    int idleheight;


    //related to jumping
    int jumpframeno = 0;
    int jumping = 0;
    double jumptime = 0, jumptime1 = 0;
    double jumpspeedy;
    double jumpspeedx;
    float g = 0.1f;
    glm::vec3 posbeforejump = glm::vec3(0.0f);
    float groundposy = -0.78f;
    float jump1widtha,jump2widtha,jump3widtha, jump1widthb, jump2widthb, jump3widthb;
    int jumptexwidth =0;
    enum class activity{RUNNING,WALKING,NONE};
    activity beforejump =activity::NONE;

    //related to attack
    int attacking = 0;
    unsigned int attackframeno = 0;
    int attnumber = 0;
    int attnumber1 =0;
    float attackx[100];

    float attacky[100];

    float attanispeed;
    int attspritesno;
    float attsumx = 0;
    float attsumy = 0;

    //related to walking
    int walking = 0;
    float walkspeed;
    float walkx[100];
  
    float walky[100];
  
    int walknumber = 0;
    int walknumber1 = 0;
    float walkanispeed;
    int walkspritesno;
    float walksumx = 0;
    float walksumy = 0;
    unsigned int walkframenumber = 0;

    //related to running
    int running = 0;
    float runspeed;
    float runx[100];
 
    float runy[100];

    int runnumber = 0;
    int runnumber1 = 0;
    float runanispeed;
    int runspritesno;
    float runsumx = 0;
    float runsumy = 0;
    unsigned int runframenumber = 0;

    //related to falling
    int falling = 0;
    double falltime = 0, falltime1 = 0;
    float fallspeedx = 0;
    int falldirectionx = 1;

    //related to frames
    int framenumber = 1;
    float add = 0;
    float add1=0;
    float addy = 0;
    double framestart, frameend, deltatime = 0;

    //related to sliding
    int sliding = 0;
    unsigned int slideframeno = 0;
    float slidex[100];

    float slidey[100];
 
    int slidenumber = 0 , slidenumber1 = 0;
    float slideanispeed;
    int slidespritesno;
    float slidesumy = 0;
    float slidesumx = 0;
    float slidespeed;


    //related to proneing
    int proneing = 0;
    int pronemoving = 0;
    float pronemovspeed;
    float pronex[100];
    float proney[100];
    int pronenumber = 0;
    int pronenumber1 = 0;
    float proneanispeed;
    int pronespritesno;
    float pronesumx = 0;
    float pronesumy = 0;
    unsigned int proneframenumber = 0;
    float pronetoggletime = FPS/4;


    int gamestart = 0;
    int playerdirectionx = 1;


    void feedframedata(int framenumber1, double deltatime1)
    {
        framenumber = framenumber1;
        deltatime = deltatime1;
    }

    void idle(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture knightidle)
    {

        if (jumptime == 0 && attacking ==0 &&sliding ==0 &&proneing ==0)
        {

            if (walking == 0 && running == 0)
            {
                *knighttex = knightidle;
                knight->add1 = 0.0f;
                knight->add = (double)idlewidth/(double)knightidle.width;
                knight->addy = 0.0f;
                knight->texdirectionx = playerdirectionx;
            }
        }

    }

    void run(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture knightrun, int runkeyright = NULL,int runkeyleft = NULL, int walkkey2 = NULL)
    {
        if (jumping == 0 && attacking == 0 && sliding == 0 && falling ==0)
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
                    knight->settranform(0.0f, glm::vec3(runspeed * deltatime, 0.0f, 0.0f));
                }
                else
                {
                    playerdirectionx = -1;
                    running = -1;
                    knight->settranform(0.0f, glm::vec3(-runspeed * deltatime, 0.0f, 0.0f));
                }

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
        if (jumping == 0 && attacking == 0 && sliding == 0 && falling ==0 &&proneing ==0)
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
                    knight->settranform(0.0f, glm::vec3(walkspeed * deltatime, 0.0f, 0.0f));
                }
                else if(glfwGetKey(window,walkkeyleft) ==GLFW_PRESS && glfwGetKey(window,walkkey2) == GLFW_PRESS)
                {
                    playerdirectionx = -1;
                    walking = -1;
                    knight->settranform(0.0f, glm::vec3(-walkspeed * deltatime, 0.0f, 0.0f));
                }
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

                if (running != 0)
                {
                    beforejump = activity::RUNNING;  
                    runframenumber = 0;
                    running = 0;
                }
                else if (walking != 0)
                {                    
                    beforejump = activity::WALKING;                    
                    walkframenumber = 0;
                    walking = 0;

                }
            }
            if (jumpframeno == FPS / knight->speed)
                jumpframeno = 1;



            jumptime1 = jumptime;
            jumptime += 7 * deltatime;

            *knighttex = *knightjump;

            if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && jumping == 0) || jumping == 1)
            {
                if (jumping == 0)
                {
                    if (beforejump == activity::RUNNING)
                    {
                        knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)(((double)jump1widthb- (double)jump1widtha)*jumptexwidth) / (double)runx[runnumber], (double)idleheight / (double)runy[runnumber], 1.0f));
                        runnumber = 0;
                        runnumber1 = 0;
                        beforejump = activity::NONE;
                    }
                    else if (beforejump == activity::WALKING)
                    {
                        knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)(((double)jump1widthb - (double)jump1widtha)*jumptexwidth) / (double)walkx[walknumber], (double)idleheight / (double)walky[walknumber], 1.0f));
                        walknumber = 0;
                        walknumber1 = 0;
                        beforejump = activity::NONE;
                    }
                    else
                    {
                        knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)(((double)jump1widthb - (double)jump1widtha) * jumptexwidth) / (double)idlewidth, 1.0f, 1.0f));

                    }
                }


                jumping = 1;
                add = jump1widthb;
                add1 = jump1widtha;
                playerdirectionx = 1;
                knight->add1 = add1;

                knight->add = add;

                knight->addy = 0.0f;
                knight->texdirectionx = 1;
                knight->settranform(0, glm::vec3(jumpspeedx * deltatime, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }
            else if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && jumping == 0) || jumping == 2)
            {
                if (jumping == 0)
                {
                    if (beforejump == activity::RUNNING)
                    {
                        knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)(((double)jump2widthb - (double)jump2widtha)* (double)jumptexwidth) / (double)runx[runnumber], (double)idleheight / (double)runy[runnumber], 1.0f));
                        runnumber = 0;
                        runnumber1 = 0;
                        beforejump = activity::NONE;
                    }
                    else if (beforejump == activity::WALKING)
                    {
                        knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)(((double)jump2widthb - (double)jump2widtha) * (double)jumptexwidth) / (double)walkx[walknumber], (double)idleheight / (double)walky[walknumber], 1.0f));
                        walknumber = 0;
                        beforejump = activity::NONE;
                        walknumber1 = 0;
                    }
                    else
                    {
                        knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)(((double)jump2widthb - (double)jump2widtha) * (double)jumptexwidth) / idlewidth, (double)idleheight / (double)idleheight, 1.0f));
                    }
                }
                jumping = 2;
                playerdirectionx = -1;
                add = jump2widthb;
                add1 = jump2widtha;
                knight->add1 = add1;
                knight->add = add;
                knight->addy = 0;
                knight->texdirectionx = -1;
                knight->settranform(0, glm::vec3(-jumpspeedx * deltatime, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }

            else if (jumping == 0 || jumping == 3)
            {
                jumping = 3;
                add = jump3widthb;
                add1 = jump3widtha;

                if (playerdirectionx == -1)
                {
                    add = add + add1;
                    add1 = add - add1;
                    add = add - add1;
                }


                knight->add1 = add1;
                knight->add = add;
                knight->addy = 0;
                knight->texdirectionx = 1;
                knight->settranform(0, glm::vec3(0.0f, ((jumpspeedy * (jumptime - jumptime1)) - (0.5 * g * ((jumptime * jumptime) - (jumptime1 * jumptime1)))), 0.0f));

            }

            jumpframeno++;
        }


    }

    void attack(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture* knightattack,int attackkey = NULL)
    {
        
        if (((glfwGetMouseButton(window, attackkey) == GLFW_PRESS && attacking == 0) || attacking == 1) && gamestart ==1 && walking ==0&&sliding == 0 )
        {
            if (running != 0)
            {
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)runx[runnumber], (double)idleheight / (double)runy[runnumber], 1.0f));
                knight->settranform(0, glm::vec3(0.0f, -runsumy / 2, 0.0f));

                if (running == 1)
                    knight->settranform(0, glm::vec3(-runsumx / 2, 0.0f, 0.0f));

                else if (running == -1)
                    knight->settranform(0, glm::vec3(runsumx / 2, 0.0f, 0.0f));

                running = 0;
                runnumber = 0;
                runframenumber = 0;
                runnumber1 = 0;
                running = 0;
            }
            if (attacking == 0)
            {
             
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)attackx[0] / (double)idlewidth, (double)attacky[0] / (double)idleheight, 1.0f));
                attackframeno = 0;
                attsumx = 0;
                attsumy = 0;
            }
            
            attackframeno++;

            attacking = 1;
            *knighttex = *knightattack;

            dynamicspritespaceloop(knight, &attnumber1, &attnumber, &attackframeno, attanispeed, FPS, &attsumx, &attsumy, playerdirectionx, knightattack->width, knightattack->height, attackx, attacky, &attacking, attspritesno,idlewidth,idleheight);
        }

    }

    void slide(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture knightslide,int slidekey = NULL)
    {
        
        if (((glfwGetKey(window,slidekey) == GLFW_PRESS && sliding == 0) || sliding == 1) && gamestart == 1 && jumping ==0 && walking ==0 &&attacking == 0 && falling ==0)
        {
            
            if (sliding == 0)
            {
               
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)slidex[0] / (double)idlewidth, (double)slidey[0] / (double)idleheight, 1.0f));

                slideframeno = 0;
                slidesumy = 0;
                slidesumx = 0;
            }
            if (running != 0)
            {
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)runx[runnumber], (double)idleheight / (double)runy[runnumber], 1.0f));
                knight->settranform(0, glm::vec3(0.0f, -runsumy / 2, 0.0f));

                if (running == 1)
                    knight->settranform(0, glm::vec3(-runsumx / 2, 0.0f, 0.0f));

                else if (running == -1)
                    knight->settranform(0, glm::vec3(runsumx / 2, 0.0f, 0.0f));

                running = 0;
                runnumber = 0;
                runframenumber = 0;
                runnumber1 = 0;
                running = 0;

            }
            slideframeno++;
            sliding = 1;
            *knighttex =knightslide;

            knight->settranform(0, glm::vec3(playerdirectionx * slidespeed*deltatime, 0.0f, 0.0f));

            dynamicspritespaceloop(knight, &slidenumber1, &slidenumber, &slideframeno, slideanispeed, FPS, &slidesumx, &slidesumy, playerdirectionx, knightslide.width, knightslide.height, slidex, slidey, &sliding, slidespritesno,idlewidth,idleheight);
        }
    };

    void prone(GLFWwindow* window, objectspace* knight, objecttexture* knighttex, objecttexture knightprone, int pronekey = NULL,int pronekeyright =NULL,int pronekeyleft = NULL)
    {
        pronetoggletime -= 1.0f;
        if (glfwGetKey(window, pronekey) == GLFW_PRESS)
        {
            if (proneing == 1 &&pronetoggletime <0)
            {
                proneing = 0;
                //knight->settranform(0.0f, glm::vec3(0.0f, knight->length1.y / 2, 0.0f));
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)pronex[0],  (double)idleheight/ (double)proney[0] , 1.0f));
                pronetoggletime = FPS/4;
               
            }
            else if (proneing == 0 &&pronetoggletime <0)
            {
                proneing = 1;
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)pronex[0] / (double)idlewidth, (double)proney[0] / (double)idleheight, 1.0f));
                knight->settranform(0.0f, glm::vec3(0.0f, -((double)knight->length1.y*(1 - ((double)proney[0] / (double)idleheight))), 0.0f));
               pronetoggletime = FPS/4;
                *knighttex = knightprone;

                knight->add1 = 0.0f;
                knight->add = (double)pronex[0]/(double)knightprone.width;
                knight->addy =1 - ((double)proney[0] / (double)knightprone.height);;
                knight->texdirectionx = 1;
                
            }
        }
/*
        if (jumping == 0 && attacking == 0 && sliding == 0 && falling == 0 &&proneing ==1)
        {
            if (glfwGetKey(window, pronekeyright) == GLFW_PRESS || glfwGetKey(window, pronekeyleft) == GLFW_PRESS)
            {
                proneframenumber++;
                if (pronemoving == 0)  //when starting to pronemove
                {
                    gamestart = 1;
                    proneframenumber = 0;
                    pronesumx = 0;
                    pronesumy = 0;
                }
                if (glfwGetKey(window, pronekeyright) == GLFW_PRESS)
                {
                    playerdirectionx = 1;
                    pronemoving = 1;
                    knight->settranform(0.0f, glm::vec3(pronemovspeed * deltatime, 0.0f, 0.0f));
                }
                else
                {
                    playerdirectionx = -1;
                    pronemoving = -1;
                    knight->settranform(0.0f, glm::vec3(-pronemovspeed * deltatime, 0.0f, 0.0f));

                }

                *knighttex = knightprone;
                dynamicspritespacecont(knight, &pronenumber1, &pronenumber, &proneframenumber, proneanispeed, FPS, &pronesumx, &pronesumy, playerdirectionx, knightprone.width, knightprone.height, pronex, proney, pronemoving, pronespritesno);
            }

            else if (pronemoving == 1 || pronemoving == -1) //when the pronening is about to stop 
            {
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)pronex[pronenumber], (double)idleheight / (double)proney[pronenumber], 1.0f));
                knight->settranform(0, glm::vec3(0.0f, -pronesumy / 2, 0.0f));

                if (pronemoving == 1)
                    knight->settranform(0, glm::vec3(-pronesumx / 2, 0.0f, 0.0f));

                else if (pronemoving == -1)
                    knight->settranform(0, glm::vec3(pronesumx / 2, 0.0f, 0.0f));

                pronemoving = 0;
                pronenumber = 0;
                proneframenumber = 0;
                pronenumber1 = 0;
            }
            else
            {
                pronenumber = 0;
                pronenumber1 = 0;
                proneframenumber = 0;
                pronemoving = 0;
            }
        }
        */


    }

    void fall(GLFWwindow* window,objectspace* knight,int runwalkkeyleft =NULL,int runwalkkeyright =NULL)
    {
        if ( falling == 1)
        {
            falltime1 = falltime;
            falltime += deltatime;

            falling = 1;
            knight->settranform(0, glm::vec3(0, -20.0 * g * ((falltime * falltime) - (falltime1 * falltime1)), 0));

            if (falldirectionx == 1)
            {
                if (glfwGetKey(window, runwalkkeyright) == GLFW_RELEASE)
                    fallspeedx = 0;

                knight->settranform(0, glm::vec3(fallspeedx * deltatime, 0, 0));
            }
            else if (falldirectionx == -1)
            {
                if (glfwGetKey(window, runwalkkeyleft) == GLFW_RELEASE)
                    fallspeedx = 0;

                knight->settranform(0, glm::vec3(-fallspeedx * deltatime, 0, 0));
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
                walksumx = 0;
                walksumy = 0;
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
                runsumx = 0;
                runsumy = 0;
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
               
                knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)slidex[slidenumber], (double)idleheight / (double)slidey[slidenumber], 1.0f));
                knight->settranform(0, glm::vec3(0.0f, -slidesumy / 2, 0.0f));

                distance = slidespeed * deltatime;
                             
                sliding = 0;
                slidenumber = 0;
                slidenumber1 = 0;
                slideframeno = 0;

            }
            else if (falling != 0)
            {
                fallspeedx = 0;
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
            if (glm::distance(knight->center1.x, box->center1.x) <= ((knight->length1.x + box->length1.x) / 2)-box->length1.x/100 && glm::distance(knight->center1.y, box->center1.y) <= (knight->length1.y + box->length1.y) / 2 && knight->center1.y > (box->center1.y + box->length1.y/2))
            {
               if(jumping == 1)
                    knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)(((double)jump1widthb - (double)jump1widtha) * (double)jumptexwidth), 1.0f, 1.0f));
                   

               else if(jumping ==2)
                    knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)(((double)jump2widthb - (double)jump2widtha) * (double)jumptexwidth), 1.0f, 1.0f));
                
               

                knight->settranform(0, glm::vec3(0.0f, -(glm::distance(knight->center1.y, box->center1.y) - (knight->length1.y + box->length1.y) / 2), 0.0f));
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
        else if((glm::distance(knight->center1.x, box->center1.x) >= ((box->length1.x / 2 + knight->length1.x / 2)) && falling == 0) && box->ontop==1 &&jumping ==0)
        {
            falling = 1;
            falltime = 0;
            falltime1 = 0;

            if (running != 0)
            {
                fallspeedx = runspeed;
            }
            else if (walking != 0)
                fallspeedx =walkspeed/4;

            else
                fallspeedx = 0;

            if (playerdirectionx == 1)
                falldirectionx = 1;

            else
                falldirectionx = -1;
            
        }

    }

    void bottomcollisionsolid(objectspace* knight, objectspace* box)
    {
        if (jumping != 0 &&falling ==0)
        {
            if (glm::distance(knight->center1.x, box->center1.x) <= (knight->length1.x + box->length1.x) / 2 && glm::distance(knight->center1.y, box->center1.y) <= (knight->length1.y + box->length1.y) / 2 && knight->center1.y < box->center1.y)
            {
                if (jumping == 1)
                    knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)(((double)jump1widthb - (double)jump1widtha) * (double)jumptexwidth), 1.0f, 1.0f));


                else if (jumping == 2)
                    knight->changemodel(0, glm::vec3(0.0f), glm::vec3((double)idlewidth / (double)(((double)jump2widthb - (double)jump2widtha) * (double)jumptexwidth), 1.0f, 1.0f));

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

    objecttexture knightattack = objecttexture("textures/attack.png", 1);

    objecttexture knightjump = objecttexture("textures/jump.png", 1);

    objecttexture knightidle = objecttexture("textures/idle.png", 1);

    objecttexture knightslide = objecttexture("textures/slide1.png", 1);

    objecttexture knightrun = objecttexture("textures/run.png", 1);

    objecttexture knightwalk = objecttexture("textures/walk.png", 1);

    objecttexture knightprone = objecttexture("textures/prone.png", 1);
    void initialize()
    {
        knight.intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        knight.setmodel(0, glm::vec3(0.5f, -0.9f, 0.0f), glm::vec3(0.25f));
        knighttex = knightidle;
        knight.addy = 0.0f;

         knightmovement.jumpspeedy = 0.28f;
         knightmovement.jumpspeedx = 0.2f;
         knightmovement.attanispeed = 2.5f;
         knightmovement.slidespeed = 0.6f;
         knightmovement.slideanispeed = 1.5f;
         knightmovement.walkspeed = 0.15f;
         knightmovement.walkanispeed = 1.4f;
         knightmovement.runspeed = 0.2f;
         knightmovement.runanispeed = 1.7f;
         knightmovement.jumptexwidth = knightjump.width;

         std::stringstream geek;
         std::string name;
         std::ifstream file("characters/knight.txt");
         
         //for attack
         for (int i = 0; i > -1; i++)
         {
             std::getline(file, name);

             if (name == "attack")
             {
                 break;
             }
         }
         std::getline(file, name);
         geek = (std::stringstream)name;

         geek >> knightmovement.attspritesno;

         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.attspritesno; i++)
         {
             float temp;
             geek >> temp;
             knightmovement.attackx[i] = temp * knightattack.width;
         }
         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.attspritesno; i++)
         {
             float temp;
             geek >> temp;
             knightmovement.attacky[i] = temp*knightattack.height;
         }
         //for slide
         for (int i = 0; i > -1; i++)
         {
             std::getline(file, name);

             if (name == "slide")
             {
                 break;
             }
         
         }
         
         std::getline(file, name);
         geek = (std::stringstream)name;

         geek >> knightmovement.slidespritesno;
         
         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.slidespritesno; i++)
         {
             float temp = 0;
             geek >> temp;
             knightmovement.slidex[i] = temp * knightslide.width;
         }
         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.slidespritesno; i++)
         {
             float temp = 0;
             geek >> temp;
             knightmovement.slidey[i] = temp * knightslide.height;
         }
         //for run

         for (int i = 0; i > -1; i++)
         {
             std::getline(file, name);

             if (name == "run")
             {
                 break;
             }
            
         }
         std::getline(file, name);
         geek = (std::stringstream)name;

         geek >> knightmovement.runspritesno;
         
         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.runspritesno; i++)
         {
             float temp = 0;
             geek >> temp;
             knightmovement.runx[i] = temp * knightrun.width;
         }
         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.runspritesno; i++)
         {
             float temp = 0;
             geek >> temp;
             knightmovement.runy[i] = temp * knightrun.height;
         }
         //for walk

         for (int i = 0; i > -1; i++)
         {
             std::getline(file, name);

             if (name == "walk")
             {
                 break;
             }
         }
         std::getline(file, name);
         geek = (std::stringstream)name;

         geek >> knightmovement.walkspritesno;
         
         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.walkspritesno; i++)
         {
             float temp = 0;
             geek >> temp;
             knightmovement.walkx[i] = temp * knightwalk.width;
         }
         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.walkspritesno; i++)
         {
             float temp = 0;
             geek >> temp;
             knightmovement.walky[i] = temp * knightwalk.height;
         }

         //for proning

         for (int i = 0; i > -1; i++)
         {
             std::getline(file, name);

             if (name == "prone")
             {
                 break;
             }

         }
         std::getline(file, name);
         geek = (std::stringstream)name;

         geek >> knightmovement.pronespritesno;

         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.pronespritesno; i++)
         {
             float temp = 0;
             geek >> temp;
             knightmovement.pronex[i] = temp * knightprone.width;
         }
         std::getline(file, name);
         geek = (std::stringstream)name;

         for (int i = 0; i < knightmovement.pronespritesno; i++)
         {
             float temp = 0;
             geek >> temp;
             knightmovement.proney[i] = temp * knightprone.height;
         }

         //for idlewidth and idleheight

         for (int i = 0; i > -1; i++)
         {
             std::getline(file, name);

             if (name == "idle")
             {
                 break;
             }

         }
         std::getline(file, name);
         geek = (std::stringstream)name;
         float temp =0;
         geek >> temp;
         knightmovement.idlewidth = temp * knightidle.width;

         std::getline(file, name);
         geek = (std::stringstream)name;
         temp =0;
         geek >> temp;
         knightmovement.idleheight = temp* knightidle.height;
         //for jumping sprite values

         for (int i = 0; i > -1; i++)
         {
             std::getline(file, name);

             if (name == "jump1")
             {
                 break;
             }
         }
         std::getline(file, name);
         geek = (std::stringstream)name;
         geek >> knightmovement.jump1widtha;

         std::getline(file, name);
         geek = (std::stringstream)name;
         geek >> knightmovement.jump1widthb;

         for (int i = 0; i > -1; i++)
         {
             std::getline(file, name);

             if (name == "jump2")
             {
                 break;
             }
         }
         std::getline(file, name);
         geek = (std::stringstream)name;
         geek >> knightmovement.jump2widtha;

         std::getline(file, name);
         geek = (std::stringstream)name;
         geek >> knightmovement.jump2widthb;

         for (int i = 0; i > -1; i++)
         {
             std::getline(file, name);

             if (name == "jump3")
             {
                 break;
             }
         }
         std::getline(file, name);
         geek = (std::stringstream)name;
         geek >> knightmovement.jump3widtha;

         std::getline(file, name);
         geek = (std::stringstream)name;
         geek >> knightmovement.jump3widthb;

    }

    void knightanimations(GLFWwindow *window)
    {
     
        knightmovement.jump(window, &knight, &knighttex, &knightjump,GLFW_KEY_SPACE);

        knightmovement.attack(window, &knight, &knighttex, &knightattack, GLFW_MOUSE_BUTTON_LEFT);

        knightmovement.slide(window, &knight, &knighttex, knightslide, GLFW_KEY_LEFT_SHIFT);
             
        knightmovement.fall(window,&knight,GLFW_KEY_A,GLFW_KEY_D);

        knightmovement.walk(window, &knight, &knighttex, knightwalk, GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_LEFT_CONTROL);

        knightmovement.run(window, &knight, &knighttex, knightrun, GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_LEFT_CONTROL);

        knightmovement.prone(window, &knight, &knighttex,knightprone, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_A);

        knightmovement.idle(window, &knight, &knighttex, knightidle); //always put this in last
    }
    
    void draw()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, knighttex.texture);
        knight.drawquad();
    }
    
    void deinitialize()
    {
        knight.deinitialize();
    }

};

class Level0
{
public:
    
    objectspace bg;
    
    objectspace bg2;

    objectspace container[15];

    int containerno = 15; //when adding new containers do comment the game.getmatrix() function for one time

    objectspace coins[4];

    int coinnumber = 4;

    objectspace slime;

    objecttexture bgtex = objecttexture("textures/bg.png", 1);

    objecttexture containertex = objecttexture("textures/container.jpg", 2);

    objecttexture cointex = objecttexture("textures/coins.png", 1);

    objecttexture tilemaptex = objecttexture("textures/level00.png", 1);

    objecttexture slimeidle = objecttexture("textures/slimeidle.png", 1);

    objecttexture slimehurt = objecttexture("textures/slimehurt.png", 1);

    objecttexture slimedie = objecttexture("textures/slimedie.png", 1);


    //related to background moving
    float slidebgspeed = 0.03f;

    //general
    int changecursor = 0;

    //related to frames
    int framenumber = 1;
    double framestart, frameend, deltatime = 0;
    float spriteanimationspeed = 1.3f; 
    float add = 0;
    float add1 = 0;

    //related to coins
    float coinanispeed = 2.3f;
    float coinnsprites = 6;

    //related to slime
    float slimeanispeed = 1.5f;
    float slimensprites = 25;


    Knight knightclass;

    void transform()
    {
        framenumber++;
        if (framenumber == FPS/spriteanimationspeed)
            framenumber = 1;

    }

    void initialize()
    {
        bg.intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        bg.setmodel(0, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(6.0f, 3.0f, 1.0f));

        bg2.intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        bg2.setmodel(0, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(6.0f, 3.0f, 1.0f));

        knightclass.initialize();

        for (int i = 0; i < containerno; i++)
        {

            container[i].intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
            container[i].setmodel(0, glm::vec3(-0.5f, -0.7f, 0.0f), glm::vec3(0.5f));
        }


        for (int i = 0; i < coinnumber; i++)
        {
            coins[i].intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
            coins[i].setmodel(0.1f, glm::vec3(-0.7f, 0.7f, 0.0f));
       
        }
        slime.intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
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
           if( mousetranslatesprite(window, &container[i], &changecursor)==1)
                   break;
         }
         for (int i = 0; i < coinnumber; i++)
            {
                if (mousetranslatesprite(window, &coins[i], &changecursor) == 1)
                    break;
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

        for (int i = 0; i <containerno; i++)
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
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS )
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
        
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            if (knightclass.knightmovement.gamestart == 1)
            {
                bg.settranform(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));
                bg2.settranform(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));

                knightclass.knight.settranform(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));

                for (int i = 0; i < containerno; i++)
                {
                    container[i].settranform(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].settranform(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));
                }
                slime.settranform(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));


            }
            else
            {

                bg.setview(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));
                bg2.setview(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));

                knightclass.knight.setview(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));
                for (int i = 0; i < containerno; i++)
                {
                    container[i].setview(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].setview(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));

                }
                slime.setview(0, glm::vec3(0.0f, -slidebgspeed, 0.0f));

            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if (knightclass.knightmovement.gamestart == 1)
            {
                bg.settranform(0, glm::vec3(0.0f, slidebgspeed, 0.0f));
                bg2.settranform(0, glm::vec3(0.0f, slidebgspeed, 0.0f));

                knightclass.knight.settranform(0, glm::vec3(0.0f, slidebgspeed, 0.0f));
                for (int i = 0; i < containerno; i++)
                {
                    container[i].settranform(0, glm::vec3(0.0f, slidebgspeed, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].settranform(0, glm::vec3(0.0f, slidebgspeed, 0.0f));

                }
                slime.settranform(0, glm::vec3(0.0f, slidebgspeed, 0.0f));

            }
            else
            {

                bg.setview(0, glm::vec3(0.0f, slidebgspeed, 0.0f));
                bg2.setview(0, glm::vec3(0.0f, slidebgspeed, 0.0f));

                knightclass.knight.setview(0, glm::vec3(0.0f, slidebgspeed, 0.0f));
                for (int i = 0; i < containerno; i++)
                {
                    container[i].setview(0, glm::vec3(0.0f, slidebgspeed, 0.0f));
                }
                for (int i = 0; i < coinnumber; i++)
                {
                    coins[i].setview(0, glm::vec3(0.0f, slidebgspeed, 0.0f));

                }
                slime.setview(0, glm::vec3(0.0f, slidebgspeed, 0.0f));


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

        for(int i =0;i<coinnumber;i++)
        knightclass.knightmovement.collectiblecollision(&knightclass.knight, &coins[i],"audio/coin touch.wav");

        //coins
        for (int i = 0; i < coinnumber; i++)
        {
            coins[i].nsprites = coinnsprites;
            
            add = (1 / coins[i].nsprites) * (int)((framenumber *coinanispeed * coins[i].nsprites) / FPS);
            add1 = (1 / coins[i].nsprites) * ((int)((framenumber * coinanispeed * coins[i].nsprites) / FPS) -1);
            coins[i].add = add;
            coins[i].add1 = add1;
            coins[i].addy = 0.0f;
        }

        //slime
        slime.nsprites = slimensprites;

        add = (1 / slime.nsprites) * (int)((framenumber * slimeanispeed * slime.nsprites) / FPS);
        add1 = (1 / slime.nsprites) * ((int)((framenumber * slimeanispeed * slime.nsprites) / FPS) - 1);

        slime.add = add;
        slime.add1 = add1;
        slime.addy = 0.0f;
    }

    void draw()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bgtex.texture);
        bg2.drawquad();
        

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tilemaptex.texture);
        bg.drawquad();
        
        knightclass.draw();

        if (knightclass.knightmovement.gamestart == 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, containertex.texture);
            
            for (int i = 0; i < containerno; i++)
                container[i].drawquad();
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cointex.texture);
        for (int i = 0; i < coinnumber; i++)
        {           
            if (coins[i].contact == 0)
                coins[i].drawquad();
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, slimeidle.texture);
        slime.drawquad();

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
        bg.intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        bg.setmodel(0, glm::vec3(0.0f, -0.75f, 0.0f), glm::vec3(20.0f, 0.5f, 1.0f));

        container[0].intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        container[0].setmodel(0, glm::vec3(0.5f, 0.7f, 0.0f), glm::vec3(0.5f));

        container[1].intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        container[1].setmodel(0, glm::vec3(0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        container[2].intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        container[2].setmodel(0, glm::vec3(-0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        container1[0].intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        container1[0].setmodel(0, glm::vec3(0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        container1[1].intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        container1[1].setmodel(0, glm::vec3(-0.5f, 0.7f, 0.0f), glm::vec3(0.5f));

        container1[2].intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        container1[2].setmodel(0, glm::vec3(-0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        container1[3].intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        container1[3].setmodel(0, glm::vec3(-0.5f, -0.7f, 0.0f), glm::vec3(0.5f));

        pointer.intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
        pointer.setmodel(0, glm::vec3(-0.5f, 0.7f, 0.0f), glm::vec3(0.25f));

        pointer1.intitialize(objvertices, indices, "attacksprite.vs", "attacksprite.fs");
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
            pointer.texdirectionx = -1;

            if (pointerpos == 0 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                *currentclass = 0;

            if (pointerpos == 2 && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, 1);
        }
        if (menustate == 1)
        {
            pointer1.texdirectionx = -1;

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
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, starttex.texture);
            container[0].drawquad();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, optionstex.texture);
            container[1].drawquad();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, exittex.texture);
            container[2].drawquad();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, arrowtex.texture);
            pointer.drawquad();
        }
        else if (menustate == 1)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, resumetex.texture);
            container1[0].drawquad();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, restarttex.texture);
            container1[1].drawquad();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, optionstex.texture);
            container1[2].drawquad();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, exittex.texture);
            container1[3].drawquad();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, arrowtex.texture);
            pointer1.drawquad();
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
        level0.knightclass.knightmovement.gamestart = 0;
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

    GLFWwindow* window = glfwCreateWindow(scrwidth, scrheight, "A Knight's Tale",NULL , NULL);

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
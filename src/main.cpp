#include "config.h"

#include "nonoIcon.h"

#include "nonogram.h"

#include "benchmarking.h"

void drawWorkingNonogramMatrix(ImDrawList* drawList, nonogram* RandoNono, std::vector<std::vector<bool>> *matrix, float horOffset, float verOffset, float horWidth, float verWidth);
void drawSolutionNonogramMatrix(ImDrawList* drawList, nonogram* RandoNono, float horOffset, float verOffset, float horWidth, float verWidth);

int main(){
    //Uncomment if benchmarking wanted, Uncomment #include "benchmarking.h" too
    //getStandardDeviationDuration(200); //This takes around two hours.
    //runCoveragePercentage(52); //This takes around half an hour
    //std::cout << std::endl;
    //runDFSBenchmark(100); //NP Complete problem, exponential time complexity. will outlast earth's lifespan
    
    //default values
    int* rowsIn = new int(5);
    int* colsIn = new int(5);

    //CREATE RANDOM NONOGRAM
    nonogram* RandoNono = new nonogram(*rowsIn, *colsIn);
    RandoNono->solveZeroCase();
    //RandoNono->print(); //Debug Statement 
    //RandoNono->mainLogicFuux();

    GLFWwindow* window;
    if(!glfwInit()){
        std::cerr << "GLFW NOT FOUND, FAILED TO INITIALIZE";
        return 1;
    }

    //create window. 
    window = glfwCreateWindow(640, 640, "Nonogram", NULL, NULL);
    glfwMakeContextCurrent(window);

    GLFWimage icons[1];
    icons[0].pixels = nonoIcon; //this icon is defined as a global variable in nonoIcon.h
    icons[0].height= NONOICON_HEIGHT;
    icons[0].width = NONOICON_WIDTH;
    
    glfwSetWindowIcon(window, 1, icons);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        std::cerr << "Failed loading openGL with Glad";
        return 1;
    }

    glClearColor(0.025f, 0.025f, 0.025f, 1.0f); //background color.

    int w,h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0,0,w,h);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    bool* renderText = new bool(1);
    bool* seeSolution = new bool(0);


    //
    ///MAIN LOOP FOR WINDOW HERE
    //

    //DEBUG PRINTING RANDONONO
    //RandoNono->print();

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        int w,h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0,0,w,h);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //
        /// Text for numbers
        //
        
        float horWidth, OneNHalfHorWidth, verWidth, OneNhalfVerWidth ;
  
        ImVec2 textSize = ImGui::CalcTextSize("1");

        float verOffsetTxt = ( (RandoNono->maxVer) * textSize.y); //this is the vertical offset caused when writing the row text input for nono
        float horOffsetTxt = ( (RandoNono->maxHor) * textSize.x); //this is the horizontal offset caused when writing the cols text input for nono
  
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        if(*renderText){

        horWidth = ((w - ( (w/40) + horOffsetTxt + 15.0f))/(*colsIn));
        OneNHalfHorWidth = horWidth/2 + horOffsetTxt + textSize.x/2 + 5.0f;
        verWidth = ((h - ( (h/40) + verOffsetTxt + 15.0f))/(*rowsIn));
        OneNhalfVerWidth = verWidth/2 + verOffsetTxt + textSize.y/2;

            for(uintmax_t i =0; i< RandoNono->rows; i++ ){
                drawList->AddText(NULL, 13.0f, 
    
                ImVec2(10.0f, OneNhalfVerWidth  + (verWidth * i)),
    
                IM_COL32(200, 200, 200, 235), &((RandoNono->nonoInputString[i])[0]));
            }
            for(uintmax_t i =0; i< RandoNono->cols; i++ ){
                drawList->AddText(NULL, 13.0f, 
    
                ImVec2(OneNHalfHorWidth + (horWidth * i), 10.0f), 
    
                IM_COL32(200, 200, 200, 235), &((RandoNono->nonoInputString[(RandoNono->rows + i)])[0]));
            }

        }


        //
        /// DRAW THE SQUARE MATRIX ITSELF
        //


        if(!*renderText){
            horWidth = (w/(*colsIn + 2));
            verWidth = (h/(*rowsIn + 2));

            if(!*seeSolution){
                drawWorkingNonogramMatrix(drawList, RandoNono, &RandoNono->nonoWorking, horWidth, verWidth, horWidth, verWidth);
            }else{
                drawSolutionNonogramMatrix(drawList, RandoNono, horWidth, verWidth, horWidth, verWidth);
            }
        }else {
            verOffsetTxt += 15.0f;
            horOffsetTxt += 15.0f;

            if(!*seeSolution){

                drawWorkingNonogramMatrix(drawList, RandoNono, &RandoNono->nonoWorking, horOffsetTxt, verOffsetTxt, horWidth, verWidth);
            } else{
                drawSolutionNonogramMatrix(drawList, RandoNono, horOffsetTxt, verOffsetTxt, horWidth, verWidth);
            }
        }
        

        //draw_list->AddRectFilled(ImVec2(horWidth, verWidth), ImVec2(horWidth*2.0f, verWidth*2.0f), IM_COL32(60, 0, 0, 255));

        ImGui::Begin("Reform Matrix");
        ImGui::SetNextItemWidth(100.0f);
        ImGui::InputInt("Number of Rows", rowsIn, 5);
        ImGui::SetNextItemWidth(100.0f);
        ImGui::InputInt("Number of Cols", colsIn, 5);


        
        if (ImGui::Button("Recreate")){
            if(*rowsIn > 0 && *colsIn >0){
                delete RandoNono;
                RandoNono = new nonogram(*rowsIn, *colsIn);
            }
        }

        if (ImGui::Button("Pure Logic")){
            RandoNono->solveLogicMethod(&RandoNono->nonoWorking);
        }
 
        if (ImGui::Button("DFS")){
            
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            //ImGui::Begin("DFSWorking");

            RandoNono->solveDFS(window);


            //ImGui::Render();
            //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            //glfwSwapBuffers(window);
            continue;
        }

        ImGui::Checkbox("Show Solution", seeSolution);
        ImGui::Checkbox("Show Numbers", renderText);
        

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    delete RandoNono;

    glfwTerminate();
    return 0;
}

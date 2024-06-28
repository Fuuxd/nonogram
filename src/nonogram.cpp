#include "nonogram.h"

/// @brief Create a random nonogram with a set # of rows and cols
/// @param rowsIn Number of Rows. must be an unsigned integer <= 65,535
/// @param colsIn Number of Columns. must be an unsigned integer <= 65,535
nonogram::nonogram(uint16_t rowsIn, uint16_t colsIn)
{
    rows = rowsIn;
    cols = colsIn;

    //
    /// Generate random matrix to solve 
    //

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(seed);
    std::uint64_t data;
    
    //solution to be worked in in the nonoWorking matrix
    std::vector<std::vector<bool>> matrix2( (rows), std::vector<bool>( (2 * cols), false) );
    nonoWorking = matrix2;
    nonoWorkingDFS = matrix2;

    // Create the matrix from the binary representation
    unsigned int bitcount = 0;
    std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols, false));

    for (uint16_t i = 0; i < rows; ++i) {
        for (uint16_t j = 0; j < cols; ++j) {
            if(!bitcount){ 
                data = rng();
                bitcount = 64;
            }
            matrix[i][j] = data & 1;
            data >>= 1;
            bitcount--;
        }
    }

    nonoSolved = matrix;

    //
    /// Create nonogram input (vector of vectors of ints) from created matrix above
    //

    //create first rows inputs, top to bottom
    for(uint16_t i = 0; i < rows; i++){
        uint16_t inputMax{};
        uint16_t inputSum{};
        std::vector<uint16_t> toBePushed;
        uint16_t counter  = 0;
        bool concurring = 0;
        for(uint16_t j = 0; j < cols; j++){
            if(nonoSolved[i][j]){
                concurring = 1;
                counter++;
                inputSum++;
            } else if(concurring){
                toBePushed.push_back(counter);
                if(counter > inputMax) inputMax = counter;
                counter = 0;
                concurring = 0;
            }
        }
        if(counter > 0){
            toBePushed.push_back(counter);
            if(counter > inputMax) inputMax = counter;
            counter = 0;
            concurring = 0;
        } else if (toBePushed.size() < 1){
            toBePushed.push_back(0);
        }
        nonoInputSum.push_back(inputSum);
        nonoInputMax.push_back(inputMax);
        nonoInput.push_back(toBePushed);
    }

    //create next cols inputs, left to right
    for(uint16_t i=0; i < cols; i++) {//more pagefaults
        uint16_t inputMax{};
        uint16_t inputSum{};
        std::vector<uint16_t> toBePushed;
        uint16_t counter = 0;
        bool concurring =0;
        for(uint16_t j=0; j < rows; j++){
            if(nonoSolved[j][i]){
                concurring = 1;
                counter++;
                inputSum++;
            } else if(concurring){
                toBePushed.push_back(counter);
                if(counter > inputMax) inputMax = counter;
                counter = 0;
                concurring = 0;
            }
        }
        if(counter > 0){
            toBePushed.push_back(counter);
            if(counter > inputMax) inputMax = counter;
            counter = 0;
            concurring = 0;
        } else if (toBePushed.size() < 1){
            toBePushed.push_back(0);
        }
        nonoInputSum.push_back(inputSum);
        nonoInputMax.push_back(inputMax);
        nonoInput.push_back(toBePushed);
    }

    //
    /// Create String Concept for its display
    //

    //this could be excluded from a "pure" only solver no UI but the time it
    //takes to create the nonogram and its structures is not part of the benchmark itself (actually solving the nonogram)

    for(uint16_t i = 0; i < rows; i++){
        std::ostringstream ossInts;
        
        for (size_t j = 0; j< nonoInput[i].size(); j++){
            if(j != 0){
                ossInts << ' ';
            }
            ossInts << nonoInput[i][j];
        }
        if(maxHor < ossInts.str().size()) maxHor = ossInts.str().size();
        nonoInputString.push_back(ossInts.str());
    }

    for(uint16_t i = 0; i < cols; i++){
        std::ostringstream ossInts;
        
        for (size_t j = 0; j< nonoInput[(rows +i)].size(); j++){
            if(j != 0){
                ossInts << '\n';
            }
            ossInts << nonoInput[(rows +i)][j];
        }
        if(maxVer < nonoInput[(rows +i)].size()) maxVer = nonoInput[(rows +i)].size();
        nonoInputString.push_back(ossInts.str());
    }
    
}

nonogram::nonogram(const int *data){
    size_t currIndexData = 0;
    cols = (uint16_t)data[currIndexData];
    currIndexData++;
    rows = (uint16_t)data[currIndexData];
    currIndexData++;

    std::vector<std::vector<bool>> matrix2( (rows), std::vector<bool>( (2 * cols), false) );
    nonoWorking = matrix2;
    nonoWorkingDFS = matrix2;

    std::vector<std::vector<uint16_t>> columnVectors;
    std::vector<uint16_t> maxesCols;
    std::vector<uint16_t> sumsCols;
    for(uint16_t i = 0; i < cols; i++){
        uint16_t sizeOfColVec = (uint16_t)data[currIndexData];
        currIndexData++;
        std::vector<uint16_t> toBePushed;
        uint16_t maxColPush = 0, sumColPush = 0;
        for(uint16_t j = 0; j < sizeOfColVec; j++){
            toBePushed.push_back((uint16_t)data[currIndexData]);
            sumColPush += (uint16_t)data[currIndexData];
            if( (uint16_t)data[currIndexData] > maxColPush){
                maxColPush = (uint16_t)data[currIndexData];
            }
            currIndexData++;
        }
        maxesCols.push_back(maxColPush);
        sumsCols.push_back(sumColPush);
        columnVectors.push_back(toBePushed);
    }

    for(uint16_t i = 0; i < rows; i++){
        uint16_t sizeOfRowVec = (uint16_t)data[currIndexData];
        currIndexData++;
        std::vector<uint16_t> toBePushed;
        uint16_t maxRowPush =0, sumRowPush =0;
        for(uint16_t j = 0; j < sizeOfRowVec; j++){
            toBePushed.push_back((uint16_t)data[currIndexData]);
            sumRowPush += (uint16_t)data[currIndexData];
            if( (uint16_t)data[currIndexData] > maxRowPush){
                maxRowPush = (uint16_t)data[currIndexData];
            }
            currIndexData++;
        }
        nonoInputMax.push_back(maxRowPush);
        nonoInputSum.push_back(sumRowPush);
        nonoInput.push_back(toBePushed);
    }

    nonoInput.insert(nonoInput.end(), columnVectors.begin(), columnVectors.end());
    nonoInputMax.insert(nonoInputMax.end(), maxesCols.begin(), maxesCols.end());
    nonoInputSum.insert(nonoInputSum.end(), sumsCols.begin(), sumsCols.end());

    //
    /// Create String Concept for its display
    //

    //this could be excluded from a "pure" only solver no UI but the time it
    //takes to create the nonogram and its structures is not part of the benchmark itself (actually solving the nonogram)

    for(uint16_t i = 0; i < rows; i++){
        std::ostringstream ossInts;
        
        for (size_t j = 0; j< nonoInput[i].size(); j++){
            if(j != 0){
                ossInts << ' ';
            }
            ossInts << nonoInput[i][j];
        }
        if(maxHor < ossInts.str().size()) maxHor = ossInts.str().size();
        nonoInputString.push_back(ossInts.str());
    }

    for(uint16_t i = 0; i < cols; i++){
        std::ostringstream ossInts;
        
        for (size_t j = 0; j< nonoInput[(rows +i)].size(); j++){
            if(j != 0){
                ossInts << '\n';
            }
            ossInts << nonoInput[(rows +i)][j];
        }
        if(maxVer < nonoInput[(rows +i)].size()) maxVer = nonoInput[(rows +i)].size();
        nonoInputString.push_back(ossInts.str());
    }
}

/// @brief Destructor for nonogram object
nonogram::~nonogram()
{
    //all dynamic allocation is primarily handled by vectors that already have their own destructors
}

/// @brief print nonogram matrix (std::vector<std::vector<bool>>) as 0s and 1s to console (std::cout)
/// @param matrix a matrix holding a nonogram 
/// @param isWorkingMatrix whether the current matrix is being used to solve (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name.
void nonogram::print(std::vector<std::vector<bool>> *matrix, bool isWorkingMatrix){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if((*matrix)[i][(isWorkingMatrix * 2)*j]){
                std::cout << "1";
            } else{
                std::cout << "0";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/// @brief solve all logically obtainable data. calls mainLogic() multiple times until no more information can be obtained
void nonogram::solveLogicMethod(std::vector<std::vector<bool>> *matrix){

    this->solveZeroCase();
    float prevProgress = this->currentProgress();
    float progress;
    do{
        prevProgress = progress;
            do{
                prevProgress = progress;
                
                this->mainLogic(matrix);

                progress = this->currentProgress();
            } while(progress != prevProgress);
        this->greyoutCompletedLines();
        progress = this->currentProgress();
    }while(progress != prevProgress);
}

/// @brief if input is 0 for any row or column, mark it grey completely.
void nonogram::solveZeroCase(){
    //
    ///check for 0 case where a whole row or col is empty
    //

    //rows
    for(int i =0; i < rows; i++){
        if(nonoInput[i][0] == 0){
            for(int j =0; j< cols; j++){
                nonoWorking[i][(2*j) + 1] = 1;
            }
        }
    }

    //cols
    for(int i = rows; i < rows+cols; i++){
        if(nonoInput[i][0] == 0){
            for(int j =0; j< rows; j++){
                nonoWorking[j][(2*i) + 1] = 1;
            }
        }
    }
}

/// @brief pass through all rows and columns once, obtains all information possible this passthrough. all methods based on Wikipedia's "Solution Techniques" section
/// @param matrix a matrix where nonogram is trying to be solved (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name
void nonogram::mainLogic(std::vector<std::vector<bool>> *matrix){
    //left = top, right = bottom
    //rows
    for(int i =0; i < rows; i++){
        //this following for loop is specifically interesting to multithread/parallelize/unroll
        for(int j =0; j < nonoInput[i].size(); j++){ //iterating through current std::vector<int> 
            bool edge = 1, edgemostInt = 1;

            //start with left k to j = index of number goal
            int maxLeft = 0;  //SIMPLE BOXES
            for(int k = 0; k <= j; k++){ //iterate through indexes until j
                logicRowLeftToRight(i, j, k, &edgemostInt, &edge, &maxLeft, matrix);
            }
            
            edge =1, edgemostInt = 1;


            //now right, (starting from the right ) 
            int maxRight = cols-1;
            for(int k = nonoInput[i].size()-1; k >= j; k--){ //iterate through indexes until j
                logicRowRightToLeft(i, j, k, &edgemostInt, &edge, &maxRight, matrix);
            }      

            for(int k = maxRight; k <= maxLeft; k++){  
                if(k < 0 || k >= cols){
                    throw std::runtime_error("Pre-SegFault mainLogRows");
                }
                (*matrix)[i][2*k] = 1;
            }

        }
    }

    //cols
    //top = left, bottom = right
    for(int i =rows; i < rows+cols; i++){
        int columnsHere = i - rows;
        //this following for loop is specifically interesting to multithread/parallelize/unroll
        for(int j =0; j < nonoInput[i].size(); j++){ //iterating through current std::vector<int> 
        //keep current maximum index obtained 
            bool edge = 1, edgemostInt = 1; 

            //from top, j = index of number are getting to 
            int maxTop = 0;  //SIMPLE BOXES
            for(int k = 0; k <= j; k++){ //iterate through indexes until j
                logicColTopToBot(i, j, k, columnsHere, &edgemostInt, &edge, &maxTop, matrix);
            }
            
            edge =1, edgemostInt = 1;

            //now bottom, (bottom to top) 
            int maxBot = rows -1;
            for(int k = nonoInput[i].size()-1; k >= j; k--){ //iterate through indexes until j
                logicColBotToTop(i, j, k, columnsHere, &edgemostInt, &edge, &maxBot, matrix);
            }      

            for(int k = maxBot; k <= maxTop; k++){
                if(k < 0 || k >= rows){
                    throw std::runtime_error("Pre-SegFault mainLogCols");
                }
                (*matrix)[k][2*columnsHere] = 1;
            }

        }
    }
}

/// @brief calculate how many cells have been solved 
/// @return ratio of how filled nonoWorking matrix is 0 - 1 (so for example 0.33 is 33%)
double_t nonogram::currentProgress(){
    uintmax_t totalSquares = rows*cols;
    for(uint16_t i=0; i < rows; i++){
        for(uint16_t j =0; j < cols; j++){
            if(nonoWorking[i][2*j]){
                totalSquares--;
            }
            else if (nonoWorking[i][(2*j) + 1]){
                totalSquares--;
            }
        }
    }
    double_t printThis = (double_t)totalSquares / (double_t)(rows*cols);
    return double_t(1) - ((double_t)totalSquares / (double_t)(rows*cols));
}

/// @brief calculates what input would correspond to current solution in nonoWorking and determines if row is complete
/// @param passedIndex row index. If the passedIndex >= rows it will cause a segfault or unpredictable behaviour. 
/// @param matrix a matrix where nonogram is trying to be solved (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name
/// @return returns whether row is already complete, that is, reconstructed input == nonogram input.
bool nonogram::isRowComplete(std::vector<std::vector<bool>> *matrix, uint16_t passedIndex){
    std::vector<uint16_t> reconstructedInput;
    uint16_t counter  = 0;
    bool concurring = 0;
    for(uint16_t j = 0; j < cols; j++){
        if((*matrix)[passedIndex][2 * j]){
            concurring = 1;
            counter++;
        } else if(concurring){
            reconstructedInput.push_back(counter);
            counter = 0;
            concurring = 0;
        }
    }
    if(counter > 0){
        reconstructedInput.push_back(counter);
        counter = 0;
        concurring = 0;
    } else if (reconstructedInput.size() < 1){
        reconstructedInput.push_back(0);
    }
    return reconstructedInput == nonoInput[passedIndex];
}

/// @brief calculates what input would correspond to current solution in passed matrix and determines if column is complete
/// @param passedIndex column index. If the passedIndex >= cols it will cause a segfault or unpredictable behaviour.
/// @param matrix a matrix where nonogram is trying to be solved (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name
/// @return returns True if column is already complete, that is, reconstructed input == nonogram input. False otherwise
bool nonogram::isColComplete(std::vector<std::vector<bool>> *matrix, uint16_t passedIndex){
    
    std::vector<uint16_t> reconstructedInput;
    uint16_t counter  = 0;
    bool concurring = 0;
    for(uint16_t i = 0; i < rows; i++){
        if((*matrix)[i][2*passedIndex]){ //check if contradiction
            if((*matrix)[i][(2*passedIndex)+1]){
                return false;
            }
            concurring = 1;
            counter++;
        } else if(concurring){
            reconstructedInput.push_back(counter);
            counter = 0;
            concurring = 0;
        }
    }
    if(counter > 0){
        reconstructedInput.push_back(counter);
        counter = 0;
        concurring = 0;
    } else if (reconstructedInput.size() < 1){
        reconstructedInput.push_back(0);
    }
    
    return reconstructedInput == nonoInput[passedIndex + rows];
}

/// @brief whether column is possible or not. 
/// calculates Sum of marked cells in column and Maximum longest marked(black) run. Focuses on making sure the negative case is shown rather than the positive one. 
/// @param passedIndex column index. If the passedIndex >= cols it will cause a segfault or unpredictable behaviour.
/// @param matrix a matrix where nonogram is trying to be solved (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name
/// @return returns False if nonoInputMax < colMax or nonoSum < colSum. True otherwise
bool nonogram::isColPossible(std::vector<std::vector<bool>> *matrix, uint16_t passedIndex){
    uint16_t colSum{};
    uint16_t colMax{};
    uint16_t counter  = 0;
    bool concurring = 0;
    for(uint16_t i = 0; i < rows; i++){
        if((*matrix)[i][2*passedIndex]){ 
            if((*matrix)[i][(2*passedIndex)+1]){
                return false;
            }
            concurring = 1;
            counter++;
            colSum++;
        } else if(concurring){
            if(counter > colMax ) colMax = counter;
            counter = 0;
            concurring = 0;
        }
    }
    if(counter > 0){
        if(counter > colMax ) colMax = counter;
        counter = 0;
        concurring = 0;
    } 

    if(nonoInputMax[rows+passedIndex] < colMax) {
        return false;
    }
    if(nonoInputSum[rows+passedIndex] < colSum) {
        return false;
    }

    return true;

}

/// @brief checks if all cols are possible. calls isColPossible for all Columns.
/// @param matrix a matrix where nonogram is trying to be solved (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name
/// @return True if all columns are possible, false otherwise
bool nonogram::isAllColsPossible(std::vector<std::vector<bool>> *matrix){
    for(uint16_t i = 0; i < rows; i++){
        if(!(this->isColPossible(matrix, i))){
            return false;
        }
    }
    return true;
}

/// @brief checks if all cols are complete, thus being a col solution. calls isColComplete for all Columns.
/// @return False if any column is not complete, true otherwise.
bool nonogram::isColsSolutionDFS( std::vector<std::vector<bool>> *rowVectors ){
    if(rowVectors->size() != rows){
        std::cerr << "Checking ColsSolutionDFS with invalid rowVector Size";
        return false;
    }
    for(uint16_t i = 0; i < (rowVectors->size() - 1); i++){
        nonoWorkingDFS[i] = (*rowVectors)[i];
    }

    for(uint16_t i =0; i < cols; i++){
        if(!this->isColComplete(&nonoWorkingDFS, i)){
            //clearing the matrix not needed as isPermutationPossible() handles it
            return false;
        }
    }
    return true;
}

/// @brief calls isRowComplete or isColComplete for all rows and cols. if true for any, greys out any empty cells for that line. 
void nonogram::greyoutCompletedLines(){
    for(uint16_t i = 0; i < rows; i++){ 
        if(isRowComplete(&nonoWorking, i)){
            for(uint16_t j = 0; j < cols; j++){
                if(!nonoWorking[i][2*j]){
                    nonoWorking[i][(2 * j) + 1] = 1;
                }
            }
        }
    }
    for(uint16_t i = 0; i < cols; i++){
        if(isColComplete(&nonoWorking, i)){
            for(uint16_t j = 0; j < rows; j++){
                if(!nonoWorking[j][(2*i)]){
                    nonoWorking[j][(2 * i) + 1] = 1;
                }
            }
        }
    }

}

/// @brief optimized logic as described on wikipedia
/// @param matrix a matrix where nonogram is trying to be solved (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name
void nonogram::logicRowLeftToRight(int i, int j, int k, bool* edgemostInt, bool* edge, int* maxLeft, std::vector<std::vector<bool>> *matrix){

    int inputIntCurr =nonoInput[i][k];
    
    bool edgeFilled = 0; //to avoid filling whole matrix
    for(int l = 0; l < inputIntCurr; l++){ //iterate through matrix current integer times
        if((*maxLeft) >= cols){ //ERROR CHECKING 
            //std::cerr << "ERROR ON SIMPLE BOXES 1";
            throw std::runtime_error("Pre-SegFault EBoxes1");
        }
        //EDGE FILL
            
        if((*matrix)[i][(2*(*maxLeft))]){ //FILLED BOX
            if((*edgemostInt) && (*edge) && !edgeFilled){
                // FILL IN this edge and add grey at the end
                for(int m = 0; m < inputIntCurr; m++){
                    if( (*maxLeft)+m >= rows){
                        throw std::runtime_error("Pre-SegFault edgeFill EB1");
                    }
                    (*matrix)[i][2*((*maxLeft)+m)] = 1;
                }

                if( (*maxLeft) + inputIntCurr < cols){
                    (*matrix)[i][2*((*maxLeft)+inputIntCurr)+1] =1;
                }
                edgeFilled = 1;
            } else if((*edgemostInt) && !edgeFilled){ 
                for(int m = 0; m <inputIntCurr-l; m++){ //GLUE
                    //Glue happens 
                    if( (*maxLeft)+m >= rows){
                        throw std::runtime_error("Pre-SegFault edgeFill EB3");
                    }
                    (*matrix)[i][2* ((*maxLeft)+ m )] = 1;
                }

            } 

            
        }
        else if ((*matrix)[i][(2*(*maxLeft))+1]){ //GREY BOX - (crossed out box)
                
            //SIMPLE SPACES
            if(!(*edge) && (*edgemostInt)){ 
                //ALL PREVIOUS holes backwards until metting a grey space or getting to 0 -> become grey
                for (int m = 1 ; m <= inputIntCurr; m++){
                    if((*maxLeft)-m < 0){ // //check here to reduce indentations
                        break;
                    } else if((*matrix)[i][(2* ((*maxLeft) - m))+1]){
                        break;
                    }else{
                        (*matrix)[i][(2* ((*maxLeft) - m))+1] = 1;
                    }


                }
                
            }

            l = -1; //SIMPLE BOXES
            //if gray spaces placed erroneously, simple boxes will overfill
        }
        else{
            (*edge) = 0;// could take this out of else and add continue after the if and else if above
        }

        if(k == j && l == inputIntCurr-1){
            continue;
        }
        (*maxLeft)++;
    }


    //if k==j this is the last index 
    if(k==j && (*maxLeft) + 1 < cols && (*matrix)[i][2*( (*maxLeft) + 1)] && (*edgemostInt)){ //IF NOT OUT OF BOUNDS, check if next square (after loops above) has the NONOGRAM FILLED (SHORT CIRCUIT EVALUATION PREVENTS ERROR)

        int l = 1; //mercuryDrip need for it to be in context for next for loop
        for(l = 1; l <= inputIntCurr; l++){
            if((*maxLeft) + l < cols){
                if((*matrix)[i][(2*((*maxLeft) + l)) + 1]){  //GREY
                    //if grey adjacent to the mercury enabled spot, fill from mercury spot until integer completely filled, 
                    for(int m = 0; m <= (inputIntCurr - l); m++){
                        (*matrix)[i][((*maxLeft) - m)*2] = 1;
                    }
                    //after filling break and go to greyout below
                    break;
                } else if( !(*matrix)[i][2*((*maxLeft) + l)]){ //BLANK
                    break; // break and go to greyout below
                }
            } else{
                break;
            }
        }

        for(int m = ((*maxLeft) - inputIntCurr + 1); m < ((*maxLeft) - inputIntCurr + l); m++){
            //greyout at m = maxLeft
            (*matrix)[i][(m * 2)+1] = 1;
        }

    }                
    
    if(k != j){
        (*maxLeft)++; //space to go to next PAY CLOSE ATTENTION HERE WHEN DEBUGGING COULD CAUSE LOGIC ERRORS
    }
    
    if(!(*edge)){(*edgemostInt) =0;}
    
    return;
}

/// @brief optimized logic as described on wikipedia
/// @param matrix a matrix where nonogram is trying to be solved (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name
void nonogram::logicRowRightToLeft(int i, int j, int k, bool* edgemostInt, bool* edge, int* maxRight, std::vector<std::vector<bool>> *matrix){
        
    int inputIntCurr =nonoInput[i][k];

    bool edgeFilled = 0; //avoid filling whole matrix
    for(int l = 0; l < inputIntCurr; l++){ //iterate through matrix current integer times
        if((*maxRight) < 0){ //ERROR CHECKING 
            //std::cerr << "ERROR ON SIMPLE BOXES 2, on row:"<< i;
            throw std::runtime_error("Pre-SegFault EBoxes2");
        }
        //EDGE FILL
        
        if((*matrix)[i][(2*(*maxRight))]){ //FILLED BOX
            if((*edgemostInt) && (*edge) && !edgeFilled){
                // FILL IN this edge and add grey at the end
                for(int m = 0; m < inputIntCurr; m++){
                    if( (*maxRight)-m < 0){
                        throw std::runtime_error("Pre-SegFault Edge EB2");
                    }
                    (*matrix)[i][2*((*maxRight)-m)] = 1;
                }
                if( (*maxRight)-inputIntCurr >= 0){
                    (*matrix)[i][(2*((*maxRight)-inputIntCurr))+1] =1;
                }
                
                edgeFilled = 1;
            } else if((*edgemostInt)){
                for(int m = 0; m <inputIntCurr-l; m++){ //GLUE
                    if( (*maxRight)-m < 0){
                        throw std::runtime_error("Pre-SegFault Glue EB2");
                    }
                    (*matrix)[i][2* ((*maxRight)- m )] = 1;
                }

            } 

            
        }
        else if ((*matrix)[i][(2*(*maxRight))+1]){ //GREY BOX - (crossed out box)
            //SIMPLE SPACES
            if(!(*edge) && (*edgemostInt)){ 
                //ALL PREVIOUS holes backwards until meeting a grey space or getting to 0 -> become grey
                for (int m = 1 ; m <= inputIntCurr; m++){
                    if((*maxRight)+m >= cols){ //check here to reduce indentations
                        break;
                    } else if((*matrix)[i][(2* ((*maxRight) + m))+1]){
                        break;
                    }else{
                        (*matrix)[i][(2* ((*maxRight) + m))+1] = 1;
                    }
                }
            }
            l = -1; //SIMPLE BOXES
            //if gray spaces placed erroneously, simple boxes will overfill
        }
        else{
            (*edge) = 0;
        }
        if(k == j && l == inputIntCurr-1){
            continue;
        }
        (*maxRight)--;
        
    }

    if(k==j && (*maxRight) - 1 >= 0 && (*matrix)[i][2*( (*maxRight) - 1)] && (*edgemostInt)){ //IF NOT OUT OF BOUNDS, check if next square has the NONOGRAM FILLED (SHORT CIRCUIT EVALUATION PREVENTS ERROR)
        int l = 1; //mercuryDrip need for it to be in context for next for loop
        for(l = 1; l <= inputIntCurr; l++){
            if((*maxRight) - l >= 0){
                if((*matrix)[i][(2*((*maxRight) - l)) + 1]){  //GREY
                    //if grey adjacent to the mercury enabled spot, fill from mercury spot until integer completely filled, 
                    for(int m = 0; m <= (inputIntCurr - l); m++){
                        (*matrix)[i][((*maxRight) + m)*2] = 1;
                    }
                    //after filling break and go to greyout below
                    break;
                } else if( !(*matrix)[i][2*((*maxRight) - l)]){ //BLANK
                    break; // break and go to greyout below
                } 
            }else{
                break;
            }
        }

        for(int m = ((*maxRight) + inputIntCurr - 1); m > ((*maxRight) + inputIntCurr - l); m--){
            //greyout at m = maxRight
            (*matrix)[i][(m * 2)+1] = 1;
        }

    }

    if(k != j){
        (*maxRight)--; //space to go to next PAY CLOSE ATTENTION HERE WHEN DEBUGGING COULD CAUSE LOGIC ERRORS
    }
    if(!(*edge)){(*edgemostInt) =0;}
}

/// @brief optimized logic as described on wikipedia
/// @param matrix a matrix where nonogram is trying to be solved (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name
void nonogram::logicColTopToBot(int i, int j, int k, int columnsHere, bool* edgemostInt, bool* edge, int* maxTop, std::vector<std::vector<bool>> *matrix){
    int inputIntCurr =nonoInput[i][k];

    bool edgeFilled = 0;//avoid filling whole matrix
    for(int l = 0; l < inputIntCurr; l++){ //iterate through matrix current integer times
        if((*maxTop) >= rows){ //ERROR CHECKING 
            //std::cerr << "ERROR ON SIMPLE BOXES 3";
            throw std::runtime_error("Pre-SegFault EBoxes3");
        }
        //EDGE FILL
        
        if((*matrix)[(*maxTop)][2 * columnsHere]){ //FILLED BOX
            if((*edgemostInt) && (*edge) && !edgeFilled){
                // FILL IN this edge and add grey at the end
                for(int m = 0; m < inputIntCurr; m++){
                    if( (*maxTop)+m >= rows){
                        throw std::runtime_error("Pre-SegFault edgeFill EB3");
                    }
                    (*matrix)[(*maxTop)+m][2*columnsHere] = 1;
                }

                if( (*maxTop) + inputIntCurr < rows){
                    (*matrix)[(*maxTop)+inputIntCurr][(2*columnsHere)+1] =1;
                }
                edgeFilled = 1;
            } else if((*edgemostInt) && !edgeFilled){
                for(int m = 0; m <inputIntCurr-l; m++){ //GLUE
                    //Glue happens 
                    if( (*maxTop) + m >= rows){
                        throw std::runtime_error("Pre-SegFault Glue EB3");
                    }
                    (*matrix)[(*maxTop)+ m][2* columnsHere] = 1;
                }

            } 

            
        }
        else if ((*matrix)[(*maxTop)][(2*columnsHere)+1]){ //GREY BOX - (crossed out box)
                

            //SIMPLE SPACES
            if(!(*edge) && (*edgemostInt)){
                //ALL PREVIOUS holes backwards until meeting a grey space or getting to 0 -> become grey
                for (int m = 1 ; m <= inputIntCurr; m++){
                    if((*maxTop)-m < 0){ //check here to reduce indentations
                        break;
                    } else if((*matrix)[(*maxTop) - m][(2* columnsHere)+1]){
                        break;
                    }else{
                        (*matrix)[(*maxTop) - m][(2* columnsHere)+1] = 1;
                    }


                }
                
            }

            l = -1; //SIMPLE BOXES
            //if gray spaces placed erroneously, simple boxes will overfill
        }
        else{
            (*edge) = 0;
        }
        if (k == j && l == inputIntCurr-1){
            continue;
        }
        (*maxTop)++;
    }


    //if k==j this is the last index 
    if(k==j && (*maxTop) + 1 < rows && (*matrix)[( (*maxTop) + 1)][2*columnsHere] && (*edgemostInt)){ //IF NOT OUT OF BOUNDS, check if next square has the NONOGRAM FILLED (SHORT CIRCUIT EVALUATION PREVENTS ERROR)
        int l = 1; //mercuryDrip need for it to be in context for next for loop
        for(l = 1; l <= inputIntCurr; l++){ 
            if((*maxTop) + l < rows){
                if((*matrix)[((*maxTop) + l)][(2*columnsHere) + 1]){  //GREY
                    //if grey adjacent to the mercury enabled spot, fill from mercury spot until integer completely filled, 
                    for(int m = 0; m <= (inputIntCurr - l); m++){
                        (*matrix)[((*maxTop) - m)][columnsHere*2] = 1;
                    }
                    //after filling break and go to greyout below
                    break;
                } else if( !(*matrix)[((*maxTop) + l)][2*columnsHere]){ //BLANK
                    break; // break and go to greyout below
                }                          
            }else{
                break;  // BIG ADDITION HERE
            }
        }
        for(int m = ((*maxTop) - inputIntCurr + 1); m < ((*maxTop) - inputIntCurr + l); m++){
            //greyout at m = maxTop
            (*matrix)[m][(columnsHere * 2)+1] = 1;
                
        }

    }     
    



    if( k != j){
        (*maxTop)++; //space to go to next PAY CLOSE ATTENTION HERE WHEN DEBUGGING COULD CAUSE LOGIC ERRORS
    }
    if(!(*edge)){(*edgemostInt) =0;}
}

/// @brief optimized logic as described on wikipedia
/// @param matrix a matrix where nonogram is trying to be solved (it holds grey cells, two bools per cell). Normally denoted by "Working" in matrix name
void nonogram::logicColBotToTop(int i, int j, int k, int columnsHere, bool* edgemostInt, bool* edge, int* maxBot, std::vector<std::vector<bool>> *matrix){
    
    int inputIntCurr =nonoInput[i][k];
    
    bool edgeFilled =0; // avoid filling whole matrix
    for(int l = 0; l < inputIntCurr; l++){ //iterate through matrix current integer times
        if((*maxBot) < 0){ //ERROR CHECKING 
            //std::cerr << "ERROR ON SIMPLE BOXES 4";
            throw std::runtime_error("Pre-SegFault EBoxes4");
        }
        //EDGE FILL
        
        if((*matrix)[(*maxBot)][2* columnsHere]){ //FILLED BOX
            if((*edgemostInt) && (*edge) && !edgeFilled){ //this condition could be wonky here 
                // FILL IN this edge and add grey at the end
                for(int m = 0; m < inputIntCurr; m++){
                    if( (*maxBot)-m < 0){
                        throw std::runtime_error("Pre-SegFault edgeFill EB4");
                    }
                    (*matrix)[(*maxBot)-m][2*columnsHere] = 1;
                }
                
                if( (*maxBot)-inputIntCurr >= 0){
                    (*matrix)[(*maxBot)-inputIntCurr][(2 * columnsHere) +1] =1;
                }
                edgeFilled = 1; 
                
            } else if((*edgemostInt)){
                for(int m = 0; m <inputIntCurr-l; m++){ //GLUE
                    if( (*maxBot)-m < 0){
                        throw std::runtime_error("Pre-SegFault Glue EB4");
                    }
                    (*matrix)[(*maxBot)- m][2* columnsHere] = 1;
                }

            } 

        }
        else if ((*matrix)[(*maxBot)][(2*columnsHere)+1]){ //GREY BOX - (crossed out box)
                

            //SIMPLE SPACES
            if(!(*edge) && (*edgemostInt)){
                //ALL PREVIOUS holes backwards until meeting a grey space or getting to 0 -> become grey
                for (int m = 1 ; m <= inputIntCurr; m++){
                    if((*maxBot)+m >= rows){ //check here to reduce indentations
                        break;
                    } else if((*matrix)[(*maxBot) + m][(2* columnsHere)+1]){
                        break;
                    }else{
                        (*matrix)[(*maxBot) + m][(2* columnsHere)+1] = 1;
                    }


                }
                
            }

            l = -1; //SIMPLE BOXES
            //if gray spaces placed erroneously, simple boxes will overfill
        }
        else{
            (*edge) = 0;
        }
        if (k == j && l == inputIntCurr-1){
            continue;
        }
        (*maxBot)--;
    }

    //if k==j this is the last index 
    if(k==j && (*maxBot) - 1 >= 0 && (*matrix)[( (*maxBot) - 1)][2*columnsHere] && (*edgemostInt)){ //IF NOT OUT OF BOUNDS, check if next square has the NONOGRAM FILLED (SHORT CIRCUIT EVALUATION PREVENTS ERROR)
        int l = 1; //mercuryDrip need for it to be in context for next for loop
        for(l = 1; l <= inputIntCurr; l++){
            if((*maxBot) - l >= 0){
                if((*matrix)[((*maxBot) - l)][(2*columnsHere) + 1]){  //GREY
                    //if grey adjacent to the mercury enabled spot, fill from mercury spot until integer completely filled, 
                    for(int m = 0; m <= (inputIntCurr - l); m++){
                        (*matrix)[((*maxBot) + m)][columnsHere*2] = 1;
                    }
                    //after filling break and go to greyout below
                    break;
                } else if( !(*matrix)[((*maxBot) - l)][2*columnsHere]){ //BLANK
                    break; // break and go to greyout below
                }
            }else{
                break;
            }
        }

        for(int m = ((*maxBot) + inputIntCurr - 1); m > ((*maxBot) + inputIntCurr - l); m--){
            //greyout at m = maxBot
            (*matrix)[m][(columnsHere * 2)+1] = 1;
        }
    } 
    
    if( k != j){
        (*maxBot)--; //space to go to next PAY CLOSE ATTENTION HERE WHEN DEBUGGING COULD CAUSE LOGIC ERRORS
    }
    if(!(*edge)){(*edgemostInt) =0;}
}

/// @brief A depth first search/Inorder traversal of all possibilities. 
///Before calling this method make sure to call nonoWorkingDFS = nonoWorking and optionally solveFuuxMethod();
/// @param index Current row being checked, should be set to 0 unless called from inside the function recursively.
void nonogram::DFS(std::vector<std::vector<bool>> *rowVectors, uint16_t index, bool *solutionFound){

    if(index >= rows){ //boolean 
        if( this->isColsSolutionDFS(rowVectors) ) {
            *solutionFound = true;
        }
        return;
    }

    //TODO: change printDFS to drawNonoMatrix and link with main
    uint16_t spaceAvailable = cols - nonoInputSum[index]; //space available (permitted number of blank space) for this given input
    uint16_t whiteRuns = nonoInput[index].size() - 1; // the amount of white runs inbetween black runs for this input
    if( spaceAvailable == 0) { //This case should have been handled by logic and as such this row is already handled

        //this->printDFS();
        //this->print(&nonoWorkingDFS, true);

        std::vector<bool> rowToPush;
        for(uint16_t i = 0; i < cols; i++){
            rowToPush.push_back(true);
            rowToPush.push_back(false);
        }

        rowVectors->push_back(rowToPush);

        DFS(rowVectors, index + 1, solutionFound);
    
        if(*solutionFound){
            return;
        }else { //clear all below if solution not found
            rowVectors->pop_back();
        }

        //this->print(&nonoWorkingDFS, true);
    }
    if(whiteRuns == 0){
        //run all possibilities of this specific case;
        for(uint16_t i = 0; i <= spaceAvailable; i++){ //careful with the <=, check isPermutationPossible if confusing.
            if(isPermutationPossible(rowVectors, nonoInput[index][0], i, index)){

                //this->printDFS();
                //this->print(&nonoWorkingDFS, true);
                DFS(rowVectors, index+ 1, solutionFound);

                if(*solutionFound){
                    return;
                }else { //clear all below if solution not found
                    rowVectors->pop_back();
                }
            }

            //this->print(&nonoWorkingDFS, true);

        }
    }
    else{
        if(whiteRuns == 1){ // TODO: SO CHECK HERE basically I think trouble here, maybe go back to main and comment out solvefuux method to try compute
            spaceAvailable++;
        }
    
        for(permutationVector rowSpace(spaceAvailable, whiteRuns); !rowSpace.allPermutationsIterated; rowSpace++){
            uint16_t maxLeftmostSpace = (cols - nonoInputSum[index]) - rowSpace.getSum(); //can't reuse spaceAvailable (whiteRuns case above)
            for(uint16_t i = 0; i <= maxLeftmostSpace; i++){  //careful with the <=, check isPermutationPossible if confusing.
                if(isPermutationPossible(rowVectors, &rowSpace, i, index)){
                    //this->printDFS();

                    //this->print(&nonoWorkingDFS, true);
                    DFS(rowVectors, index + 1, solutionFound);

                    if(*solutionFound){
                        return;
                    }else {//clear all below if solution not found
                        rowVectors->pop_back();
                    }

                    //this->print(&nonoWorkingDFS, true);

                }
            }
        }
    }

    return;

}

/// @brief checks if the permutation based on the permutationVector is possible.
/// builds a candidate row based on the current logic-found matrix solution (nonoWorking), if the row being built does not clash with the logic-solution, it then assigns it to the current 
/// @param rowSpace permutationVector object being checked. 
/// @param leftmostSpace white space to the left of first black run
/// @param rowIndex current row being checked
/// @return True if permutationVector is possible for given row, False otherwise. 
bool nonogram::isPermutationPossible(std::vector<std::vector<bool>> *rowVectors, permutationVector* rowSpace, uint16_t leftmostSpace, uint16_t rowIndex){
    //DEBUG STATEMENTS
    //std::cout << "checking row:" << rowIndex << " leftmostSpace:" << leftmostSpace << " permVec:";
    //rowSpace->print();
    //std::cout << std::endl;

    for(uint16_t i = 0; i < rowVectors->size(); i++){
        nonoWorkingDFS[i] = (*rowVectors)[i];
    }
    for(uint16_t i = rowVectors->size(); i < rows; i++){
        nonoWorkingDFS[i] = nonoWorking[i];
    }

    // 
    /// CHECK IF rowSpace permutation works for current context 
    //

    //check if ROW works

    std::vector<bool> rowPermutationBuilt(2*cols);
    //clear current index to hold only the solid logic truth

    uint16_t currIndexBuilding{};
    for(uint16_t i = 0; i< leftmostSpace; i++){
        if(nonoWorkingDFS[rowIndex][(2*currIndexBuilding)]){ 
            return false;
        }
        if(currIndexBuilding >= cols){
            return false;
        }
        rowPermutationBuilt[(2*currIndexBuilding)+1] = 1;
        currIndexBuilding++;
    }

    for(size_t i = 0; i < rowSpace->getNumber().size(); i++){

        // marked cells
        for(uint16_t j = 0; j < nonoInput[rowIndex][i]; j++){
            if(nonoWorkingDFS[rowIndex][(2*currIndexBuilding)+1]){ 
                return false;
            }
            if(currIndexBuilding >= cols){
                return false;
            }
            rowPermutationBuilt[(2*currIndexBuilding)] = 1;
            currIndexBuilding++;
        }

        //greyed cells
        for(uint16_t j = 0; j < rowSpace->getNumber()[i]; j++){
            if(nonoWorkingDFS[rowIndex][(2*currIndexBuilding)]){ 
                return false;
            }
            if(currIndexBuilding >= cols){
                return false;
            }
            rowPermutationBuilt[(2*currIndexBuilding)+1] = 1;
            currIndexBuilding++;
        }

    }
    
    // final marked cells
    for(uint16_t i = 0; i < nonoInput[rowIndex][(nonoInput[rowIndex].size()-1)]; i++){
        if(nonoWorkingDFS[rowIndex][(2*currIndexBuilding)+1]){ 
            return false;
        }
        if(currIndexBuilding >= cols){
            return false;
        }
        rowPermutationBuilt[(2*currIndexBuilding)] = 1;
        currIndexBuilding++;
    }

    // fill the rest with grey
    for(;currIndexBuilding < cols; currIndexBuilding++){
        if(nonoWorkingDFS[rowIndex][(2*currIndexBuilding)]){
            return false;
        }
        if(currIndexBuilding >= cols){
            return false;
        }
        rowPermutationBuilt[(2 * currIndexBuilding) + 1] = 1;
    }

    nonoWorkingDFS[rowIndex] = rowPermutationBuilt;
    //TODO: TRY CLEARING ALL ROWS BELOW so we can try{ solvefuuxmethod } catch(){return false;)

    //
    ///check if COLS works 
    //
    try
    {
        this->solveLogicMethod(&nonoWorkingDFS);
    }
    catch(const std::exception& e)// if literally anthing wrong just clear below and return false
    {
        return false;
    }
    
    

    if(!this->isAllColsPossible(&nonoWorkingDFS)){
        return false;
    };

    rowVectors->push_back(rowPermutationBuilt);
    return true;
}

/// @brief Handles specific case where input for row is single non-zero digit
/// @param blackRun length of single black run for this row
/// @param leftmostSpace white space to the left of first black run
/// @param rowIndex current row being checked
/// @return True if permutationVector is possible for given row, False otherwise. 
bool nonogram::isPermutationPossible(std::vector<std::vector<bool>> *rowVectors, uint16_t blackRun, uint16_t leftmostSpace, uint16_t rowIndex){
    //DEBUG STATEMENTS
    //std::cout << "checking row:" << rowIndex << " leftmostSpace:" << leftmostSpace << " ";
    //std::cout << blackRun << std::endl;

    for(uint16_t i = 0; i < rowVectors->size(); i++){
        nonoWorkingDFS[i] = (*rowVectors)[i];
    }
    for(uint16_t i = rowVectors->size(); i < rows; i++){
        nonoWorkingDFS[i] = nonoWorking[i];
    }

    //
    /// CHECK IF blackRun works for current context
    //

    //check if ROW works

    std::vector<bool> rowPermutationBuilt(2*cols);
    //clear current index to hold only the solid logic truth
    ////nonoWorkingDFS[rowIndex] = nonoWorking[rowIndex];
    uint16_t currIndexBuilding{};
    for(uint16_t i = 0; i< leftmostSpace; i++){
        if(nonoWorkingDFS[rowIndex][(2*currIndexBuilding)]){ 
            return false;
        }
        rowPermutationBuilt[(2*currIndexBuilding)+1] = 1;
        currIndexBuilding++;
    }
    
    // only black run here
    for(uint16_t j = 0; j < blackRun; j++){
        if(nonoWorkingDFS[rowIndex][(2*currIndexBuilding)+1]){ 
            return false;
        }
        rowPermutationBuilt[(2*currIndexBuilding)] = 1;
        currIndexBuilding++;
    }

    // fill the rest with grey
    for(;currIndexBuilding < cols; currIndexBuilding++){
        if(nonoWorkingDFS[rowIndex][(2*currIndexBuilding)]){
            return false;
        }
        rowPermutationBuilt[(2 * currIndexBuilding) + 1] = 1;
    }

    nonoWorkingDFS[rowIndex] = rowPermutationBuilt;

    //ADD PRINTING STATEMENT HERE

    //
    ///check if COLS works 
    //

    try
    {
        this->solveLogicMethod(&nonoWorkingDFS);
    }
    catch(const std::exception& e)// if literally anthing wrong just clear below and return false
    {
        return false;
    }
    
    

    if(!this->isAllColsPossible(&nonoWorkingDFS)){
        return false;
    };

    rowVectors->push_back(rowPermutationBuilt);
    return true;
}

/// @brief A depth first search/Inorder traversal of all possibilities. wraps DFS() method. 
/// @return whether a solution was found or not. If error occurs, its assumed that a solution was not found and there is some sort of problem with the input.
bool nonogram::solveDFS(){

    //if benchmarking, consider commenting out the next line.
    //this->solveLogicMethod(&nonoWorking);
    nonoWorkingDFS = nonoWorking;
    bool solutionFound(false);
    std::vector<std::vector<bool>> rowVectors;
    try
    {
        this->DFS(&rowVectors, 0, &solutionFound);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    catch(...){
        std::cerr << "Unknown exception caught in solveDFS\n";
        return false;
    }
    
    if (solutionFound){
        nonoWorking = nonoWorkingDFS;
    }
    return solutionFound;
}
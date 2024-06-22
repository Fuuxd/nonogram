#pragma once

#include "config.h"
#include "permutationVector.h"

class nonogram
{
public: // small performance boost by struct-like behaviour of all public class.
    uint16_t rows;
    uint16_t cols;

    std::vector<std::vector<bool>> nonoSolved;
    //nonoworking will hold three states per cell: not marked, marked, certainly not marked (grey space / X)
    std::vector<std::vector<bool>> nonoWorking; //working on solution here holds two bools per cell
    std::vector<std::vector<bool>> nonoWorkingDFS; //working on solution FOR DFS here holds two bools per cell
    std::vector<std::vector<uint16_t>> nonoInput; // first rows vectors are the rows input top to bottom and next cols vectors are the cols input left to right.
    std::vector<uint16_t> nonoInputMax;
    std::vector<uint16_t> nonoInputSum;
    

    //for display 
    std::vector<std::string> nonoInputString; //nonoInput but in string format
    size_t maxHor =0; // maximum number of digits horizontally
    size_t maxVer =0; // maximum number of digits vertically

    nonogram(uint16_t rowsIn, uint16_t colsIn);
    ~nonogram();

    bool isRowComplete(std::vector<std::vector<bool>> *matrix, uint16_t passedIndex);
    bool isColComplete(std::vector<std::vector<bool>> *matrix, uint16_t passedIndex);
    bool isColPossible(std::vector<std::vector<bool>> *matrix, uint16_t passedIndex);
    bool isAllColsPossible(std::vector<std::vector<bool>> *matrix);

    double_t currentProgress();

    void print(std::vector<std::vector<bool>> *matrix, bool isWorkingMatrix = false);

    void solveZeroCase();

    void greyoutCompletedLines();

    void solveLogicMethod();

    void mainLogic();

    void logicRowLeftToRight(int i, int j, int k, bool* edgemostInt, bool* edge, int* maxLeft);
    void logicRowRightToLeft(int i, int j, int k, bool* edgemostInt, bool* edge, int* maxRight);
    void logicColTopToBot(int i, int j, int k, int columnsHere, bool* edgemostInt, bool* edge, int* maxTop);
    void logicColBotToTop(int i, int j, int k, int columnsHere, bool* edgemostInt, bool* edge, int* maxBot);


    bool isPermutationPossible(permutationVector* rowSpace, uint16_t leftmostSpace, uint16_t rowIndex);
    bool isPermutationPossible(uint16_t blackRun, uint16_t leftmostSpace, uint16_t rowIndex);
    bool isColsSolutionDFS();

    void DFS(uint16_t index, bool *solutionFound);
    bool solveDFS();

};

/// @brief Contradiction that can arise when solving nonogram
/// plan is to use this to optimize DFS a bit further. 
class nonoContradiction : public std::exception {
public:
    nonoContradiction(char* passedMessage) : message(passedMessage) {}
    
    const char* what() const throw() {
        return message;
    }

private:
    char* message;
};
